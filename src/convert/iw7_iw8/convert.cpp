#include <std_include.hpp>
#include "export.hpp"

#include <game.hpp>

namespace zonetool::iw7
{
	const game& definition();
}

namespace zonetool::convert::iw3_iw8
{
	void convert_extraction(const std::filesystem::path& zone_path, const std::filesystem::path& extraction,
		const zone_conversion& conversion);
}

namespace zonetool::convert::iw7_iw8
{
	namespace
	{
		void write_marker(const std::filesystem::path& path)
		{
			std::filesystem::create_directories(path.parent_path());
			std::ofstream(path, std::ios::binary | std::ios::trunc) << "1";
		}
	}

	// Converts an IW7 map: its world, collision, lights, entities, models, materials and images go to an extraction in
	// the layout of the IW3 extraction cache, which the IW3 map pipeline turns into the Replay 1.20 map zones.
	void convert_zone(const std::filesystem::path& zone_path, const zone_conversion& conversion)
	{
		const auto _ = gsl::finally(db_clear);

		// IW7_REUSE_EXTRACTION=1 converts a complete extraction of an earlier run again.
		const auto previous = std::filesystem::absolute(conversion.output_folder / ".iw7-extract" / zone_path.stem());
		if (GetEnvironmentVariableA("IW7_REUSE_EXTRACTION", nullptr, 0) && std::filesystem::exists(previous / "complete"))
		{
			iw3_iw8::convert_extraction(zone_path, previous, conversion);
			return;
		}

		source_map source(zone_path);
		{
			const auto& world = *source.world();
			// The surfaces share the vertices of the transient zones.
			std::size_t vertices{};
			std::size_t triangles{};
			for (auto i = 0u; i < world.draw.transientZoneCount; i++)
			{
				vertices += world.draw.transientZones[i] ? world.draw.transientZones[i]->vertexCount : 0;
			}

			for (auto i = 0u; i < world.surfaceCount; i++)
			{
				triangles += world.dpvs.surfaces[i].tris.triCount;
			}

			const auto fx_world = source.fx_world();
			ZONETOOL_INFO("iw7: %s: %u surfaces, %zu vertices, %zu triangles, %u static models, %u transient zones, %u lightmaps, %u glass pieces",
				source.name().data(), world.surfaceCount, vertices, triangles, world.dpvs.smodelCount, world.draw.transientZoneCount,
				world.draw.lightMapCount, fx_world ? fx_world->glassSys.initPieceCount : 0);
		}

		extraction output{};
		output.root = std::filesystem::absolute(conversion.output_folder / ".iw7-extract" / source.name());
		output.extracted = output.root / "extracted";
		std::filesystem::remove_all(output.root);
		std::filesystem::create_directories(output.extracted);

		collect_static_models(source, output);
		export_entities(source, output);
		export_animations(source, output);
		export_effects(source, output, conversion);
		// The sound globals (SndGlobals "singleton": the falloff, filter and reverb send curves that aliases name) are in
		// code_post_gfx, which the game loads before every map; as a common zone it only fills in what the others lack.
		source.load("patch_code_post_gfx");
		if (!source.load("code_post_gfx"))
		{
			ZONETOOL_WARNING("iw7: zone \"code_post_gfx\" was not found; sound aliases keep the default curves");
		}

		export_sounds(source, output);
		// A model that the loaded zones lack may have a zone of its own.
		for (auto& [name, model] : output.models)
		{
			if (!model && source.load_model_zone(name))
			{
				model = static_cast<const iw7::XModel*>(source.find(iw7::ASSET_TYPE_XMODEL, name));
			}
		}

		export_models(source, output);
		export_skeletons(source, output);
		remove_missing_entity_models(source, output);
		export_world(source, output);
		export_collision(source, output);
		export_com_world(source, output);
		export_materials(source, output);

		// The zone declarations of the IW3 export name the map's raw files and effects, which do not convert yet.
		std::filesystem::create_directories(output.extracted / "zone_source");
		std::ofstream(output.extracted / "zone_source" / (source.name() + ".zone"), std::ios::binary | std::ios::trunc)
			<< "// " << source.name() << " (IW7)\n";

		// The skeletons of the models with bones (export_animations, export_skeletons) are the whole skeleton export.
		write_marker(output.root / "skeleton" / "complete");
		write_marker(output.root / "complete");
		ZONETOOL_INFO("iw7: extraction written to %s", output.root.string().data());

		// IW7_EXPORT_ONLY=1 stops after the extraction, to check it.
		if (!GetEnvironmentVariableA("IW7_EXPORT_ONLY", nullptr, 0))
		{
			iw3_iw8::convert_extraction(zone_path, output.root, conversion);
		}
	}
}
