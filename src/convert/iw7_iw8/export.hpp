#pragma once

#include "source.hpp"

#include <game.hpp>

namespace zonetool::iw7
{
	// Decompresses the LZ4 blocks of a stream in a pak file (assets/gfximage.cpp).
	std::vector<std::uint8_t> decompress_lz4_block(const void* data, std::size_t size);
}

namespace zonetool::convert::iw7_iw8
{
	struct mesh_surface;

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
		// The world surfaces beyond the range of Replay world vertices (Parkour's mountains), in world space: the sky draws
		// them.
		std::vector<mesh_surface> distant_surfaces;
		// The materials of glass panes, which convert as lit glass (IW7 draws them premultiplied).
		std::set<std::string> glass_materials;
		// The materials of effects, which convert as the pipeline's effect materials.
		std::set<std::string> effect_materials;
		// The effect materials that particles with an atlas module draw. IW7 gives each particle its texture rectangle from
		// its atlas module (the particle vertex shader reads the rectangle from the particle data), so a particle without one
		// draws the whole texture: Scorch's sun body and glow are single images in materials of a 4x2 atlas. Replay's atlas
		// techniques cut every particle to an atlas cell, so the other effect materials convert without an atlas.
		std::set<std::string> atlas_materials;
		// The world materials of particle decals, which convert as the pipeline's decal materials (Replay decal volumes).
		std::set<std::string> decal_materials;
		// IW7's light-grid intensity tweak, which the map script sets (r_lightGridEnableTweaks, r_lightGridIntensity: 1.33 on
		// Frontier and Breakneck) and Replay lacks: the converted light grid holds it.
		float light_grid_intensity = 1.0f;
		// The map script turns IW7's volumetric lighting off (r_volumetrics 0: Frontier). Logged only: the lights keep their
		// own volumetric setting and the converted script drops the dvar (export_effects).
		bool volumetrics = true;
		// The layers of layered IW7 materials (techsets "..._b1..._b2..."), which convert as materials of their own: by
		// name in the extraction, the layer (1 to 3) of the material that `materials` holds.
		std::map<std::string, int> material_layers;
		// The strings of the map's scripts that can name sound aliases (createfx loop sounds, playsound): export_sounds
		// writes the aliases of the map's sound bank among them.
		std::set<std::string> sound_names;

		// The effects that the first states of the placed scriptables play (IW7's ops table: its planets and asteroids on
		// the tags of the solar system model), at the bind pose of their tags: export_effects places them as createfx
		// one-shot effects.
		struct scriptable_effect
		{
			std::string effect;
			std::array<float, 3> origin{};
			std::array<float, 3> angles{};
		};
		std::vector<scriptable_effect> scriptable_effects;

		// The kinds of placed scriptables that break (cups, bottles, screens): a health state, then a state that swaps the model
		// (none: the piece goes away), plays effects and a sound. Their instances are script models of this targetname, and the
		// map script breaks them when they took the health in damage.
		struct scriptable_destructible
		{
			std::string targetname;
			std::string dead_model;
			std::vector<std::string> effects;
			std::string sound;
			int health{};
		};
		std::map<std::string, scriptable_destructible> scriptable_destructibles;

		// The placed scriptables whose first state plays an animation (the ops table's solar system, the floor panels'
		// closed pose): their instances are script models of a targetname that the map script animates. The animations
		// export as IW3 XAnim dumps (extracted/xanim/<name>.iw3.json), the models with their skeletons (skeleton/).
		std::map<std::string, std::string> scriptable_animations;
		std::map<std::string, std::set<std::string>> scriptable_animation_models;
		std::map<std::string, const iw7::XAnimParts*> xanims;
		std::map<std::string, const iw7::XModel*> animated_models;
		// The instances of the animated scriptables: their model (by its name in the extraction) at its initial origin, with
		// the forward, left and up axes of its initial angles.
		struct scriptable_placement
		{
			std::string model;
			std::array<float, 3> origin{};
			std::array<std::array<float, 3>, 3> axes{};
		};
		std::vector<scriptable_placement> scriptable_placements;
		// The states of the animated scriptables' parts that play an animation, by model: the map script sets them
		// (IW7's setscriptablepartstate, and each part's first state from the start). A model that is one bone, which the
		// animations only move, moves as an entity (its collision with it): by the bone's translation in model space at
		// the end of the animation, over its length.
		struct scriptable_state
		{
			std::string part;
			std::string state;
			const iw7::XAnimParts* anim{};
			bool first{};
			bool moves{};
			std::array<float, 3> to{};
			float time{};
		};
		std::map<std::string, std::vector<scriptable_state>> scriptable_states;
		// The models of animated scriptables that move as entities (export_animations).
		std::set<std::string> moving_models;

		// Adds the material or model of a header and returns its name in the extraction.
		std::string add_material(const source_map& source, iw7::Material* header);
		// Adds a layer (1 to 3) of a layered material as a material of its own.
		std::string add_material_layer(const source_map& source, iw7::Material* header, int layer);
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
		// "blendadd": premultiplied alpha (one, inverse source alpha), which classifies as additive.
		bool premultiplied{};

		// Decals, effects and the sky do not block players and bullets.
		bool solid() const
		{
			return this->kind == surface_kind::opaque || this->kind == surface_kind::alpha_test;
		}
	};

	material_class classify_material(const iw7::Material* material);

	// The colorTint of a material as IW7 keeps it (an unlit material's emission can reach far above 1: Frontier's
	// Neptune is its colour map times 20); white without one.
	std::array<float, 4> material_color_tint(const iw7::Material* material);

	// The layers of a material: IW7 world techsets blend up to three layers over the first ("r0..._b1..._b2..._b3..."),
	// each with its own textures (whose shader names end with the layer's digit) and tint (colorTint1 to colorTint3), by
	// the weights in the green, blue and alpha channels of the vertex colours.
	int material_layer_count(const iw7::Material* material);
	// The technique-set token of a layer (1-3) of a layered material ("b1c1s1nl1pa1": blend b, alpha test t, add a,
	// multiply m, then its maps), empty without one.
	std::string_view material_layer_token(const iw7::Material* material, int layer);
	// The bytes that a layer (1-3) keeps per vertex in GfxWorldTransientZone.vld: its UV (8), and a packed tangent frame
	// (4) when it has a normal map of its own (nl, nal, nbl).
	unsigned int material_layer_vertex_size(const iw7::Material* material, int layer);

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

	// A piece of IW7's breakable glass: a rectangle in the world, which the extraction gives a brush model after the
	// trigger models and an IW3 glass entity (a script_brushmodel of intact glass), which the pipeline turns into
	// Replay's breakable glass.
	struct glass_pane
	{
		std::array<std::array<float, 3>, 4> corners{};
		std::array<std::array<float, 2>, 4> uvs{};
		std::array<float, 3> normal{};
		std::array<float, 3> tangent{};
		std::array<float, 3> mins{};
		std::array<float, 3> maxs{};
		iw7::Material* material{};
	};

	std::vector<glass_pane> glass_panes(const source_map& source);
	unsigned int glass_brush_model(const source_map& source, unsigned int pane);

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
	// Whether the loaded zones or the stream files hold the surfaces of every LOD of a model, as export_models needs.
	bool model_has_surfaces(const source_map& source, const iw7::XModel* model);

	// The collision that the Havok data of an IW7 clip map or map entities holds: a HavokPhysicsShapeList of shapes
	// with their shape tags, which give the contents, the surface type and flags (the encoding of Replay's tags) and the
	// glass piece (one-based) of each piece.
	struct havok_tag
	{
		std::uint32_t contents{};
		std::uint32_t surface_type{};
		std::uint32_t surface_flags{};
		std::uint16_t glass_id{};
	};

	// A convex piece (the brushes of the map), by its points, in the space of its shape (game units).
	struct havok_convex
	{
		std::vector<std::array<float, 3>> points;
		std::uint16_t tag{};
	};

	// Seen from the front, (b - a) x (c - a) points out (Havok's winding).
	struct havok_triangle
	{
		std::array<std::array<float, 3>, 3> corners{};
		std::uint16_t tag{};
	};

	struct havok_shape
	{
		std::vector<havok_convex> convexes;
		std::vector<havok_triangle> triangles;
	};

	struct havok_shapes
	{
		std::vector<havok_tag> tags;
		// By entry of the shape list; an entry without a shape is empty.
		std::vector<havok_shape> shapes;
		// The pieces and shapes of forms that the reader does not know, which it leaves out.
		std::size_t unknown{};
		// The points that lie outside the bounds of their mesh section (a layout that the reader does not know).
		std::size_t misplaced{};
	};

	// Reads the Havok data of an IW7 clip map or map entities: a binary packfile of Havok 2014 (hk_2014.2.5-r1).
	havok_shapes read_havok_shapes(const char* data, std::size_t size);

	// Writes the effects that the map places with its createfx script: IW3 map scripts that place and load them, and the
	// particle systems as native Replay effects (extracted/vfx/<name>.json), with the materials and models that they use.
	void export_effects(const source_map& source, extraction& output, const zone_conversion& conversion);
	// Converts IW7 particle systems and the effects that they start to native Replay effects.
	void export_effect_graphs(const source_map& source, extraction& output, const std::vector<std::string>& effects);
	// Writes the aliases of the map's sound bank (<map>.all) that the scripts name, with their secondary aliases, as IW3
	// alias definitions (extracted/soundaliases/<alias>.iw3.json), and their sounds of the bank's .sabl (FLAC frames)
	// as wave files, which the pipeline converts.
	void export_sounds(const source_map& source, extraction& output);

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
	// Writes the animations of the animated scriptables (IW3 XAnim dumps, extracted/xanim/<name>.iw3.json) and the
	// skeletons of their models (skeleton/: an XMODEL_EXPORT of each LOD), which the pipeline reads for entity models.
	// A scriptable whose model or animation does not convert keeps its bind pose and its tag effects their bind pose.
	void export_animations(const source_map& source, extraction& output);
	void export_skeletons(const source_map& source, extraction& output);
	// Removes the model of each entity whose model did not export (it is in a zone that the map does not load).
	void remove_missing_entity_models(const source_map& source, const extraction& output);
	// Writes a neutral cube map for the reflection probe of the world and returns its image name.
	std::string export_default_probe(extraction& output);
	// Writes the sky cube map of the world and returns its image name, or an empty name (the pipeline's dark sky).
	std::string export_sky(const source_map& source, extraction& output);
	// The settings of the map's vision file (vision/<map>.vision, a raw file of the map zone) by name, values unquoted.
	std::map<std::string, std::string> vision_settings(const source_map& source);
	// The map's fog for the pipeline's world export: IW7's vision fog fields, which Replay's vision has under the same
	// names, with the colour intensities in the units of the converted lights. Null without a vision file.
	json export_fog(const source_map& source);
	// Writes the vision's colour grading lookup table into the extraction; returns its file and size for the world
	// export, or null.
	json export_grading_clut(const source_map& source, const extraction& output);

	// Writes the sun file of the extraction from the world's sun flare data.
	void export_sun(const source_map& source, extraction& output);
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
	// The lightmap layers keep IW7's range: log2(1 + scale * value) / log2(1 + scale * maximum) in a byte, which the
	// pipeline's world shader decodes (MAP_LIGHTMAP_LOG; the world's "lightmap_encoding").
	constexpr float lightmap_log_scale = 8.0f;
	constexpr float lightmap_maximum = 16.0f;
	float lightmap_level(float value);
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

	// Writes a json file of the extraction on another thread (at most `limit` wait in memory); finish_json_writes
	// waits for them and rethrows the first failure.
	void write_json_async(std::filesystem::path path, json value, std::size_t limit = 4);
	void finish_json_writes();
}
