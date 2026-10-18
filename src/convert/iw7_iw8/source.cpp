#include <std_include.hpp>
#include "source.hpp"

#include <game.hpp>

namespace zonetool::iw7
{
	const game& definition();
}

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		// The pointer value that the IW7 loader reads as data that follows.
		constexpr std::uintptr_t follows = 0xFFFFFFFFFFFFFFFF;
		constexpr std::size_t block_count = 10;
	}

	std::string_view asset_name(const char* name)
	{
		const std::string_view view = name ? name : "";
		return view.starts_with(',') ? view.substr(1) : view;
	}

	source_map::source_map(const std::filesystem::path& zone_path)
		: name_(zone_path.stem().string())
	{
		ZONETOOL_INFO("Loading IW7 zone \"%s\"...", this->name_.data());
		this->add(iw7::load_zone(zone_path));

		const auto world = this->world();
		if (!world)
		{
			throw std::runtime_error(std::format("zone \"{}\" holds no gfx world", this->name_));
		}

		// The world draws its vertices from transient zones, which other fastfiles hold.
		for (auto i = 0u; i < world->draw.transientZoneCount; i++)
		{
			const auto zone = world->draw.transientZones[i];
			if (zone && !this->held_.contains(zone))
			{
				const auto name = asset_name(zone->name);
				if (!this->find(iw7::ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, name) && !this->load(name))
				{
					throw std::runtime_error(std::format("transient zone \"{}\" of \"{}\" was not found", name, this->name_));
				}
			}
		}

		// The materials and techsets of a map are in techsets_<map>.
		if (!this->load("techsets_" + this->name_))
		{
			ZONETOOL_WARNING("iw7: zone \"techsets_%s\" was not found; materials keep default images", this->name_.data());
		}

		this->load_shared_streams();
	}

	// IW7 streams the surfaces of models from shared stream files (assetfile<N>.pak). Each stream holds one asset entry
	// and its data in the zone stream format: behind an asset list of one entry it loads as a zone of its own. The
	// surfaces are copied, so that each stream zone can go.
	void source_map::load_shared_streams()
	{
		std::set<std::uint16_t> missing;
		std::size_t surfaces{};
		for (const auto& file : this->zones_.front()->file.shared_stream_files)
		{
			std::string chunk(file.offsetEnd - file.offset, '\0');
			const auto pak = std::format("assetfile{}.pak", file.fileIndex);
			if (!game_files::get().read(pak, file.offset, chunk.size(), chunk.data()))
			{
				if (missing.insert(file.fileIndex).second)
				{
					ZONETOOL_WARNING("iw7: model surfaces stream from %s, which is not in the game folder", pak.data());
				}

				continue;
			}

			const auto stream = xfile::read_iw7_shared_stream(chunk, iw7::definition().format);
			iw7::XAssetList list{};
			list.assetCount = 1;
			list.assets = reinterpret_cast<iw7::XAsset*>(follows);

			xfile::fastfile zone_file{};
			zone_file.payload.assign(reinterpret_cast<const char*>(&list), sizeof(list));
			zone_file.payload += stream;
			zone_file.block_sizes.assign(block_count, zone_file.payload.size() * 2 + 0x10000);

			const auto zone = iw7::load_zone(std::format("{}_stream_{}", this->name_, surfaces), std::move(zone_file));
			for (const auto& asset : zone->assets)
			{
				if (asset.type != iw7::ASSET_TYPE_XMODEL_SURFS || asset.referenced)
				{
					continue;
				}

				const auto model_surfaces = static_cast<const iw7::XModelSurfs*>(asset.header);
				auto& copies = this->streamed_[std::string(asset_name(model_surfaces->name))];
				for (auto i = 0u; i < model_surfaces->numsurfs && model_surfaces->surfs; i++)
				{
					const auto& surface = model_surfaces->surfs[i];
					auto& copy = copies.emplace_back();
					copy.motion = (surface.flags & 8) != 0;
					if (copy.motion)
					{
						copy.motion_vertices.assign(surface.verts0.packedMotionVerts0, surface.verts0.packedMotionVerts0 + surface.vertCount);
					}
					else
					{
						copy.vertices.assign(surface.verts0.packedVerts0, surface.verts0.packedVerts0 + surface.vertCount);
					}

					copy.faces.assign(surface.triIndices, surface.triIndices + surface.triCount);
				}
			}

			surfaces++;
		}

		ZONETOOL_INFO("iw7: %zu model surfaces streamed from shared stream files", this->streamed_.size());
	}

	const std::string& source_map::name() const
	{
		return this->name_;
	}

	template <typename T>
	T* source_map::first(const iw7::XAssetType type) const
	{
		for (const auto& zone : this->zones_)
		{
			for (const auto& asset : zone->assets)
			{
				if (asset.type == type && !asset.referenced)
				{
					return static_cast<T*>(asset.header);
				}
			}
		}

		return nullptr;
	}

	iw7::GfxWorld* source_map::world() const
	{
		return this->first<iw7::GfxWorld>(iw7::ASSET_TYPE_GFXWORLD);
	}

	iw7::clipMap_t* source_map::clip_map() const
	{
		return this->first<iw7::clipMap_t>(iw7::ASSET_TYPE_CLIPMAP);
	}

	iw7::ComWorld* source_map::com_world() const
	{
		return this->first<iw7::ComWorld>(iw7::ASSET_TYPE_COMWORLD);
	}

	iw7::MapEnts* source_map::map_ents() const
	{
		return this->first<iw7::MapEnts>(iw7::ASSET_TYPE_MAP_ENTS);
	}

	void* source_map::find(const iw7::XAssetType type, const std::string_view name) const
	{
		const auto pool = this->assets_.find(type);
		if (pool == this->assets_.end())
		{
			return nullptr;
		}

		const auto found = pool->second.find(name);
		return found != pool->second.end() ? found->second : nullptr;
	}

	bool source_map::load(const std::string_view zone_name)
	{
		const auto path = game_files::find_zone(zone_name);
		if (!path)
		{
			return false;
		}

		ZONETOOL_INFO("Loading IW7 zone \"%s\"...", std::string(zone_name).data());
		this->add(iw7::load_zone(*path));
		return true;
	}

	void source_map::add(std::unique_ptr<iw7::loaded_zone> zone)
	{
		for (const auto& asset : zone->assets)
		{
			if (!asset.referenced)
			{
				this->held_.insert(asset.header);
				this->assets_[asset.type].try_emplace(std::string(asset_name(iw7::get_asset_name(asset.type, asset.header))),
					asset.header);
			}
		}

		this->zones_.push_back(std::move(zone));
	}
}
