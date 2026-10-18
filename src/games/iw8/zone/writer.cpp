#include <std_include.hpp>
#include "../iw8.hpp"
#include "loader.hpp"
#include "writer.hpp"

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
						this->write_asset(s, type, field);
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
				this->writer_.push_stream(write_stream::temp_block);
				this->writer_.write_bytes(&header, sizeof(header));

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
						this->write_list_asset(stream, entry);
					}
				}

				this->writer_.pop_stream();
				this->writer_.pop_stream();

				written_zone zone{};
				zone.data = this->writer_.data();
				zone.block_sizes = this->writer_.block_sizes();
				// The block 2 size is the peak of the map preload. Stock zones declare 0x40 more.
				zone.block_sizes[preload_block] += 0x40;
				// The preload plan records [block u8][int32 delta] for each of 8 moved blocks and a terminator per
				// asset of the list (see vault re/replay-120-postload-plan).
				zone.preload_walk_size = 41 * entries.size() + 16;
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

			void write_asset(write_stream& stream, const std::int32_t type, void** field, const bool insert_by_default = true)
			{
				auto& writer = stream.writer();
				const auto header = *field;
				const asset_key key{type, name_of(type, header)};

				// A kept marker writes the asset again, as the zone did; a kept offset points where the zone did.
				const auto kept = this->marker_of(field);
				if (kept && writer.is_offset(*kept))
				{
					writer.set_pointer(field, *kept);
					return;
				}

				if (const auto alias = this->aliases_.find(key); alias != this->aliases_.end() && !kept)
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
				const auto stub = !this->headers_.contains(header) && (this->references_.contains(header) ||
					!this->assets_.contains(key));
				const auto remember = [&](const std::size_t position)
				{
					if (stub)
					{
						this->aliases_.try_emplace(key, position);
					}
					else
					{
						this->aliases_.insert_or_assign(key, position);
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
				if (!this->headers_.contains(header))
				{
					const auto full = this->references_.contains(header) ? this->assets_.end() : this->assets_.find(key);
					source = full != this->assets_.end() ? full->second : this->stub(*format, header, key.second);
				}

				writer.align(format->alignment);
				this->asset_offsets_.emplace_back(writer.data_size(), type, key.second);
				writer.write_copy(source, format->size);
				try
				{
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
			}

			void* stub(const asset_format& format, const void* header, const std::string& name)
			{
				auto& data = this->stubs_.emplace_back(format.size);
				std::memcpy(data.data(), header, format.size);
				this->stub_sources_[reinterpret_cast<std::uintptr_t>(data.data())] = {format.size, static_cast<const std::uint8_t*>(header)};

				// A loaded stub names the asset past its ',': keep the zone string, so that sharing stays the same.
				const auto loaded_name = *static_cast<const char* const*>(header);
				if (this->references_.contains(header) && loaded_name && loaded_name[-1] == ',')
				{
					*reinterpret_cast<const char**>(data.data()) = loaded_name - 1;
					return data.data();
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
		script_string_table strings(script_strings);
		{
			zone_assets_writer collector(assets, strings, options, true);
			collector.write(list);
		}

		zone_assets_writer writer(assets, strings, options, false);
		return writer.write(list);
	}
}
