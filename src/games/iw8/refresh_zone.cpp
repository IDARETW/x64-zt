#include <std_include.hpp>
#include "iw8.hpp"
#include "zone/loader.hpp"
#include "zone/writer.hpp"

#include <game.hpp>

// The live refresh zone of a map package, which the DW client's map_refresh loads into a running match.
//
// Replay replaces an asset in place when a later zone links one of the same name and type (DB_LinkXAssetEntry
// 0x1411AC760, DB_ReplaceLiveHeadAssetEntries 0x1411AF9B0, DB_ReplaceHeadAssetEntry 0x1411AEC50): the new asset moves
// into the header that every reference of the level holds. The zone therefore holds each changed asset and writes
// each use of another asset as a name stub, which the game resolves to that live header.
//
// A replaced asset loses the deployed original in the match. A later refresh zone unloads the earlier one first,
// which leaves the replaced assets as default stubs until the new zone fills them, so each refresh zone also holds
// every asset that the previous one named (its manifest).
namespace zonetool::iw8
{
	const game& definition();

	namespace
	{
		constexpr std::string_view manifest_magic = "x64-zt refresh 1";

		// The types that the native replacement fixes up: DB_ReplaceHeadAssetEntry resolves stub materials, pauses the
		// PSO queue for shaders and marks techsets for fixup; DB_PostLoadXZone rebuilds the material pipeline states.
		bool is_refreshable(const std::int32_t type)
		{
			switch (type)
			{
			case ASSET_TYPE_MATERIAL:
			case ASSET_TYPE_COMPUTESHADER:
			case ASSET_TYPE_LIBSHADER:
			case ASSET_TYPE_VERTEXSHADER:
			case ASSET_TYPE_HULLSHADER:
			case ASSET_TYPE_DOMAINSHADER:
			case ASSET_TYPE_PIXELSHADER:
			case ASSET_TYPE_TECHNIQUE_SET:
			case ASSET_TYPE_IMAGE:
				return true;
			default:
				return false;
			}
		}

		using asset_key = std::pair<std::int32_t, std::string>;

		// FNV-1a over the zone data and its block sizes.
		std::uint64_t digest(const std::string_view data, const std::span<const std::uint64_t> block_sizes)
		{
			auto hash = 0xCBF29CE484222325ull;
			const auto add = [&](const void* bytes, const std::size_t size)
			{
				for (const auto byte : std::span(static_cast<const std::uint8_t*>(bytes), size))
				{
					hash = (hash ^ byte) * 0x100000001B3ull;
				}
			};

			add(data.data(), data.size());
			add(block_sizes.data(), block_sizes.size_bytes());
			return hash;
		}

		const std::array<const char*, 1> no_strings{nullptr};

		// One asset written alone, each of its uses a name stub: a change of an asset that it uses does not change it.
		std::uint64_t asset_digest(const loaded_asset& asset)
		{
			const zone_asset entry{asset.type, asset.header, false};
			const auto written = write_zone(std::span(&entry, 1), std::span(&entry, 1), no_strings,
				{.reference_uses = true});
			return digest(written.data, written.block_sizes);
		}

		// The main zone and the techsets zone of a package; srv_, eng_ and ww_ hold server data and localized text.
		struct package_zones
		{
			std::vector<std::unique_ptr<loaded_zone>> zones;
			std::map<asset_key, const loaded_asset*> assets;
		};

		package_zones load_package(const std::filesystem::path& folder, const std::string& id)
		{
			package_zones package;
			for (const auto& name : {id, "techsets_" + id})
			{
				const auto path = folder / (name + ".ff");
				if (!std::filesystem::exists(path))
				{
					if (name == id)
					{
						throw std::runtime_error(std::format("{} has no zone {}", folder.string(), path.filename().string()));
					}

					continue;
				}

				auto& zone = *package.zones.emplace_back(load_zone(path, {.keep_temp_data = true}));
				for (const auto& asset : zone.assets)
				{
					if (!asset.referenced && asset.header && is_refreshable(asset.type))
					{
						package.assets.try_emplace({asset.type, get_asset_name(asset.type, asset.header)}, &asset);
					}
				}
			}

			return package;
		}

		std::set<asset_key> read_manifest(const std::filesystem::path& path)
		{
			std::set<asset_key> keys;
			std::ifstream stream(path);
			std::string line;
			if (!stream || !std::getline(stream, line) || line != manifest_magic)
			{
				return keys;
			}

			while (std::getline(stream, line))
			{
				const auto space = line.find(' ');
				if (space == std::string::npos || space == 0)
				{
					continue;
				}

				keys.emplace(std::stoi(line.substr(0, space)), line.substr(space + 1));
			}

			return keys;
		}

		// Written next to its final name, then moved over it: the game never reads a partial file.
		void write_file(const std::filesystem::path& path, const std::function<void(const std::filesystem::path&)>& write)
		{
			auto pending = path;
			pending += ".pending";
			write(pending);
			std::filesystem::rename(pending, path);
		}
	}

	void refresh_zone(const std::filesystem::path& deployed, const std::filesystem::path& rebuilt,
		const std::filesystem::path& output)
	{
		const auto _ = gsl::finally(db_clear);

		const auto id = rebuilt.lexically_normal().filename().empty() ? rebuilt.lexically_normal().parent_path().filename().string()
			: rebuilt.lexically_normal().filename().string();
		const auto zone_name = "techsets_" + id + "_refresh";
		const auto zone_path = output / (zone_name + ".ff");
		const auto manifest_path = output / (zone_name + ".txt");

		// The deployed assets are those of the running match.
		std::map<asset_key, std::uint64_t> deployed_digests;
		{
			const auto package = load_package(deployed, id);
			for (const auto& [key, asset] : package.assets)
			{
				deployed_digests.emplace(key, asset_digest(*asset));
			}
		}
		db_clear();

		auto previous = read_manifest(manifest_path);
		const auto package = load_package(rebuilt, id);
		std::vector<zone_asset> selected;
		std::size_t changed{}, added{}, carried{};
		for (const auto& [key, asset] : package.assets)
		{
			const auto earlier = previous.erase(key) != 0;
			const auto found = deployed_digests.find(key);
			const auto same = found != deployed_digests.end() && found->second == asset_digest(*asset);
			if (same && !earlier)
			{
				continue;
			}

			++(found == deployed_digests.end() ? added : same ? carried : changed);
			selected.emplace_back(asset->type, asset->header, false);
		}

		for (const auto& [type, name] : previous)
		{
			ZONETOOL_WARNING("%s (type %d) was in the previous refresh zone but is not in the rebuilt package: it stays "
				"default in a running match until the map loads again", name.data(), type);
		}

		const auto written = write_zone(selected, selected, no_strings, {.reference_uses = true});
		xfile::fastfile file{};
		file.payload = written.data;
		file.block_sizes = written.block_sizes;
		file.xfile_version = package.zones.front()->file.xfile_version;
		file.preload_walk_size = written.preload_walk_size;
		std::filesystem::create_directories(output);
		write_file(zone_path, [&](const std::filesystem::path& path)
		{
			xfile::write(path, definition().format, file);
		});

		write_file(manifest_path, [&](const std::filesystem::path& path)
		{
			std::ofstream stream(path, std::ios::binary | std::ios::trunc);
			stream << manifest_magic << '\n';
			for (const auto& asset : selected)
			{
				stream << asset.type << ' ' << get_asset_name(asset.type, asset.header) << '\n';
			}

			if (!stream)
			{
				throw std::runtime_error(std::format("failed to write {}", path.string()));
			}
		});

		ZONETOOL_INFO("Refresh zone \"%s\" written to %s: %zu assets (%zu changed, %zu new, %zu carried from the previous "
			"refresh), 0x%zX bytes", zone_name.data(), zone_path.string().data(), selected.size(), changed, added, carried,
			written.data.size());
	}
}
