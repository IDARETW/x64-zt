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

		struct xasset_entry
		{
			std::uint32_t type;
			std::uint32_t pad;
			void* header;
		};

		struct xasset_list
		{
			std::uint32_t string_count;
			std::uint32_t strings_loaded;
			const char** strings;
			std::uint32_t asset_count;
			std::uint32_t assets_loaded;
			xasset_entry* assets;
		};

		static_assert(sizeof(xasset_entry) == 16);
		static_assert(sizeof(xasset_list) == 32);

		std::uint16_t next_zone_index()
		{
			static std::atomic_uint16_t index{};
			return ++index;
		}

		class asset_list_reader
		{
		public:
			asset_list_reader(loaded_zone& zone)
				: zone_(zone)
				, reader_(*zone.reader)
			{
			}

			void read_inline_asset(const std::int32_t type, void** field, void** insert_slot)
			{
				const auto loader = find_asset_reader(type);
				if (!loader)
				{
					throw std::runtime_error(std::format("asset type {} has no reader", type));
				}

				this->reader_.align(loader->alignment);
				const auto data = this->reader_.read_bytes(loader->size);
				try
				{
					loader->read(this->reader_, data);
				}
				catch (const std::exception& e)
				{
					const auto name = *static_cast<const char**>(data);
					throw std::runtime_error(std::format("{} \"{}\": {}", loader->name,
						name && this->reader_.contains(name) ? name : "", e.what()));
				}

				// Block 1 rewinds after the asset: keep the header in reader memory.
				auto header = static_cast<void*>(this->reader_.allocate<std::uint8_t>(loader->size));
				std::memcpy(header, data, loader->size);

				const auto name = static_cast<const char**>(header);
				const auto referenced = *name && (*name)[0] == ',';
				if (referenced)
				{
					++*name;
				}

				const auto existing = *name ? db_find_asset(type, *name) : nullptr;
				if (referenced && existing)
				{
					header = existing;
				}
				else
				{
					this->replaced_assets_.emplace_back(type, *name ? *name : "", db_find_asset(type, *name ? *name : ""));
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
			std::vector<std::tuple<std::int32_t, std::string, void*>> replaced_assets_;
		};

		void read_asset_list(loaded_zone& zone, zone_reader& reader, std::ofstream* trace)
		{
			xasset_list list{};
			reader.read_raw(&list, sizeof(list));

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
					ids.push_back(SL_GetString(strings[i] ? strings[i] : ""));
				}
			}

			zone.script_string_count = ids.size();
			reader.set_script_strings(std::move(ids));

			if (list.assets)
			{
				reader.align(7);
				const auto assets = reader.read<xasset_entry>(list.asset_count);
				for (auto i = 0u; i < list.asset_count; i++)
				{
					if (trace)
					{
						*trace << "# asset " << i << " type " << assets[i].type << "\n";
					}

					reader.read_asset(static_cast<std::int32_t>(assets[i].type), assets[i].header);
				}
			}
			reader.pop_stream();

			if (reader.remaining())
			{
				throw std::runtime_error(std::format("0x{:X} bytes remain after the last asset", reader.remaining()));
			}
		}
	}

	const char* get_asset_name(const std::int32_t, const void* header)
	{
		return header ? *static_cast<const char* const*>(header) : "";
	}

	std::unique_ptr<loaded_zone> load_zone(const std::filesystem::path& path)
	{
		auto zone = std::make_unique<loaded_zone>();
		zone->name = path.stem().string();
		zone->file = xfile::read(path, definition().format);
		zone->reader = std::make_unique<zone_reader>(zone->file.payload, zone->file.block_sizes, layout);

		auto& reader = *zone->reader;
		reader.set_zone_index(next_zone_index());

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

		asset_list_reader assets(*zone);
		reader.set_inline_asset_loader([&assets](const std::int32_t type, void** field, void** insert_slot)
		{
			assets.read_inline_asset(type, field, insert_slot);
		});
		read_asset_list(*zone, reader, trace.get());
		reader.set_inline_asset_loader({});
		reader.set_read_trace({});

		return zone;
	}
}
