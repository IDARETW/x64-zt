#pragma once

#include "source.hpp"

namespace zonetool::iw7
{
	// Decompresses the LZ4 blocks of a stream in a pak file (assets/gfximage.cpp).
	std::vector<std::uint8_t> decompress_lz4_block(const void* data, std::size_t size);
}

namespace zonetool::convert::iw7_iw8
{
	// The extraction that the IW3 map pipeline reads (the layout of its OAT Unlinker export): extracted/maps/mp/ holds
	// the world, collision and common world exports with the entities, the other folders hold materials, images and
	// models. Every writer adds to `extracted`.
	struct extraction
	{
		std::filesystem::path root;
		std::filesystem::path extracted;
		// The materials and models that the exports use, by their names in the extraction, with their assets (nullptr
		// for one that the loaded zones do not hold). They are written after the world.
		std::map<std::string, const iw7::Material*> materials;
		std::map<std::string, const iw7::XModel*> models;
		// The models that export_models wrote.
		std::set<std::string> exported_models;
		// The models beyond the range of Replay model bounds (planets): the sky draws them.
		std::set<std::string> distant_models;

		// Adds the material or model of a header and returns its name in the extraction.
		std::string add_material(const source_map& source, iw7::Material* header);
		std::string add_model(const source_map& source, iw7::XModel* header);
	};

	// A name that a file of the extraction can take: '*' and the other characters that paths reject become '_'.
	std::string file_name(std::string_view name);

	enum class surface_kind
	{
		opaque,
		alpha_test,
		blend,
		additive,
		multiply,
		sky,
		distortion,
	};

	// How a material draws. The name of an IW7 technique set gives its blend after "sm" in lit techniques and after
	// "unlit" in unlit ones (w_l_sm_ndw_blend_i0c0s0, wc_unlit_add_lin_ct_ndw); "ndw" (no depth write) can come
	// between.
	struct material_class
	{
		surface_kind kind{};
		bool unlit{};
		bool falloff{};

		// Decals, effects and the sky do not block players and bullets.
		bool solid() const
		{
			return this->kind == surface_kind::opaque || this->kind == surface_kind::alpha_test;
		}
	};

	material_class classify_material(const iw7::Material* material);

	// The brush model of trigger model N of the map entities: the trigger models follow the world's brush models.
	unsigned int trigger_brush_model(const source_map& source, unsigned int trigger);

	struct box
	{
		std::array<float, 3> mins{};
		std::array<float, 3> maxs{};
	};

	// The bounds of a trigger model: its hulls, in the space of the entity that places it, 1 unit larger (the margin
	// of IW3 brush model bounds).
	box trigger_bounds(const iw7::MapTriggers& triggers, unsigned int model);

	struct mesh_vertex
	{
		std::array<float, 3> position{};
		std::array<float, 3> normal{};
		std::array<float, 2> uv{};
	};

	struct mesh_surface
	{
		const iw7::Material* material{};
		std::vector<mesh_vertex> vertices;
		std::vector<std::array<std::uint16_t, 3>> triangles;
	};

	// The surfaces of the first level of detail of a model.
	std::vector<mesh_surface> model_mesh(const source_map& source, const iw7::XModel& model);

	std::string bsp_name(const source_map& source);

	// The primary light that is the sun (light 0 is empty), in IW7 and IW3 worlds.
	constexpr unsigned int sun_light = 1;

	// IW7 lights are in the high dynamic range units that its exposure scales: the factor that takes them to IW3
	// units, in which the sun has a luminance of 1 (Frontier's sun: 9.5).
	float light_unit_scale(const source_map& source);

	// Adds the models of the world's static models, which export_models writes before export_world.
	void collect_static_models(const source_map& source, extraction& output);
	void export_world(const source_map& source, extraction& output);
	void export_com_world(const source_map& source, extraction& output);
	void export_entities(const source_map& source, extraction& output);
	void export_collision(const source_map& source, extraction& output);
	void export_materials(const source_map& source, extraction& output);
	void export_models(const source_map& source, extraction& output);
	// Removes the model of each entity whose model did not export (it is in a zone that the map does not load).
	void remove_missing_entity_models(const source_map& source, const extraction& output);
	// Writes a neutral cube map for the reflection probe of the world and returns its image name.
	std::string export_default_probe(extraction& output);
	// Writes the sky cube map of the world and returns its image name, or an empty name (the pipeline's dark sky).
	std::string export_sky(const source_map& source, extraction& output);
	// The lightmaps of the world: their entries for the world export (the lightmap index of a surface converts when
	// it is below their count), and for the static models, which take the light of the floor under them, the ambient
	// light (A + B / 2, in IW7 units) and the sun visibility of each texel.
	struct world_lightmaps
	{
		struct page
		{
			std::size_t width{};
			std::size_t height{};
			std::vector<std::array<float, 3>> ambient;
			std::vector<std::uint8_t> visibility;
		};

		json entries = json::array();
		std::vector<page> pages;
	};

	// Writes the lightmaps of the world as IW3 lightmap files.
	world_lightmaps export_lightmaps(const source_map& source, extraction& output);

	// The level that IW3 lightmaps and probes take for an IW7 irradiance or radiance (a soft knee above 0.5).
	float light_level(float value);
	float linear_to_srgb(float value);

	// The reflection probes of the world: entry 0 is a neutral probe at the origin, as in IW3 worlds, then a probe
	// for each cube map of the IW7 probe array.
	struct reflection_probes
	{
		struct volume
		{
			std::array<float, 3> center{};
			std::array<std::array<float, 3>, 3> axes{};
			std::array<float, 3> half_size{};
			float priority{};
			unsigned int probe{};
		};

		json entries = json::array();
		std::vector<std::array<float, 3>> origins;
		std::vector<volume> volumes;

		// The probe for a point: the probe of the volume with the highest priority that holds it, else the
		// nearest probe.
		unsigned int find(const float* point) const;
	};

	reflection_probes export_reflection_probes(const source_map& source, extraction& output);

	// Writes a json file of the extraction.
	void write_json(const std::filesystem::path& path, const json& value);
}
