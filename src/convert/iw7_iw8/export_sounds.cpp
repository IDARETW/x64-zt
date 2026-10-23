#include <std_include.hpp>
#include "export.hpp"

#include <io/game_files.hpp>

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// The bits of a FLAC stream, most significant first.
		class bit_reader
		{
		public:
			bit_reader(const std::uint8_t* data, const std::size_t size)
				: data_(data), size_(size)
			{
			}

			std::uint32_t read(unsigned bits)
			{
				std::uint32_t value{};
				while (bits)
				{
					if ((this->position_ >> 3) >= this->size_)
					{
						throw std::runtime_error("FLAC frame ends early");
					}

					const auto offset = static_cast<unsigned>(this->position_ & 7);
					const auto take = std::min(bits, 8 - offset);
					const auto byte = this->data_[this->position_ >> 3];
					value = (value << take) | ((byte >> (8 - offset - take)) & ((1u << take) - 1));
					this->position_ += take;
					bits -= take;
				}

				return value;
			}

			std::int32_t read_signed(const unsigned bits)
			{
				if (!bits)
				{
					return 0;
				}

				const auto value = this->read(bits);
				return bits >= 32 ? static_cast<std::int32_t>(value)
					: static_cast<std::int32_t>(value << (32 - bits)) >> (32 - bits);
			}

			std::uint32_t read_unary()
			{
				std::uint32_t count{};
				while (!this->read(1))
				{
					count++;
				}

				return count;
			}

			void align()
			{
				this->position_ = (this->position_ + 7) & ~static_cast<std::size_t>(7);
			}

			std::size_t byte_position() const
			{
				return this->position_ >> 3;
			}

		private:
			const std::uint8_t* data_;
			std::size_t size_;
			std::size_t position_{};
		};

		// The Rice-coded residual of a subframe, after its warm-up samples.
		void decode_residual(bit_reader& reader, const std::size_t block, const unsigned order, std::vector<std::int32_t>& out)
		{
			const auto method = reader.read(2);
			if (method > 1)
			{
				throw std::runtime_error("FLAC residual coding method is reserved");
			}

			const auto parameter_bits = method ? 5u : 4u;
			const auto escape = method ? 31u : 15u;
			const auto partition_order = reader.read(4);
			const auto partitions = std::size_t{1} << partition_order;
			auto at = static_cast<std::size_t>(order);
			for (std::size_t partition = 0; partition < partitions; partition++)
			{
				const auto samples = (block >> partition_order) - (partition == 0 ? order : 0);
				const auto parameter = reader.read(parameter_bits);
				if (parameter == escape)
				{
					// An escaped partition holds its samples unencoded.
					const auto bits = reader.read(5);
					for (std::size_t i = 0; i < samples && at < block; i++, at++)
					{
						out[at] = reader.read_signed(bits);
					}

					continue;
				}

				for (std::size_t i = 0; i < samples && at < block; i++, at++)
				{
					const auto value = (reader.read_unary() << parameter) | (parameter ? reader.read(parameter) : 0u);
					out[at] = static_cast<std::int32_t>(value >> 1) ^ -static_cast<std::int32_t>(value & 1);
				}
			}
		}

		void decode_subframe(bit_reader& reader, const std::size_t block, unsigned bits, std::vector<std::int32_t>& out)
		{
			out.assign(block, 0);
			reader.read(1);
			const auto type = reader.read(6);
			unsigned wasted{};
			if (reader.read(1))
			{
				wasted = reader.read_unary() + 1;
				bits -= wasted;
			}

			if (type == 0)
			{
				std::ranges::fill(out, reader.read_signed(bits));
			}
			else if (type == 1)
			{
				for (auto& sample : out)
				{
					sample = reader.read_signed(bits);
				}
			}
			else if (type >= 8 && type <= 12)
			{
				const auto order = type - 8;
				for (auto i = 0u; i < order; i++)
				{
					out[i] = reader.read_signed(bits);
				}

				decode_residual(reader, block, order, out);
				for (auto i = static_cast<std::size_t>(order); i < block; i++)
				{
					const auto* s = out.data() + i;
					switch (order)
					{
					case 1: out[i] += s[-1]; break;
					case 2: out[i] += 2 * s[-1] - s[-2]; break;
					case 3: out[i] += 3 * s[-1] - 3 * s[-2] + s[-3]; break;
					case 4: out[i] += 4 * s[-1] - 6 * s[-2] + 4 * s[-3] - s[-4]; break;
					default: break;
					}
				}
			}
			else if (type >= 32)
			{
				const auto order = type - 31;
				for (auto i = 0u; i < order; i++)
				{
					out[i] = reader.read_signed(bits);
				}

				const auto precision = reader.read(4) + 1;
				const auto shift = reader.read_signed(5);
				std::array<std::int32_t, 32> coefficients{};
				for (auto i = 0u; i < order; i++)
				{
					coefficients[i] = reader.read_signed(precision);
				}

				decode_residual(reader, block, order, out);
				for (auto i = static_cast<std::size_t>(order); i < block; i++)
				{
					std::int64_t sum{};
					for (auto j = 0u; j < order; j++)
					{
						sum += static_cast<std::int64_t>(coefficients[j]) * out[i - 1 - j];
					}

					out[i] += static_cast<std::int32_t>(sum >> std::max(shift, 0));
				}
			}
			else
			{
				throw std::runtime_error(std::format("FLAC subframe type {} is reserved", type));
			}

			if (wasted)
			{
				for (auto& sample : out)
				{
					sample <<= wasted;
				}
			}
		}

		// Decodes FLAC frames (IW7 sound asset banks keep them without the stream header) into interleaved 16-bit PCM.
		std::vector<std::int16_t> decode_flac(const std::uint8_t* data, const std::size_t size, const unsigned channels,
			const std::size_t frames)
		{
			std::vector<std::int16_t> pcm;
			pcm.reserve(frames * channels);
			bit_reader reader(data, size);
			std::array<std::vector<std::int32_t>, 2> decoded;
			while (reader.byte_position() + 4 < size && pcm.size() < frames * channels)
			{
				if (reader.read(14) != 0x3FFE)
				{
					throw std::runtime_error("FLAC frame has no sync code");
				}

				reader.read(2);
				const auto block_code = reader.read(4);
				const auto rate_code = reader.read(4);
				const auto assignment = reader.read(4);
				const auto size_code = reader.read(3);
				reader.read(1);
				const auto first = reader.read(8);
				auto ones = 0u;
				while (ones < 8 && (first & (0x80u >> ones)))
				{
					ones++;
				}

				for (auto i = 1u; i < ones; i++)
				{
					reader.read(8);
				}

				std::size_t block = block_code == 1 ? 192 : block_code >= 2 && block_code <= 5 ? std::size_t{576} << (block_code - 2)
					: block_code == 6 ? reader.read(8) + 1 : block_code == 7 ? reader.read(16) + 1
					: block_code >= 8 ? std::size_t{256} << (block_code - 8) : 0;
				if (rate_code == 12)
				{
					reader.read(8);
				}
				else if (rate_code == 13 || rate_code == 14)
				{
					reader.read(16);
				}

				reader.read(8);
				const auto bits = size_code == 1 ? 8u : size_code == 2 ? 12u : size_code == 5 ? 20u : size_code == 6 ? 24u : 16u;
				const auto frame_channels = assignment < 8 ? assignment + 1 : 2u;
				if (!block || frame_channels != channels)
				{
					throw std::runtime_error("FLAC frame does not match its sample");
				}

				for (auto c = 0u; c < frame_channels; c++)
				{
					const auto side = (assignment == 8 && c == 1) || (assignment == 9 && c == 0) || (assignment == 10 && c == 1);
					decode_subframe(reader, block, bits + (side ? 1 : 0), decoded[c]);
				}

				reader.align();
				reader.read(16);
				const auto clamp = [&](const std::int32_t value)
				{
					const auto scaled = bits == 16 ? value : bits > 16 ? value >> (bits - 16) : value << (16 - bits);
					return static_cast<std::int16_t>(std::clamp(scaled, -32768, 32767));
				};

				for (std::size_t i = 0; i < block; i++)
				{
					auto left = decoded[0][i];
					auto right = channels == 2 ? decoded[1][i] : 0;
					if (assignment == 8)
					{
						right = left - right;
					}
					else if (assignment == 9)
					{
						left += right;
					}
					else if (assignment == 10)
					{
						const auto mid = (static_cast<std::int64_t>(left) << 1) | (right & 1);
						left = static_cast<std::int32_t>((mid + right) >> 1);
						right = static_cast<std::int32_t>((mid - right) >> 1);
					}

					pcm.push_back(clamp(left));
					if (channels == 2)
					{
						pcm.push_back(clamp(right));
					}
				}
			}

			pcm.resize(std::min(pcm.size(), frames * channels));
			return pcm;
		}

		void write_wav(const std::filesystem::path& path, const std::vector<std::int16_t>& pcm, const unsigned rate,
			const unsigned channels)
		{
			std::filesystem::create_directories(path.parent_path());
			std::ofstream file(path, std::ios::binary | std::ios::trunc);
			const auto data_bytes = static_cast<std::uint32_t>(pcm.size() * 2);
			const auto put32 = [&](const std::uint32_t value) { file.write(reinterpret_cast<const char*>(&value), 4); };
			const auto put16 = [&](const std::uint16_t value) { file.write(reinterpret_cast<const char*>(&value), 2); };
			file.write("RIFF", 4);
			put32(36 + data_bytes);
			file.write("WAVEfmt ", 8);
			put32(16);
			put16(1);
			put16(static_cast<std::uint16_t>(channels));
			put32(rate);
			put32(rate * channels * 2);
			put16(static_cast<std::uint16_t>(channels * 2));
			put16(16);
			file.write("data", 4);
			put32(data_bytes);
			file.write(reinterpret_cast<const char*>(pcm.data()), data_bytes);
		}

		// A sound asset bank (.sabl) of the zone folder: its entries by id, and the bytes of each.
		class asset_bank
		{
		public:
			explicit asset_bank(const std::string& name)
				: name_(name)
			{
				iw7::SndAssetBankHeader header{};
				if (!game_files::get().read(name, 0, sizeof(header), &header) || header.entrySize != sizeof(iw7::SndAssetBankEntry))
				{
					return;
				}

				std::vector<iw7::SndAssetBankEntry> entries(header.entryCount);
				if (!game_files::get().read(name, header.entryOffset, entries.size() * sizeof(iw7::SndAssetBankEntry), entries.data()))
				{
					return;
				}

				for (const auto& entry : entries)
				{
					this->entries_.emplace(entry.id, entry);
				}
			}

			const iw7::SndAssetBankEntry* find(const std::uint32_t id) const
			{
				const auto found = this->entries_.find(id);
				return found != this->entries_.end() ? &found->second : nullptr;
			}

			std::optional<std::vector<std::uint8_t>> read(const iw7::SndAssetBankEntry& entry) const
			{
				std::vector<std::uint8_t> bytes(entry.size);
				if (!game_files::get().read(this->name_, entry.offset + entry.seekTableSize, bytes.size(), bytes.data()))
				{
					return {};
				}

				return bytes;
			}

			std::size_t size() const
			{
				return this->entries_.size();
			}

		private:
			std::string name_;
			std::unordered_map<std::uint32_t, iw7::SndAssetBankEntry> entries_;
		};
	}

	namespace
	{
		// The Replay entity channel for an IW7 channel index (games/iw7/assets/sound_globals.hpp ENTCHANNELS): the
		// same name, or the nearest Replay channel for the IW7-only ones.
		std::string replay_channel(const int index)
		{
			if (index < 0 || static_cast<std::size_t>(index) >= std::size(iw7::ENTCHANNELS))
			{
				return "scn_fx_unres_3d";
			}

			static const std::unordered_map<std::string, std::string> nearest =
			{
				{"reload_plr_akimbo_res_2d", "reload_plr_res2_2d"},
				{"reload_plr_akimbo_unres_2d", "reload_plr_unres_2d"},
				{"spear_refl_close_unres_3d_lim", "spear_refl_front_unres_3d_lim"},
				{"spear_refl_unres_3d_lim", "spear_refl_dist_unres_3d_lim"},
				{"ability_plr_unres_2d", "plr_use_misc_unres_2d"},
				{"ability_npc_unres_3d", "equip_use_unres_3d"},
				{"explo_1_3d", "explo_main_3d"},
				{"explo_2_3d", "explo_close_3d"},
				{"explo_3_3d", "explo_body_3d"},
				{"explo_4_3d", "explo_body2_3d"},
				{"explo_5_3d", "explo_debris_3d"},
				{"explo_dist_1_3d", "explo_far_3d"},
				{"explo_dist_2_3d", "explo_dist_3d"},
				{"foley_c12_step_3d", "foley_npc_step_3d"},
				{"foley_c12_lfe_3d", "vehicle_lfe_3d"},
				{"beamimpact_unres_3d_lim", "bulletimpact_unres_3d_lim"},
				{"foley_npc_zmb_step_3d_lim", "foley_npc_step_unres_3d"},
				{"vehicle_weap_close_3d_lim", "veh_weap_npc_main_3d"},
			};
			const auto& name = iw7::ENTCHANNELS[index];
			const auto found = nearest.find(name);
			return found != nearest.end() ? found->second : name;
		}

		// IW7 volmods all exist in Replay under the same names.
		std::string replay_volmod(const int index)
		{
			return index >= 0 && static_cast<std::size_t>(index) < std::size(iw7::VOLMODS) ? iw7::VOLMODS[index] : "default";
		}

		json string_or_null(const char* text)
		{
			return text && *text ? json(text) : json();
		}

		// sound/soundtable.json for the importer (iw3_sound.cpp ReadSoundTable): the bank's SoundTable (zones, reverbs,
		// filters, mixes, occlusion, ambience) and send effects (the reverbs' room types) under the Replay field names. IW7's
		// ZoneDef has no exterior flag; it follows the zone's weapon reflection preset ("exterior..." presets, as the
		// Frontier zones use them; inferred). IW7's unknown 56-byte table (SoundTable +200) has no Replay counterpart.
		// Returns the aliases that the table plays.
		std::set<std::string> export_sound_table(const source_map& source, const iw7::SndBank& bank, extraction& output)
		{
			const auto& table = bank.soundTable;
			json out = {{"map", source.name()}};
			auto& send = out["send_effects"] = json::array();
			for (auto i = 0u; i < bank.sendEffectCount; i++)
			{
				const auto& e = bank.sendEffects[i];
				send.push_back({{"name", std::string(e.name, strnlen(e.name, sizeof(e.name)))}, {"id", e.id}, {"smoothing", e.smoothing},
					{"earlyTime", e.earlyTime}, {"lateTime", e.lateTime}, {"earlyGain", e.earlyGain}, {"lateGain", e.lateGain},
					{"lateGainProx", e.lateGainProx}, {"returnGain", e.returnGain}, {"earlyLpf", e.earlyLpf}, {"lateLpf", e.lateLpf},
					{"inputLpf", e.inputLpf}, {"dampLpf", e.dampLpf}, {"wallReflect", e.wallReflect}, {"dryGain", e.dryGain},
					{"earlySize", e.earlySize}, {"lateSize", e.lateSize}, {"diffusion", e.diffusion}, {"rearLevel", e.rearLevel}});
			}

			const auto adsr_zones = [](const iw7::AdsrZoneEntry* entries, const unsigned count)
			{
				auto items = json::array();
				for (auto i = 0u; i < count; i++)
				{
					items.push_back({{"name", string_or_null(entries[i].name)}, {"id", entries[i].id},
						{"weaponName", string_or_null(entries[i].weaponName)}, {"adsrIdx", entries[i].adsrIdx}, {"weaponIdx", entries[i].weaponIdx}});
				}

				return items;
			};

			auto& adsr = out["adsr"] = json::array();
			for (auto i = 0u; i < table.adsrCount; i++)
			{
				const auto& a = table.adsrSettings[i];
				adsr.push_back({{"name", string_or_null(a.name)}, {"id", a.id}, {"attackLength", a.attackLength},
					{"attackCurve", static_cast<int>(a.attackCurve)}, {"decayLength", a.decayLength}, {"decayCurve", static_cast<int>(a.decayCurve)},
					{"releaseLength", a.releaseLength}, {"releaseCurve", static_cast<int>(a.releaseCurve)}, {"sustainLevel", a.sustainLevel},
					{"sustainLength", a.sustainLength}});
			}

			auto& defs = out["ambient_defs"] = json::array();
			for (auto i = 0u; i < table.ambientDefCount; i++)
			{
				const auto& d = table.ambientDefs[i];
				defs.push_back({{"name", string_or_null(d.name)}, {"id", d.id}, {"ambientEventIndex", d.ambientEventIndex},
					{"numEvents", d.numEvents}, {"timeMin", d.timeMin}, {"timeMax", d.timeMax}});
			}

			auto& events = out["ambient_events"] = json::array();
			for (auto i = 0u; i < table.ambientEventCount; i++)
			{
				const auto& e = table.ambientEvents[i];
				events.push_back({{"id", e.id}, {"ambientElementIndex", e.ambientElementIndex}, {"weight", e.weight}});
			}

			std::set<std::string> aliases;
			auto& elements = out["ambient_elements"] = json::array();
			for (auto i = 0u; i < table.ambientElementsCount; i++)
			{
				const auto& e = table.ambientElements[i];
				elements.push_back({{"id", e.id}, {"aliasName", string_or_null(e.aliasName)}, {"rangeMin", e.rangeMin},
					{"rangeMax", e.rangeMax}, {"coneMin", e.coneMin}, {"coneMax", e.coneMax}});
				if (e.aliasName && *e.aliasName)
				{
					aliases.insert(std::string(asset_name(e.aliasName)));
				}
			}

			auto& filters = out["filters"] = json::array();
			for (auto i = 0u; i < table.filterCount; i++)
			{
				const auto& f = table.filters[i];
				filters.push_back({{"id", f.id}, {"entChannel", replay_channel(f.entChannelIdx)}, {"band", f.band},
					{"type", static_cast<int>(f.type)}, {"freq", f.freq}, {"gain", f.gain}, {"q", f.q}});
			}

			auto& mixes = out["mixes"] = json::array();
			for (auto i = 0u; i < table.mixCount; i++)
			{
				const auto& m = table.mixes[i];
				mixes.push_back({{"id", m.id}, {"volMod", replay_volmod(m.volModIndex)}, {"volume", m.volume}, {"fade", m.fade}});
			}

			out["npc_adsr_zones"] = adsr_zones(table.npcADSRZones, table.npcADSRZoneCount);
			out["player_adsr_zones"] = adsr_zones(table.playerADSRZones, table.playerADSRZoneCount);

			auto& occlusion = out["occlusion_filters"] = json::array();
			for (auto i = 0u; i < table.occlusionFilterCount; i++)
			{
				const auto& o = table.occlusionFilters[i];
				occlusion.push_back({{"id", o.id}, {"entChannel", replay_channel(o.entChannelIdx)}, {"cutoffLevel1", o.cutoffLevel1},
					{"cutoffLevel2", o.cutoffLevel2}, {"cutoffLevel3", o.cutoffLevel3}, {"cutoffLevel4", o.cutoffLevel4},
					{"attnLevel1", o.attnLevel1}, {"attnLevel2", o.attnLevel2}, {"attnLevel3", o.attnLevel3}, {"attnLevel4", o.attnLevel4}});
			}

			auto& reverbs = out["reverbs"] = json::array();
			for (auto i = 0u; i < table.reverbCount; i++)
			{
				const auto& r = table.reverbs[i];
				reverbs.push_back({{"id", r.id}, {"roomTypeId", r.roomTypeId},
					{"roomType", std::string(r.roomType, strnlen(r.roomType, sizeof(r.roomType)))}, {"dryLevel", r.dryLevel},
					{"wetLevel", r.wetLevel}, {"fadeTime", r.fadeTime}});
			}

			auto& timescales = out["timescales"] = json::array();
			for (auto i = 0u; i < table.timeScaleSettingCount; i++)
			{
				const auto& t = table.timeScaleSettings[i];
				timescales.push_back({{"id", t.id}, {"presetName", t.presetName}, {"entChannel", replay_channel(t.entChannelIdx)}, {"scale", t.scale}});
			}

			auto& zones = out["zones"] = json::array();
			for (auto i = 0u; i < table.zoneCount; i++)
			{
				const auto& z = table.zones[i];
				const auto reflection = std::string(z.weapReflName ? z.weapReflName : "");
				zones.push_back({{"id", z.id}, {"stateId", z.stateId}, {"reverbIndex", z.reverbIndex},
					{"startOcclusionIndex", z.startOcclusionIndex}, {"numOcclusion", z.numOcclusion}, {"startFilterIndex", z.startFilterIndex},
					{"numFilter", z.numFilter}, {"startMixIndex", z.startMixIndex}, {"numMix", z.numMix},
					{"ambientStream", string_or_null(z.ambientStream)}, {"startAdsrZoneIndex", z.startAdsrZoneIndex},
					{"numAdsrZone", z.numAdsrZone}, {"startNPCAdsrZoneIndex", z.startNPCAdsrZoneIndex}, {"numNPCAdsrZone", z.numNPCAdsrZone},
					{"ambientDefIndex", z.ambientDefIndex}, {"startFullOccIndex", z.startFullOccIndex}, {"numDisableFullOcc", z.numDisableFullOcc},
					{"startPlayerBreathStateIndex", z.startPlayerBreathStateIndex}, {"numPlayerBreathStates", z.numPlayerBreathStates},
					{"weapReflId", z.weapReflId}, {"exterior", reflection.starts_with("exterior")},
					{"zoneName", string_or_null(z.zoneName)}, {"stateName", string_or_null(z.stateName)},
					{"ambientName", string_or_null(z.ambientName)}, {"mixName", string_or_null(z.mixName)},
					{"reverbName", string_or_null(z.reverbName)}, {"filterName", string_or_null(z.filterName)},
					{"occlusionName", string_or_null(z.occlusionName)}, {"plrAdsrName", string_or_null(z.plrAdsrName)},
					{"npcAdsrName", string_or_null(z.npcAdsrName)}, {"weapReflName", string_or_null(z.weapReflName)},
					{"fullOccName", string_or_null(z.fullOccName)}, {"playerBreathStateName", string_or_null(z.playerBreathStateName)},
					{"contextType1", z.contextType1}, {"contextValue1", z.contextValue1}, {"contextType2", z.contextType2},
					{"contextValue2", z.contextValue2}, {"contextType3", z.contextType3}, {"contextValue3", z.contextValue3},
					{"contextType4", z.contextType4}, {"contextValue4", z.contextValue4}});
				if (z.ambientStream && *z.ambientStream)
				{
					aliases.insert(std::string(asset_name(z.ambientStream)));
				}
			}

			auto& full = out["full_occlusions"] = json::array();
			for (auto i = 0u; i < table.fullOcclusionDefCount; i++)
			{
				const auto& f = table.fullOcclusionDefs[i];
				full.push_back({{"id", f.id}, {"presetName", f.presetName}, {"entChannel", replay_channel(f.entChannelIdx)}});
			}

			auto& breath = out["breath_states"] = json::array();
			for (auto i = 0u; i < table.plrBreathStateDefCount; i++)
			{
				const auto& b = table.plrBreathStateDefs[i];
				breath.push_back({{"stateType", b.stateType}, {"inhaleAlias", string_or_null(b.inhaleAlias)},
					{"exhaleAlias", string_or_null(b.exhaleAlias)}, {"minBreaths", b.minBreaths}});
			}

			if (table.unkCount)
			{
				ZONETOOL_INFO("iw7: sound table drops %u entries of its unknown table (no Replay counterpart)", table.unkCount);
			}

			write_json(output.extracted / "sound" / "soundtable.json", out);
			ZONETOOL_INFO("iw7: exported sound table: %u zones, %u reverbs, %u filters, %u mixes, %u occlusion filters, %u send effects",
				table.zoneCount, table.reverbCount, table.filterCount, table.mixCount, table.occlusionFilterCount, bank.sendEffectCount);
			return aliases;
		}
	}

	void export_sounds(const source_map& source, extraction& output)
	{
		const auto bank = static_cast<const iw7::SndBank*>(source.find(iw7::ASSET_TYPE_SOUND_BANK, source.name() + ".all"));
		if (!bank || !bank->alias)
		{
			ZONETOOL_WARNING("iw7: the map has no sound bank %s.all; it has no sounds", source.name().data());
			return;
		}

		// The table's aliases (zone ambience beds, ambient elements) export with the aliases that the scripts name.
		for (const auto& name : export_sound_table(source, *bank, output))
		{
			output.sound_names.insert(name);
		}

		if (output.sound_names.empty())
		{
			return;
		}

		// Loaded sounds sit in <zone>.sabl, streamed ones (ambience beds) in <zone>.sabs, for each bank's zone.
		std::map<std::string, std::pair<asset_bank, asset_bank>> sound_files;
		const auto files_of = [&](const iw7::SndBank& owner) -> const std::pair<asset_bank, asset_bank>&
		{
			const std::string zone = owner.zone ? owner.zone : source.name();
			auto found = sound_files.find(zone);
			if (found == sound_files.end())
			{
				found = sound_files.try_emplace(zone, asset_bank(zone + ".sabl"), asset_bank(zone + ".sabs")).first;
				if (!found->second.first.size() && !found->second.second.size())
				{
					ZONETOOL_WARNING("iw7: the zone folder has no %s.sabl or %s.sabs; the aliases of %s have no sounds", zone.data(),
						zone.data(), owner.name ? owner.name : "?");
				}
			}

			return found->second;
		};

		// The aliases of the map bank, then of the other loaded banks (the common zones' banks hold the aliases that common
		// effects play: Frontier's emt_sparks_runner, emt_steam_hiss_long_dist).
		std::unordered_map<std::string, std::pair<const iw7::SndAliasList*, const iw7::SndBank*>> lists;
		const auto add_lists = [&](const iw7::SndBank& owner)
		{
			for (auto i = 0u; owner.alias && i < owner.aliasCount; i++)
			{
				if (owner.alias[i].aliasName)
				{
					lists.try_emplace(std::string(asset_name(owner.alias[i].aliasName)), &owner.alias[i], &owner);
				}
			}
		};

		add_lists(*bank);
		for (const auto& [bank_name, asset] : source.assets(iw7::ASSET_TYPE_SOUND_BANK))
		{
			if (asset != bank)
			{
				add_lists(*static_cast<const iw7::SndBank*>(asset));
			}
		}

		// The curves that the aliases name (SndGlobals "singleton" of code_post_gfx; games/iw7/assets/sound_globals.hpp
		// lists their names in the same order).
		const auto globals = static_cast<const iw7::SndGlobals*>(source.find(iw7::ASSET_TYPE_SOUND_GLOBALS, "singleton"));
		struct curve_kind
		{
			const char* key;
			const iw7::SndCurve* curves;
			unsigned count;
			const std::string* names;
			std::size_t name_count;
		};
		const std::array<curve_kind, 4> curve_kinds{{
			{"volume_falloff", globals ? globals->vfCurves : nullptr, globals ? globals->vfCurveCount : 0, iw7::vf_curves_s, std::size(iw7::vf_curves_s)},
			{"lpf", globals ? globals->lpfCurves : nullptr, globals ? globals->lpfCurveCount : 0, iw7::lpf_curves_s, std::size(iw7::lpf_curves_s)},
			{"hpf", globals ? globals->hpfCurves : nullptr, globals ? globals->hpfCurveCount : 0, iw7::hpf_curves_s, std::size(iw7::hpf_curves_s)},
			{"reverb_send", globals ? globals->rvbCurves : nullptr, globals ? globals->rvbCurveCount : 0, iw7::rvb_curves_s, std::size(iw7::rvb_curves_s)},
		}};
		std::array<std::map<std::string, json>, 4> used_curves;
		const auto curve_name = [&](const std::size_t kind, const unsigned index) -> std::string
		{
			const auto& k = curve_kinds[kind];
			if (k.curves && index < k.count)
			{
				const auto& curve = k.curves[index];
				const auto name = std::string(curve.name, strnlen(curve.name, sizeof(curve.name)));
				if (name != "default" && !used_curves[kind].contains(name))
				{
					auto points = json::array();
					for (auto p = 0; p < curve.numPoints && p < 16; p++)
					{
						points.push_back({curve.points[p][0], curve.points[p][1]});
					}

					used_curves[kind][name] = std::move(points);
				}

				return name;
			}

			return index < k.name_count ? k.names[index] : "default";
		};

		// The speaker maps, occlusion shapes and doppler presets that the aliases name (same globals), by name.
		std::map<std::string, json> speaker_maps, occlusion_shapes, doppler_presets;
		const auto floats = [](const float* values, const std::size_t count)
		{
			return json(std::vector<float>(values, values + count));
		};
		const auto preset_name = [&](const char* text, const std::size_t size)
		{
			return std::string(text, strnlen(text, size));
		};
		const auto speaker_map = [&](const unsigned index) -> std::string
		{
			if (!globals || index >= globals->speakerMapCount)
			{
				return index < std::size(iw7::speaker_maps_s) ? iw7::speaker_maps_s[index] : "default";
			}

			const auto& map = globals->speakerMaps[index];
			const auto map_name = preset_name(map.name, sizeof(map.name));
			if (map_name != "default")
			{
				speaker_maps.try_emplace(map_name, json{{"monoToStereo", floats(map.monoToStereo, 2)},
					{"stereoToStereo", floats(map.stereoToStereo, 4)}, {"monoTo51", floats(map.monoTo51, 6)},
					{"stereoTo51", floats(map.stereoTo51, 12)}, {"monoTo71", floats(map.monoTo71, 8)},
					{"stereoTo71", floats(map.stereoTo71, 16)}});
			}

			return map_name;
		};
		const auto occlusion_shape = [&](const unsigned index) -> std::string
		{
			if (!globals || index >= globals->occlusionShapeCount)
			{
				return index < std::size(iw7::occlusion_shapes_s) ? iw7::occlusion_shapes_s[index] : "default";
			}

			const auto& shape = globals->occlusionShapes[index];
			const auto shape_name = preset_name(shape.name, sizeof(shape.name));
			if (shape_name != "default")
			{
				occlusion_shapes.try_emplace(shape_name, json{{"innerAngleVolume", shape.innerAngleVolume},
					{"outerAngleVolume", shape.outerAngleVolume}, {"occludedVolume", shape.occludedVolume},
					{"innerAngleLPF", shape.innerAngleLPF}, {"outerAngleLPF", shape.outerAngleLPF}, {"occludedLPF", shape.occludedLPF}});
			}

			return shape_name;
		};
		const auto doppler_preset = [&](const unsigned index) -> std::string
		{
			if (!globals || index >= globals->dopplerPresetCount)
			{
				return index < std::size(iw7::doppler_presets_s) ? iw7::doppler_presets_s[index] : "default";
			}

			const auto& preset = globals->dopplerPresets[index];
			const auto doppler_name = preset_name(preset.name, sizeof(preset.name));
			if (doppler_name != "default")
			{
				doppler_presets.try_emplace(doppler_name, json{{"speedOfSound", preset.speedOfSound},
					{"playerVelocityScale", preset.playerVelocityScale}, {"minPitch", preset.minPitch}, {"maxPitch", preset.maxPitch},
					{"smoothing", preset.smoothing}});
			}

			return doppler_name;
		};

		std::set<std::uint32_t> used_ducks;

		// The aliases that the scripts name, with their secondary aliases.
		std::vector<std::string> pending(output.sound_names.begin(), output.sound_names.end());
		std::set<std::string> visited;
		std::set<std::uint32_t> written;
		std::size_t aliases{};
		std::size_t samples{};
		std::size_t global_samples{};
		// IW7 finds a sound by its id in any loaded bank: after the alias's own zone, the banks of the other loaded zones
		// (common_mp holds those of common effects' aliases, Frontier's emt_sparks_runner), then the global stream bank,
		// where base-game maps keep their streamed sounds (the zone ambience beds; their own .sabs hold a header only).
		std::set<std::string> bank_zones;
		for (const auto& [bank_name, asset] : source.assets(iw7::ASSET_TYPE_SOUND_BANK))
		{
			const auto* other = static_cast<const iw7::SndBank*>(asset);
			bank_zones.insert(other->zone ? other->zone : source.name());
		}

		bank_zones.erase(source.name());
		std::vector<asset_bank> global_banks;
		for (const auto& zone : bank_zones)
		{
			global_banks.emplace_back(zone + ".sabl");
			global_banks.emplace_back(zone + ".sabs");
		}

		for (const std::string zone : {"patch_code_post_gfx", "code_post_gfx"})
		{
			global_banks.emplace_back(zone + ".sabl");
			global_banks.emplace_back(zone + ".sabs");
		}
		while (!pending.empty())
		{
			const auto name = pending.back();
			pending.pop_back();
			const auto found = lists.find(name);
			if (found == lists.end() || !visited.insert(name).second)
			{
				continue;
			}

			auto variations = json::array();
			const auto& [loaded, streamed] = files_of(*found->second.second);
			for (auto v = 0; v < found->second.first->count; v++)
			{
				const auto& alias = found->second.first->head[v];
				const auto* source_bank = loaded.find(alias.assetId) ? &loaded : streamed.find(alias.assetId) ? &streamed : nullptr;
				for (auto global = global_banks.begin(); !source_bank && global != global_banks.end(); ++global)
				{
					if (global->find(alias.assetId))
					{
						source_bank = &*global;
					}
				}

				const auto entry = source_bank ? source_bank->find(alias.assetId) : nullptr;
				if (!entry || entry->channelCount < 1 || entry->channelCount > 2)
				{
					ZONETOOL_WARNING("iw7: sound %08x of alias \"%s\" %s", alias.assetId, name.data(), entry ?
						std::format("has {} channels", entry->channelCount).data() : "is in no loaded sound bank");
					continue;
				}

				const auto file = std::format("iw7/{:08x}.wav", alias.assetId);
				if (written.insert(alias.assetId).second)
				{
					const auto bytes = source_bank->read(*entry);
					try
					{
						if (!bytes)
						{
							throw std::runtime_error("cannot be read");
						}

						const auto pcm = decode_flac(bytes->data(), bytes->size(), entry->channelCount, entry->frameCount);
						write_wav(output.extracted / "sound" / file, pcm, static_cast<unsigned>(entry->frameRate), entry->channelCount);
						samples++;
						if (source_bank != &loaded && source_bank != &streamed)
						{
							global_samples++;
						}
					}
					catch (const std::exception& e)
					{
						ZONETOOL_WARNING("iw7: sound %08x of alias \"%s\" does not convert: %s", alias.assetId, name.data(), e.what());
						continue;
					}
				}

				// IW3's flags: bit 0 loops, bits 8-13 the channel (1 = auto).
				const auto looping = alias.flags.looping != 0;
				json variation =
				{
					{"file", {{"name", file}}},
					{"volume", {{"min", alias.volMin}, {"max", alias.volMax}}},
					{"pitch", {{"min", alias.pitchMin}, {"max", alias.pitchMax}}},
					{"distance", {{"min", alias.distMin}, {"max", alias.distMax}}},
					{"probability", alias.probability},
					{"lfe_percentage", alias.lfePercentage},
					{"center_percentage", alias.centerPercentage},
					{"start_delay", alias.startDelay},
					{"flags", (looping ? 1u : 0u) | (1u << 8)},
					{"envelope", {{"min", alias.envelopMin}, {"max", alias.envelopMax}, {"percentage", alias.envelopPercentage}}},
				};

				if (alias.secondaryAliasName && *alias.secondaryAliasName)
				{
					const auto secondary = std::string(asset_name(alias.secondaryAliasName));
					variation["secondary_alias"] = secondary;
					pending.push_back(secondary);
				}

				// IW7's own mix fields: the importer (iw3_sound.cpp) takes them over the IW3 role templates. Channels and
				// volmods by Replay name; curves by IW7 name (sound/curves.json holds their points).
				variation["iw7"] =
				{
					{"volmod", replay_volmod(alias.volModIndex)},
					{"channel", replay_channel(static_cast<int>(alias.flags.channel))},
					{"looping", static_cast<unsigned>(alias.flags.looping)},
					{"full_dry_level", alias.flags.reverb != 0},
					{"shape", static_cast<unsigned>(alias.flags.shape)},
					{"precached", alias.flags.precached != 0},
					{"donut_fade_end", alias.donutFadeEnd},
					{"velocity_min", alias.velocityMin},
					{"curves", {{"volume_falloff", curve_name(0, alias.volumeFalloffCurveIndex)}, {"lpf", curve_name(1, alias.lpfCurveIndex)},
						{"hpf", curve_name(2, alias.hpfCurveIndex)}, {"reverb_send", curve_name(3, alias.reverbSendCurveIndex)}}},
					{"reverb_multiplier", alias.reverbMultiplier},
					{"far_reverb_multiplier", alias.farReverbMultiplier},
					{"smart_pan", {alias.smartPanDistance2d, alias.smartPanDistance3d, alias.smartPanAttenuation2d}},
					{"stereo_spread", {alias.stereoSpreadMinDist, alias.stereoSpreadMaxDist, alias.stereoSpreadMidPoint,
						std::clamp(alias.stereoSpreadMaxAngle, 0, 255)}},
					{"master", {alias.masterPriority, alias.masterPercentage, alias.slavePercentage}},
					{"context", {alias.contextType, alias.contextValue}},
					{"speaker_map", speaker_map(alias.speakerMapIndex)},
					{"occlusion_shape", occlusion_shape(alias.occlusionShapeIndex)},
					{"doppler_preset", doppler_preset(alias.dopplerPresetIndex)},
					{"duck", alias.duck},
				};
				if (alias.duck)
				{
					used_ducks.insert(alias.duck);
				}
				variations.push_back(std::move(variation));
			}

			if (variations.empty())
			{
				// Why none converts: no bank holds the sound, or it has more channels than the importer takes.
				std::string sounds;
				for (auto v = 0; v < found->second.first->count; v++)
				{
					const auto id = found->second.first->head[v].assetId;
					const iw7::SndAssetBankEntry* entry = loaded.find(id) ? loaded.find(id) : streamed.find(id);
					for (auto global = global_banks.begin(); !entry && global != global_banks.end(); ++global)
					{
						entry = global->find(id);
					}

					sounds += std::format("{}{:08x} ({})", v ? ", " : "", id,
						entry ? std::format("{} channels", static_cast<int>(entry->channelCount)) : std::string("in no bank"));
				}

				ZONETOOL_WARNING("iw7: alias \"%s\" has no sound that converts: %s", name.data(), sounds.data());
				continue;
			}

			write_json(output.extracted / "soundaliases" / (file_name(name) + ".iw3.json"), {{"name", name}, {"aliases", std::move(variations)}});
			aliases++;
		}

		json curves = json::object();
		std::size_t curve_count{};
		for (auto kind = 0u; kind < curve_kinds.size(); kind++)
		{
			auto& items = curves[curve_kinds[kind].key] = json::object();
			for (auto& [curve, points] : used_curves[kind])
			{
				items[curve] = std::move(points);
				curve_count++;
			}
		}

		// IW7's occlusion shapes keep a float at +68 that Replay's lack (Replay adds yaw/pitch offsets): it does not convert.
		curves["speaker_maps"] = json(speaker_maps);
		curves["occlusion_shapes"] = json(occlusion_shapes);
		curves["doppler_presets"] = json(doppler_presets);
		write_json(output.extracted / "sound" / "curves.json", curves);

		// sound/ducks.json: the ducks that the exported aliases start (SndDuck: fades, the attenuation and low-pass of each
		// volmod while it holds; the 128-entry arrays are indexed by volmod, here by Replay volmod name).
		auto ducks = json::array();
		for (auto d = 0u; bank->ducks && d < bank->duckCount; d++)
		{
			const auto& duck = bank->ducks[d];
			if (!used_ducks.contains(duck.id))
			{
				continue;
			}

			const auto by_volmod = [&](const float* values)
			{
				json result = json::object();
				for (auto v = 0u; values && v < std::size(iw7::VOLMODS); v++)
				{
					result[iw7::VOLMODS[v]] = values[v];
				}

				return result;
			};
			ducks.push_back({{"name", std::string(duck.name, strnlen(duck.name, sizeof(duck.name)))}, {"id", duck.id},
				{"fadeIn", duck.fadeIn}, {"fadeOut", duck.fadeOut}, {"startDelay", duck.startDelay}, {"minDistance", duck.minDistance},
				{"distance", duck.distance}, {"length", duck.length}, {"fadeInCurve", duck.fadeInCurve}, {"fadeOutCurve", duck.fadeOutCurve},
				{"updateWhilePaused", duck.updateWhilePaused}, {"trackAmplitude", duck.trackAmplitude},
				{"disableInSplitscreen", duck.disableInSplitscreen}, {"aliasAttenuation", duck.aliasAttenuation}, {"aliasLpf", duck.aliasLpf},
				{"duckAlias", duck.duckAlias}, {"attenuation", by_volmod(duck.attenuation)}, {"lpf", by_volmod(duck.lpf)}});
		}

		const auto duck_count = ducks.size();
		write_json(output.extracted / "sound" / "ducks.json", std::move(ducks));
		if (duck_count < used_ducks.size())
		{
			ZONETOOL_WARNING("iw7: %zu ducks that aliases start are not in the bank", used_ducks.size() - duck_count);
		}
		if (!globals)
		{
			ZONETOOL_WARNING("iw7: no sound globals (code_post_gfx); the aliases' curves export by name only and keep the default");
		}

		ZONETOOL_INFO("iw7: exported %zu of %zu sound aliases that the scripts name, with %zu sounds (%zu from other zones' banks "
			"or the global stream bank) and %zu curves of %s", aliases, output.sound_names.size(), samples, global_samples,
			curve_count, bank->name ? bank->name : "?");
	}
}
