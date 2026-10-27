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
	// The physics body of IW7 clutter: its mass in kg and the IW7 surface type that covers most of it, which names its
	// physics material.
	struct clutter_body
	{
		float mass{};
		std::string surface;
	};

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
		// The materials (and material layers) of world surfaces, whose colour maps keep more texels (world_color_size).
		std::set<std::string> world_materials;
		// The materials of world surfaces that IW7 draws in its lit decal range (GfxWorldDpvsStatic litDecalSurfsBegin to
		// litDecalSurfsEnd), after the opaque surfaces they lie on: they take the decal depth bias.
		std::set<std::string> decal_range_materials;
		// IW7's light-grid intensity tweak, which the map script sets (r_lightGridEnableTweaks, r_lightGridIntensity: 1.33 on
		// Frontier and Breakneck) and Replay lacks: the converted light grid holds it.
		float light_grid_intensity = 1.0f;
		// The map script turns IW7's volumetric lighting off (r_volumetrics 0: Frontier). Logged only: the lights keep their
		// own volumetric setting and the converted script drops the dvar (export_effects).
		bool volumetrics = true;
		// The layers of layered IW7 materials (techsets "..._b1..._b2..."), which convert as materials of their own: by
		// name in the extraction, the layer (1 to 3) of the material that `materials` holds.
		std::map<std::string, int> material_layers;
		// The materials whose layered surfaces draw as one stock Replay layered surface (material_stock_layered), with
		// their layer count; their layers are materials of their own (material_layers) without surfaces.
		std::map<std::string, int> stock_layered_materials;
		// The world materials of a surface without a lightmap: Replay's stock world materials draw lightmapped surfaces
		// only, so these keep their converted material beside a stock one.
		std::set<std::string> lightless_world_materials;
		// The materials of the surfaces that draw in the sky at run time (runtime_sky).
		std::set<std::string> sky_object_materials;
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
		// The placed scriptables of the other kinds that take damage (several parts, stages, reactions, explosions, IW7's
		// destructible cars): their instances are script models of this targetname, whose state machine (export_entities'
		// scriptable_machine) the map script runs (iw7_scriptables).
		std::map<std::string, json> scriptable_machines;
		// The instances with an IW7 targetname (which the map script finds them by) whose kind has a state machine (Neon's
		// holograms, which the map script sets the states of): the machine's name in scriptable_machines, the targetname and
		// the instance's initial origin, which the map script finds the instance by.
		struct scriptable_target
		{
			std::string machine;
			std::string targetname;
			std::array<float, 3> origin{};
		};
		std::vector<scriptable_target> scriptable_targets;
		// The models that the map script sets by name (precachemodel, setmodel: Rivet's drones, the collision patches that
		// the scripts spawn), which convert as script models.
		std::set<std::string> script_models;
		// The models that the entity string places (script models), which keep the keyframed body of script models.
		std::set<std::string> entity_models;
		// The map has IW7 out-of-bounds volumes (OutOfBounds triggers), which the map script watches (iw7_out_of_bounds).
		bool out_of_bounds{};
		// IW7's clutter (the dynamic entities that its physics simulates: Frontier's fire extinguishers, Depot's buckets,
		// cups and tools): Replay clutter of the model at its initial pose (a quaternion x, y, z, w), which the collision
		// export writes as the map's dynamic models with their physics presets.
		struct dynamic_model
		{
			std::string model;
			std::array<float, 4> quaternion{};
			std::array<float, 3> origin{};
			clutter_body body;
		};
		std::vector<dynamic_model> dynamic_models;
		// The models that the state machines throw as debris (SpawnDynent), with their bodies (none for a model whose hull
		// spans no volume), and those of them that fly as physics props: thrown only, never set, attached or placed (a
		// model has one physics asset, and the others keep the keyframed body of script models).
		std::map<std::string, std::optional<clutter_body>> debris_models;
		// The models that the last states of the machines swap in before they push the piece (picture frames that fall,
		// broken cash registers that a push kicks away), by the number of those swaps, and the number of times that the
		// machines set or attach each model: a model that only such swaps set flies as a physics prop too.
		std::map<std::string, std::size_t> swap_models;
		std::map<std::string, std::size_t> machine_models;
		// The models that the map script sets by name (in script_models too).
		std::set<std::string> map_script_models;
		std::map<std::string, clutter_body> physics_models;

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
			struct animation_step
			{
				const iw7::ScriptableEventAnimationDef* event{};
				const iw7::XAnimParts* anim{};
			};
			std::string part;
			std::string state;
			const iw7::XAnimParts* anim{};
			bool first{};
			bool moves{};
			std::array<float, 3> to{};
			float time{};
			// Source event records remain available while the map is being extracted.
			const iw7::ScriptableEventAnimationDef* event{};
			std::vector<animation_step> on_end;
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

	// Semantic rumble definitions referenced by the placed Scriptables, with source evidence kept separately.
	void export_rumbles(const source_map& source, const std::filesystem::path& extracted);

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

	// The layers of a material: IW7 world techsets blend up to four layers over the first ("r0..._b1..._b2..._b3..._b4"),
	// each with its own textures (whose shader names end with the layer's digit) and tint (colorTint1 to colorTint4), by
	// the weights in the green, blue and alpha channels of the vertex colours and, for the fourth, in the layer vertex
	// data (layer_vertex_layout).
	constexpr int max_material_layer = 4;
	// MW120R_STOCK_WORLD=1: world surfaces draw with Replay's stock world technique sets (AGENTS.md plan 1.4), from the
	// stock images that the extraction then carries beside the converted ones.
	inline bool replay_stock_world()
	{
		static const bool enabled = [] { const char* value = std::getenv("MW120R_STOCK_WORLD"); return value && *value == '1'; }();
		return enabled;
	}

	// MW120R_RUNTIME_SKY=1: the distant models and world surfaces draw in the sky at run time with their own materials
	// (AGENTS.md plan 2.1), where they were drawn into the sky's cube map.
	// MW120R_IW7_EXPOSURE=1: the map keeps IW7's light units throughout (the sun, lights, emission, fog and sky as the
	// lightmaps keep them) and Replay meters it with IW7's own auto exposure (AGENTS.md plan 3.4): Replay's vision
	// keeps IW7's tonemap exposure fields, with IW7's defaults.
	inline bool iw7_exposure()
	{
		static const bool enabled = [] { const char* value = std::getenv("MW120R_IW7_EXPOSURE"); return value && *value == '1'; }();
		return enabled;
	}

	inline bool runtime_sky()
	{
		static const bool enabled = [] { const char* value = std::getenv("MW120R_RUNTIME_SKY"); return value && *value == '1'; }();
		return enabled;
	}
	int material_layer_count(const iw7::Material* material);
	// The technique-set token of a layer (1 to max_material_layer) of a layered material ("b1c1s1nl1pa1": blend b, alpha
	// test t, add a, multiply m, then its maps), empty without one.
	std::string_view material_layer_token(const iw7::Material* material, int layer);
	// What a layered material keeps per vertex in GfxWorldTransientZone.vld, in the order the vertex declarations of its
	// IW7 techset route it (".ft1t3.ft2t4.fx0t5.fx1t6.fc2c1"): the UVs of layers 1 to 4 in turn (t1 holds the first
	// two, t2 the others), a packed tangent frame (x0 on) for each layer with a normal map of its own (nl, nal, nbl)
	// but the first of the material, whose frame is the vertex's own (the base's when the base has a normal map), then
	// the weight of layer 4 (c2). Carnage's strides match it for every technique.
	struct layer_vertex_layout
	{
		unsigned int size{};
		std::optional<unsigned int> fourth_weight;

		static constexpr unsigned int uv(const int layer)
		{
			return 8u * static_cast<unsigned int>(layer - 1);
		}

		// The tangent frame of the n-th normal-mapped layer after the material's first (n from 0), after the UVs of the
		// layers.
		static constexpr unsigned int tangent(const int layers, const unsigned int n)
		{
			return uv(layers) + 4u * n;
		}
	};
	layer_vertex_layout material_layer_vertex_layout(const iw7::Material* material);
	// The reveal sharpness of a layer (0 to max_material_layer) that blends by its reveal map (its token holds
	// v<layer>), none for a layer that blends by its weight alone.
	std::optional<float> material_layer_reveal(const iw7::Material* material, int layer);
	// The reveal sharpness that our shader ramps a layer, a decal or a blended layered base by (kRevealLayerFlag).
	std::optional<float> material_reveal_ramp(const iw7::Material* material, int layer);
	// Whether a layered material draws as one stock Replay layered surface (MW120R_STOCK_WORLD): two to four layers, each
	// blending over the ones before by its weight (b) or multiplying them (m), as Replay's stock twc4 world shader does.
	bool material_stock_layered(const iw7::Material* material);
	// Whether a layer (0 to max_material_layer) of a material has a normal map of its own (nl, nal, nbl).
	bool material_layer_normal_map(const iw7::Material* material, int layer);

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
	// A vertex of the geometry files (models' .replay.json, the world's surfaces), written by glaze: the members are
	// their keys and the values the doubles that json values held.
	struct geometry_vertex
	{
		std::array<double, 3> position{}, normal{}, tangent{};
		double binormal_sign{};
		std::array<double, 2> uv{}, lightmap_uv{};
		std::array<int, 4> color{};
		// A stock layered surface's vertex (MW120R_STOCK_WORLD): the UV of each layer after the first, and the tangent
		// transform of each (four bytes of a 2x2 matrix as Replay decodes them, byte * 2 / 255 - 1; 0 keeps the
		// surface's own frame). Its colour holds the layers' weights.
		std::optional<std::vector<std::array<double, 2>>> layer_uvs;
		std::optional<std::vector<std::uint32_t>> layer_transforms;
	};

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
		// RGBA from 0 to 1 (white for motion vertices, which keep no colour).
		std::array<float, 4> color{1.0f, 1.0f, 1.0f, 1.0f};
	};

	struct mesh_surface
	{
		const iw7::Material* material{};
		std::vector<mesh_vertex> vertices;
		std::vector<std::array<std::uint16_t, 3>> triangles;
	};

	// The surfaces of the first level of detail of a model.
	std::vector<mesh_surface> model_mesh(const source_map& source, const iw7::XModel& model);
	// The physics body of a model as IW7 clutter, which IW7 keeps nowhere; nothing for a model whose hull spans no volume.
	std::optional<clutter_body> clutter_physics(const source_map& source, const iw7::XModel& model);
	// Writes the physics preset of a clutter model (extracted/physic/iw7_<model>, IW3's "PHYSIC\mass\...\sndAliasPrefix\..."
	// text, which the pipeline reads) and returns its name.
	std::string write_physics_preset(const extraction& output, const std::string& model, const clutter_body& body);
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
	// The text of a raw file that the loaded zones hold, or nothing.
	std::string raw_file(const source_map& source, const std::string& name);

	// The primary light that is the sun (light 0 is empty), in IW7 and IW3 worlds.
	constexpr unsigned int sun_light = 1;

	// IW7 lights are in the high dynamic range units that its exposure scales: the factor that takes them to IW3
	// units, in which the sun has a luminance of 1 (Frontier's sun: 9.5).
	float light_unit_scale(const source_map& source);

	// Replay's world vertices reach 100000 units (replay_render: "Position out of range").
	constexpr float largest_world_coordinate = 100000.0f;
	// The play space: the bounds of the spawn points (mins, then maxs), or nothing for a map without spawn points.
	std::optional<std::array<float, 6>> spawn_bounds(const source_map& source);
	// Whether a static model of the world reaches beyond the range of the world: the sky draws it, not the world
	// (Breakneck's black hole sphere, placed 85000 units from the middle of the world at scale 58).
	bool beyond_world(const iw7::GfxWorld& world, unsigned int instance);

	// Adds the models of the world's static models, which export_models writes before export_world.
	void collect_static_models(const source_map& source, extraction& output);
	void export_world(const source_map& source, extraction& output);
	void export_com_world(const source_map& source, extraction& output);
	void export_entities(const source_map& source, extraction& output);
	void export_collision(const source_map& source, extraction& output);
	void export_materials(const source_map& source, extraction& output);
	// IW7's flag vertex wave of a material, which the pipeline's static models apply: the flag techsets' three fixed waves
	// on a time of the material's phase (classic, MAP_IW7_FLAG_CLASSIC), or the flagext techsets' two waves of the material
	// (MAP_IW7_FLAG_WAVE); values are flagParms, flagWave0Parms and flagWave1Parms.
	struct flag_wave_parameters
	{
		bool classic{};
		std::array<float, 12> values{};
	};

	std::optional<flag_wave_parameters> flag_wave(const iw7::Material* material);
	// Whether a material bends its surfaces by reactive motion (the *_reactivemotion_* techsets, around players and
	// explosions), which the pipeline's static models take from Replay's reactive-motion sim.
	bool reactive_motion(const iw7::Material* material);
	void export_loadscreen(const source_map& source, const extraction& output);
	void export_compass(const source_map& source, const extraction& output);
	void export_models(const source_map& source, extraction& output);
	// Writes the animations of the animated scriptables (IW3 XAnim dumps, extracted/xanim/<name>.iw3.json) and the
	// skeletons of their models (skeleton/: an XMODEL_EXPORT of each LOD), which the pipeline reads for entity models.
	// A scriptable whose model or animation does not convert keeps its bind pose and its tag effects their bind pose.
	void export_animations(const source_map& source, extraction& output);
	// Writes an animation that the map script plays by name (an IW3 XAnim dump, extracted/xanim/<name>.iw3.json) and
	// returns its length in seconds, or nothing when the zones lack it or it does not convert.
	std::optional<float> export_script_animation(const source_map& source, const std::string& name, extraction& output);
	void export_skeletons(const source_map& source, extraction& output);
	// Removes the model of each entity whose model did not export (it is in a zone that the map does not load).
	void remove_missing_entity_models(const source_map& source, const extraction& output);
	// Writes a neutral cube map for the reflection probe of the world and returns its image name.
	std::string export_default_probe(extraction& output);
	// Writes the sky cube map of the world and returns its image name, or an empty name (the pipeline's dark sky).
	std::string export_sky(const source_map& source, extraction& output);
	// The tonemap fields of the map's vision file (tonemap*, toneWhitePoint) as it keeps them.
	json iw7_tonemap(const source_map& source);
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
	// Writes a text file of the extraction.
	void write_text(const std::filesystem::path& path, std::string_view text);

	// Writes a json file of the extraction on another thread (at most `limit` wait in memory); finish_json_writes
	// waits for them and rethrows the first failure.
	void write_json_async(std::filesystem::path path, json value, std::size_t limit = 4);
	// Runs a write of the extraction on another thread, as write_json_async does.
	void write_async(std::move_only_function<void()> write, std::size_t limit = 4);
	void finish_json_writes();
}
