#pragma once

#include <iw7/iw7.hpp>
#include <iw7/zone/loader.hpp>

#include <io/game_files.hpp>

namespace zonetool::convert::iw7_iw8
{
	// An asset name without the ',' that marks a reference.
	std::string_view asset_name(const char* name);

	// A surface of a model that IW7 streams from a shared stream file: the vertices and faces that the exports read.
	struct streamed_surface
	{
		bool motion{};
		std::vector<iw7::GfxPackedVertex> vertices;
		std::vector<iw7::GfxPackedMotionVertex> motion_vertices;
		std::vector<iw7::Face> faces;
	};

	// The zones of an IW7 map: the map zone, the transient zones of its world and any zone that holds an asset that the
	// map zone references, with the assets that they hold by type and name.
	class source_map
	{
	public:
		explicit source_map(const std::filesystem::path& zone_path);

		const std::string& name() const;
		iw7::GfxWorld* world() const;
		iw7::clipMap_t* clip_map() const;
		iw7::ComWorld* com_world() const;
		iw7::MapEnts* map_ents() const;
		iw7::FxWorld* fx_world() const;

		// The asset of a type and name that a loaded zone holds, or nullptr.
		void* find(iw7::XAssetType type, std::string_view name) const;

		// The asset that a header names: the header when it holds the asset, else the held asset of that name. A zone
		// holds assets that another asset loads inline (the static models of a world) outside its asset list; a
		// reference names its asset with a leading ','.
		template <typename T>
		T* resolve(const iw7::XAssetType type, T* header) const
		{
			if (!header || this->held_.contains(header))
			{
				return header;
			}

			const auto name = iw7::get_asset_name(type, header);
			if (const auto found = this->find(type, asset_name(name)))
			{
				return static_cast<T*>(found);
			}

			return name && name[0] != ',' ? header : nullptr;
		}

		// Loads another zone of the game folder, for the assets that the map references.
		bool load(std::string_view zone_name);

		// The surfaces of a model surfaces asset that the map streams, or nullptr.
		const std::vector<streamed_surface>* streamed_surfaces(std::string_view name) const
		{
			const auto found = this->streamed_.find(std::string(name));
			return found != this->streamed_.end() ? &found->second : nullptr;
		}

		std::size_t stream_file_count() const
		{
			return this->zones_.front()->file.stream_files.size();
		}

		std::size_t shared_stream_file_count() const
		{
			return this->zones_.front()->file.shared_stream_files.size();
		}

		const std::vector<XStreamFile>& shared_stream_files() const
		{
			return this->zones_.front()->file.shared_stream_files;
		}

		// The held assets of a type by name.
		const std::map<std::string, void*, std::less<>>& assets(iw7::XAssetType type) const
		{
			static const std::map<std::string, void*, std::less<>> none;
			const auto found = this->assets_.find(type);
			return found != this->assets_.end() ? found->second : none;
		}

	private:
		std::string name_;
		std::vector<std::unique_ptr<iw7::loaded_zone>> zones_;
		std::unordered_map<std::int32_t, std::map<std::string, void*, std::less<>>> assets_;
		std::unordered_set<const void*> held_;
		std::unordered_map<std::string, std::vector<streamed_surface>> streamed_;

		void add(std::unique_ptr<iw7::loaded_zone> zone);
		void load_shared_streams(const iw7::loaded_zone& zone);
		template <typename T>
		T* first(iw7::XAssetType type) const;
	};

}
