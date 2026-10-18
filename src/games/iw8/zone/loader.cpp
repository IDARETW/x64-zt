#include <std_include.hpp>
#include "../iw8.hpp"
#include "loader.hpp"

#include <game.hpp>

namespace zonetool::iw8
{
	const game& definition();

	namespace
	{
		// Replay 1.20 streams: block 1 holds asset headers and rewinds on pop, block 2 aligns to 32 bytes on
		// pop, block 4 is zero-filled and block 5 holds the asset bodies and the inserted pointers.
		const zone_layout layout
		{
			.offset_tag = 0,
			.following_markers = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE},
			.insert_marker = 0xFFFFFFFFFFFFFFFD,
			.runtime_block = 4,
			.virtual_block = 5,
			.pop_aligned_block = 2,
			.temp_block = 1,
			.pop_alignment = 31,
			.relative_offsets = true,
		};

		std::uint16_t next_zone_index()
		{
			static std::atomic_uint16_t index{};
			return ++index;
		}

		class asset_list_reader
		{
		public:
			asset_list_reader(loaded_zone& zone, const bool keep_markers)
				: zone_(zone)
				, reader_(*zone.reader)
				, keep_markers_(keep_markers)
			{
			}

			void read_inline_asset(const std::int32_t type, void** field, void** insert_slot)
			{
				const auto format = find_asset_format(type);
				if (!format)
				{
					throw std::runtime_error(std::format("asset type {} has no reader", type));
				}

				this->reader_.align(format->alignment);
				const auto first_alias = this->reader_.alias_count();
				const auto data = this->reader_.read_bytes(format->size);
				std::vector<std::pair<const void*, std::uint64_t>> markers;
				try
				{
					load_stream stream(this->reader_, this->keep_markers_ ? &markers : nullptr);
					format->load(stream, data);
				}
				catch (const std::exception& e)
				{
					const auto name = *static_cast<const char**>(data);
					throw std::runtime_error(std::format("{} \"{}\": {}", format->name,
						name && this->reader_.contains(name) ? name : "", e.what()));
				}

				// Block 1 rewinds after the asset: keep the header in reader memory.
				auto header = static_cast<void*>(this->reader_.allocate<std::uint8_t>(format->size));
				std::memcpy(header, data, format->size);
				this->reader_.move_aliases(data, format->size, header, first_alias);

				const auto start = static_cast<const std::uint8_t*>(data);
				for (const auto& [marked, value] : markers)
				{
					const auto address = static_cast<const std::uint8_t*>(marked);
					const auto in_header = address >= start && address < start + format->size;
					this->zone_.markers[in_header ? static_cast<std::uint8_t*>(header) + (address - start) : marked] = value;
				}

				const auto name = static_cast<const char**>(header);
				const auto referenced = *name && (*name)[0] == ',';
				if (referenced)
				{
					++*name;
				}

				// Each use of a referenced asset keeps its own stub: the database names the asset once.
				const auto existing = *name ? db_find_asset(type, *name) : nullptr;
				if (!referenced || !existing)
				{
					this->replaced_assets_.emplace_back(type, *name ? *name : "", existing);
					db_add_asset(type, *name ? *name : "", header);
				}

				this->zone_.assets.emplace_back(type, header, referenced);
				*field = header;
				if (insert_slot)
				{
					*insert_slot = header;
				}
			}

		private:
			loaded_zone& zone_;
			zone_reader& reader_;
			bool keep_markers_;
			std::vector<std::tuple<std::int32_t, std::string, void*>> replaced_assets_;
		};

		void read_asset_list(loaded_zone& zone, zone_reader& reader, std::ofstream* trace)
		{
			// The game reads the asset list header outside the blocks: zones that declare no block 1 load.
			xasset_list list{};
			reader.read_raw(&list, sizeof(list));
			zone.strings_marker = reinterpret_cast<std::uint64_t>(list.strings);
			zone.assets_marker = reinterpret_cast<std::uint64_t>(list.assets);

			reader.push_stream(layout.virtual_block);
			std::vector<std::uint32_t> ids;
			if (list.strings)
			{
				reader.align(7);
				const auto strings = reader.read<const char*>(list.string_count);
				ids.reserve(list.string_count);
				for (auto i = 0u; i < list.string_count; i++)
				{
					reader.read_string(strings[i]);
					ids.push_back(strings[i] ? SL_GetString(strings[i]) : 0);
					zone.script_strings.push_back(strings[i]);
				}
			}

			reader.set_script_strings(std::move(ids));

			if (list.assets)
			{
				reader.align(7);
				const auto assets = reader.read<xasset_entry>(list.asset_count);
				std::vector<std::uint64_t> markers;
				for (auto i = 0u; i < list.asset_count; i++)
				{
					if (trace)
					{
						*trace << "# asset " << i << " type " << assets[i].type << "\n";
					}

					markers.push_back(reinterpret_cast<std::uint64_t>(assets[i].header));
					reader.read_asset(static_cast<std::int32_t>(assets[i].type), assets[i].header);
				}

				std::unordered_map<const void*, bool> referenced;
				for (const auto& asset : zone.assets)
				{
					referenced.try_emplace(asset.header, asset.referenced);
				}

				for (auto i = 0u; i < list.asset_count; i++)
				{
					const auto found = referenced.find(assets[i].header);
					const auto marker = markers[i];
					zone.list.emplace_back(static_cast<std::int32_t>(assets[i].type), assets[i].header,
						found != referenced.end() && found->second, marker);
				}
			}
			reader.pop_stream();

			if (reader.remaining())
			{
				throw std::runtime_error(std::format("0x{:X} bytes remain after the last asset", reader.remaining()));
			}
		}
	}

	const zone_layout& replay_layout()
	{
		return layout;
	}

	const char* get_asset_name(const std::int32_t, const void* header)
	{
		return header ? *static_cast<const char* const*>(header) : "";
	}

	std::unique_ptr<loaded_zone> load_zone(const std::filesystem::path& path, const load_options& options)
	{
		return load_zone(path.stem().string(), xfile::read(path, definition().format), options);
	}

	std::unique_ptr<loaded_zone> load_zone(const std::string& name, xfile::fastfile file, const load_options& options)
	{
		auto zone = std::make_unique<loaded_zone>();
		zone->name = name;
		zone->file = std::move(file);
		zone->reader = std::make_unique<zone_reader>(zone->file.payload, zone->file.block_sizes, layout);

		auto& reader = *zone->reader;
		reader.set_zone_index(next_zone_index());
		if (options.keep_temp_data)
		{
			reader.keep_temp_data();
		}

		// IW8_READ_TRACE=<file> writes every read of zone data (input offset, size, block, position).
		std::unique_ptr<std::ofstream> trace;
		char trace_path[MAX_PATH]{};
		if (GetEnvironmentVariableA("IW8_READ_TRACE", trace_path, sizeof(trace_path)))
		{
			trace = std::make_unique<std::ofstream>(trace_path, std::ios::binary | std::ios::trunc);
			reader.set_read_trace([&trace](const std::size_t input, const std::size_t size, const std::uint8_t block,
				const std::size_t position)
			{
				*trace << input << ' ' << size << ' ' << static_cast<int>(block) << ' ' << position << '\n';
			});
		}

		asset_list_reader assets(*zone, options.keep_markers);
		reader.set_inline_asset_loader([&assets](const std::int32_t type, void** field, void** insert_slot)
		{
			assets.read_inline_asset(type, field, insert_slot);
		});
		read_asset_list(*zone, reader, trace.get());
		reader.resolve_aliases();
		reader.set_inline_asset_loader({});
		reader.set_read_trace({});

		return zone;
	}
}
