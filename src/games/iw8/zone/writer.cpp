#include <std_include.hpp>
#include "../iw8.hpp"
#include "loader.hpp"
#include "writer.hpp"

#include <limits>
#include <set>

namespace zonetool::iw8
{
	namespace
	{
		constexpr std::size_t block_count = 11;
		constexpr std::uint8_t preload_block = 2;

		using asset_key = std::pair<std::int32_t, std::string>;

		std::string name_of(const std::int32_t type, const void* header)
		{
			const auto name = get_asset_name(type, header);
			return name ? name : "";
		}

		void validate_scriptable(const ScriptableDef& definition)
		{
			if (definition.name && definition.name[0] == ',')
			{
				return;
			}

			std::unordered_set<const ScriptablePartDef*> active;
			std::function<void(const ScriptablePartDef*, std::uint32_t)> visit;
			visit = [&](const ScriptablePartDef* parts, const std::uint32_t count)
			{
				if (count && !parts)
				{
					throw std::runtime_error("part array is null with a nonzero count");
				}
				for (auto i = 0u; i < count; i++)
				{
					const auto& part = parts[i];
					if (!active.insert(&part).second)
					{
						throw std::runtime_error("cyclic Scriptable part tree");
					}
					if (part.flatId >= definition.partCount)
					{
						throw std::runtime_error(std::format("part flatId {} exceeds declared partCount {}", part.flatId, definition.partCount));
					}
					if (part.numStates && !part.states)
					{
						throw std::runtime_error("part states are null with a nonzero count");
					}
					// Replay 0x12EA830 initializes first-state health at the part's client stream offset.
					if (part.numStates && (part.states[0].base.flags & 0x80) &&
						static_cast<std::size_t>(part.eventStreamBufferOffsetClient) + sizeof(float) > definition.eventStreamSizeRequiredClient)
					{
						throw std::runtime_error("part initial health exceeds the declared client event stream");
					}
					visit(part.childParts, part.numChildParts);
					active.erase(&part);
				}
			};
			visit(definition.parts, definition.numParts);
		}

		class script_string_table
		{
		public:
			explicit script_string_table(const std::span<const char* const> start)
			{
				for (const auto string : start)
				{
					this->add(string);
				}
			}

			std::uint32_t index_of(const std::uint32_t id, const bool collect)
			{
				if (const auto found = this->indices_.find(id); found != this->indices_.end())
				{
					return found->second;
				}

				if (!collect)
				{
					throw std::runtime_error(std::format("script string \"{}\" is not in the zone list", SL_ConvertToString(id)));
				}

				return this->add(SL_ConvertToString(id));
			}

			const std::vector<const char*>& strings() const
			{
				return this->strings_;
			}

		private:
			std::uint32_t add(const char* string)
			{
				const auto index = static_cast<std::uint32_t>(this->strings_.size());
				this->strings_.push_back(string);
				this->indices_.try_emplace(string ? SL_GetString(string) : 0, index);
				return index;
			}

			std::vector<const char*> strings_;
			std::unordered_map<std::uint32_t, std::uint32_t> indices_;
		};

		struct preload_link
		{
			std::string label;
			std::int32_t type;
			void* header;
		};

		// A list entry (its header `top`) whose preloaded assets nest deeper than Replay's preload mark stack (2): the
		// second asset of the chain (`type`, `header`) is to be listed just before it.
		struct preload_move
		{
			void* top;
			std::int32_t type;
			void* header;
			std::string chain;
		};

		class zone_assets_writer
		{
		public:
			zone_assets_writer(const std::span<const zone_asset> assets, script_string_table& strings,
				const write_options& options, const bool collect)
				: writer_(block_count, replay_layout(), !collect)
				, strings_(strings)
				, options_(options)
				, collect_(collect)
			{
				this->writer_.reserve_data(options.expected_size);
				for (const auto& asset : assets)
				{
					if (!asset.referenced)
					{
						this->assets_.try_emplace(asset_key{asset.type, name_of(asset.type, asset.header)}, asset.header);
						this->headers_.insert(asset.header);
					}
					else
					{
						this->references_.insert(asset.header);
					}
				}
			}

			written_zone write(const std::span<const zone_asset> list)
			{
				write_stream stream(this->writer_,
					[this](write_stream& s, const std::int32_t type, void** field)
					{
						this->write_asset(s, type, field, true, true);
					},
					[this](const std::uint32_t id)
					{
						return this->strings_.index_of(id, this->collect_);
					},
					[this](const void* field)
					{
						return this->marker_of(field);
					});

				std::vector<const char*> strings(this->strings_.strings());
				std::vector<xasset_entry> entries;
				entries.reserve(list.size());
				for (const auto& asset : list)
				{
					entries.emplace_back(static_cast<std::uint32_t>(asset.type), 0, asset.header);
					if (asset.marker)
					{
						this->list_markers_[&entries.back().header] = asset.marker;
					}
				}

				// A kept marker also holds for an empty list: the loader aligns for any pointer that is not null.
				const auto list_marker = [](const std::uint64_t kept, const bool empty)
				{
					return kept ? kept : empty ? 0 : write_stream::following;
				};

				xasset_list header{};
				header.string_count = static_cast<std::uint32_t>(strings.size());
				header.strings = reinterpret_cast<const char**>(list_marker(this->options_.strings_marker, strings.empty()));
				header.asset_count = static_cast<std::uint32_t>(entries.size());
				header.assets = reinterpret_cast<xasset_entry*>(list_marker(this->options_.assets_marker, entries.empty()));
				this->writer_.write_raw(&header, sizeof(header));

				this->writer_.push_stream(write_stream::virtual_block);
				if (header.strings)
				{
					this->writer_.align(7);
					this->writer_.write_copy(strings.data(), strings.size() * sizeof(const char*));
					for (auto& string : strings)
					{
						stream.string(string);
					}
				}

				if (header.assets)
				{
					this->writer_.align(7);
					this->writer_.write_copy(entries.data(), entries.size() * sizeof(xasset_entry));
					for (auto& entry : entries)
					{
						this->list_header_ = entry.header;
						this->write_list_asset(stream, entry);
					}
				}

				this->writer_.pop_stream();

				written_zone zone{};
				zone.data = this->writer_.take_data();
				zone.block_sizes = this->writer_.block_sizes();
				// Stock zones declare 0x20 more for block 1 (the size of the asset list header) and 0x40 more for
				// block 2 than the peaks of the loader and the map preload.
				zone.block_sizes[write_stream::temp_block] += sizeof(xasset_list);
				zone.block_sizes[preload_block] += 0x40;
				// The preload plan records [block u8][int32 delta] for each of 8 moved blocks and a terminator per
				// asset of the list (see vault re/replay-120-postload-plan).
				zone.preload_walk_size = 41 * entries.size() + 16;
				zone.list_asset_count = entries.size();
				zone.asset_offsets = std::move(this->asset_offsets_);
				return zone;
			}

		private:
			zone_writer writer_;
			script_string_table& strings_;
			const write_options& options_;
			bool collect_;
			std::map<asset_key, void*> assets_;
			// The headers with data: a zone can hold one name twice, and each use writes the header it points at.
			std::unordered_set<const void*> headers_;
			// The headers of referenced assets: a use of one writes a name stub.
			std::unordered_set<const void*> references_;
			// Virtual block positions of the inserted pointers and list entries that hold each written asset.
			std::map<asset_key, std::size_t> aliases_;
			// The keys of aliases_ that point at a name stub (no definition written yet).
			std::set<asset_key> stub_keys_;
			// Inline preloaded assets in progress (type name and asset name), outermost first: Replay's preload mark
			// stack holds two (see write_asset).
			std::vector<preload_link> preload_chain_;
			void* list_header_ = nullptr;  // the header of the list entry being written
			std::vector<preload_move> preload_moves_;

		public:
			const std::vector<preload_move>& preload_moves() const
			{
				return this->preload_moves_;
			}

		private:
			// The same for the stubs of write_options::reference_uses.
			std::map<asset_key, std::size_t> reference_aliases_;
			std::deque<std::vector<std::uint8_t>> stubs_;
			std::deque<std::string> stub_names_;
			// stub copy -> header of the referenced asset, for the markers of its fields
			std::map<std::uintptr_t, std::pair<std::size_t, const std::uint8_t*>> stub_sources_;
			std::unordered_map<const void*, std::uint64_t> list_markers_;
			std::vector<std::tuple<std::size_t, std::int32_t, std::string>> asset_offsets_;

			std::optional<std::uint64_t> marker_of(const void* field) const
			{
				if (const auto found = this->list_markers_.find(field); found != this->list_markers_.end())
				{
					return found->second;
				}

				if (!this->options_.markers)
				{
					return {};
				}

				auto address = static_cast<const std::uint8_t*>(field);
				auto stub = this->stub_sources_.upper_bound(reinterpret_cast<std::uintptr_t>(field));
				if (stub != this->stub_sources_.begin())
				{
					--stub;
					const auto offset = reinterpret_cast<std::uintptr_t>(field) - stub->first;
					if (offset < stub->second.first)
					{
						address = stub->second.second + offset;
					}
				}

				const auto found = this->options_.markers->find(address);
				return found != this->options_.markers->end() ? std::optional(found->second) : std::nullopt;
			}

			void write_list_asset(write_stream& stream, xasset_entry& entry)
			{
				auto& writer = stream.writer();
				writer.push_stream(write_stream::temp_block);
				if (entry.header)
				{
					// Stock zones follow (-2) the list entries, and later uses alias the entry itself.
					this->write_asset(stream, static_cast<std::int32_t>(entry.type), &entry.header, this->options_.insert_list_assets);
				}

				writer.pop_stream();
			}

			void write_asset(write_stream& stream, const std::int32_t type, void** field, const bool insert_by_default = true,
				const bool use = false)
			{
				auto& writer = stream.writer();
				const auto header = *field;
				const asset_key key{type, name_of(type, header)};
				// write_options::reference_uses: a use is a stub, which never aliases the data of a list entry.
				const auto reference = use && this->options_.reference_uses;
				auto& aliases = reference ? this->reference_aliases_ : this->aliases_;

				// A kept marker writes the asset again, as the zone did; a kept offset points where the zone did.
				const auto kept = this->marker_of(field);
				if (kept && writer.is_offset(*kept))
				{
					writer.set_pointer(field, *kept);
					return;
				}

				// A zone can hold a name stub and the definition of the same asset (stock mp_br_quarry: scriptable
				// ks_ac130_mp). When the stub was used first, the definition must still be written: aliasing it to the
				// stub would drop it. Later uses then point at the definition.
				const auto definition_after_stub = !reference && this->headers_.contains(header) && this->stub_keys_.contains(key);
				if (const auto alias = aliases.find(key); alias != aliases.end() && !kept && !definition_after_stub)
				{
					writer.set_pointer(field, writer.offset_value(write_stream::virtual_block, alias->second));
					return;
				}

				const auto format = find_asset_format(type);
				if (!format)
				{
					throw std::runtime_error(std::format("asset type {} has no format", type));
				}

				const auto insert = kept ? *kept == write_stream::insert : insert_by_default;
				writer.set_pointer(field, kept.value_or(insert ? write_stream::insert : write_stream::following));
				writer.align(3);
				// Later uses point at the asset data: a stub keeps the first place, a written asset the latest.
				const auto stub = reference || (!this->headers_.contains(header) && (this->references_.contains(header) ||
					!this->assets_.contains(key)));
				const auto remember = [&](const std::size_t position)
				{
					if (stub)
					{
						if (aliases.try_emplace(key, position).second && !reference)
						{
							this->stub_keys_.insert(key);
						}
					}
					else
					{
						aliases.insert_or_assign(key, position);
						if (!reference)
						{
							this->stub_keys_.erase(key);
						}
					}
				};

				if (insert)
				{
					// Load_*Ptr: an inserted asset reserves an aligned pointer slot in the virtual block first.
					writer.push_stream(write_stream::virtual_block);
					writer.align(7);
					const auto slot = writer.reserve(sizeof(void*));
					writer.pop_stream();
					remember(slot);
				}
				else if (const auto position = writer.position_of(field))
				{
					remember(*position);
				}

				// The map preload loads the header of most types to block 2, which keeps it until the zone is loaded:
				// reserve it there too, around the asset, so that nested assets come after it.
				// Replay 1.20's loaders push every asset they load onto DB_PatchMem's asset stack, which holds two records
				// and has no bound check (q_DB_PatchMem_PushAsset 0x140D97A80 records at [counter]; stack 0x145DA4830,
				// counter 0x145DA48B8): an asset loaded inline in an inline asset of a list entry overwrites the counter
				// and the next push faults (0xC0000005 at 0x140D97ADD). Every type counts (210 loaders push, the
				// map-preloaded ones and the others alike). Refuse such a zone here; collecting, record it to move.
				preload_chain_.push_back({std::string(format->name) + " \"" + key.second + "\"", type, header});
				if (preload_chain_.size() > 2)
				{
					std::string chain;
					for (const auto& link : preload_chain_)
					{
						chain += (chain.empty() ? "" : " > ") + link.label;
					}
					if (!this->collect_)
					{
						throw std::runtime_error(std::format("assets nest {} deep inline ({}); the game holds 2 and crashes "
							"on load", preload_chain_.size(), chain));
					}
					// Collecting: write_zone lists the middle asset before this entry and collects again.
					this->preload_moves_.push_back({this->list_header_, preload_chain_[1].type, preload_chain_[1].header, chain});
				}

				const auto preloaded = format->preload_block == preload_block;
				if (preloaded)
				{
					writer.push_stream(preload_block);
					writer.align(3);
					writer.align(format->alignment);
					writer.reserve(format->size);
					writer.push_stream(write_stream::temp_block);
				}

				void* source = header;
				if (reference)
				{
					source = this->stub(*format, header, key.second);
				}
				else if (!this->headers_.contains(header))
				{
					const auto full = this->references_.contains(header) ? this->assets_.end() : this->assets_.find(key);
					source = full != this->assets_.end() ? full->second : this->stub(*format, header, key.second);
				}

				writer.align(format->alignment);
				this->asset_offsets_.emplace_back(writer.data_size(), type, key.second);
				writer.write_copy(source, format->size);
				try
				{
					if (type == ASSET_TYPE_SCRIPTABLE && !stub)
					{
						validate_scriptable(*static_cast<const ScriptableDef*>(source));
					}
					format->write(stream, source);
				}
				catch (const std::exception& e)
				{
					throw std::runtime_error(std::format("{} \"{}\": {}", format->name, key.second, e.what()));
				}

				if (preloaded)
				{
					writer.pop_stream();
					writer.pop_stream();
				}
				preload_chain_.pop_back();
			}

			// A referenced asset keeps the header bytes of its zone. An asset that the zone does not hold (an asset of
			// a required zone) gets a header with only its name.
			void* stub(const asset_format& format, const void* header, const std::string& name)
			{
				auto& data = this->stubs_.emplace_back(format.size);
				if (this->references_.contains(header))
				{
					std::memcpy(data.data(), header, format.size);
					this->stub_sources_[reinterpret_cast<std::uintptr_t>(data.data())] = {format.size, static_cast<const std::uint8_t*>(header)};

					// A loaded stub names the asset past its ',': keep the zone string, so that sharing stays the same.
					const auto loaded_name = *static_cast<const char* const*>(header);
					if (loaded_name && loaded_name[-1] == ',')
					{
						*reinterpret_cast<const char**>(data.data()) = loaded_name - 1;
						return data.data();
					}
				}

				const auto& stub_name = this->stub_names_.emplace_back("," + name);
				*reinterpret_cast<const char**>(data.data()) = stub_name.data();
				return data.data();
			}
		};
	}

	written_zone write_zone(const std::span<const zone_asset> list, const std::span<const zone_asset> assets,
		const std::span<const char* const> script_strings, const write_options& options)
	{
		// Replay's preload mark stack holds two nested preloaded assets (see write_asset). An entry that nests three deep
		// gets its middle asset listed just before it (it then aliases that one), as stock zones order a scriptable's
		// models before it; the collecting pass runs again until no chain is left.
		std::vector<zone_asset> ordered(list.begin(), list.end());
		for (std::size_t round = 0;; ++round)
		{
			script_string_table strings(script_strings);
			std::vector<preload_move> moves;
			{
				zone_assets_writer collector(assets, strings, options, true);
				collector.write(ordered);
				moves = collector.preload_moves();
			}

			if (moves.empty())
			{
				zone_assets_writer writer(assets, strings, options, false);
				return writer.write(ordered);
			}

			if (round >= 64)
			{
				throw std::runtime_error(std::format("preloaded assets still nest 3 deep after listing their middle assets "
					"first ({})", moves.front().chain));
			}

			std::size_t moved = 0;
			for (const auto& move : moves)
			{
				const auto find = [&](void* header, const std::int32_t type)
				{
					return std::find_if(ordered.begin(), ordered.end(), [&](const zone_asset& a)
					{
						return a.header == header && (type < 0 || a.type == type);
					});
				};
				auto top = find(move.top, -1);
				auto middle = find(move.header, move.type);
				if (top == ordered.end() || (middle != ordered.end() && middle < top))
				{
					continue;  // already listed before it (a later move did it), or no list entry to put it before
				}
				zone_asset entry{move.type, move.header, false};
				if (middle != ordered.end())
				{
					entry = *middle;
					ordered.erase(middle);
					top = find(move.top, -1);
				}
				ordered.insert(top, entry);
				++moved;
			}

			if (!moved)
			{
				throw std::runtime_error(std::format("preloaded assets nest 3 deep inline ({}); the game holds 2 and "
					"crashes on load, and listing the middle asset first is not possible", moves.front().chain));
			}

			printf("[info] zone writer: %zu asset(s) listed before their users (Replay's asset stack holds 2 levels)\n", moved);
			for (const auto& move : moves)
			{
				printf("[info]   %s\n", move.chain.c_str());
			}
		}
	}

	written_zone write_zone(const loaded_zone& zone, const write_options& options)
	{
		return write_zone(zone, options, {});
	}

	written_zone write_zone(const loaded_zone& zone, const write_options& options,
		const std::span<const zone_asset> additional)
	{
		std::vector<zone_asset> list;
		list.reserve(zone.list.size());
		for (const auto& asset : zone.list)
		{
			list.emplace_back(asset.type, asset.header, asset.referenced);
		}

		std::vector<zone_asset> assets;
		assets.reserve(zone.assets.size());
		for (const auto& asset : zone.assets)
		{
			assets.emplace_back(asset.type, asset.header, asset.referenced);
		}

		if (!additional.empty())
		{
			if (additional.size() > std::numeric_limits<std::uint32_t>::max() - list.size())
			{
				throw std::runtime_error("appended assets exceed the native asset list count");
			}

			std::set<asset_key> keys;
			for (const auto& asset : assets)
			{
				keys.emplace(asset.type, name_of(asset.type, asset.header));
			}
			for (const auto& asset : list)
			{
				keys.emplace(asset.type, name_of(asset.type, asset.header));
			}
			for (const auto& asset : additional)
			{
				const auto format = find_asset_format(asset.type);
				if (!format || !asset.header || asset.referenced)
				{
					throw std::runtime_error("appended asset must have a native type and a full header");
				}
				const auto name = name_of(asset.type, asset.header);
				if (name.empty() || !keys.emplace(asset.type, name).second)
				{
					throw std::runtime_error(std::format("appended {} asset has an empty or duplicate name: {}", format->name, name));
				}
				list.push_back(asset);
				assets.push_back(asset);
			}
		}

		return write_zone(list, assets, zone.script_strings, options);
	}

	xfile::fastfile rewrite_fastfile(const std::string& name, xfile::fastfile file)
	{
		return rewrite_fastfile(name, std::move(file), {});
	}

	std::string zone_asset_name(const std::int32_t type, const void* header)
	{
		return name_of(type, header);
	}

	std::vector<zone_asset> asset_closure(const loaded_zone& zone, const std::span<const zone_asset> roots,
		std::vector<std::pair<std::int32_t, std::string>>* references)
	{
		// A scratch zone that lists only the roots, written with every definition of the source zone at hand: the
		// writer emits each asset at its first use, so what it emitted in full is the closure.
		std::vector<zone_asset> assets;
		std::map<asset_key, zone_asset> full;
		assets.reserve(zone.assets.size());
		for (const auto& asset : zone.assets)
		{
			assets.emplace_back(asset.type, asset.header, asset.referenced);
			if (!asset.referenced)
			{
				full.try_emplace(asset_key{asset.type, name_of(asset.type, asset.header)}, zone_asset{asset.type, asset.header, false});
			}
		}

		const std::vector<zone_asset> list(roots.begin(), roots.end());
		const auto written = write_zone(list, assets, zone.script_strings, {});

		std::vector<zone_asset> closure;
		std::set<asset_key> seen;
		for (const auto& [offset, type, name] : written.asset_offsets)
		{
			const asset_key key{type, name};
			const auto found = full.find(key);
			if (found != full.end() && seen.insert(key).second)
			{
				closure.push_back(found->second);
			}
			else if (found == full.end() && references && seen.insert(key).second)
			{
				references->push_back(key);
			}
		}
		return closure;
	}

	xfile::fastfile rewrite_fastfile(const std::string& name, xfile::fastfile file,
		const std::function<std::vector<zone_asset>(loaded_zone&)>& prepare, zone_inventory* inventory)
	{
		xfile::fastfile result{};
		result.xfile_version = file.xfile_version;
		result.transient = file.transient;
		result.transient_type = file.transient_type;
		const auto preload_walk_size = file.preload_walk_size;
		const auto expected_size = file.payload.size() + file.payload.size() / 16;

		auto zone = load_zone(name, std::move(file), {.keep_temp_data = true});
		const auto additional = prepare ? prepare(*zone) : std::vector<zone_asset>{};
		auto written = write_zone(*zone, {.expected_size = expected_size}, additional);
		zone.reset();

		result.payload = std::move(written.data);
		result.block_sizes = std::move(written.block_sizes);
		result.preload_walk_size = std::max<std::uint64_t>(written.preload_walk_size, preload_walk_size);
		if (inventory)
		{
			inventory->list_asset_count = written.list_asset_count;
			inventory->asset_offsets = std::move(written.asset_offsets);
		}
		return result;
	}

	xfile::fastfile rewrite_fastfile(const std::string& name, xfile::fastfile file,
		const std::span<const zone_asset> additional)
	{
		return rewrite_fastfile(name, std::move(file), additional, nullptr);
	}

	xfile::fastfile rewrite_fastfile(const std::string& name, xfile::fastfile file,
		const std::span<const zone_asset> additional, zone_inventory* inventory)
	{
		xfile::fastfile result{};
		result.xfile_version = file.xfile_version;
		result.transient = file.transient;
		result.transient_type = file.transient_type;
		const auto preload_walk_size = file.preload_walk_size;
		// The zone is written again at about its size: reserved, a 2 GB zone does not peak at twice that.
		const auto expected_size = file.payload.size() + file.payload.size() / 16;

		auto zone = load_zone(name, std::move(file), {.keep_temp_data = true});
		auto written = write_zone(*zone, {.expected_size = expected_size}, additional);
		zone.reset();

		result.payload = std::move(written.data);
		result.block_sizes = std::move(written.block_sizes);
		result.preload_walk_size = std::max<std::uint64_t>(written.preload_walk_size, preload_walk_size);
		if (inventory)
		{
			inventory->list_asset_count = written.list_asset_count;
			inventory->asset_offsets = std::move(written.asset_offsets);
		}
		return result;
	}
}
