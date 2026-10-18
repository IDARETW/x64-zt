#include <std_include.hpp>
#include "iw8.hpp"
#include "havok/objects.hpp"
#include "havok/tagfile.hpp"
#include "zone/asset_files.hpp"
#include "zone/loader.hpp"
#include "zone/writer.hpp"

#include <game.hpp>

namespace zonetool::iw8
{
	const game& definition();

	namespace
	{
		using asset_key = std::pair<std::int32_t, std::string>;

		std::string asset_name(const std::int32_t type, const void* header)
		{
			const auto name = get_asset_name(type, header);
			return name ? name : "";
		}

		std::vector<zone_asset> to_zone_assets(const std::vector<loaded_asset>& assets)
		{
			std::vector<zone_asset> result;
			result.reserve(assets.size());
			for (const auto& asset : assets)
			{
				result.emplace_back(asset.type, asset.header, asset.referenced, asset.marker);
			}

			return result;
		}

		// The assets of a zone by type and name, with the references to assets that the zone does not hold. A zone can
		// hold an asset twice, and a writer can write it once: a reference to an asset of the zone uses that asset.
		std::set<std::tuple<std::int32_t, std::string, bool>> distinct_assets(const loaded_zone& zone)
		{
			std::set<std::pair<std::int32_t, std::string>> held;
			for (const auto& asset : zone.assets)
			{
				const auto name = get_asset_name(asset.type, asset.header);
				if (!asset.referenced)
				{
					held.emplace(asset.type, name ? name : "");
				}
			}

			std::set<std::tuple<std::int32_t, std::string, bool>> assets;
			for (const auto& asset : zone.assets)
			{
				const std::string name = get_asset_name(asset.type, asset.header) ? get_asset_name(asset.type, asset.header) : "";
				assets.emplace(asset.type, name, asset.referenced && !held.contains({asset.type, name}));
			}

			return assets;
		}

		// IW8_REWRITE_OUT=<folder> also writes the zone there, to load it with the game loaders.
		void save_written_zone(const loaded_zone& zone, const written_zone& written)
		{
			char output[MAX_PATH]{};
			if (!GetEnvironmentVariableA("IW8_REWRITE_OUT", output, sizeof(output)))
			{
				return;
			}

			xfile::fastfile file{};
			file.payload = written.data;
			file.block_sizes = written.block_sizes;
			file.xfile_version = zone.file.xfile_version;
			file.transient = zone.file.transient;
			file.preload_walk_size = std::max(written.preload_walk_size, zone.file.preload_walk_size);
			xfile::write(std::filesystem::path(output) / (zone.name + ".ff"), definition().format, file);
		}

		// IW8_REWRITE=fresh writes the zone with the choices of the writer, loads the result and compares the assets.
		void verify_fresh_write(const loaded_zone& zone)
		{
			const auto list = to_zone_assets(zone.list);
			const auto assets = to_zone_assets(zone.assets);
			const auto written = write_zone(list, assets, zone.script_strings);
			save_written_zone(zone, written);

			xfile::fastfile file{};
			file.payload = written.data;
			file.block_sizes = written.block_sizes;
			const auto reloaded = load_zone(zone.name, std::move(file));

			const auto written_assets = distinct_assets(*reloaded);
			const auto zone_assets = distinct_assets(zone);
			if (written_assets != zone_assets || reloaded->list.size() != zone.list.size())
			{
				throw std::runtime_error(std::format("the written zone holds other assets: {} of {} assets, {} of {} list entries",
					written_assets.size(), zone_assets.size(), reloaded->list.size(), zone.list.size()));
			}

			ZONETOOL_INFO("%s: the written zone loads with the same assets (0x%zX of 0x%zX bytes)", zone.name.data(),
				written.data.size(), zone.file.payload.size());
		}

		std::map<asset_key, asset_file> dump_assets(const loaded_zone& zone)
		{
			std::map<asset_key, asset_file> dumps;
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (!asset.referenced && !dumps.contains(key))
				{
					dumps.emplace(key, dump_asset_file(asset.type, asset.header));
				}
			}

			return dumps;
		}

		// IW8_REWRITE=dump dumps each asset, parses the dumps, writes them as a zone, loads it and dumps it again.
		void verify_dump_round_trip(const loaded_zone& zone)
		{
			const auto dumps = dump_assets(zone);

			asset_parser parser;
			std::map<asset_key, void*> parsed;
			std::vector<zone_asset> assets;
			std::size_t shared{};
			for (const auto& [key, file] : dumps)
			{
				const auto header = parser.parse_asset_file(key.first, key.second, file);
				parsed.emplace(key, header);
				assets.emplace_back(key.first, header, false);
				shared += file.shared;

				// An asset of a type with a shared file that the shared file does not hold.
				if (!file.shared && asset_file_locations(key.first, key.second).size() > 1)
				{
					ZONETOOL_INFO("%s: %s", zone.name.data(), file.path.data());
				}
			}

			std::vector<zone_asset> list;
			for (const auto& entry : zone.list)
			{
				const asset_key key{entry.type, asset_name(entry.type, entry.header)};
				const auto found = parsed.find(key);
				list.emplace_back(entry.type, found != parsed.end() ? found->second : parser.stub(entry.type, key.second),
					found == parsed.end());
			}

			const std::array<const char*, 1> strings{nullptr};
			const auto written = write_zone(list, assets, strings);

			xfile::fastfile fastfile{};
			fastfile.payload = written.data;
			fastfile.block_sizes = written.block_sizes;
			const auto reloaded = load_zone(zone.name, std::move(fastfile), {.keep_temp_data = true});
			if (distinct_assets(*reloaded) != distinct_assets(zone))
			{
				throw std::runtime_error("the zone that the dumps make holds other assets");
			}

			const auto again = dump_assets(*reloaded);
			for (const auto& [key, file] : dumps)
			{
				const auto found = again.find(key);
				if (found == again.end() || found->second.path != file.path || found->second.data != file.data)
				{
					char debug[MAX_PATH]{};
					if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
					{
						std::ofstream(std::filesystem::path(debug) / "first", std::ios::binary).write(file.data.data(), static_cast<std::streamsize>(file.data.size()));
						if (found != again.end())
						{
							std::ofstream(std::filesystem::path(debug) / "again", std::ios::binary).write(found->second.data.data(), static_cast<std::streamsize>(found->second.data.size()));
						}
					}

					throw std::runtime_error(std::format("{} dumps differently from the zone that its dump makes", file.path));
				}
			}

			ZONETOOL_INFO("%s: %zu assets (%zu in shared files) dump, parse and write again to the same files", zone.name.data(),
				dumps.size(), shared);
		}

		// IW8_REWRITE=1 writes the zone again from the loaded assets and compares it with the zone data.
		void verify_rewrite(const loaded_zone& zone)
		{
			const auto list = to_zone_assets(zone.list);
			const auto assets = to_zone_assets(zone.assets);
			const auto written = write_zone(list, assets, zone.script_strings, {.markers = &zone.markers,
				.strings_marker = zone.strings_marker, .assets_marker = zone.assets_marker});

			save_written_zone(zone, written);

			const auto& original = zone.file.payload;
			const auto size = std::min(original.size(), written.data.size());
			const auto difference = std::mismatch(original.begin(), original.begin() + size, written.data.begin());
			const auto offset = static_cast<std::size_t>(difference.first - original.begin());

			std::string blocks;
			for (std::size_t i = 0; i < written.block_sizes.size(); i++)
			{
				if (written.block_sizes[i] != zone.file.block_sizes[i])
				{
					blocks += std::format(" {}:0x{:X}/0x{:X}", i, written.block_sizes[i], zone.file.block_sizes[i]);
				}
			}

			if (offset == size && original.size() == written.data.size())
			{
				// Stock block sizes add slack to the peaks that the writer computes (decal volume materials).
				ZONETOOL_INFO("%s: the written zone data is equal to the zone data; block sizes (written/zone)%s",
					zone.name.data(), blocks.empty() ? " equal" : blocks.data());
				return;
			}

			const auto bytes_at = [offset](const std::string& data)
			{
				std::string text;
				for (auto i = offset & ~std::size_t(15); i < std::min(data.size(), (offset & ~std::size_t(15)) + 32); i++)
				{
					text += std::format("{:02X}", static_cast<std::uint8_t>(data[i]));
				}

				return text;
			};

			std::string asset = "the asset list";
			for (const auto& [start, type, name] : written.asset_offsets)
			{
				if (start > offset)
				{
					break;
				}

				const auto format = find_asset_format(type);
				asset = std::format("{} \"{}\" at 0x{:X}", format ? format->name : "?", name, start);
			}

			throw std::runtime_error(std::format("the written zone differs: 0x{:X} bytes, zone data 0x{:X} bytes, first "
				"difference at 0x{:X} in {} (written {} zone {}); block sizes (written/zone){}", written.data.size(),
				original.size(), offset, asset, bytes_at(written.data), bytes_at(original), blocks.empty() ? " equal" : blocks));
		}

		// The Replay global Havok type compendium: the code_post_gfx physics library physicsglobaltypecompendium.
		const havok::type_table& replay_compendium(const std::filesystem::path& zone_folder)
		{
			static const auto compendium = [&zone_folder]
			{
				const auto zone = load_zone(zone_folder / "code_post_gfx.ff");
				for (const auto& asset : zone->assets)
				{
					const auto& library = *static_cast<const PhysicsLibrary*>(asset.header);
					if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && !asset.referenced && library.isGlobalTypeCompendium)
					{
						return havok::type_table::read({reinterpret_cast<const std::uint8_t*>(library.havokData), library.havokDataSize});
					}
				}

				throw std::runtime_error("code_post_gfx has no Havok type compendium");
			}();

			return compendium;
		}

		// The Havok tagfiles in the data records of an asset dump.
		std::vector<std::string_view> tagfile_records(const std::string_view dump)
		{
			const auto read = [&dump](const std::size_t offset, auto& value)
			{
				if (offset + sizeof(value) > dump.size())
				{
					throw std::runtime_error("the asset dump is truncated");
				}

				std::memcpy(&value, dump.data() + offset, sizeof(value));
			};

			std::uint32_t count{};
			read(12, count);
			std::size_t offset = 16;
			std::vector<std::string_view> tagfiles;
			for (std::uint32_t i = 0; i < count; i++)
			{
				std::uint8_t kind{};
				read(offset++, kind);
				switch (static_cast<dump_record>(kind))
				{
				case dump_record::data:
				case dump_record::string:
				case dump_record::asset:
				case dump_record::script_string:
				{
					std::uint64_t size{};
					read(offset + 8, size);
					const auto bytes = dump.substr(offset + 16, size);
					if (static_cast<dump_record>(kind) == dump_record::data && bytes.size() > 8 && bytes.substr(4, 4) == "TAG0")
					{
						tagfiles.push_back(bytes);
					}

					offset += 16 + size;
					break;
				}
				case dump_record::runtime:
				case dump_record::temp:
					offset += 8;
					break;
				case dump_record::offset:
					offset += 16;
					break;
				default:
					break;
				}
			}

			return tagfiles;
		}

		// IW8_REWRITE=havok reads the Havok tagfiles of each asset and writes them again. Compact tagfiles must give the
		// same bytes, and self-describing ones the same items when they are read again.
		void verify_havok(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			std::set<asset_key> seen;
			std::size_t compact{};
			std::size_t self_describing{};
			std::size_t other{};
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				// The compendium holds types, not objects.
				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					const auto format = find_asset_format(asset.type);
					const auto fail = [&](const std::string& what, const std::span<const std::uint8_t> written = {})
					{
						char debug[MAX_PATH]{};
						if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
						{
							std::ofstream(std::filesystem::path(debug) / "havok_first.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
							std::ofstream(std::filesystem::path(debug) / "havok_again.hkx", std::ios::binary).write(reinterpret_cast<const char*>(written.data()), static_cast<std::streamsize>(written.size()));
						}

						return std::runtime_error(std::format("{} \"{}\": {}", format ? format->name : "?", key.second, what));
					};

					// Tagfiles of other type compendiums (cloth) name types that this compendium does not have.
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						other++;
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						other++;
						continue;
					}
					catch (const std::exception& e)
					{
						throw fail(e.what());
					}

					if (file.compendium)
					{
						const auto written = havok::write_compact_tagfile(file, compendium);
						if (!std::ranges::equal(written, bytes))
						{
							throw fail("the compact tagfile is written differently", written);
						}

						compact++;
						continue;
					}

					const auto written = havok::write_self_describing_tagfile(file, compendium);
					const auto again = havok::read_tagfile(written, &compendium);
					const auto same_items = std::ranges::equal(again.items, file.items, [](const havok::item& a, const havok::item& b)
					{
						return a.type == b.type && a.flags == b.flags && a.count == b.count && a.data == b.data &&
							std::ranges::equal(a.pointers, b.pointers, [](const havok::pointer_field& x, const havok::pointer_field& y)
							{
								return x.offset == y.offset && x.pointer_type == y.pointer_type && x.target == y.target;
							});
					});

					if (!same_items)
					{
						throw fail("the self-describing tagfile reads back with other items", written);
					}

					self_describing++;
				}
			}

			ZONETOOL_INFO("%s: %zu compact and %zu self-describing Havok tagfiles write again (%zu with other types)", zone.name.data(),
				compact, self_describing, other);
		}

		// IW8_REWRITE=convex builds each convex polytope shape of the zone again from its vertices. The two hulls must hold
		// each other's vertices, and the built shape must write and read back as the same polytope.
		void verify_convex(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			const auto convex_type = *compendium.find("hknpConvexPolytopeShape");
			std::set<asset_key> seen;
			std::size_t shapes{};
			std::size_t same_counts{};
			double worst{};
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						continue;
					}

					for (std::uint32_t index = 1; index < file.items.size(); index++)
					{
						if (file.items[index].type != convex_type)
						{
							continue;
						}

						const auto original = havok::read_convex_polytope_shape(file, compendium, index);
						havok::polytope built;
						try
						{
							built = havok::convex_hull(original.vertices, 0.0f);
						}
						catch (const std::exception&)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream(std::filesystem::path(debug) / "convex.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
								std::ofstream(std::filesystem::path(debug) / "convex_item.txt") << index;
							}

							throw;
						}

						// Each hull holds the vertices of the other.
						const auto outside = [](const havok::polytope& hull, const std::vector<havok::vector3>& points)
						{
							double result{};
							for (const auto& plane : hull.planes)
							{
								for (const auto& p : points)
								{
									result = std::max(result, static_cast<double>(plane[0]) * p[0] + static_cast<double>(plane[1]) * p[1] +
										static_cast<double>(plane[2]) * p[2] + plane[3]);
								}
							}

							return result;
						};

						const auto distance = std::max(outside(original, built.vertices), outside(built, original.vertices));
						worst = std::max(worst, distance);
						if (distance > 1e-3)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream(std::filesystem::path(debug) / "convex.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
								std::ofstream text(std::filesystem::path(debug) / "convex.txt");
								for (const auto& v : original.vertices)
								{
									text << std::format("v {} {} {}\n", v[0], v[1], v[2]);
								}

								for (std::size_t f = 0; f < built.faces.size(); f++)
								{
									text << "f";
									for (const auto v : built.faces[f])
									{
										text << ' ' << v;
									}

									text << std::format(" | {} {} {} {}\n", built.planes[f][0], built.planes[f][1], built.planes[f][2], built.planes[f][3]);
								}

								for (const auto& v : built.vertices)
								{
									text << std::format("b {} {} {}\n", v[0], v[1], v[2]);
								}
							}

							const auto format = find_asset_format(asset.type);
							throw std::runtime_error(std::format("{} \"{}\": convex shape {} differs by {:.6f}", format ? format->name : "?",
								key.second, index, distance));
						}

						same_counts += built.vertices.size() == original.vertices.size() && built.faces.size() == original.faces.size();

						havok::object_builder builder(compendium);
						const auto shape = havok::add_convex_polytope_shape(builder, built);
						const auto written = havok::write_compact_tagfile(builder.finish(file.sdk_version, havok::replay_compendium_signature), compendium);
						const auto again = havok::read_tagfile(written, &compendium);
						const auto read = havok::read_convex_polytope_shape(again, compendium, 1);
						if (shape != 1 || read.vertices != built.vertices || read.faces != built.faces || read.planes != built.planes)
						{
							throw std::runtime_error("a built convex shape reads back differently");
						}

						shapes++;
					}
				}
			}

			ZONETOOL_INFO("%s: %zu convex shapes build again (%zu with the same vertex and face counts), largest distance %.6f",
				zone.name.data(), shapes, same_counts, worst);
		}

		template <typename T>
		T item_value(const havok::item& item, const std::size_t offset)
		{
			if (offset + sizeof(T) > item.data.size())
			{
				throw std::runtime_error("a Havok item is shorter than its members");
			}

			T value{};
			std::memcpy(&value, item.data.data() + offset, sizeof(T));
			return value;
		}

		std::uint32_t item_target(const havok::item& item, const std::uint32_t offset)
		{
			const auto found = std::ranges::find(item.pointers, offset, &havok::pointer_field::offset);
			return found != item.pointers.end() ? found->target : 0;
		}

		// The instances that the leaves of a SIMD tree hold, leaf by leaf from the left. With check_bounds, each inner slot must
		// hold the bounds of its child node.
		std::vector<std::uint32_t> tree_instances(const havok::item& nodes, const bool check_bounds)
		{
			const auto bound = [&](const std::uint32_t node, const std::size_t side, const std::size_t slot)
			{
				return item_value<float>(nodes, node * 128 + side * 16 + slot * 4);
			};

			const auto is_leaf = [&](const std::uint32_t node)
			{
				return item_value<std::uint8_t>(nodes, node * 128 + 112) != 0;
			};

			// The used slots of a node.
			const auto slots = [&](const std::uint32_t node)
			{
				std::vector<std::pair<std::size_t, std::uint32_t>> used;
				for (std::size_t slot = 0; slot < 4; slot++)
				{
					const auto data = item_value<std::uint32_t>(nodes, node * 128 + 96 + slot * 4);
					if (is_leaf(node) ? data != 0xFFFFFFFF : data != 0)
					{
						used.emplace_back(slot, data);
					}
				}

				return used;
			};

			std::vector<std::uint32_t> instances;
			std::vector<std::uint32_t> pending{1};
			while (!pending.empty())
			{
				const auto node = pending.back();
				pending.pop_back();
				if (node == 0 || node >= nodes.count)
				{
					throw std::runtime_error("a SIMD tree node is outside the tree");
				}

				const auto used = slots(node);
				if (is_leaf(node))
				{
					for (const auto& [slot, instance] : used)
					{
						instances.push_back(instance);
					}

					continue;
				}

				for (auto entry = used.rbegin(); entry != used.rend(); ++entry)
				{
					const auto [slot, child] = *entry;
					if (child >= nodes.count)
					{
						throw std::runtime_error("a SIMD tree node is outside the tree");
					}

					for (std::size_t side = 0; side < 6 && check_bounds; side++)
					{
						std::optional<float> expected;
						for (const auto& [child_slot, data] : slots(child))
						{
							const auto value = bound(child, side, child_slot);
							expected = !expected ? value : side % 2 ? std::max(*expected, value) : std::min(*expected, value);
						}

						if (expected != bound(node, side, slot))
						{
							throw std::runtime_error("a SIMD tree node does not hold the bounds of its child");
						}
					}

					pending.push_back(child);
				}
			}

			return instances;
		}

		// IW8_REWRITE=compound builds each compound shape of the zone again from its instances and child shapes. The built
		// shape must have the same header, instances, bounds and radius, and a tree that holds each instance once.
		void verify_compound(const std::filesystem::path& zone_folder, const loaded_zone& zone)
		{
			const auto& compendium = replay_compendium(zone_folder);
			const auto compound_type = *compendium.find("hknpCompoundShape");
			const auto convex_type = *compendium.find("hknpConvexShape");
			const auto polytope_type = *compendium.find("hknpConvexPolytopeShape");
			const auto offset_of = [&](const std::uint32_t type, const std::string_view name)
			{
				const auto found = compendium.member(type, name);
				if (!found)
				{
					throw std::runtime_error(std::format("{} has no member {}", compendium.identity(type), name));
				}

				return found->first;
			};

			// Capsules and spheres are convex shapes with a radius about their vertices, and the Replay tools give them
			// larger bounds than the radius (0.0314788 for a radius of 0.03125).
			const auto is_polytope = [&](std::uint32_t type)
			{
				for (std::size_t depth = 0; type && depth < 64; depth++)
				{
					if (type == polytope_type)
					{
						return true;
					}

					type = compendium.at(type).parent;
				}

				return false;
			};

			const auto elements_at = offset_of(compound_type, "instances.elements");
			const auto aabb_at = offset_of(compound_type, "aabb");
			const auto radius_at = offset_of(compound_type, "boundingRadius");
			const auto volume_type_at = offset_of(compound_type, "boundingVolumeData.type");
			const auto nodes_at = offset_of(compound_type, "boundingVolumeData.simdTree.nodes");
			const auto points_at = offset_of(compound_type, "boundingVolumeData.simdTree.points");
			const auto vertices_at = offset_of(convex_type, "vertices");
			const auto convex_radius_at = offset_of(convex_type, "convexRadius");

			std::set<asset_key> seen;
			std::size_t shapes{};
			std::size_t other_children{};
			std::size_t other_volumes{};
			std::size_t same_trees{};
			std::size_t same_orders{};
			std::size_t same_bounds{};
			double worst_bounds{};
			double worst_radius{};
			for (const auto& asset : zone.assets)
			{
				const asset_key key{asset.type, asset_name(asset.type, asset.header)};
				if (asset.referenced || !seen.emplace(key).second)
				{
					continue;
				}

				if (asset.type == ASSET_TYPE_PHYSICSLIBRARY && static_cast<const PhysicsLibrary*>(asset.header)->isGlobalTypeCompendium)
				{
					continue;
				}

				const auto dump = dump_asset_data(asset.type, asset.header);
				for (const auto record : tagfile_records(dump))
				{
					const std::span bytes(reinterpret_cast<const std::uint8_t*>(record.data()), record.size());
					if (const auto reference = havok::compendium_reference(bytes); reference && *reference != havok::replay_compendium_signature)
					{
						continue;
					}

					havok::tagfile file;
					try
					{
						file = havok::read_tagfile(bytes, &compendium);
					}
					catch (const havok::unknown_type&)
					{
						continue;
					}

					for (std::uint32_t index = 1; index < file.items.size(); index++)
					{
						const auto& stock = file.items[index];
						if (stock.type != compound_type)
						{
							continue;
						}

						const auto format = find_asset_format(asset.type);
						const auto fail = [&](const std::string& what)
						{
							char debug[MAX_PATH]{};
							if (GetEnvironmentVariableA("IW8_DUMP_DEBUG", debug, sizeof(debug)))
							{
								std::ofstream(std::filesystem::path(debug) / "compound.hkx", std::ios::binary).write(record.data(), static_cast<std::streamsize>(record.size()));
								std::ofstream(std::filesystem::path(debug) / "compound_item.txt") << index;
							}

							return std::runtime_error(std::format("{} \"{}\": compound shape {}: {}", format ? format->name : "?", key.second, index, what));
						};

						if (item_value<std::uint8_t>(stock, volume_type_at) != 2)
						{
							other_volumes++;
							continue;
						}

						const auto& elements = file.items.at(item_target(stock, elements_at));
						havok::object_builder builder(compendium);
						std::unordered_map<std::uint32_t, std::uint32_t> copies;
						std::vector<havok::compound_instance> instances;
						auto convex_children = true;
						for (std::uint32_t i = 0; i < elements.count && convex_children; i++)
						{
							const auto child_index = item_target(elements, i * 112 + 80);
							const auto& child = file.items.at(child_index);
							if (!is_polytope(child.type))
							{
								convex_children = false;
								break;
							}

							havok::compound_instance instance;
							for (std::size_t column = 0; column < 3; column++)
							{
								for (std::size_t axis = 0; axis < 3; axis++)
								{
									instance.rotation[column][axis] = item_value<float>(elements, i * 112 + column * 16 + axis * 4);
								}
							}

							for (std::size_t axis = 0; axis < 3; axis++)
							{
								instance.translation[axis] = item_value<float>(elements, i * 112 + 48 + axis * 4);
							}

							instance.shape_tag = item_value<std::uint16_t>(elements, i * 112 + 88);
							instance.convex_radius = item_value<float>(child, convex_radius_at);
							const auto& vertices = file.items.at(item_target(child, vertices_at));
							for (std::uint32_t v = 0; v < vertices.count; v++)
							{
								instance.vertices.push_back({item_value<float>(vertices, v * 16), item_value<float>(vertices, v * 16 + 4),
									item_value<float>(vertices, v * 16 + 8)});
							}

							instance.shape = havok::copy_item(builder, file, child_index, copies);
							instances.push_back(std::move(instance));
						}

						if (!convex_children)
						{
							other_children++;
							continue;
						}

						const auto shape = havok::add_compound_shape(builder, instances);
						const auto built = builder.finish(file.sdk_version, havok::replay_compendium_signature, shape);
						const auto& ours = built.items[1];

						// flags, type, numShapeKeyBits, dispatchType, convexRadius, userData; shapeTagCodecInfo; instances
						// firstFree and numAllocated; isMutable, estimatedNumShapeKeys; isCompact and the volume type.
						for (const auto [begin, end] : {std::pair(24, 48), std::pair(56, 60), std::pair(88, 96), std::pair(148, 156),
							std::pair(256, 257), std::pair(280, 281)})
						{
							if (!std::equal(ours.data.begin() + begin, ours.data.begin() + end, stock.data.begin() + begin))
							{
								throw fail(std::format("bytes 0x{:X}-0x{:X} differ", begin, end));
							}
						}

						const auto& our_elements = built.items.at(item_target(ours, elements_at));
						for (std::uint32_t i = 0; i < elements.count; i++)
						{
							// All but the shape pointer and the tree position in the w of the translation.
							for (const auto [begin, end] : {std::pair(0u, 60u), std::pair(64u, 80u), std::pair(88u, 112u)})
							{
								if (!std::equal(our_elements.data.begin() + i * 112 + begin, our_elements.data.begin() + i * 112 + end,
									elements.data.begin() + i * 112 + begin))
								{
									throw fail(std::format("instance {} differs in bytes 0x{:X}-0x{:X}", i, begin, end));
								}
							}
						}

						auto bounds_equal = true;
						for (std::size_t k = 0; k < 8; k++)
						{
							const auto a = item_value<float>(ours, aabb_at + k * 4);
							const auto b = item_value<float>(stock, aabb_at + k * 4);
							bounds_equal &= a == b;
							worst_bounds = std::max(worst_bounds, static_cast<double>(std::abs(a - b)));
						}

						same_bounds += bounds_equal;
						const auto radius = item_value<float>(stock, radius_at);
						const auto radius_error = std::abs(static_cast<double>(item_value<float>(ours, radius_at)) - radius) / std::max(radius, 1e-6f);
						worst_radius = std::max(worst_radius, radius_error);
						if (worst_bounds > 1e-4 || radius_error > 1e-5)
						{
							throw fail(std::format("the bounds differ by {:.7f}, the radius by {:.7f} of it", worst_bounds, radius_error));
						}

						const auto& our_nodes = built.items.at(item_target(ours, nodes_at));
						const auto& stock_nodes = file.items.at(item_target(stock, nodes_at));
						const auto our_order = tree_instances(our_nodes, true);
						const auto stock_order = tree_instances(stock_nodes, false);
						auto held = our_order;
						std::ranges::sort(held);
						if (held.size() != elements.count || std::ranges::adjacent_find(held) != held.end() || held.back() >= elements.count)
						{
							throw fail("the tree does not hold each instance once");
						}

						same_orders += our_order == stock_order;
						same_trees += our_nodes.data == stock_nodes.data &&
							built.items.at(item_target(ours, points_at)).data == file.items.at(item_target(stock, points_at)).data;

						const auto written = havok::write_compact_tagfile(built, compendium);
						if (havok::write_compact_tagfile(havok::read_tagfile(written, &compendium), compendium) != written)
						{
							throw fail("the built shape reads back differently");
						}

						shapes++;
					}
				}
			}

			ZONETOOL_INFO("%s: %zu compound shapes build again (%zu with the same tree, %zu the same leaf order, %zu the same bounds); "
				"largest bounds difference %.7f, radius %.7f; %zu with other children, %zu with other trees", zone.name.data(), shapes, same_trees,
				same_orders, same_bounds, worst_bounds, worst_radius, other_children, other_volumes);
		}

		// The asset list of the zone: <pool>,<name> for an asset, <pool>,,<name> for a reference.
		void write_asset_csv(const loaded_zone& zone)
		{
			std::string csv;
			for (const auto& entry : zone.list)
			{
				const auto format = find_asset_format(entry.type);
				csv += std::format("{},{}{}\n", format ? format->name : std::to_string(entry.type), entry.referenced ? "," : "",
					asset_name(entry.type, entry.header));
			}

			filesystem::file file(zone.name + ".csv");
			file.open("wb");
			file.write(csv.data(), csv.size(), 1);
			file.close();
		}
	}

	void dump_zone(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path, const asset_dumper converter)
	{
		const auto _ = gsl::finally(db_clear);

		filesystem::set_zone_folder(zone_folder);
		filesystem::set_fastfile(zone_path.stem().string());
		ZONETOOL_INFO("Loading zone \"%s\"...", zone_path.stem().string().data());
		const auto zone = load_zone(zone_path, {.keep_temp_data = true});

		std::set<asset_key> dumped;
		for (const auto& asset : zone->assets)
		{
			const asset_key key{asset.type, asset_name(asset.type, asset.header)};
			if (asset.referenced || !dumped.emplace(key).second)
			{
				continue;
			}

			try
			{
				if (converter)
				{
					converter(asset.type, asset.header);
				}
				else
				{
					dump_asset(asset.type, asset.header);
				}
			}
			catch (const std::exception& e)
			{
				const auto format = find_asset_format(asset.type);
				ZONETOOL_ERROR("Failed to dump %s \"%s\": %s", format ? format->name : "?", key.second.data(), e.what());
			}
		}

		write_asset_csv(*zone);
		ZONETOOL_INFO("Zone \"%s\" dumped to %s", zone->name.data(), filesystem::get_dump_path().data());
	}

	void verify_zone(const std::filesystem::path& zone_folder, const std::filesystem::path& zone_path)
	{
		const auto _ = gsl::finally(db_clear);

		char rewrite[16]{};
		const auto check_rewrite = GetEnvironmentVariableA("IW8_REWRITE", rewrite, sizeof(rewrite)) != 0;
		const auto zone = load_zone(zone_path, {.keep_temp_data = check_rewrite, .keep_markers = check_rewrite});
		std::map<std::string, std::size_t> counts;
		for (const auto& asset : zone->assets)
		{
			const auto format = find_asset_format(asset.type);
			++counts[format ? format->name : std::to_string(asset.type)];
		}

		ZONETOOL_INFO("%s: %zu assets, %zu script strings, all bytes read", zone->name.data(), zone->assets.size(),
			zone->script_strings.size());
		for (const auto& [name, count] : counts)
		{
			ZONETOOL_INFO("  %s: %zu", name.data(), count);
		}

		if (check_rewrite)
		{
			if (std::string_view(rewrite) == "fresh")
			{
				verify_fresh_write(*zone);
			}
			else if (std::string_view(rewrite) == "dump")
			{
				verify_dump_round_trip(*zone);
			}
			else if (std::string_view(rewrite) == "havok")
			{
				verify_havok(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "convex")
			{
				verify_convex(zone_folder, *zone);
			}
			else if (std::string_view(rewrite) == "compound")
			{
				verify_compound(zone_folder, *zone);
			}
			else
			{
				verify_rewrite(*zone);
			}
		}
	}
}
