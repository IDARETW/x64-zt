#pragma once

#include "asset_data.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

// Neutral gfxworld assets (docs/NEUTRAL_ASSETS.md), emitted by tools/gen_iw8_neutral.py from the IW8 structs and stream
// traversals: do not edit; change the spec (tools/neutral_families/gfxworld.json) or the generator.
namespace zonetool::asset_data
{
namespace gfxworld
{
inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 83> bsp_version_names{{
	{164, "navpower_5_4_0"},
	{165, "havok_2016_2_0"},
	{166, "havok_vertex_error_threshold"},
	{167, "edge_data"},
	{168, "ir_light"},
	{169, "contents_to_tools"},
	{170, "audio_propagation"},
	{171, "edge_octree"},
	{172, "havok_2017_1_0"},
	{173, "change_stairs_override"},
	{174, "support_aligned_lump"},
	{175, "edge_adjacency"},
	{176, "edge_mantle_on_over_flags"},
	{177, "edge_query_buckets"},
	{178, "physics_decal"},
	{179, "dynamic_edges"},
	{180, "disk_audio_pa_speakers"},
	{181, "dynamiclightsets"},
	{182, "skydome_kvp_removed"},
	{183, "decal_volume_blend_map"},
	{184, "decal_volume_normal_blend_mode"},
	{185, "havok_2018_1_0"},
	{186, "umbra_occluder_metadata"},
	{187, "decal_volume_uv_tranform"},
	{188, "mdao_volumes"},
	{189, "light_color_intensity"},
	{190, "disk_triangule_soup"},
	{191, "collison_partition_fix"},
	{192, "radiometric_scaled_removed"},
	{193, "transient_assets_removed"},
	{194, "lgv"},
	{195, "transient_grid"},
	{196, "increase_max_edge_octree_node"},
	{197, "transient_glassdef"},
	{198, "lgv_only"},
	{199, "static_model_collections"},
	{200, "32_bit_static_model_id"},
	{201, "skipped"},
	{202, "super_terrain"},
	{203, "edge_index_increase"},
	{204, "more_client_triggers"},
	{205, "ushort_lightmapindex"},
	{206, "edge_adjacency_corruption_fix"},
	{207, "normalized_reflection_probe"},
	{208, "client_script_triggers"},
	{209, "client_trigger_priority"},
	{210, "surface_increase"},
	{211, "havok_2018_2_0"},
	{212, "light_near_plane_bias"},
	{213, "client_trigger_localization"},
	{214, "soundclip_weapon_item_clip"},
	{215, "bsp_curvature"},
	{216, "collisiontiles"},
	{217, "trisoup_tree"},
	{218, "volumetric_fog_heightfade"},
	{219, "runtime_clutter"},
	{220, "smodel_collections_adaptive"},
	{221, "compressed_sun_shadow"},
	{222, "reflection_probe_octahedron"},
	{223, "light_collimation_factor"},
	{224, "super_terrain_lightmaps"},
	{225, "trisoup_tree_arrays"},
	{226, "smodel_collections_model_valid"},
	{227, "edge_adjacency_layout"},
	{228, "map_assets_tri_chart_info"},
	{229, "reflection_probe_transients"},
	{230, "rprobe_dls_flag"},
	{231, "skylight"},
	{232, "bspinfo_expanded"},
	{233, "smaller_collision_tiles"},
	{234, "runtime_clutter_bounds"},
	{235, "transient_glasspiece"},
	{236, "edge_transients"},
	{237, "physics_triangleobj"},
	{238, "decal_volume_transients"},
	{239, "client_trigger_soundbank"},
	{240, "smodel_collections_model_valid2"},
	{241, "simple_layering"},
	{242, "crc_flush"},
	{243, "volumetric_aniso_color"},
	{244, "world_transient_metadata"},
	{245, "sun_volume"},
	{4294967295, "invalid"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> gfx_surface_flags_names{{
	{1, "casts_sun_shadow"},
	{2, "casts_sun0_shadow"},
	{4, "casts_sun1_shadow"},
	{8, "casts_sun2_shadow"},
	{16, "casts_sun3_shadow"},
	{32, "casts_sun4_shadow"},
	{64, "is_umbra_occluder"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> static_model_flags_names{{
	{1, "lightprobe_lighting"},
	{2, "lightmap_lighting"},
	{4, "animated_opacity"},
	{8, "reactivemotion"},
	{16, "splined"},
	{32, "disable_volume_decals"},
	{64, "clutter_tint"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 8> static_model_collection_flags_names{{
	{1, "sun_shadow_1"},
	{2, "sun_shadow_2"},
	{4, "sun_shadow_3"},
	{8, "sun_shadow_4"},
	{16, "sun_shadow_x"},
	{32, "clutter"},
	{64, "splined"},
	{128, "umbra_occluder"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> gfx_light_grid_type_names{{
	{0, "none"},
	{1, "single"},
	{2, "multi"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> gfx_lightmap_type_names{{
	{0, "regular"},
	{1, "regular_uncompressed"},
	{2, "ab_lighting"},
	{3, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 4> volumetric_mask_type_names{{
	{0, "unused"},
	{1, "heightmap"},
	{2, "opacitymap"},
	{3, "type_count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 5> volumetric_axis_names{{
	{0, "x_axis"},
	{1, "y_axis"},
	{2, "z_axis"},
	{3, "axis_count"},
	{4, "negatve_axis_flag"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 3> gfx_dynamic_lightset_bf_type_names{{
	{0, "dls"},
	{1, "portalgi"},
	{2, "product"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 7> gfx_dynamic_lightset_g_chunk_flags_names{{
	{0, "count_empty"},
	{255, "count_mask"},
	{1792, "type_blend_count_mask"},
	{2048, "type_specialize_fast"},
	{4096, "type_specialize_slow"},
	{8192, "type_mixed_small"},
	{16384, "type_mixed_large"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 51> gfx_pixel_format_names{{
	{0, "invalid"},
	{1, "r8"},
	{2, "a8"},
	{3, "l8"},
	{4, "r8_g8"},
	{5, "l8_a8"},
	{6, "r8_g8_b8_a8"},
	{7, "r8_g8_b8_a8_srgb"},
	{8, "r8_snorm"},
	{9, "r8_g8_snorm"},
	{10, "r16"},
	{11, "r16_g16"},
	{12, "r16_g16_b16_a16"},
	{13, "r16_snorm"},
	{14, "r16_f"},
	{15, "r16_g16_f"},
	{16, "r16_g16_b16_a16_f"},
	{17, "r32_f"},
	{18, "r32_g32_f"},
	{19, "r32_g32_b32_a32_f"},
	{20, "d16"},
	{21, "d32_f"},
	{22, "d32_f_s8"},
	{23, "r8_uint"},
	{24, "r16_uint"},
	{25, "r32_uint"},
	{26, "r32_g32_uint"},
	{27, "r32_g32_b32_a32_uint"},
	{28, "r10_g10_b10_a2_uint"},
	{29, "r5_g6_b5"},
	{30, "r10_g10_b10_a2"},
	{31, "r9_g9_b9_e5_sharedexp"},
	{32, "r11_g11_b10_f"},
	{33, "bc1"},
	{34, "bc1_srgb"},
	{35, "bc2"},
	{36, "bc2_srgb"},
	{37, "bc3"},
	{38, "bc3_srgb"},
	{39, "bc4"},
	{40, "bc5"},
	{41, "bc5_s"},
	{42, "bc6_h"},
	{43, "bc6_hs"},
	{44, "bc7"},
	{45, "bc7_srgb"},
	{46, "r8_g8_b8_a8_snorm"},
	{47, "r1"},
	{48, "r4_g4"},
	{49, "r10_g10_b10_a2_snorm"},
	{50, "count"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 37> gfx_image_flags_names{{
	{0, "flag_none"},
	{1, "disk_flag_nopicmip"},
	{2, "disk_flag_nomipmaps"},
	{4, "disk_flag_unused"},
	{8, "disk_flag_normal_occluson_gloss"},
	{16, "disk_flag_clamp_u"},
	{32, "disk_flag_clamp_v"},
	{64, "disk_flag_streamed"},
	{128, "disk_flag_use_oodle_compression"},
	{256, "disk_flag_gamma_srgb"},
	{512, "disk_flag_packed_atlas"},
	{1024, "create_flag_untiled"},
	{2048, "create_flag_cpu_read"},
	{4096, "create_flag_cpu_write"},
	{8192, "disk_flag_autometalness"},
	{16384, "disk_flag_autodisplacement"},
	{32768, "disk_flag_maptype_cube"},
	{65536, "disk_flag_maptype_3_d"},
	{98304, "disk_flag_maptype_1_d"},
	{131072, "disk_flag_maptype_array"},
	{163840, "disk_flag_maptype_cube_array"},
	{262144, "disk_flag_invert_alpha"},
	{524288, "disk_flag_premul_alpha"},
	{1048576, "disk_flag_mipgen_lanczos3"},
	{2097152, "disk_flag_mipgen_catmull_rom"},
	{3145728, "disk_flag_mipgen_cubic_bspline"},
	{4194304, "disk_flag_mipgen_box"},
	{5242880, "disk_flag_mipgen_coverage_preserving"},
	{8388608, "create_flag_rw_view"},
	{16777216, "create_flag_dynamic"},
	{33554432, "disk_flag_premul_keep_alpha"},
	{67108864, "disk_flag_rtt"},
	{134217728, "disk_flag_extract_alpha"},
	{268435456, "disk_flag_octahedron"},
	{536870912, "create_flag_staging"},
	{1073741824, "create_flag_volumetric_layout_override"},
	{2147483648, "create_flag_typeless"},
}};

inline constexpr std::array<std::pair<std::int64_t, std::string_view>, 1> gfx_texture_id_names{{
	{0, "nullid"},
}};

struct gfx_world;
struct bounds;
struct gfx_world_dpvs_planes;
struct gfx_world_dpvs_plane;
struct gfx_cell;
struct gfx_portal;
struct gfx_portal_writable;
struct dpvs_plane;
struct gfx_cell_transient_info;
struct gfx_world_surfaces;
struct gfx_surface;
struct srf_triangles_t;
struct gfx_surface_bounds;
struct replay_surface_bounds_extra;
struct gfx_world_surf_data;
struct gfx_wrapped_buffer;
struct gfx_wrapped_base_buffer;
struct gfx_buffer_resource;
struct gfx_shader_buffer_view;
struct gfx_world_static_models;
struct gfx_clutter_instance_pool;
struct gfx_static_model_surface;
struct gfx_static_model;
struct gfx_clutter_collection;
struct gfx_static_model_collection;
struct gfx_static_model_instance;
struct gfx_model_static_indirection;
struct gfx_lightmap_transform_packed;
struct gfx_base_s_model_surf_data;
struct gfx_base_surf_data;
struct gfx_s_model_instance_data;
struct gfx_wrapped_rw_buffer;
struct gfx_shader_buffer_rw_view;
struct gfx_clutter_collection_gpu_data;
struct gfx_static_model_expansion;
struct gfx_static_model_lod;
struct gfx_s_model_surf_mat_indirection;
struct hlsl_gpu_splined_model_instance;
struct hlsl_gpu_spline_segment;
struct hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t;
struct bitarray_dynamic;
struct gfx_world_draw;
struct gfx_world_reflection_probe_data;
struct gfx_reflection_probe;
struct gfx_reflection_probe_relighting_data;
struct gfx_reflection_probe_instance;
struct gfx_reflection_probe_obb;
struct gfx_sh3_vec;
struct gfx_world_lightmap_reindex_data;
struct gfx_world_lightmap_reindex_data_element;
struct gfx_world_packed_lightmap;
struct gfx_static_decal_volume_material;
struct gfx_static_decal_volume_mask;
struct gfx_decal_volume_collection;
struct gfx_world_volumetrics;
struct gfx_volumetric;
struct gfx_oriented_bounding_box;
struct gfx_volumetric_mask;
struct gfx_compressed_sun_shadow_params;
struct gfx_dynamic_lightset;
struct gfx_dynamic_lightset_bf_data;
struct gfx_dynamic_lightset_light_info;
struct gfx_dynamic_lightset_portal_gi_data;
struct gfx_dynamic_lightset_product_gi_data;
struct gfx_dynamic_lightset_lm_data;
struct gfx_dynamic_lightset_g_chunk;
struct gfx_dynamic_lightset_g_chunk_info;
struct gfx_dynamic_lightset_probe_data;
struct gfx_sh_compressed_probe_data;
struct gfx_mayhem_self_vis;
struct gfx_frustum_lights;
struct gfx_light_view_frustum;
struct gfx_light;
struct gfx_voxel_tree;
struct gfx_voxel_tree_header;
struct gfx_voxel_top_down_view_node;
struct gfx_voxel_internal_node;
struct gfx_voxel_leaf_node;
struct replay_voxel_runtime_state;
struct gfx_heightfield;
struct gfx_precomputed_sky_illumination;
struct gfx_brush_model;
struct gfx_brush_model_writable;
struct gfx_model_lightmap_inst;
struct gfx_map_ent_lookup;
struct sunflare_t;
struct gfx_scene_dyn_model;
struct x_model_draw_info;
struct gfx_scene_dyn_brush;
struct b_model_draw_info;
struct gfx_shadow_geometry;
struct gfx_light_aabb;
struct gfx_light_aabb_node;
struct gfx_world_dpvs_static;
struct gfx_world_dpvs_dynamic;
struct umbra_gate;
struct mdao_volume;
struct extent_bounds;
struct stream_tree;
struct stream_tree_node;
struct stream_mtl_img_info;
struct stream_mtl_group;
struct stream_tree_grid;
struct gfx_world_transient_zone;
struct gfx_world_draw_verts;
struct gfx_index_buffer;
struct gfx_world_draw_cells;
struct gfx_cell_tree_count;
struct gfx_cell_tree;
struct gfx_aabb_tree;
struct gfx_lightmap_atlas_data_temp;
struct gfx_lightmap_raw;
struct gfx_image_raw;
struct gfx_image_desc;
struct gfx_gpu_light_grid_transient_data;
struct gfx_gpu_light_grid;
struct gfx_gpu_light_grid_zone;
struct gfx_sh_probe_data;
struct gfx_gpu_light_grid_voxel_tree;
struct gfx_gpu_light_grid_tetrahedron;
struct gfx_gpu_light_grid_tetrahedron_neighbors;
struct gfx_gpu_light_grid_tetrahedron_visibility;
struct gfx_gpu_light_grid_voxel_start_tetrahedron;
struct gfx_static_model_light_grid_volume;
struct gfx_dynamic_lightset_l_gpp_zone_info;
struct gfx_gpu_dyn_lightsets_lgv_reference;
struct gfx_light_grid_volume_atlas_data_temp;
struct light_grid_volume_packing_params;
struct light_grid_volume_transform_params;
struct gfx_reflection_probe_transient_data;
struct gfx_image_reflection_probe;
struct gfx_decal_volume_transient_data;
struct gfx_static_decal_volume_packed;
struct gfx_static_decal_volume_uid;
struct gfx_compressed_sun_shadow_data_temp;

inline void validate(const gfx_world& data);
inline void validate(const bounds& data);
inline void validate(const gfx_world_dpvs_planes& data);
inline void validate(const gfx_world_dpvs_plane& data);
inline void validate(const gfx_cell& data);
inline void validate(const gfx_portal& data);
inline void validate(const gfx_portal_writable& data);
inline void validate(const dpvs_plane& data);
inline void validate(const gfx_cell_transient_info& data);
inline void validate(const gfx_world_surfaces& data);
inline void validate(const gfx_surface& data);
inline void validate(const srf_triangles_t& data);
inline void validate(const gfx_surface_bounds& data);
inline void validate(const replay_surface_bounds_extra& data);
inline void validate(const gfx_world_surf_data& data);
inline void validate(const gfx_wrapped_buffer& data);
inline void validate(const gfx_wrapped_base_buffer& data);
inline void validate(const gfx_buffer_resource& data);
inline void validate(const gfx_shader_buffer_view& data);
inline void validate(const gfx_world_static_models& data);
inline void validate(const gfx_clutter_instance_pool& data);
inline void validate(const gfx_static_model_surface& data);
inline void validate(const gfx_static_model& data);
inline void validate(const gfx_clutter_collection& data);
inline void validate(const gfx_static_model_collection& data);
inline void validate(const gfx_static_model_instance& data);
inline void validate(const gfx_model_static_indirection& data);
inline void validate(const gfx_lightmap_transform_packed& data);
inline void validate(const gfx_base_s_model_surf_data& data);
inline void validate(const gfx_base_surf_data& data);
inline void validate(const gfx_s_model_instance_data& data);
inline void validate(const gfx_wrapped_rw_buffer& data);
inline void validate(const gfx_shader_buffer_rw_view& data);
inline void validate(const gfx_clutter_collection_gpu_data& data);
inline void validate(const gfx_static_model_expansion& data);
inline void validate(const gfx_static_model_lod& data);
inline void validate(const gfx_s_model_surf_mat_indirection& data);
inline void validate(const hlsl_gpu_splined_model_instance& data);
inline void validate(const hlsl_gpu_spline_segment& data);
inline void validate(const hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& data);
inline void validate(const bitarray_dynamic& data);
inline void validate(const gfx_world_draw& data);
inline void validate(const gfx_world_reflection_probe_data& data);
inline void validate(const gfx_reflection_probe& data);
inline void validate(const gfx_reflection_probe_relighting_data& data);
inline void validate(const gfx_reflection_probe_instance& data);
inline void validate(const gfx_reflection_probe_obb& data);
inline void validate(const gfx_sh3_vec& data);
inline void validate(const gfx_world_lightmap_reindex_data& data);
inline void validate(const gfx_world_lightmap_reindex_data_element& data);
inline void validate(const gfx_world_packed_lightmap& data);
inline void validate(const gfx_static_decal_volume_material& data);
inline void validate(const gfx_static_decal_volume_mask& data);
inline void validate(const gfx_decal_volume_collection& data);
inline void validate(const gfx_world_volumetrics& data);
inline void validate(const gfx_volumetric& data);
inline void validate(const gfx_oriented_bounding_box& data);
inline void validate(const gfx_volumetric_mask& data);
inline void validate(const gfx_compressed_sun_shadow_params& data);
inline void validate(const gfx_dynamic_lightset& data);
inline void validate(const gfx_dynamic_lightset_bf_data& data);
inline void validate(const gfx_dynamic_lightset_light_info& data);
inline void validate(const gfx_dynamic_lightset_portal_gi_data& data);
inline void validate(const gfx_dynamic_lightset_product_gi_data& data);
inline void validate(const gfx_dynamic_lightset_lm_data& data);
inline void validate(const gfx_dynamic_lightset_g_chunk& data);
inline void validate(const gfx_dynamic_lightset_g_chunk_info& data);
inline void validate(const gfx_dynamic_lightset_probe_data& data);
inline void validate(const gfx_sh_compressed_probe_data& data);
inline void validate(const gfx_mayhem_self_vis& data);
inline void validate(const gfx_frustum_lights& data);
inline void validate(const gfx_light_view_frustum& data);
inline void validate(const gfx_light& data);
inline void validate(const gfx_voxel_tree& data);
inline void validate(const gfx_voxel_tree_header& data);
inline void validate(const gfx_voxel_top_down_view_node& data);
inline void validate(const gfx_voxel_internal_node& data);
inline void validate(const gfx_voxel_leaf_node& data);
inline void validate(const replay_voxel_runtime_state& data);
inline void validate(const gfx_heightfield& data);
inline void validate(const gfx_precomputed_sky_illumination& data);
inline void validate(const gfx_brush_model& data);
inline void validate(const gfx_brush_model_writable& data);
inline void validate(const gfx_model_lightmap_inst& data);
inline void validate(const gfx_map_ent_lookup& data);
inline void validate(const sunflare_t& data);
inline void validate(const gfx_scene_dyn_model& data);
inline void validate(const x_model_draw_info& data);
inline void validate(const gfx_scene_dyn_brush& data);
inline void validate(const b_model_draw_info& data);
inline void validate(const gfx_shadow_geometry& data);
inline void validate(const gfx_light_aabb& data);
inline void validate(const gfx_light_aabb_node& data);
inline void validate(const gfx_world_dpvs_static& data);
inline void validate(const gfx_world_dpvs_dynamic& data);
inline void validate(const umbra_gate& data);
inline void validate(const mdao_volume& data);
inline void validate(const extent_bounds& data);
inline void validate(const stream_tree& data);
inline void validate(const stream_tree_node& data);
inline void validate(const stream_mtl_img_info& data);
inline void validate(const stream_mtl_group& data);
inline void validate(const stream_tree_grid& data);
inline void validate(const gfx_world_transient_zone& data);
inline void validate(const gfx_world_draw_verts& data);
inline void validate(const gfx_index_buffer& data);
inline void validate(const gfx_world_draw_cells& data);
inline void validate(const gfx_cell_tree_count& data);
inline void validate(const gfx_cell_tree& data);
inline void validate(const gfx_aabb_tree& data);
inline void validate(const gfx_lightmap_atlas_data_temp& data);
inline void validate(const gfx_lightmap_raw& data);
inline void validate(const gfx_image_raw& data);
inline void validate(const gfx_image_desc& data);
inline void validate(const gfx_gpu_light_grid_transient_data& data);
inline void validate(const gfx_gpu_light_grid& data);
inline void validate(const gfx_gpu_light_grid_zone& data);
inline void validate(const gfx_sh_probe_data& data);
inline void validate(const gfx_gpu_light_grid_voxel_tree& data);
inline void validate(const gfx_gpu_light_grid_tetrahedron& data);
inline void validate(const gfx_gpu_light_grid_tetrahedron_neighbors& data);
inline void validate(const gfx_gpu_light_grid_tetrahedron_visibility& data);
inline void validate(const gfx_gpu_light_grid_voxel_start_tetrahedron& data);
inline void validate(const gfx_static_model_light_grid_volume& data);
inline void validate(const gfx_dynamic_lightset_l_gpp_zone_info& data);
inline void validate(const gfx_gpu_dyn_lightsets_lgv_reference& data);
inline void validate(const gfx_light_grid_volume_atlas_data_temp& data);
inline void validate(const light_grid_volume_packing_params& data);
inline void validate(const light_grid_volume_transform_params& data);
inline void validate(const gfx_reflection_probe_transient_data& data);
inline void validate(const gfx_image_reflection_probe& data);
inline void validate(const gfx_decal_volume_transient_data& data);
inline void validate(const gfx_static_decal_volume_packed& data);
inline void validate(const gfx_static_decal_volume_uid& data);
inline void validate(const gfx_compressed_sun_shadow_data_temp& data);

struct bounds
{
	std::array<float, 3> mid_point{};
	std::array<float, 3> half_size{};

	bool operator==(const bounds&) const = default;
};

struct gfx_world_dpvs_planes
{
	std::uint32_t cell_count{};
	std::uint16_t plane_count{};
	std::optional<std::vector<gfx_world_dpvs_plane>> planes{};
	std::uint16_t node_count{};
	std::optional<std::vector<std::uint16_t>> nodes{};
	std::optional<std::vector<std::uint32_t>> scene_ent_cell_bits{};

	bool operator==(const gfx_world_dpvs_planes&) const = default;
};

struct gfx_buffer_resource
{
	std::uint64_t gpu_address{};
	std::uint64_t allocation{};
	std::uint32_t size{};

	bool operator==(const gfx_buffer_resource&) const = default;
};

struct gfx_shader_buffer_view
{
	std::uint32_t view{};

	bool operator==(const gfx_shader_buffer_view&) const = default;
};

struct gfx_wrapped_base_buffer
{
	gfx_buffer_resource buffer{};
	gfx_shader_buffer_view view{};

	bool operator==(const gfx_wrapped_base_buffer&) const = default;
};

struct gfx_wrapped_buffer
{
	gfx_wrapped_base_buffer base{};
	std::uint8_t flags{};

	bool operator==(const gfx_wrapped_buffer&) const = default;
};

struct gfx_world_surfaces
{
	std::uint32_t count{};
	std::uint32_t surf_data_count{};
	std::uint32_t lit_opaque_surfs_begin{};
	std::uint32_t lit_opaque_surfs_end{};
	std::uint32_t lit_decal_surfs_begin{};
	std::uint32_t lit_decal_surfs_end{};
	std::uint32_t lit_trans_surfs_begin{};
	std::uint32_t lit_trans_surfs_end{};
	std::uint32_t emissive_surfs_begin{};
	std::uint32_t emissive_surfs_end{};
	std::optional<std::vector<gfx_surface>> surfaces{};
	std::optional<std::vector<gfx_surface_bounds>> surface_bounds{};
	std::optional<std::vector<std::array<std::uint64_t, 2>>> surface_materials{};
	std::optional<std::vector<gfx_world_surf_data>> surf_data{};
	gfx_wrapped_buffer surf_data_buffer{};

	bool operator==(const gfx_world_surfaces&) const = default;
};

struct gfx_clutter_instance_pool
{
	std::uint32_t start{};
	std::uint32_t size{};

	bool operator==(const gfx_clutter_instance_pool&) const = default;
};

struct gfx_shader_buffer_rw_view
{
	std::uint64_t gpu_address{};
	std::uint64_t allocation{};
	std::uint32_t view{};

	bool operator==(const gfx_shader_buffer_rw_view&) const = default;
};

struct gfx_wrapped_rw_buffer
{
	gfx_wrapped_buffer buffer{};
	gfx_shader_buffer_rw_view rw_view{};

	bool operator==(const gfx_wrapped_rw_buffer&) const = default;
};

struct bitarray_dynamic
{
	std::optional<std::vector<std::uint32_t>> array{};
	std::uint32_t word_count{};
	std::uint32_t bit_count{};

	bool operator==(const bitarray_dynamic&) const = default;
};

struct gfx_world_static_models
{
	std::uint32_t surface_count{};
	std::uint32_t model_count{};
	std::uint32_t collection_count{};
	std::uint32_t clutter_collection_count{};
	std::uint32_t instance_count{};
	std::uint32_t clutter_tint_count{};
	std::uint32_t lmap_transforms_count{};
	std::uint32_t material_count{};
	std::uint32_t collection_vis_data_count{};
	std::uint32_t splined_model_instance_count{};
	std::uint32_t spline_segment_count{};
	gfx_clutter_instance_pool clutter_instance_pool{};
	std::optional<std::vector<gfx_static_model_surface>> surfaces{};
	std::optional<std::vector<gfx_static_model>> models{};
	std::optional<std::vector<gfx_clutter_collection>> clutter_collections{};
	std::optional<std::vector<gfx_static_model_collection>> collections{};
	std::optional<std::vector<gfx_static_model_instance>> instances{};
	std::optional<std::vector<std::uint8_t>> instance_flags{};
	std::optional<std::vector<bounds>> collection_bounds{};
	std::optional<std::vector<std::optional<std::string>>> materials{};
	std::optional<std::vector<gfx_model_static_indirection>> model_static_indirection{};
	gfx_wrapped_buffer model_static_indirection_buffer{};
	std::optional<std::vector<gfx_lightmap_transform_packed>> lmap_transforms{};
	gfx_wrapped_buffer lmap_transforms_buffer{};
	std::optional<std::vector<gfx_base_s_model_surf_data>> smodel_surf_data{};
	gfx_wrapped_buffer smodel_surf_data_buffer{};
	std::optional<std::vector<gfx_s_model_instance_data>> smodel_instance_data{};
	gfx_wrapped_rw_buffer smodel_instance_data_buffer{};
	std::optional<std::vector<gfx_clutter_collection_gpu_data>> clutter_collection_gpu_data{};
	gfx_wrapped_rw_buffer clutter_collection_gpu_data_buffer{};
	std::optional<std::vector<std::array<float, 3>>> clutter_tint_data{};
	gfx_wrapped_rw_buffer clutter_tint_data_buffer{};
	std::optional<std::vector<gfx_static_model_expansion>> smodel_expansion_data{};
	gfx_wrapped_buffer smodel_expansion_data_buffer{};
	std::optional<std::vector<gfx_s_model_surf_mat_indirection>> smodel_surf_mat_indirection{};
	gfx_wrapped_buffer smodel_surf_mat_indirection_buffer{};
	gfx_wrapped_buffer clutter_collections_data_buffer{};
	gfx_wrapped_buffer smodel_collections_data_buffer{};
	std::optional<std::vector<std::uint32_t>> smodel_surf_ugb_indirection{};
	gfx_wrapped_rw_buffer smodel_surf_ugb_indirection_buffer{};
	std::optional<std::vector<hlsl_gpu_splined_model_instance>> splined_model_instance_data{};
	gfx_wrapped_buffer splined_model_instance_buffer{};
	std::optional<std::vector<hlsl_gpu_spline_segment>> spline_segment_data{};
	gfx_wrapped_buffer spline_segment_buffer{};
	std::optional<std::vector<bounds>> splined_model_instance_bounds{};
	gfx_wrapped_buffer splined_model_instance_bounds_buffer{};
	bitarray_dynamic collection_transient_zone_prev_frame_visibility{};
	std::optional<std::vector<std::uint32_t>> collection_vis_data{};
	std::optional<std::vector<std::uint8_t>> collection_vis_ref_count{};

	bool operator==(const gfx_world_static_models&) const = default;
};

struct gfx_world_reflection_probe_data
{
	std::uint32_t reflection_probe_count{};
	std::uint32_t shared_reflection_probe_count{};
	std::optional<std::vector<std::optional<std::string>>> reflection_probe_images{};
	std::optional<std::vector<std::optional<std::string>>> reflection_probe_images_octahedron{};
	std::optional<std::vector<gfx_reflection_probe>> reflection_probes{};
	std::optional<std::string> reflection_probe_array_image{};
	std::optional<std::string> reflection_probe_array_image_octahedron{};
	std::uint32_t probe_relighting_count{};
	std::optional<std::vector<gfx_reflection_probe_relighting_data>> probe_relighting_data{};
	std::uint32_t reflection_probe_g_buffer_image_count{};
	std::optional<std::vector<std::optional<std::string>>> reflection_probe_g_buffer_images{};
	std::optional<std::vector<std::optional<std::string>>> reflection_probe_g_buffer_images_octahedron{};
	std::uint32_t reflection_probe_instance_count{};
	std::optional<std::vector<gfx_reflection_probe_instance>> reflection_probe_instances{};
	std::optional<std::vector<gfx_sh3_vec>> reflection_probe_sh{};
	gfx_wrapped_rw_buffer reflection_probe_sh_buffer{};

	bool operator==(const gfx_world_reflection_probe_data&) const = default;
};

struct gfx_world_lightmap_reindex_data
{
	std::uint32_t image_pixel_size{};
	std::uint32_t reindex_count{};
	std::optional<std::vector<gfx_world_lightmap_reindex_data_element>> reindex_element{};
	std::uint32_t packed_lightmap_count{};
	std::optional<std::vector<gfx_world_packed_lightmap>> packed_lightmap{};

	bool operator==(const gfx_world_lightmap_reindex_data&) const = default;
};

struct gfx_world_volumetrics
{
	std::uint32_t volumetric_count{};
	std::optional<std::vector<gfx_volumetric>> volumetrics{};
	std::optional<std::string> volumetric_mask_atlas_image{};

	bool operator==(const gfx_world_volumetrics&) const = default;
};

struct gfx_compressed_sun_shadow_params
{
	std::array<float, 3> sun_direction{};
	std::uint32_t resolution{};
	float center_x{};
	float center_y{};
	float sample_size{};
	float near_plane{};
	float far_plane{};
	std::uint32_t forest_size{};
	std::uint32_t flags{};
	std::uint32_t reserved{};

	bool operator==(const gfx_compressed_sun_shadow_params&) const = default;
};

struct gfx_world_draw
{
	gfx_world_reflection_probe_data reflection_probe_data{};
	gfx_world_lightmap_reindex_data lightmap_reindex_data{};
	std::optional<std::string> ies_lookup_texture{};
	std::uint32_t decal_volume_version{};
	std::uint32_t decal_volume_reserved0{};
	std::uint32_t decal_volume_reserved1{};
	std::uint32_t decal_volume_reserved2{};
	std::uint32_t decal_volume_material_count{};
	std::uint32_t decal_volume_mask_count{};
	std::uint32_t map_decal_volume_count{};
	std::uint32_t decal_volume_collection_count{};
	std::optional<std::vector<gfx_static_decal_volume_material>> decal_volume_materials{};
	std::optional<std::vector<gfx_static_decal_volume_mask>> decal_volume_masks{};
	std::optional<std::vector<gfx_decal_volume_collection>> decal_volume_collections{};
	std::optional<std::vector<std::uint32_t>> material_and_mask_indirection{};
	gfx_wrapped_buffer material_and_mask_indirection_buffer{};
	std::string light_grid_type{};
	std::string lightmap_type{};
	std::int32_t lightmap_count{};
	std::uint32_t transient_zone_count{};
	std::optional<std::vector<std::optional<std::string>>> transient_zones{};
	std::optional<std::vector<std::uint32_t>> lightmap_transient_index{};
	gfx_world_volumetrics volumetrics{};
	float baked_light_scale{};
	std::uint32_t compressed_sun_shadow_size{};
	std::optional<std::vector<std::uint8_t>> compressed_sun_shadow{};
	gfx_wrapped_rw_buffer compressed_sun_shadow_buffer{};
	gfx_compressed_sun_shadow_params compressed_sun_shadow_params{};

	bool operator==(const gfx_world_draw&) const = default;
};

struct gfx_dynamic_lightset_lm_data
{
	std::uint32_t num_g_chunk_lm{};
	std::optional<std::vector<gfx_dynamic_lightset_g_chunk>> chunk_glm{};
	std::uint32_t num_g_chunk_lmbf{};
	std::optional<std::vector<std::uint16_t>> chunk_glmbf{};
	std::uint32_t num_lm_have{};
	std::optional<std::vector<std::uint16_t>> chunk_which_lm{};
	std::optional<std::vector<std::uint32_t>> chunk_lm_start{};
	std::uint32_t num_orig_texels{};
	std::optional<std::vector<std::uint32_t>> orig_texels{};
	gfx_wrapped_buffer orig_texels_buffer{};
	std::optional<std::vector<std::uint32_t>> orig_records{};
	gfx_wrapped_buffer orig_records_buffer{};
	std::uint32_t num_bflm_texels{};
	std::optional<std::vector<std::uint32_t>> bf_texels{};
	gfx_wrapped_buffer bf_texels_buffer{};
	std::uint32_t num_skip_records{};
	std::optional<std::vector<std::uint16_t>> skip_records{};
	gfx_wrapped_buffer skip_records_buffer{};
	std::optional<std::vector<std::uint32_t>> pallette_records{};
	gfx_wrapped_buffer pallette_records_buffer{};

	bool operator==(const gfx_dynamic_lightset_lm_data&) const = default;
};

struct gfx_dynamic_lightset_probe_data
{
	std::uint32_t num_g_chunk{};
	std::optional<std::vector<gfx_dynamic_lightset_g_chunk>> chunk_g{};
	std::uint32_t num_g_chunk_bf{};
	std::optional<std::vector<std::uint16_t>> chunk_gbf{};
	std::uint32_t orig_start{};
	std::uint32_t num_orig_probes{};
	std::optional<std::vector<gfx_sh_compressed_probe_data>> orig_probes{};
	gfx_wrapped_buffer orig_probes_buffer{};
	std::uint32_t num_bf_probes{};
	std::optional<std::vector<gfx_sh_compressed_probe_data>> bf_probes{};
	gfx_wrapped_buffer bf_probes_buffer{};
	std::uint32_t num_skip_records{};
	std::optional<std::vector<std::uint16_t>> skip_records{};
	gfx_wrapped_buffer skip_records_buffer{};
	std::optional<std::vector<std::uint32_t>> pallette_records{};
	gfx_wrapped_buffer pallette_records_buffer{};
	std::optional<std::vector<std::uint32_t>> temp_remap{};
	gfx_wrapped_buffer temp_remap_buffer{};

	bool operator==(const gfx_dynamic_lightset_probe_data&) const = default;
};

struct gfx_dynamic_lightset
{
	std::uint32_t num_basis_functions{};
	std::optional<std::vector<gfx_dynamic_lightset_bf_data>> basis_functions{};
	std::uint32_t num_basis_function_primary_lights{};
	std::optional<std::vector<gfx_dynamic_lightset_light_info>> basis_function_lights{};
	std::uint32_t num_portal_gi{};
	std::optional<std::vector<gfx_dynamic_lightset_portal_gi_data>> portal_gi_data{};
	std::uint32_t num_product_gi{};
	std::optional<std::vector<gfx_dynamic_lightset_product_gi_data>> product_gi_data{};
	gfx_dynamic_lightset_lm_data lm_data{};
	gfx_dynamic_lightset_probe_data lg_probe_data{};
	std::uint32_t num_dls_basis_functions{};

	bool operator==(const gfx_dynamic_lightset&) const = default;
};

struct gfx_mayhem_self_vis
{
	std::uint32_t self_vis_buffer_size{};
	std::optional<std::vector<std::uint8_t>> self_vis{};
	gfx_wrapped_rw_buffer self_vis_buffer{};

	bool operator==(const gfx_mayhem_self_vis&) const = default;
};

struct gfx_frustum_lights
{
	std::uint32_t light_count{};
	std::uint32_t total_indices_count{};
	std::uint32_t total_vertices_count{};
	std::optional<std::vector<std::uint32_t>> indices_offsets{};
	std::optional<std::vector<std::uint32_t>> vertices_offsets{};
	std::optional<std::vector<std::uint16_t>> indices_counts{};
	std::optional<std::vector<std::uint16_t>> vertices_counts{};
	std::optional<std::vector<std::uint16_t>> indices{};
	std::optional<std::vector<std::uint8_t>> vertices{};
	gfx_wrapped_buffer index_buffer{};
	gfx_wrapped_buffer vertex_buffer{};

	bool operator==(const gfx_frustum_lights&) const = default;
};

struct gfx_precomputed_sky_illumination
{
	std::optional<std::string> detail_mask{};
	std::optional<std::string> height_map{};
	std::optional<std::string> svd_coefficient{};
	std::optional<std::string> svd_basis{};
	std::array<float, 4> box_min{};
	std::array<float, 4> box_max{};
	std::array<float, 4> svd_coefficient_min_max{};
	std::array<float, 4> height_consts{};

	bool operator==(const gfx_precomputed_sky_illumination&) const = default;
};

struct sunflare_t
{
	bool has_valid_data{};
	std::optional<std::string> sprite_material{};
	std::optional<std::string> flare_material{};
	float sprite_size{};
	float flare_min_size{};
	float flare_min_dot{};
	float flare_max_size{};
	float flare_max_dot{};
	float flare_max_alpha{};
	std::int32_t flare_fade_in_time{};
	std::int32_t flare_fade_out_time{};
	float blind_min_dot{};
	float blind_max_dot{};
	float blind_max_darken{};
	std::int32_t blind_fade_in_time{};
	std::int32_t blind_fade_out_time{};
	float glare_min_dot{};
	float glare_max_dot{};
	float glare_max_lighten{};
	std::int32_t glare_fade_in_time{};
	std::int32_t glare_fade_out_time{};
	std::array<float, 3> sun_fx_position{};

	bool operator==(const sunflare_t&) const = default;
};

struct gfx_light_aabb
{
	std::uint16_t node_count{};
	std::uint16_t light_count{};
	std::optional<std::vector<gfx_light_aabb_node>> node_array{};
	std::optional<std::vector<std::uint16_t>> light_array{};

	bool operator==(const gfx_light_aabb&) const = default;
};

struct gfx_world_dpvs_static
{
	std::uint32_t smodel_vis_data_count{};
	std::uint32_t surface_vis_data_count{};
	std::uint32_t primary_light_vis_data_count{};
	std::uint32_t reflection_probe_vis_data_count{};
	std::uint32_t volumetric_vis_data_count{};
	std::uint32_t decal_vis_data_count{};
	std::array<std::optional<std::vector<std::uint32_t>>, 33> smodel_vis_data{};
	std::array<std::optional<std::vector<std::uint32_t>>, 33> surface_vis_data{};
	std::optional<std::vector<std::uint32_t>> primary_light_vis_data{};
	std::optional<std::vector<std::uint32_t>> reflection_probe_vis_data{};
	std::optional<std::vector<std::uint32_t>> volumetric_vis_data{};
	std::optional<std::vector<std::uint32_t>> decal_vis_data{};
	std::optional<std::vector<std::uint32_t>> sorted_surf_index{};
	std::uint32_t static_model_id_count{};
	std::optional<std::vector<std::uint16_t>> sorted_smodel_indices{};
	std::optional<std::vector<std::uint32_t>> surface_casts_sun_shadow{};
	std::uint32_t sun_shadow_opt_count{};
	std::uint32_t sun_surf_vis_data_count{};
	std::optional<std::vector<std::uint32_t>> surface_casts_sun_shadow_opt{};

	bool operator==(const gfx_world_dpvs_static&) const = default;
};

struct gfx_world_dpvs_dynamic
{
	std::array<std::uint32_t, 2> dyn_ent_client_word_count{};
	std::array<std::uint32_t, 2> dyn_ent_client_count{};
	std::optional<std::vector<std::uint32_t>> dyn_ent_cell_bits_0{};
	std::optional<std::vector<std::uint32_t>> dyn_ent_cell_bits_1{};
	std::array<std::array<std::optional<std::vector<std::uint8_t>>, 33>, 2> dyn_ent_vis_data{};

	bool operator==(const gfx_world_dpvs_dynamic&) const = default;
};

struct stream_mtl_group
{
	std::optional<std::string> volumes_key{};
	std::optional<std::vector<stream_mtl_img_info>> infos{};
	std::uint16_t mtl_info_count{};
	std::uint16_t img_info_count{};
	std::uint16_t transient_zone{};

	bool operator==(const stream_mtl_group&) const = default;
};

struct stream_tree
{
	std::optional<std::vector<stream_tree_node>> nodes{};
	std::optional<std::vector<std::optional<std::string>>> materials{};
	std::optional<std::vector<std::optional<std::string>>> images{};
	std::uint32_t node_count{};
	std::uint32_t material_count{};
	std::uint32_t image_count{};
	stream_mtl_group large_mtl_group{};
	std::uint32_t memory_size{};
	std::array<float, 3> mins{};
	std::array<float, 3> maxs{};

	bool operator==(const stream_tree&) const = default;
};

struct extent_bounds
{
	std::array<float, 3> mins{};
	std::array<float, 3> maxs{};

	bool operator==(const extent_bounds&) const = default;
};

struct stream_tree_grid
{
	std::optional<std::vector<std::optional<std::string>>> cell_keys{};
	std::optional<std::vector<std::optional<std::string>>> materials{};
	std::optional<std::vector<std::optional<std::string>>> images{};
	std::optional<std::vector<std::optional<std::string>>> xmodels{};
	std::optional<std::vector<std::uint16_t>> smodel_collection_indices{};
	extent_bounds bounds{};
	std::uint32_t version{};
	std::uint16_t row_count{};
	std::uint16_t column_count{};
	std::uint32_t material_count{};
	std::uint32_t image_count{};
	std::uint32_t xmodel_count{};
	std::uint32_t smodel_collection_index_count{};

	bool operator==(const stream_tree_grid&) const = default;
};

struct gfx_world
{
	std::optional<std::string> name{};
	std::optional<std::string> base_name{};
	std::string bsp_version{};
	std::uint32_t last_sun_primary_light_index{};
	std::uint32_t primary_light_count{};
	std::uint32_t first_mutable_primary_light{};
	std::uint32_t mutable_primary_light_count{};
	std::uint32_t first_static_scriptables_primary_light{};
	std::uint32_t static_scriptables_primary_light_count{};
	std::uint32_t first_static_scriptable_primary_light{};
	std::uint32_t static_scriptable_primary_light_count{};
	std::uint32_t first_moving_scriptable_primary_light{};
	std::uint32_t moving_scriptable_primary_light_count{};
	std::uint32_t sort_key_lit_decal{};
	std::uint32_t sort_key_effect_decal{};
	std::uint32_t sort_key_top_decal{};
	std::uint32_t sort_key_effect_auto{};
	std::uint32_t sort_key_distortion{};
	std::uint32_t sort_key_effect_distortion{};
	std::uint32_t sort_key2_d{};
	std::uint32_t sort_key_opaque_begin{};
	std::uint32_t sort_key_opaque_end{};
	std::uint32_t sort_key_decal_begin{};
	std::uint32_t sort_key_decal_end{};
	std::uint32_t sort_key_trans_begin{};
	std::uint32_t sort_key_trans_end{};
	std::uint32_t sort_key_emissive_begin{};
	std::uint32_t sort_key_emissive_end{};
	bounds bounds{};
	gfx_world_dpvs_planes dpvs_planes{};
	std::optional<std::vector<gfx_cell>> cells{};
	std::optional<std::vector<gfx_cell_transient_info>> cell_transient_infos{};
	gfx_world_surfaces surfaces{};
	gfx_world_static_models smodels{};
	gfx_world_draw draw{};
	gfx_dynamic_lightset dynamic_lightset{};
	gfx_mayhem_self_vis mayhem_self_vis{};
	gfx_frustum_lights frustum_lights{};
	std::optional<std::vector<gfx_light_view_frustum>> light_view_frustums{};
	std::optional<std::vector<gfx_light>> primary_lights{};
	std::int32_t voxel_tree_count{};
	std::optional<std::vector<gfx_voxel_tree>> voxel_tree{};
	std::int32_t heightfield_count{};
	std::optional<std::vector<gfx_heightfield>> heightfields{};
	gfx_precomputed_sky_illumination precomputed_sky_illumination{};
	std::int32_t model_count{};
	std::optional<std::vector<gfx_brush_model>> models{};
	std::uint32_t lmap_lookup_count{};
	std::optional<std::vector<gfx_model_lightmap_inst>> lmap_lookup{};
	std::uint32_t gfx_map_ent_lookup_count{};
	std::optional<std::vector<gfx_map_ent_lookup>> gfx_map_ent_lookup{};
	std::uint32_t checksum{};
	sunflare_t sun{};
	std::array<std::array<float, 4>, 4> outdoor_lookup_matrix{};
	std::optional<std::string> outdoor_image{};
	std::optional<std::string> dust_material{};
	float material_lod0_size_threshold{};
	std::optional<std::vector<std::uint32_t>> cell_caster_bits{};
	std::optional<std::vector<std::uint32_t>> cell_has_sun_lit_surfs_bits{};
	std::optional<std::vector<gfx_scene_dyn_model>> scene_dyn_model{};
	std::optional<std::vector<gfx_scene_dyn_brush>> scene_dyn_brush{};
	std::uint32_t primary_light_motion_detect_bits_entries{};
	std::optional<std::vector<std::uint32_t>> primary_light_motion_detect_bits{};
	std::uint32_t entity_motion_bits_entries{};
	std::optional<std::vector<std::uint32_t>> entity_motion_bits{};
	std::uint32_t static_spot_omni_primary_light_count_aligned{};
	std::uint32_t shadow_bits_array_pitch{};
	std::uint32_t num_primary_light_entity_shadow_vis_entries{};
	std::optional<std::vector<std::uint32_t>> primary_light_entity_shadow_vis{};
	std::array<std::uint32_t, 2> dyn_ent_motion_bits_entries{};
	std::optional<std::vector<std::uint32_t>> dyn_ent_motion_bits_0{};
	std::optional<std::vector<std::uint32_t>> dyn_ent_motion_bits_1{};
	std::array<std::uint32_t, 2> num_primary_light_dyn_ent_shadow_vis_entries{};
	std::optional<std::vector<std::uint32_t>> primary_light_dyn_ent_shadow_vis_0{};
	std::optional<std::vector<std::uint32_t>> primary_light_dyn_ent_shadow_vis_1{};
	std::optional<std::vector<gfx_shadow_geometry>> shadow_geom_optimized{};
	gfx_light_aabb light_aabb{};
	gfx_world_dpvs_static dpvs{};
	gfx_world_dpvs_dynamic dpvs_dyn{};
	std::uint32_t num_umbra_gates{};
	std::optional<std::vector<umbra_gate>> umbra_gates{};
	std::uint32_t umbra_tome_size{};
	std::optional<std::vector<std::uint8_t>> umbra_tome_data{};
	std::uint32_t mdao_volume_count{};
	std::optional<std::vector<mdao_volume>> mdao_volumes{};
	bool enable_mdao{};
	stream_tree stream_tree{};
	stream_tree_grid material_stream_tree_grid{};
	stream_tree_grid xmodel_stream_tree_grid{};

	bool operator==(const gfx_world&) const = default;
};

struct gfx_world_dpvs_plane
{
	std::array<float, 3> normal{};
	float dist{};
	std::uint8_t type{};

	bool operator==(const gfx_world_dpvs_plane&) const = default;
};

struct gfx_cell
{
	bounds bounds{};
	std::uint16_t portal_count{};
	std::optional<std::vector<gfx_portal>> portals{};

	bool operator==(const gfx_cell&) const = default;
};

struct gfx_portal_writable
{
	bool is_queued{};
	bool is_ancestor{};
	std::uint8_t recursion_depth{};
	std::uint8_t hull_point_count{};

	bool operator==(const gfx_portal_writable&) const = default;
};

struct dpvs_plane
{
	std::array<float, 4> coeffs{};

	bool operator==(const dpvs_plane&) const = default;
};

struct gfx_portal
{
	gfx_portal_writable writable{};
	dpvs_plane plane{};
	std::optional<std::vector<std::array<float, 3>>> vertices{};
	std::uint16_t cell_index{};
	std::uint16_t close_distance{};
	std::uint8_t vertex_count{};
	std::array<std::array<float, 3>, 2> hull_axis{};

	bool operator==(const gfx_portal&) const = default;
};

struct gfx_cell_transient_info
{
	std::uint16_t aabb_tree_index{};
	std::uint16_t transient_zone{};

	bool operator==(const gfx_cell_transient_info&) const = default;
};

struct srf_triangles_t
{
	std::uint32_t pos_offset{};
	std::int32_t displacement_parms_offset{};
	std::uint16_t vertex_count{};
	std::uint16_t tri_count{};
	std::uint32_t base_index{};

	bool operator==(const srf_triangles_t&) const = default;
};

struct gfx_surface
{
	srf_triangles_t tris{};
	std::optional<std::string> material{};
	std::uint32_t surf_data_index{};
	std::uint16_t transient_zone{};
	std::uint16_t lightmap_index{};
	std::vector<std::string> flags{};

	bool operator==(const gfx_surface&) const = default;
};

struct replay_surface_bounds_extra
{
	float scalar{};
	std::array<std::uint32_t, 7> reserved{};

	bool operator==(const replay_surface_bounds_extra&) const = default;
};

struct gfx_surface_bounds
{
	bounds bounds{};
	replay_surface_bounds_extra serialized_extra{};

	bool operator==(const gfx_surface_bounds&) const = default;
};

struct gfx_world_surf_data
{
	std::uint32_t transient_zone{};
	std::uint32_t layer_count{};
	std::uint32_t xyz_offset{};
	std::uint32_t tangent_frame_offset{};
	std::uint32_t lmap_coord_offset{};
	std::uint32_t color_offset{};
	std::uint32_t tex_coord_offset{};
	std::array<std::uint32_t, 7> normal_transform_offset{};
	std::array<std::uint32_t, 8> displacement_offset{};

	bool operator==(const gfx_world_surf_data&) const = default;
};

struct gfx_static_model_surface
{
	std::uint16_t smodel_index{};
	std::uint8_t lod_index{};
	std::uint8_t surf_index{};

	bool operator==(const gfx_static_model_surface&) const = default;
};

struct gfx_static_model
{
	std::optional<std::string> model{};
	std::vector<std::string> flags{};
	std::uint8_t first_mtl_skin_index{};
	std::uint16_t first_static_model_surface_index{};

	bool operator==(const gfx_static_model&) const = default;
};

struct gfx_clutter_collection
{
	std::uint32_t max_instance_count{};
	std::uint16_t smodel_collection_index{};
	std::uint16_t terrain_surface_index{};
	std::uint16_t clutter_layer_index{};

	bool operator==(const gfx_clutter_collection&) const = default;
};

struct gfx_static_model_collection
{
	std::uint32_t first_instance{};
	std::uint32_t instance_count{};
	std::uint16_t smodel_index{};
	std::uint16_t transient_gfx_world_placed{};
	std::uint16_t clutter_index{};
	std::vector<std::string> flags{};

	bool operator==(const gfx_static_model_collection&) const = default;
};

struct gfx_static_model_instance
{
	std::uint32_t lgv_index_transient_index{};

	bool operator==(const gfx_static_model_instance&) const = default;
};

struct gfx_model_static_indirection
{
	std::uint32_t lighting_instance_indirection{};
	std::uint16_t splined_model_instance_index{};
	std::uint16_t world_transient{};

	bool operator==(const gfx_model_static_indirection&) const = default;
};

struct gfx_lightmap_transform_packed
{
	std::uint32_t top_left_index_low_packed{};
	std::uint32_t bottom_right_index_high_packed{};

	bool operator==(const gfx_lightmap_transform_packed&) const = default;
};

struct gfx_base_surf_data
{
	std::array<float, 4> bounding_box_data{};

	bool operator==(const gfx_base_surf_data&) const = default;
};

struct gfx_base_s_model_surf_data
{
	gfx_base_surf_data unnamed{};
	std::array<float, 3> bounding_box_half_size{};
	float draw_distance{};

	bool operator==(const gfx_base_s_model_surf_data&) const = default;
};

struct gfx_s_model_instance_data
{
	std::array<std::int32_t, 3> translation{};
	std::array<std::uint32_t, 2> orientation{};
	float scale{};

	bool operator==(const gfx_s_model_instance_data&) const = default;
};

struct gfx_clutter_collection_gpu_data
{
	std::uint32_t first_instance{};
	std::uint32_t instance_count{};

	bool operator==(const gfx_clutter_collection_gpu_data&) const = default;
};

struct gfx_static_model_lod
{
	std::uint8_t surf_offset{};
	std::uint8_t surf_count{};

	bool operator==(const gfx_static_model_lod&) const = default;
};

struct gfx_static_model_expansion
{
	std::uint16_t first_static_model_surface_index{};
	std::uint8_t num_surfs{};
	std::uint8_t num_lods{};
	bounds model_bounds{};
	std::array<gfx_static_model_lod, 6> lod_info{};
	std::array<float, 6> lod_dist{};

	bool operator==(const gfx_static_model_expansion&) const = default;
};

struct gfx_s_model_surf_mat_indirection
{
	std::uint16_t smodel_material_index{};
	std::uint8_t cull_type{};
	std::uint8_t emit_type{};

	bool operator==(const gfx_s_model_surf_mat_indirection&) const = default;
};

struct hlsl_gpu_splined_model_instance
{
	float model_x_extent{};
	float instance_length{};
	std::array<float, 3> model_spline_origin{};
	float dist_from_start_node{};
	float model_scale{};
	float up_down_offset{};
	float left_right_offset{};
	std::uint32_t segment_begin{};
	std::uint32_t segment_end{};
	std::uint32_t splining_axis_type{};

	bool operator==(const hlsl_gpu_splined_model_instance&) const = default;
};

struct hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t
{
	std::array<std::array<float, 3>, 3> m{};

	bool operator==(const hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t&) const = default;
};

struct hlsl_gpu_spline_segment
{
	float dist_from_start_node{};
	float length{};
	std::array<std::array<float, 3>, 4> bezier_eval{};
	std::array<std::array<float, 3>, 3> ent_bezier_der_eval{};
	hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t ent_to_aligned{};
	hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t model_to_wld{};
	std::array<float, 3> model_to_wld_origin{};
	std::array<float, 4> bank_angle_bezier_eval{};

	bool operator==(const hlsl_gpu_spline_segment&) const = default;
};

struct gfx_reflection_probe
{
	std::optional<std::string> live_path{};
	std::array<float, 3> origin{};
	std::array<float, 3> angles{};
	std::optional<std::vector<std::uint16_t>> probe_instances{};
	std::uint16_t probe_instance_count{};
	std::uint16_t probe_relighting_index{};
	std::uint32_t probe_image_indirection{};

	bool operator==(const gfx_reflection_probe&) const = default;
};

struct gfx_reflection_probe_relighting_data
{
	std::uint32_t reflection_probe_index{};
	std::uint16_t relighting_flags{};
	std::uint16_t zone_count{};
	std::uint32_t g_buffer_albedo_image_index{};
	std::uint32_t g_buffer_normal_image_index{};
	std::uint32_t g_buffer_secondary_diffuse_image_index{};
	float relighting_scale{};
	std::optional<std::vector<std::uint16_t>> zones{};

	bool operator==(const gfx_reflection_probe_relighting_data&) const = default;
};

struct gfx_reflection_probe_obb
{
	std::array<float, 3> center{};
	std::array<float, 3> x_axis{};
	std::array<float, 3> y_axis{};
	std::array<float, 3> z_axis{};
	std::array<float, 3> half_size{};

	bool operator==(const gfx_reflection_probe_obb&) const = default;
};

struct gfx_reflection_probe_instance
{
	std::optional<std::string> live_path{};
	std::array<float, 3> probe_position{};
	std::uint16_t probe_image_index{};
	std::uint8_t lod_level{};
	std::uint8_t flags{};
	std::array<float, 4> probe_rotation{};
	gfx_reflection_probe_obb volume_obb{};
	float priority{};
	std::array<float, 3> feather{};
	std::array<float, 3> expand_projection_neg{};
	std::array<float, 3> expand_projection_pos{};

	bool operator==(const gfx_reflection_probe_instance&) const = default;
};

struct gfx_sh3_vec
{
	std::array<float, 9> coeffs{};

	bool operator==(const gfx_sh3_vec&) const = default;
};

struct gfx_world_lightmap_reindex_data_element
{
	std::uint32_t index{};
	std::uint32_t pos_x{};
	std::uint32_t pos_y{};
	std::uint32_t width{};
	std::uint32_t height{};

	bool operator==(const gfx_world_lightmap_reindex_data_element&) const = default;
};

struct gfx_world_packed_lightmap
{
	std::uint32_t image_width{};
	std::uint32_t image_height{};

	bool operator==(const gfx_world_packed_lightmap&) const = default;
};

struct gfx_static_decal_volume_material
{
	std::optional<std::string> material{};

	bool operator==(const gfx_static_decal_volume_material&) const = default;
};

struct gfx_static_decal_volume_mask
{
	std::optional<std::string> blend_map_override{};

	bool operator==(const gfx_static_decal_volume_mask&) const = default;
};

struct gfx_decal_volume_collection
{
	std::uint16_t transient_zone_index{};
	std::uint16_t decal_count{};
	std::uint32_t first_zone_decal{};

	bool operator==(const gfx_decal_volume_collection&) const = default;
};

struct gfx_oriented_bounding_box
{
	std::array<float, 3> center{};
	std::array<float, 3> x_axis{};
	std::array<float, 3> y_axis{};
	std::array<float, 3> z_axis{};
	std::array<float, 3> half_size{};

	bool operator==(const gfx_oriented_bounding_box&) const = default;
};

struct gfx_volumetric_mask
{
	std::string type{};
	std::string axis{};
	std::optional<std::string> image{};
	std::array<float, 2> scale{};
	std::array<float, 2> offset{};
	std::array<float, 2> scroll{};

	bool operator==(const gfx_volumetric_mask&) const = default;
};

struct gfx_volumetric
{
	std::optional<std::string> live_path{};
	std::uint32_t flags{};
	gfx_oriented_bounding_box obb{};
	std::array<float, 3> color{};
	float density{};
	float falloff{};
	float height_fade{};
	float anisotropy{};
	std::array<gfx_volumetric_mask, 4> masks{};

	bool operator==(const gfx_volumetric&) const = default;
};

struct gfx_dynamic_lightset_bf_data
{
	std::array<float, 3> cur_value{};
	std::array<float, 3> last_value{};
	std::string bf_type{};
	std::uint16_t num_rec{};
	std::uint32_t start_rec{};

	bool operator==(const gfx_dynamic_lightset_bf_data&) const = default;
};

struct gfx_dynamic_lightset_light_info
{
	std::array<float, 3> base_intensity{};
	std::array<float, 3> last_intensity{};
	std::uint32_t which_primary{};

	bool operator==(const gfx_dynamic_lightset_light_info&) const = default;
};

struct gfx_dynamic_lightset_portal_gi_data
{
	std::uint32_t which_entity{};
	std::array<float, 4> base_quat{};
	std::array<float, 4> extra_info{};

	bool operator==(const gfx_dynamic_lightset_portal_gi_data&) const = default;
};

struct gfx_dynamic_lightset_product_gi_data
{
	std::uint16_t which_dls{};
	std::uint16_t which_portal{};

	bool operator==(const gfx_dynamic_lightset_product_gi_data&) const = default;
};

struct gfx_dynamic_lightset_g_chunk_info
{
	std::uint32_t num_records{};
	std::uint32_t bf_start{};
	std::uint32_t org_start{};
	std::uint32_t tmp_start{};

	bool operator==(const gfx_dynamic_lightset_g_chunk_info&) const = default;
};

struct gfx_dynamic_lightset_g_chunk
{
	gfx_dynamic_lightset_g_chunk_info chunk_info{};
	std::vector<std::string> flags{};
	std::uint16_t which_lm{};
	std::uint32_t bf_chunk_start{};

	bool operator==(const gfx_dynamic_lightset_g_chunk&) const = default;
};

struct gfx_sh_compressed_probe_data
{
	std::array<std::uint32_t, 8> data{};

	bool operator==(const gfx_sh_compressed_probe_data&) const = default;
};

struct gfx_light_view_frustum
{
	std::uint8_t plane_count{};
	std::uint8_t index_count{};
	std::uint8_t vertex_count{};
	std::optional<std::vector<std::array<float, 4>>> planes{};
	std::optional<std::vector<std::uint8_t>> indices{};
	std::optional<std::vector<std::array<float, 3>>> vertices{};

	bool operator==(const gfx_light_view_frustum&) const = default;
};

struct gfx_light
{
	std::uint8_t type{};
	std::uint8_t can_use_shadow_map{};
	std::uint8_t needs_dynamic_shadows{};
	std::uint8_t is_volumetric{};
	float uv_intensity{};
	float ir_intensity{};
	float heat_intensity{};
	float intensity{};
	std::array<float, 3> color_linear_srgb{};
	std::array<float, 3> dir{};
	std::array<float, 3> up{};
	std::array<float, 3> origin{};
	float radius{};
	std::array<float, 2> fade_offset_rt{};
	float bulb_radius{};
	std::array<float, 3> bulb_length{};
	float cos_half_fov_outer{};
	float cos_half_fov_inner{};
	std::uint32_t entity_id{};
	std::uint32_t flags{};
	float tonemapping_scale_factor{};
	float shadow_near_plane_bias{};
	float shadow_softness{};
	float shadow_bias{};
	float shadow_area{};
	float distance_falloff{};
	float cos_half_fov_collimation{};

	bool operator==(const gfx_light&) const = default;
};

struct gfx_voxel_tree_header
{
	std::array<std::int32_t, 4> root_node_dimension{};
	std::array<std::int32_t, 4> node_coord_bit_shift{};
	std::array<float, 4> bound_min{};
	std::array<float, 4> bound_max{};

	bool operator==(const gfx_voxel_tree_header&) const = default;
};

struct replay_voxel_runtime_state
{

	bool operator==(const replay_voxel_runtime_state&) const = default;
};

struct gfx_voxel_tree
{
	bounds zone_bound{};
	std::int32_t voxel_top_down_view_node_count{};
	std::int32_t voxel_internal_node_count{};
	std::int32_t voxel_leaf_node_count{};
	std::int32_t light_list_array_size{};
	std::optional<gfx_voxel_tree_header> voxel_tree_header{};
	std::optional<std::vector<gfx_voxel_top_down_view_node>> voxel_top_down_view_node_array{};
	std::optional<std::vector<gfx_voxel_internal_node>> voxel_internal_node_array{};
	std::optional<std::vector<gfx_voxel_leaf_node>> voxel_leaf_node_array{};
	std::optional<std::vector<std::uint16_t>> light_list_array{};
	std::optional<std::vector<std::uint32_t>> voxel_internal_node_dynamic_light_list_cpu{};
	replay_voxel_runtime_state runtime_state{};

	bool operator==(const gfx_voxel_tree&) const = default;
};

struct gfx_voxel_top_down_view_node
{
	std::int32_t first_node_index{};
	std::int32_t z_min{};
	std::int32_t z_max{};

	bool operator==(const gfx_voxel_top_down_view_node&) const = default;
};

struct gfx_voxel_internal_node
{
	std::array<std::int32_t, 2> first_node_index{};
	std::array<std::int32_t, 2> child_node_mask{};

	bool operator==(const gfx_voxel_internal_node&) const = default;
};

struct gfx_voxel_leaf_node
{
	std::uint16_t light_list_address{};

	bool operator==(const gfx_voxel_leaf_node&) const = default;
};

struct gfx_heightfield
{
	bounds bounds{};
	std::array<std::array<float, 4>, 4> lookup_matrix{};
	std::optional<std::string> image{};

	bool operator==(const gfx_heightfield&) const = default;
};

struct gfx_brush_model_writable
{
	bounds bounds{};
	std::array<float, 3> origin{};
	std::array<float, 4> quat{};
	std::int32_t mdao_volume_processed{};

	bool operator==(const gfx_brush_model_writable&) const = default;
};

struct gfx_brush_model
{
	gfx_brush_model_writable writable{};
	bounds bounds{};
	float radius{};
	std::uint32_t start_surf_index{};
	std::uint32_t surface_count{};
	std::int32_t mdao_volume_index{};

	bool operator==(const gfx_brush_model&) const = default;
};

struct gfx_model_lightmap_inst
{
	std::uint16_t lmap_index{};
	std::array<float, 2> offset{};
	float scale_width{};
	float scale_height{};

	bool operator==(const gfx_model_lightmap_inst&) const = default;
};

struct gfx_map_ent_lookup
{
	std::uint32_t lmap_lookup{};
	std::array<float, 3> probes_offset{};
	std::uint32_t flags{};
	std::uint32_t lgv_data{};
	std::int32_t portal_gi_slot{};

	bool operator==(const gfx_map_ent_lookup&) const = default;
};

struct x_model_draw_info
{
	std::uint32_t has_gfx_ent_index{};
	std::uint32_t lod{};
	std::uint32_t material_lod{};
	std::uint32_t surf_buf_size{};
	std::uint32_t surf_id{};

	bool operator==(const x_model_draw_info&) const = default;
};

struct gfx_scene_dyn_model
{
	x_model_draw_info info{};
	std::uint16_t dyn_ent_client_id{};

	bool operator==(const gfx_scene_dyn_model&) const = default;
};

struct b_model_draw_info
{
	std::uint32_t surf_id{};
	std::uint32_t surf_buf_size{};

	bool operator==(const b_model_draw_info&) const = default;
};

struct gfx_scene_dyn_brush
{
	b_model_draw_info info{};
	std::uint16_t dyn_ent_client_id{};

	bool operator==(const gfx_scene_dyn_brush&) const = default;
};

struct gfx_shadow_geometry
{
	std::uint16_t surface_count{};
	std::uint16_t smodel_count{};
	std::optional<std::vector<std::uint32_t>> sorted_surf_index{};
	std::optional<std::vector<std::uint16_t>> smodel_index{};

	bool operator==(const gfx_shadow_geometry&) const = default;
};

struct gfx_light_aabb_node
{
	bounds bound{};
	std::uint16_t first_child{};
	std::uint16_t child_count{};

	bool operator==(const gfx_light_aabb_node&) const = default;
};

struct umbra_gate
{
	std::uint32_t obj_id{};
	bounds bounds{};
	float close_distance{};

	bool operator==(const umbra_gate&) const = default;
};

struct mdao_volume
{
	extent_bounds bounds{};
	std::array<std::uint16_t, 3> cell_count{};
	std::uint16_t parent_bone_index{};
	std::optional<std::string> volume_data{};

	bool operator==(const mdao_volume&) const = default;
};

struct stream_tree_node
{
	std::optional<std::string> volumes_key{};
	std::optional<std::vector<stream_mtl_img_info>> infos{};
	std::array<std::uint16_t, 2> child_nodes{};
	std::uint16_t mtl_info_count{};
	std::uint16_t img_info_count{};
	std::uint16_t transient_zone{};
	extent_bounds aabb{};
	float min_lomip_inv_sq_radii_of_children{};

	bool operator==(const stream_tree_node&) const = default;
};

struct stream_mtl_img_info
{
	float min_himip_inv_sq_radii{};
	std::uint16_t asset_index{};
	std::uint16_t volume_count{};

	bool operator==(const stream_mtl_img_info&) const = default;
};

struct gfx_index_buffer
{
	std::uint64_t gpu_address{};
	std::uint64_t allocation{};
	std::uint32_t size{};

	bool operator==(const gfx_index_buffer&) const = default;
};

struct gfx_world_draw_verts
{
	std::uint32_t pos_data_size{};
	std::uint32_t aux_data_size{};
	std::optional<std::vector<std::uint8_t>> pos_data{};
	std::optional<std::vector<std::uint8_t>> aux_data{};
	gfx_wrapped_buffer pos_buffer{};
	gfx_wrapped_buffer aux_buffer{};
	std::uint32_t index_count{};
	std::optional<std::vector<std::uint16_t>> indices{};
	gfx_index_buffer index_buffer{};

	bool operator==(const gfx_world_draw_verts&) const = default;
};

struct gfx_world_draw_cells
{
	std::uint32_t cell_count{};
	std::optional<std::vector<gfx_cell_tree_count>> aabb_tree_counts{};
	std::optional<std::vector<gfx_cell_tree>> aabb_trees{};

	bool operator==(const gfx_world_draw_cells&) const = default;
};

struct gfx_image_desc
{
	std::string format{};
	std::vector<std::string> flags{};
	std::uint32_t total_size{};
	std::uint16_t width{};
	std::uint16_t height{};
	std::uint16_t depth{};
	std::uint16_t num_elements{};
	std::uint8_t level_count{};

	bool operator==(const gfx_image_desc&) const = default;
};

struct gfx_image_raw
{
	gfx_image_desc desc{};
	std::string texture_id{};
	std::optional<std::vector<std::uint8_t>> pixels{};

	bool operator==(const gfx_image_raw&) const = default;
};

struct gfx_lightmap_raw
{
	std::array<gfx_image_raw, 3> textures{};

	bool operator==(const gfx_lightmap_raw&) const = default;
};

struct gfx_lightmap_atlas_data_temp
{
	std::uint32_t transient_zone{};
	std::uint32_t num_lightmaps{};
	std::uint32_t first_lightmap_index{};
	std::string lightmap_type{};
	std::array<gfx_lightmap_raw, 32> lightmaps{};

	bool operator==(const gfx_lightmap_atlas_data_temp&) const = default;
};

struct gfx_gpu_light_grid
{
	std::uint32_t probe_count{};
	std::optional<std::vector<gfx_sh_compressed_probe_data>> probes{};
	gfx_wrapped_rw_buffer probes_buffer{};
	std::optional<std::vector<std::array<float, 3>>> probe_positions{};
	gfx_wrapped_buffer probe_positions_buffer{};
	bool fallback_probe_use_custom_sample{};
	std::array<float, 3> fallback_probe_custom_sample_pos{};
	std::uint32_t zone_count{};
	std::optional<std::vector<gfx_gpu_light_grid_zone>> zones{};
	std::optional<std::vector<gfx_gpu_light_grid_voxel_tree>> zone_voxel_trees{};
	std::uint32_t tetrahedron_count{};
	std::uint32_t tetrahedron_count_visible{};
	std::optional<std::vector<gfx_gpu_light_grid_tetrahedron>> tetrahedrons{};
	gfx_wrapped_buffer tetrahedron_buffer{};
	std::optional<std::vector<gfx_gpu_light_grid_tetrahedron_neighbors>> tetrahedron_neighbors{};
	gfx_wrapped_buffer tetrahedron_neighbors_buffer{};
	std::optional<std::vector<gfx_gpu_light_grid_tetrahedron_visibility>> tetrahedron_visibility{};
	gfx_wrapped_buffer tetrahedron_visibility_buffer{};
	std::uint32_t voxel_start_tetrahedron_count{};
	std::optional<std::vector<gfx_gpu_light_grid_voxel_start_tetrahedron>> voxel_start_tetrahedron{};
	gfx_wrapped_buffer voxel_start_tetrahedron_buffer{};
	std::array<std::array<float, 4>, 7> bounding_volume{};

	bool operator==(const gfx_gpu_light_grid&) const = default;
};

struct gfx_dynamic_lightset_l_gpp_zone_info
{
	std::uint32_t num_orig_sml_gpp_probes{};
	std::optional<std::vector<gfx_gpu_dyn_lightsets_lgv_reference>> lgv_references{};
	gfx_wrapped_buffer lgv_references_buffer{};
	gfx_dynamic_lightset_probe_data lgv_probe_data{};

	bool operator==(const gfx_dynamic_lightset_l_gpp_zone_info&) const = default;
};

struct gfx_light_grid_volume_atlas_data_temp
{
	std::uint32_t m_width_and_height{};
	std::uint32_t m_depth{};
	std::uint32_t m_num_volumes{};
	std::uint16_t m_world_transient{};
	std::array<gfx_image_raw, 2> m_textures{};
	std::optional<std::vector<light_grid_volume_packing_params>> m_packing_params{};
	gfx_wrapped_buffer m_packing_params_buffer{};
	std::optional<std::vector<light_grid_volume_transform_params>> m_transform_params{};
	gfx_wrapped_buffer m_transform_params_buffer{};

	bool operator==(const gfx_light_grid_volume_atlas_data_temp&) const = default;
};

struct gfx_gpu_light_grid_transient_data
{
	std::optional<gfx_gpu_light_grid> gpu_light_grid{};
	std::uint32_t static_model_volume_count{};
	std::optional<std::vector<gfx_static_model_light_grid_volume>> static_model_volumes{};
	std::optional<gfx_dynamic_lightset_l_gpp_zone_info> sml_gpp_zone{};
	std::optional<gfx_light_grid_volume_atlas_data_temp> temp_light_grid_volume_data{};

	bool operator==(const gfx_gpu_light_grid_transient_data&) const = default;
};

struct gfx_reflection_probe_transient_data
{
	std::uint16_t reflection_probe_instance_count{};
	std::uint16_t exclusive_probe_bucket_count{};
	std::uint32_t exclusive_probe_count{};
	std::optional<std::vector<std::uint16_t>> reflection_probe_instances{};
	std::optional<std::vector<gfx_image_reflection_probe>> exclusive_probe_buckets{};

	bool operator==(const gfx_reflection_probe_transient_data&) const = default;
};

struct gfx_decal_volume_transient_data
{
	std::uint32_t decal_volume_count{};
	std::uint16_t first_collection{};
	std::uint16_t collection_count{};
	std::optional<std::vector<gfx_static_decal_volume_packed>> decal_volumes{};
	gfx_wrapped_buffer decal_volumes_buffer{};
	std::optional<std::vector<bounds>> collection_bounds{};
	std::optional<std::vector<gfx_static_decal_volume_uid>> decal_volume_uids{};

	bool operator==(const gfx_decal_volume_transient_data&) const = default;
};

struct gfx_compressed_sun_shadow_data_temp
{
	std::uint16_t zone_index{};
	std::uint16_t tiles_count{};
	std::optional<std::vector<std::uint32_t>> tiles_indices{};
	std::optional<std::vector<std::uint32_t>> tiles_offsets{};
	std::uint32_t tiles_data_size{};
	std::optional<std::vector<std::uint8_t>> tiles_data{};
	gfx_wrapped_buffer tiles_data_buffer{};

	bool operator==(const gfx_compressed_sun_shadow_data_temp&) const = default;
};

struct gfx_world_transient_zone
{
	std::optional<std::string> name{};
	std::uint32_t transient_zone_index{};
	gfx_world_draw_verts draw_verts{};
	gfx_world_draw_cells draw_cells{};
	std::optional<gfx_lightmap_atlas_data_temp> temp_lightmap_data{};
	gfx_gpu_light_grid_transient_data gpu_light_grid{};
	std::optional<gfx_reflection_probe_transient_data> reflection_probes{};
	std::uint32_t collection_count{};
	std::optional<std::vector<std::uint16_t>> collections{};
	std::optional<gfx_decal_volume_transient_data> decal_volumes{};
	std::optional<gfx_compressed_sun_shadow_data_temp> compressed_sun_shadow{};

	bool operator==(const gfx_world_transient_zone&) const = default;
};

struct gfx_cell_tree_count
{
	std::int32_t aabb_tree_count{};

	bool operator==(const gfx_cell_tree_count&) const = default;
};

struct gfx_cell_tree
{
	std::optional<std::vector<gfx_aabb_tree>> aabb_tree{};

	bool operator==(const gfx_cell_tree&) const = default;
};

struct gfx_aabb_tree
{
	bounds bounds{};
	std::uint32_t surface_count{};
	std::uint32_t start_surf_index{};
	std::int32_t children_offset{};
	std::uint16_t child_count{};
	std::uint16_t smodel_index_count{};
	std::optional<std::vector<std::uint16_t>> smodel_indexes{};
	std::uint32_t smodel_indexes_id{};

	bool operator==(const gfx_aabb_tree&) const = default;
};

struct gfx_sh_probe_data
{
	std::array<std::uint16_t, 29> coeffs{};

	bool operator==(const gfx_sh_probe_data&) const = default;
};

struct gfx_gpu_light_grid_zone
{
	std::uint32_t num_probes{};
	std::uint32_t first_probe{};
	std::uint32_t num_tetrahedrons{};
	std::uint32_t first_tetrahedron{};
	std::uint32_t first_voxel_tetrahedron_index{};
	std::uint32_t voxel_tetrahedron_internal_node_shift{};
	std::uint32_t num_voxel_tetrahedron_indices{};
	gfx_sh_probe_data fallback_probe_data{};

	bool operator==(const gfx_gpu_light_grid_zone&) const = default;
};

struct gfx_gpu_light_grid_voxel_tree
{
	std::int32_t top_down_view_node_count{};
	std::int32_t internal_node_count{};
	std::optional<gfx_voxel_tree_header> tree_header{};
	std::optional<std::vector<gfx_voxel_top_down_view_node>> top_down_view_nodes{};
	std::optional<std::vector<gfx_voxel_internal_node>> internal_nodes{};
	gfx_wrapped_buffer tree_header_buffer{};
	gfx_wrapped_buffer top_down_view_node_buffer{};
	gfx_wrapped_buffer internal_node_buffer{};

	bool operator==(const gfx_gpu_light_grid_voxel_tree&) const = default;
};

struct gfx_gpu_light_grid_tetrahedron
{
	std::array<std::uint32_t, 4> index_flags{};

	bool operator==(const gfx_gpu_light_grid_tetrahedron&) const = default;
};

struct gfx_gpu_light_grid_tetrahedron_neighbors
{
	std::array<std::uint32_t, 4> neighbors{};

	bool operator==(const gfx_gpu_light_grid_tetrahedron_neighbors&) const = default;
};

struct gfx_gpu_light_grid_tetrahedron_visibility
{
	std::array<std::uint32_t, 16> visibility{};

	bool operator==(const gfx_gpu_light_grid_tetrahedron_visibility&) const = default;
};

struct gfx_gpu_light_grid_voxel_start_tetrahedron
{
	std::uint32_t index{};

	bool operator==(const gfx_gpu_light_grid_voxel_start_tetrahedron&) const = default;
};

struct gfx_static_model_light_grid_volume
{
	std::uint32_t first_probe{};
	std::array<std::uint16_t, 3> m_volume_size{};
	std::uint16_t flags{};
	std::uint32_t single_probe{};
	std::array<std::array<float, 4>, 3> volume_transform{};

	bool operator==(const gfx_static_model_light_grid_volume&) const = default;
};

struct gfx_gpu_dyn_lightsets_lgv_reference
{
	std::uint32_t volume_index{};
	std::uint32_t volume_probe_index{};

	bool operator==(const gfx_gpu_dyn_lightsets_lgv_reference&) const = default;
};

struct light_grid_volume_packing_params
{
	std::array<float, 3> m_uv_scale{};
	std::array<std::uint8_t, 3> m_volume_size{};
	std::uint8_t m_basis_type{};
	std::array<float, 3> m_uv_bias{};
	std::uint32_t m_first_static_probe_index{};

	bool operator==(const light_grid_volume_packing_params&) const = default;
};

struct light_grid_volume_transform_params
{
	std::array<std::array<float, 4>, 3> m_volume_transform{};
	std::uint32_t m_history_entry_or_static_flags{};
	std::uint32_t m_last_frame_history_entry_and_setup{};
	std::uint32_t m_lod_and_sampling_offset_z{};
	std::uint32_t m_sampling_offset_xy{};

	bool operator==(const light_grid_volume_transform_params&) const = default;
};

struct gfx_image_reflection_probe
{
	std::uint32_t total_size{};
	std::uint32_t element_count{};
	std::string texture_id{};
	std::optional<std::vector<std::uint8_t>> pixels{};

	bool operator==(const gfx_image_reflection_probe&) const = default;
};

struct gfx_static_decal_volume_packed
{
	std::array<float, 3> obb_center{};
	std::uint32_t obb_quat{};
	std::uint32_t obb_half_size{};
	std::uint32_t opacity_params{};
	std::array<std::uint32_t, 3> uv_matrix_and_offset{};
	std::uint32_t packed_material_info{};
	std::uint32_t reserved{};

	bool operator==(const gfx_static_decal_volume_packed&) const = default;
};

struct gfx_static_decal_volume_uid
{
	std::uint32_t uid{};

	bool operator==(const gfx_static_decal_volume_uid&) const = default;
};

inline void validate(const bounds& data)
{
	(void) data;
}

inline void validate(const gfx_world_dpvs_planes& data)
{
	if (data.planes)
	{
		for (const auto& item : *data.planes)
		{
			validate(item);
		}
	}

	if (data.planes && data.planes->size() != data.plane_count)
	{
		throw std::runtime_error("gfx_world_dpvs_planes.planes has a length that its count does not give");
	}

	if (data.nodes && data.nodes->size() != data.node_count)
	{
		throw std::runtime_error("gfx_world_dpvs_planes.nodes has a length that its count does not give");
	}

	if (data.scene_ent_cell_bits && static_cast<std::int64_t>(((data.cell_count) * 512)) != static_cast<std::int64_t>(data.scene_ent_cell_bits->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_planes.scene_ent_cell_bits has a length that its count does not give");
	}

}

inline void validate(const gfx_buffer_resource& data)
{
	(void) data;
}

inline void validate(const gfx_shader_buffer_view& data)
{
	(void) data;
}

inline void validate(const gfx_wrapped_base_buffer& data)
{
	validate(data.buffer);

	validate(data.view);

}

inline void validate(const gfx_wrapped_buffer& data)
{
	validate(data.base);

}

inline void validate(const gfx_world_surfaces& data)
{
	if (data.surfaces)
	{
		for (const auto& item : *data.surfaces)
		{
			validate(item);
		}
	}

	if (data.surface_bounds)
	{
		for (const auto& item : *data.surface_bounds)
		{
			validate(item);
		}
	}

	if (data.surf_data)
	{
		for (const auto& item : *data.surf_data)
		{
			validate(item);
		}
	}

	validate(data.surf_data_buffer);

	if (data.surfaces && static_cast<std::int64_t>(data.count) != static_cast<std::int64_t>(data.surfaces->size()))
	{
		throw std::runtime_error("gfx_world_surfaces.surfaces has a length that its count does not give");
	}

	if (data.surface_bounds && static_cast<std::int64_t>(data.count) != static_cast<std::int64_t>(data.surface_bounds->size()))
	{
		throw std::runtime_error("gfx_world_surfaces.surface_bounds has a length that its count does not give");
	}

	if (data.surface_materials && static_cast<std::int64_t>(data.count) != static_cast<std::int64_t>(data.surface_materials->size()))
	{
		throw std::runtime_error("gfx_world_surfaces.surface_materials has a length that its count does not give");
	}

	if (data.surf_data && data.surf_data->size() != data.surf_data_count)
	{
		throw std::runtime_error("gfx_world_surfaces.surf_data has a length that its count does not give");
	}

}

inline void validate(const gfx_clutter_instance_pool& data)
{
	(void) data;
}

inline void validate(const gfx_shader_buffer_rw_view& data)
{
	(void) data;
}

inline void validate(const gfx_wrapped_rw_buffer& data)
{
	validate(data.buffer);

	validate(data.rw_view);

}

inline void validate(const bitarray_dynamic& data)
{
	if (data.array && data.array->size() != data.word_count)
	{
		throw std::runtime_error("bitarray_dynamic.array has a length that its count does not give");
	}

}

inline void validate(const gfx_world_static_models& data)
{
	validate(data.clutter_instance_pool);

	if (data.surfaces)
	{
		for (const auto& item : *data.surfaces)
		{
			validate(item);
		}
	}

	if (data.models)
	{
		for (const auto& item : *data.models)
		{
			validate(item);
		}
	}

	if (data.clutter_collections)
	{
		for (const auto& item : *data.clutter_collections)
		{
			validate(item);
		}
	}

	if (data.collections)
	{
		for (const auto& item : *data.collections)
		{
			validate(item);
		}
	}

	if (data.instances)
	{
		for (const auto& item : *data.instances)
		{
			validate(item);
		}
	}

	if (data.collection_bounds)
	{
		for (const auto& item : *data.collection_bounds)
		{
			validate(item);
		}
	}

	if (data.model_static_indirection)
	{
		for (const auto& item : *data.model_static_indirection)
		{
			validate(item);
		}
	}

	validate(data.model_static_indirection_buffer);

	if (data.lmap_transforms)
	{
		for (const auto& item : *data.lmap_transforms)
		{
			validate(item);
		}
	}

	validate(data.lmap_transforms_buffer);

	if (data.smodel_surf_data)
	{
		for (const auto& item : *data.smodel_surf_data)
		{
			validate(item);
		}
	}

	validate(data.smodel_surf_data_buffer);

	if (data.smodel_instance_data)
	{
		for (const auto& item : *data.smodel_instance_data)
		{
			validate(item);
		}
	}

	validate(data.smodel_instance_data_buffer);

	if (data.clutter_collection_gpu_data)
	{
		for (const auto& item : *data.clutter_collection_gpu_data)
		{
			validate(item);
		}
	}

	validate(data.clutter_collection_gpu_data_buffer);

	validate(data.clutter_tint_data_buffer);

	if (data.smodel_expansion_data)
	{
		for (const auto& item : *data.smodel_expansion_data)
		{
			validate(item);
		}
	}

	validate(data.smodel_expansion_data_buffer);

	if (data.smodel_surf_mat_indirection)
	{
		for (const auto& item : *data.smodel_surf_mat_indirection)
		{
			validate(item);
		}
	}

	validate(data.smodel_surf_mat_indirection_buffer);

	validate(data.clutter_collections_data_buffer);

	validate(data.smodel_collections_data_buffer);

	validate(data.smodel_surf_ugb_indirection_buffer);

	if (data.splined_model_instance_data)
	{
		for (const auto& item : *data.splined_model_instance_data)
		{
			validate(item);
		}
	}

	validate(data.splined_model_instance_buffer);

	if (data.spline_segment_data)
	{
		for (const auto& item : *data.spline_segment_data)
		{
			validate(item);
		}
	}

	validate(data.spline_segment_buffer);

	if (data.splined_model_instance_bounds)
	{
		for (const auto& item : *data.splined_model_instance_bounds)
		{
			validate(item);
		}
	}

	validate(data.splined_model_instance_bounds_buffer);

	validate(data.collection_transient_zone_prev_frame_visibility);

	if (data.surfaces && static_cast<std::int64_t>(data.surface_count) != static_cast<std::int64_t>(data.surfaces->size()))
	{
		throw std::runtime_error("gfx_world_static_models.surfaces has a length that its count does not give");
	}

	if (data.models && data.models->size() != data.model_count)
	{
		throw std::runtime_error("gfx_world_static_models.models has a length that its count does not give");
	}

	if (data.clutter_collections && data.clutter_collections->size() != data.clutter_collection_count)
	{
		throw std::runtime_error("gfx_world_static_models.clutter_collections has a length that its count does not give");
	}

	if (data.collections && data.collections->size() != data.collection_count)
	{
		throw std::runtime_error("gfx_world_static_models.collections has a length that its count does not give");
	}

	if (data.instances && data.instances->size() != data.instance_count)
	{
		throw std::runtime_error("gfx_world_static_models.instances has a length that its count does not give");
	}

	if (data.instance_flags && data.instance_flags->size() != data.instance_count)
	{
		throw std::runtime_error("gfx_world_static_models.instance_flags has a length that its count does not give");
	}

	if (data.collection_bounds && data.collection_bounds->size() != data.collection_count)
	{
		throw std::runtime_error("gfx_world_static_models.collection_bounds has a length that its count does not give");
	}

	if (data.materials && data.materials->size() != data.material_count)
	{
		throw std::runtime_error("gfx_world_static_models.materials has a length that its count does not give");
	}

	if (data.model_static_indirection && data.model_static_indirection->size() != data.instance_count)
	{
		throw std::runtime_error("gfx_world_static_models.model_static_indirection has a length that its count does not give");
	}

	if (data.lmap_transforms && data.lmap_transforms->size() != data.lmap_transforms_count)
	{
		throw std::runtime_error("gfx_world_static_models.lmap_transforms has a length that its count does not give");
	}

	if (data.smodel_surf_data && static_cast<std::int64_t>(data.surface_count) != static_cast<std::int64_t>(data.smodel_surf_data->size()))
	{
		throw std::runtime_error("gfx_world_static_models.smodel_surf_data has a length that its count does not give");
	}

	if (data.smodel_instance_data && data.smodel_instance_data->size() != data.instance_count)
	{
		throw std::runtime_error("gfx_world_static_models.smodel_instance_data has a length that its count does not give");
	}

	if (data.clutter_collection_gpu_data && data.clutter_collection_gpu_data->size() != data.clutter_collection_count)
	{
		throw std::runtime_error("gfx_world_static_models.clutter_collection_gpu_data has a length that its count does not give");
	}

	if (data.clutter_tint_data && data.clutter_tint_data->size() != data.clutter_tint_count)
	{
		throw std::runtime_error("gfx_world_static_models.clutter_tint_data has a length that its count does not give");
	}

	if (data.smodel_expansion_data && data.smodel_expansion_data->size() != data.model_count)
	{
		throw std::runtime_error("gfx_world_static_models.smodel_expansion_data has a length that its count does not give");
	}

	if (data.smodel_surf_mat_indirection && static_cast<std::int64_t>(data.surface_count) != static_cast<std::int64_t>(data.smodel_surf_mat_indirection->size()))
	{
		throw std::runtime_error("gfx_world_static_models.smodel_surf_mat_indirection has a length that its count does not give");
	}

	if (data.smodel_surf_ugb_indirection && static_cast<std::int64_t>(data.surface_count) != static_cast<std::int64_t>(data.smodel_surf_ugb_indirection->size()))
	{
		throw std::runtime_error("gfx_world_static_models.smodel_surf_ugb_indirection has a length that its count does not give");
	}

	if (data.splined_model_instance_data && data.splined_model_instance_data->size() != data.splined_model_instance_count)
	{
		throw std::runtime_error("gfx_world_static_models.splined_model_instance_data has a length that its count does not give");
	}

	if (data.spline_segment_data && data.spline_segment_data->size() != data.spline_segment_count)
	{
		throw std::runtime_error("gfx_world_static_models.spline_segment_data has a length that its count does not give");
	}

	if (data.splined_model_instance_bounds && data.splined_model_instance_bounds->size() != data.splined_model_instance_count)
	{
		throw std::runtime_error("gfx_world_static_models.splined_model_instance_bounds has a length that its count does not give");
	}

	if (data.collection_vis_data && data.collection_vis_data->size() != data.collection_vis_data_count)
	{
		throw std::runtime_error("gfx_world_static_models.collection_vis_data has a length that its count does not give");
	}

	if (data.collection_vis_ref_count && data.collection_vis_ref_count->size() != data.collection_count)
	{
		throw std::runtime_error("gfx_world_static_models.collection_vis_ref_count has a length that its count does not give");
	}

}

inline void validate(const gfx_world_reflection_probe_data& data)
{
	if (data.reflection_probes)
	{
		for (const auto& item : *data.reflection_probes)
		{
			validate(item);
		}
	}

	if (data.probe_relighting_data)
	{
		for (const auto& item : *data.probe_relighting_data)
		{
			validate(item);
		}
	}

	if (data.reflection_probe_instances)
	{
		for (const auto& item : *data.reflection_probe_instances)
		{
			validate(item);
		}
	}

	if (data.reflection_probe_sh)
	{
		for (const auto& item : *data.reflection_probe_sh)
		{
			validate(item);
		}
	}

	validate(data.reflection_probe_sh_buffer);

	if (data.reflection_probe_images && static_cast<std::int64_t>(data.reflection_probe_count) != static_cast<std::int64_t>(data.reflection_probe_images->size()))
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.reflection_probe_images has a length that its count does not give");
	}

	if (data.reflection_probe_images_octahedron && data.reflection_probe_images_octahedron->size() != data.shared_reflection_probe_count)
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.reflection_probe_images_octahedron has a length that its count does not give");
	}

	if (data.reflection_probes && static_cast<std::int64_t>(data.reflection_probe_count) != static_cast<std::int64_t>(data.reflection_probes->size()))
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.reflection_probes has a length that its count does not give");
	}

	if (data.probe_relighting_data && data.probe_relighting_data->size() != data.probe_relighting_count)
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.probe_relighting_data has a length that its count does not give");
	}

	if (data.reflection_probe_g_buffer_images && data.reflection_probe_g_buffer_images->size() != data.reflection_probe_g_buffer_image_count)
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.reflection_probe_g_buffer_images has a length that its count does not give");
	}

	if (data.reflection_probe_g_buffer_images_octahedron && data.reflection_probe_g_buffer_images_octahedron->size() != data.reflection_probe_g_buffer_image_count)
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.reflection_probe_g_buffer_images_octahedron has a length that its count does not give");
	}

	if (data.reflection_probe_instances && data.reflection_probe_instances->size() != data.reflection_probe_instance_count)
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.reflection_probe_instances has a length that its count does not give");
	}

	if (data.reflection_probe_sh && static_cast<std::int64_t>(((data.reflection_probe_count) * 4)) != static_cast<std::int64_t>(data.reflection_probe_sh->size()))
	{
		throw std::runtime_error("gfx_world_reflection_probe_data.reflection_probe_sh has a length that its count does not give");
	}

}

inline void validate(const gfx_world_lightmap_reindex_data& data)
{
	if (data.reindex_element)
	{
		for (const auto& item : *data.reindex_element)
		{
			validate(item);
		}
	}

	if (data.packed_lightmap)
	{
		for (const auto& item : *data.packed_lightmap)
		{
			validate(item);
		}
	}

	if (data.reindex_element && data.reindex_element->size() != data.reindex_count)
	{
		throw std::runtime_error("gfx_world_lightmap_reindex_data.reindex_element has a length that its count does not give");
	}

	if (data.packed_lightmap && data.packed_lightmap->size() != data.packed_lightmap_count)
	{
		throw std::runtime_error("gfx_world_lightmap_reindex_data.packed_lightmap has a length that its count does not give");
	}

}

inline void validate(const gfx_world_volumetrics& data)
{
	if (data.volumetrics)
	{
		for (const auto& item : *data.volumetrics)
		{
			validate(item);
		}
	}

	if (data.volumetrics && static_cast<std::int64_t>(data.volumetric_count) != static_cast<std::int64_t>(data.volumetrics->size()))
	{
		throw std::runtime_error("gfx_world_volumetrics.volumetrics has a length that its count does not give");
	}

}

inline void validate(const gfx_compressed_sun_shadow_params& data)
{
	(void) data;
}

inline void validate(const gfx_world_draw& data)
{
	validate(data.reflection_probe_data);

	validate(data.lightmap_reindex_data);

	if (data.decal_volume_materials)
	{
		for (const auto& item : *data.decal_volume_materials)
		{
			validate(item);
		}
	}

	if (data.decal_volume_masks)
	{
		for (const auto& item : *data.decal_volume_masks)
		{
			validate(item);
		}
	}

	if (data.decal_volume_collections)
	{
		for (const auto& item : *data.decal_volume_collections)
		{
			validate(item);
		}
	}

	validate(data.material_and_mask_indirection_buffer);

	if (std::ranges::none_of(gfx_light_grid_type_names, [&](const auto& entry) { return entry.second == data.light_grid_type; }))
	{
		throw std::runtime_error("gfx_world_draw.light_grid_type names no value");
	}

	if (std::ranges::none_of(gfx_lightmap_type_names, [&](const auto& entry) { return entry.second == data.lightmap_type; }))
	{
		throw std::runtime_error("gfx_world_draw.lightmap_type names no value");
	}

	validate(data.volumetrics);

	validate(data.compressed_sun_shadow_buffer);

	validate(data.compressed_sun_shadow_params);

	if (data.decal_volume_materials && data.decal_volume_materials->size() != data.decal_volume_material_count)
	{
		throw std::runtime_error("gfx_world_draw.decal_volume_materials has a length that its count does not give");
	}

	if (data.decal_volume_masks && data.decal_volume_masks->size() != data.decal_volume_mask_count)
	{
		throw std::runtime_error("gfx_world_draw.decal_volume_masks has a length that its count does not give");
	}

	if (data.decal_volume_collections && data.decal_volume_collections->size() != data.decal_volume_collection_count)
	{
		throw std::runtime_error("gfx_world_draw.decal_volume_collections has a length that its count does not give");
	}

	if (data.material_and_mask_indirection && static_cast<std::int64_t>((data.decal_volume_mask_count + data.decal_volume_material_count)) != static_cast<std::int64_t>(data.material_and_mask_indirection->size()))
	{
		throw std::runtime_error("gfx_world_draw.material_and_mask_indirection has a length that its count does not give");
	}

	if (data.transient_zones && data.transient_zones->size() != data.transient_zone_count)
	{
		throw std::runtime_error("gfx_world_draw.transient_zones has a length that its count does not give");
	}

	if (data.lightmap_transient_index && data.lightmap_transient_index->size() != data.lightmap_count)
	{
		throw std::runtime_error("gfx_world_draw.lightmap_transient_index has a length that its count does not give");
	}

	if (data.compressed_sun_shadow && data.compressed_sun_shadow->size() != data.compressed_sun_shadow_size)
	{
		throw std::runtime_error("gfx_world_draw.compressed_sun_shadow has a length that its count does not give");
	}

}

inline void validate(const gfx_dynamic_lightset_lm_data& data)
{
	if (data.chunk_glm)
	{
		for (const auto& item : *data.chunk_glm)
		{
			validate(item);
		}
	}

	validate(data.orig_texels_buffer);

	validate(data.orig_records_buffer);

	validate(data.bf_texels_buffer);

	validate(data.skip_records_buffer);

	validate(data.pallette_records_buffer);

	if (data.chunk_glm && static_cast<std::int64_t>(data.num_g_chunk_lm) != static_cast<std::int64_t>(data.chunk_glm->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.chunk_glm has a length that its count does not give");
	}

	if (data.chunk_glmbf && data.chunk_glmbf->size() != data.num_g_chunk_lmbf)
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.chunk_glmbf has a length that its count does not give");
	}

	if (data.chunk_which_lm && data.chunk_which_lm->size() != data.num_lm_have)
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.chunk_which_lm has a length that its count does not give");
	}

	if (data.chunk_lm_start && static_cast<std::int64_t>((data.num_lm_have + (data.num_lm_have != 0 ? 1 : 0))) != static_cast<std::int64_t>(data.chunk_lm_start->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.chunk_lm_start has a length that its count does not give");
	}

	if (data.orig_texels && static_cast<std::int64_t>((data.num_orig_texels + data.num_orig_texels)) != static_cast<std::int64_t>(data.orig_texels->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.orig_texels has a length that its count does not give");
	}

	if (data.orig_records && data.orig_records->size() != data.num_orig_texels)
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.orig_records has a length that its count does not give");
	}

	if (data.bf_texels && static_cast<std::int64_t>((data.num_bflm_texels + data.num_bflm_texels)) != static_cast<std::int64_t>(data.bf_texels->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.bf_texels has a length that its count does not give");
	}

	if (data.skip_records && data.skip_records->size() != data.num_skip_records)
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.skip_records has a length that its count does not give");
	}

	if (data.pallette_records && data.pallette_records->size() != data.num_skip_records)
	{
		throw std::runtime_error("gfx_dynamic_lightset_lm_data.pallette_records has a length that its count does not give");
	}

}

inline void validate(const gfx_dynamic_lightset_probe_data& data)
{
	if (data.chunk_g)
	{
		for (const auto& item : *data.chunk_g)
		{
			validate(item);
		}
	}

	if (data.orig_probes)
	{
		for (const auto& item : *data.orig_probes)
		{
			validate(item);
		}
	}

	validate(data.orig_probes_buffer);

	if (data.bf_probes)
	{
		for (const auto& item : *data.bf_probes)
		{
			validate(item);
		}
	}

	validate(data.bf_probes_buffer);

	validate(data.skip_records_buffer);

	validate(data.pallette_records_buffer);

	validate(data.temp_remap_buffer);

	if (data.chunk_g && static_cast<std::int64_t>(data.num_g_chunk) != static_cast<std::int64_t>(data.chunk_g->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.chunk_g has a length that its count does not give");
	}

	if (data.chunk_gbf && data.chunk_gbf->size() != data.num_g_chunk_bf)
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.chunk_gbf has a length that its count does not give");
	}

	if (data.orig_probes && data.orig_probes->size() != data.num_orig_probes)
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.orig_probes has a length that its count does not give");
	}

	if (data.bf_probes && data.bf_probes->size() != data.num_bf_probes)
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.bf_probes has a length that its count does not give");
	}

	if (data.skip_records && data.skip_records->size() != data.num_skip_records)
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.skip_records has a length that its count does not give");
	}

	if (data.pallette_records && data.pallette_records->size() != data.num_skip_records)
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.pallette_records has a length that its count does not give");
	}

	if (data.temp_remap && data.temp_remap->size() != data.num_orig_probes)
	{
		throw std::runtime_error("gfx_dynamic_lightset_probe_data.temp_remap has a length that its count does not give");
	}

}

inline void validate(const gfx_dynamic_lightset& data)
{
	if (data.basis_functions)
	{
		for (const auto& item : *data.basis_functions)
		{
			validate(item);
		}
	}

	if (data.basis_function_lights)
	{
		for (const auto& item : *data.basis_function_lights)
		{
			validate(item);
		}
	}

	if (data.portal_gi_data)
	{
		for (const auto& item : *data.portal_gi_data)
		{
			validate(item);
		}
	}

	if (data.product_gi_data)
	{
		for (const auto& item : *data.product_gi_data)
		{
			validate(item);
		}
	}

	validate(data.lm_data);

	validate(data.lg_probe_data);

	if (data.basis_functions && static_cast<std::int64_t>(data.num_basis_functions) != static_cast<std::int64_t>(data.basis_functions->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset.basis_functions has a length that its count does not give");
	}

	if (data.basis_function_lights && data.basis_function_lights->size() != data.num_basis_function_primary_lights)
	{
		throw std::runtime_error("gfx_dynamic_lightset.basis_function_lights has a length that its count does not give");
	}

	if (data.portal_gi_data && data.portal_gi_data->size() != data.num_portal_gi)
	{
		throw std::runtime_error("gfx_dynamic_lightset.portal_gi_data has a length that its count does not give");
	}

	if (data.product_gi_data && data.product_gi_data->size() != data.num_product_gi)
	{
		throw std::runtime_error("gfx_dynamic_lightset.product_gi_data has a length that its count does not give");
	}

}

inline void validate(const gfx_mayhem_self_vis& data)
{
	validate(data.self_vis_buffer);

	if (data.self_vis && static_cast<std::int64_t>(data.self_vis_buffer_size) != static_cast<std::int64_t>(data.self_vis->size()))
	{
		throw std::runtime_error("gfx_mayhem_self_vis.self_vis has a length that its count does not give");
	}

}

inline void validate(const gfx_frustum_lights& data)
{
	validate(data.index_buffer);

	validate(data.vertex_buffer);

	if (data.indices_offsets && static_cast<std::int64_t>(data.light_count) != static_cast<std::int64_t>(data.indices_offsets->size()))
	{
		throw std::runtime_error("gfx_frustum_lights.indices_offsets has a length that its count does not give");
	}

	if (data.vertices_offsets && static_cast<std::int64_t>(data.light_count) != static_cast<std::int64_t>(data.vertices_offsets->size()))
	{
		throw std::runtime_error("gfx_frustum_lights.vertices_offsets has a length that its count does not give");
	}

	if (data.indices_counts && static_cast<std::int64_t>(data.light_count) != static_cast<std::int64_t>(data.indices_counts->size()))
	{
		throw std::runtime_error("gfx_frustum_lights.indices_counts has a length that its count does not give");
	}

	if (data.vertices_counts && static_cast<std::int64_t>(data.light_count) != static_cast<std::int64_t>(data.vertices_counts->size()))
	{
		throw std::runtime_error("gfx_frustum_lights.vertices_counts has a length that its count does not give");
	}

	if (data.indices && data.indices->size() != data.total_indices_count)
	{
		throw std::runtime_error("gfx_frustum_lights.indices has a length that its count does not give");
	}

	if (data.vertices && static_cast<std::int64_t>((data.total_vertices_count * 16)) != static_cast<std::int64_t>(data.vertices->size()))
	{
		throw std::runtime_error("gfx_frustum_lights.vertices has a length that its count does not give");
	}

}

inline void validate(const gfx_precomputed_sky_illumination& data)
{
	(void) data;
}

inline void validate(const sunflare_t& data)
{
	(void) data;
}

inline void validate(const gfx_light_aabb& data)
{
	if (data.node_array)
	{
		for (const auto& item : *data.node_array)
		{
			validate(item);
		}
	}

	if (data.node_array && static_cast<std::int64_t>(data.node_count) != static_cast<std::int64_t>(data.node_array->size()))
	{
		throw std::runtime_error("gfx_light_aabb.node_array has a length that its count does not give");
	}

	if (data.light_array && data.light_array->size() != data.light_count)
	{
		throw std::runtime_error("gfx_light_aabb.light_array has a length that its count does not give");
	}

}

inline void validate(const gfx_world_dpvs_static& data)
{
	if (data.smodel_vis_data[0] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[0]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[1] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[1]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[2] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[2]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[3] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[3]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[4] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[4]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[5] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[5]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[6] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[6]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[7] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[7]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[8] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[8]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[9] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[9]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[10] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[10]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[11] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[11]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[12] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[12]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[13] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[13]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[14] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[14]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[15] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[15]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[16] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[16]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[17] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[17]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[18] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[18]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[19] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[19]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[20] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[20]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[21] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[21]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[22] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[22]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[23] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[23]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[24] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[24]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[25] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[25]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[26] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[26]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[27] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[27]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[28] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[28]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[29] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[29]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[30] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[30]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[31] && static_cast<std::int64_t>(data.smodel_vis_data_count) != static_cast<std::int64_t>(data.smodel_vis_data[31]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.smodel_vis_data[32] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.smodel_vis_data[32]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.smodel_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[0] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[0]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[1] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[1]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[2] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[2]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[3] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[3]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[4] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[4]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[5] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[5]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[6] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[6]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[7] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[7]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[8] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[8]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[9] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[9]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[10] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[10]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[11] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[11]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[12] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[12]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[13] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[13]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[14] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[14]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[15] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[15]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[16] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[16]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[17] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[17]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[18] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[18]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[19] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[19]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[20] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[20]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[21] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[21]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[22] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[22]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[23] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[23]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[24] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[24]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[25] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[25]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[26] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[26]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[27] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[27]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[28] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[28]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[29] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[29]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[30] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[30]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[31] && static_cast<std::int64_t>(data.surface_vis_data_count) != static_cast<std::int64_t>(data.surface_vis_data[31]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.surface_vis_data[32] && static_cast<std::int64_t>(0) != static_cast<std::int64_t>(data.surface_vis_data[32]->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_vis_data has a length that its count does not give");
	}

	if (data.primary_light_vis_data && data.primary_light_vis_data->size() != data.primary_light_vis_data_count)
	{
		throw std::runtime_error("gfx_world_dpvs_static.primary_light_vis_data has a length that its count does not give");
	}

	if (data.reflection_probe_vis_data && data.reflection_probe_vis_data->size() != data.reflection_probe_vis_data_count)
	{
		throw std::runtime_error("gfx_world_dpvs_static.reflection_probe_vis_data has a length that its count does not give");
	}

	if (data.volumetric_vis_data && data.volumetric_vis_data->size() != data.volumetric_vis_data_count)
	{
		throw std::runtime_error("gfx_world_dpvs_static.volumetric_vis_data has a length that its count does not give");
	}

	if (data.decal_vis_data && data.decal_vis_data->size() != data.decal_vis_data_count)
	{
		throw std::runtime_error("gfx_world_dpvs_static.decal_vis_data has a length that its count does not give");
	}

	if (data.sorted_surf_index && static_cast<std::int64_t>((data.surface_vis_data_count * 32)) != static_cast<std::int64_t>(data.sorted_surf_index->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.sorted_surf_index has a length that its count does not give");
	}

	if (data.sorted_smodel_indices && static_cast<std::int64_t>((((data.smodel_vis_data_count) * 32) + 1)) != static_cast<std::int64_t>(data.sorted_smodel_indices->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.sorted_smodel_indices has a length that its count does not give");
	}

	if (data.surface_casts_sun_shadow && data.surface_casts_sun_shadow->size() != data.surface_vis_data_count)
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_casts_sun_shadow has a length that its count does not give");
	}

	if (data.surface_casts_sun_shadow_opt && static_cast<std::int64_t>((data.sun_surf_vis_data_count * data.sun_shadow_opt_count)) != static_cast<std::int64_t>(data.surface_casts_sun_shadow_opt->size()))
	{
		throw std::runtime_error("gfx_world_dpvs_static.surface_casts_sun_shadow_opt has a length that its count does not give");
	}

}

inline void validate(const gfx_world_dpvs_dynamic& data)
{
	(void) data;
}

inline void validate(const stream_mtl_group& data)
{
	if (data.infos)
	{
		for (const auto& item : *data.infos)
		{
			validate(item);
		}
	}

	if (data.infos && static_cast<std::int64_t>((data.img_info_count + data.mtl_info_count)) != static_cast<std::int64_t>(data.infos->size()))
	{
		throw std::runtime_error("stream_mtl_group.infos has a length that its count does not give");
	}

}

inline void validate(const stream_tree& data)
{
	if (data.nodes)
	{
		for (const auto& item : *data.nodes)
		{
			validate(item);
		}
	}

	validate(data.large_mtl_group);

	if (data.nodes && data.nodes->size() != data.node_count)
	{
		throw std::runtime_error("stream_tree.nodes has a length that its count does not give");
	}

	if (data.materials && data.materials->size() != data.material_count)
	{
		throw std::runtime_error("stream_tree.materials has a length that its count does not give");
	}

	if (data.images && data.images->size() != data.image_count)
	{
		throw std::runtime_error("stream_tree.images has a length that its count does not give");
	}

}

inline void validate(const extent_bounds& data)
{
	(void) data;
}

inline void validate(const stream_tree_grid& data)
{
	validate(data.bounds);

	if (data.cell_keys && static_cast<std::int64_t>((data.column_count * data.row_count)) != static_cast<std::int64_t>(data.cell_keys->size()))
	{
		throw std::runtime_error("stream_tree_grid.cell_keys has a length that its count does not give");
	}

	if (data.materials && data.materials->size() != data.material_count)
	{
		throw std::runtime_error("stream_tree_grid.materials has a length that its count does not give");
	}

	if (data.images && data.images->size() != data.image_count)
	{
		throw std::runtime_error("stream_tree_grid.images has a length that its count does not give");
	}

	if (data.xmodels && data.xmodels->size() != data.xmodel_count)
	{
		throw std::runtime_error("stream_tree_grid.xmodels has a length that its count does not give");
	}

	if (data.smodel_collection_indices && data.smodel_collection_indices->size() != data.smodel_collection_index_count)
	{
		throw std::runtime_error("stream_tree_grid.smodel_collection_indices has a length that its count does not give");
	}

}

inline void validate(const gfx_world& data)
{
	if (!data.name)
	{
		throw std::runtime_error("gfx_world has no name");
	}

	detail::name(*data.name, "gfx_world");

	if (std::ranges::none_of(bsp_version_names, [&](const auto& entry) { return entry.second == data.bsp_version; }))
	{
		throw std::runtime_error("gfx_world.bsp_version names no value");
	}

	validate(data.bounds);

	validate(data.dpvs_planes);

	if (data.cells)
	{
		for (const auto& item : *data.cells)
		{
			validate(item);
		}
	}

	if (data.cell_transient_infos)
	{
		for (const auto& item : *data.cell_transient_infos)
		{
			validate(item);
		}
	}

	validate(data.surfaces);

	validate(data.smodels);

	validate(data.draw);

	validate(data.dynamic_lightset);

	validate(data.mayhem_self_vis);

	validate(data.frustum_lights);

	if (data.light_view_frustums)
	{
		for (const auto& item : *data.light_view_frustums)
		{
			validate(item);
		}
	}

	if (data.primary_lights)
	{
		for (const auto& item : *data.primary_lights)
		{
			validate(item);
		}
	}

	if (data.voxel_tree)
	{
		for (const auto& item : *data.voxel_tree)
		{
			validate(item);
		}
	}

	if (data.heightfields)
	{
		for (const auto& item : *data.heightfields)
		{
			validate(item);
		}
	}

	validate(data.precomputed_sky_illumination);

	if (data.models)
	{
		for (const auto& item : *data.models)
		{
			validate(item);
		}
	}

	if (data.lmap_lookup)
	{
		for (const auto& item : *data.lmap_lookup)
		{
			validate(item);
		}
	}

	if (data.gfx_map_ent_lookup)
	{
		for (const auto& item : *data.gfx_map_ent_lookup)
		{
			validate(item);
		}
	}

	validate(data.sun);

	if (data.scene_dyn_model)
	{
		for (const auto& item : *data.scene_dyn_model)
		{
			validate(item);
		}
	}

	if (data.scene_dyn_brush)
	{
		for (const auto& item : *data.scene_dyn_brush)
		{
			validate(item);
		}
	}

	if (data.shadow_geom_optimized)
	{
		for (const auto& item : *data.shadow_geom_optimized)
		{
			validate(item);
		}
	}

	validate(data.light_aabb);

	validate(data.dpvs);

	validate(data.dpvs_dyn);

	if (data.umbra_gates)
	{
		for (const auto& item : *data.umbra_gates)
		{
			validate(item);
		}
	}

	if (data.mdao_volumes)
	{
		for (const auto& item : *data.mdao_volumes)
		{
			validate(item);
		}
	}

	validate(data.stream_tree);

	validate(data.material_stream_tree_grid);

	validate(data.xmodel_stream_tree_grid);

	if (data.cells && static_cast<std::int64_t>(data.dpvs_planes.cell_count) != static_cast<std::int64_t>(data.cells->size()))
	{
		throw std::runtime_error("gfx_world.cells has a length that its count does not give");
	}

	if (data.cell_transient_infos && static_cast<std::int64_t>(data.dpvs_planes.cell_count) != static_cast<std::int64_t>(data.cell_transient_infos->size()))
	{
		throw std::runtime_error("gfx_world.cell_transient_infos has a length that its count does not give");
	}

	if (data.light_view_frustums && data.light_view_frustums->size() != data.primary_light_count)
	{
		throw std::runtime_error("gfx_world.light_view_frustums has a length that its count does not give");
	}

	if (data.primary_lights && data.primary_lights->size() != data.primary_light_count)
	{
		throw std::runtime_error("gfx_world.primary_lights has a length that its count does not give");
	}

	if (data.voxel_tree && data.voxel_tree->size() != data.voxel_tree_count)
	{
		throw std::runtime_error("gfx_world.voxel_tree has a length that its count does not give");
	}

	if (data.heightfields && data.heightfields->size() != data.heightfield_count)
	{
		throw std::runtime_error("gfx_world.heightfields has a length that its count does not give");
	}

	if (data.models && data.models->size() != data.model_count)
	{
		throw std::runtime_error("gfx_world.models has a length that its count does not give");
	}

	if (data.lmap_lookup && data.lmap_lookup->size() != data.lmap_lookup_count)
	{
		throw std::runtime_error("gfx_world.lmap_lookup has a length that its count does not give");
	}

	if (data.gfx_map_ent_lookup && data.gfx_map_ent_lookup->size() != data.gfx_map_ent_lookup_count)
	{
		throw std::runtime_error("gfx_world.gfx_map_ent_lookup has a length that its count does not give");
	}

	if (data.cell_caster_bits && static_cast<std::int64_t>(((((data.dpvs_planes.cell_count) + 31) >> 5) * (data.dpvs_planes.cell_count))) != static_cast<std::int64_t>(data.cell_caster_bits->size()))
	{
		throw std::runtime_error("gfx_world.cell_caster_bits has a length that its count does not give");
	}

	if (data.cell_has_sun_lit_surfs_bits && static_cast<std::int64_t>((((data.dpvs_planes.cell_count) + 31) >> 5)) != static_cast<std::int64_t>(data.cell_has_sun_lit_surfs_bits->size()))
	{
		throw std::runtime_error("gfx_world.cell_has_sun_lit_surfs_bits has a length that its count does not give");
	}

	if (data.scene_dyn_model && static_cast<std::int64_t>(data.dpvs_dyn.dyn_ent_client_count[0]) != static_cast<std::int64_t>(data.scene_dyn_model->size()))
	{
		throw std::runtime_error("gfx_world.scene_dyn_model has a length that its count does not give");
	}

	if (data.scene_dyn_brush && static_cast<std::int64_t>(data.dpvs_dyn.dyn_ent_client_count[1]) != static_cast<std::int64_t>(data.scene_dyn_brush->size()))
	{
		throw std::runtime_error("gfx_world.scene_dyn_brush has a length that its count does not give");
	}

	if (data.primary_light_motion_detect_bits && data.primary_light_motion_detect_bits->size() != data.primary_light_motion_detect_bits_entries)
	{
		throw std::runtime_error("gfx_world.primary_light_motion_detect_bits has a length that its count does not give");
	}

	if (data.entity_motion_bits && data.entity_motion_bits->size() != data.entity_motion_bits_entries)
	{
		throw std::runtime_error("gfx_world.entity_motion_bits has a length that its count does not give");
	}

	if (data.primary_light_entity_shadow_vis && data.primary_light_entity_shadow_vis->size() != data.num_primary_light_entity_shadow_vis_entries)
	{
		throw std::runtime_error("gfx_world.primary_light_entity_shadow_vis has a length that its count does not give");
	}

	if (data.dyn_ent_motion_bits_0 && static_cast<std::int64_t>(data.dyn_ent_motion_bits_entries[0]) != static_cast<std::int64_t>(data.dyn_ent_motion_bits_0->size()))
	{
		throw std::runtime_error("gfx_world.dyn_ent_motion_bits_0 has a length that its count does not give");
	}

	if (data.dyn_ent_motion_bits_1 && static_cast<std::int64_t>(data.dyn_ent_motion_bits_entries[1]) != static_cast<std::int64_t>(data.dyn_ent_motion_bits_1->size()))
	{
		throw std::runtime_error("gfx_world.dyn_ent_motion_bits_1 has a length that its count does not give");
	}

	if (data.primary_light_dyn_ent_shadow_vis_0 && static_cast<std::int64_t>(data.num_primary_light_dyn_ent_shadow_vis_entries[0]) != static_cast<std::int64_t>(data.primary_light_dyn_ent_shadow_vis_0->size()))
	{
		throw std::runtime_error("gfx_world.primary_light_dyn_ent_shadow_vis_0 has a length that its count does not give");
	}

	if (data.primary_light_dyn_ent_shadow_vis_1 && static_cast<std::int64_t>(data.num_primary_light_dyn_ent_shadow_vis_entries[1]) != static_cast<std::int64_t>(data.primary_light_dyn_ent_shadow_vis_1->size()))
	{
		throw std::runtime_error("gfx_world.primary_light_dyn_ent_shadow_vis_1 has a length that its count does not give");
	}

	if (data.shadow_geom_optimized && data.shadow_geom_optimized->size() != data.primary_light_count)
	{
		throw std::runtime_error("gfx_world.shadow_geom_optimized has a length that its count does not give");
	}

	if (data.umbra_gates && data.umbra_gates->size() != data.num_umbra_gates)
	{
		throw std::runtime_error("gfx_world.umbra_gates has a length that its count does not give");
	}

	if (data.umbra_tome_data && data.umbra_tome_data->size() != data.umbra_tome_size)
	{
		throw std::runtime_error("gfx_world.umbra_tome_data has a length that its count does not give");
	}

	if (data.mdao_volumes && data.mdao_volumes->size() != data.mdao_volume_count)
	{
		throw std::runtime_error("gfx_world.mdao_volumes has a length that its count does not give");
	}

}

inline void validate(const gfx_world_dpvs_plane& data)
{
	(void) data;
}

inline void validate(const gfx_cell& data)
{
	validate(data.bounds);

	if (data.portals)
	{
		for (const auto& item : *data.portals)
		{
			validate(item);
		}
	}

	if (data.portals && data.portals->size() != data.portal_count)
	{
		throw std::runtime_error("gfx_cell.portals has a length that its count does not give");
	}

}

inline void validate(const gfx_portal_writable& data)
{
	(void) data;
}

inline void validate(const dpvs_plane& data)
{
	(void) data;
}

inline void validate(const gfx_portal& data)
{
	validate(data.writable);

	validate(data.plane);

	if (data.vertices && data.vertices->size() != data.vertex_count)
	{
		throw std::runtime_error("gfx_portal.vertices has a length that its count does not give");
	}

}

inline void validate(const gfx_cell_transient_info& data)
{
	(void) data;
}

inline void validate(const srf_triangles_t& data)
{
	(void) data;
}

inline void validate(const gfx_surface& data)
{
	validate(data.tris);

	for (const auto& name : data.flags)
	{
		if (std::ranges::none_of(gfx_surface_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("gfx_surface.flags names no flag");
		}
	}

}

inline void validate(const replay_surface_bounds_extra& data)
{
	(void) data;
}

inline void validate(const gfx_surface_bounds& data)
{
	validate(data.bounds);

	validate(data.serialized_extra);

}

inline void validate(const gfx_world_surf_data& data)
{
	(void) data;
}

inline void validate(const gfx_static_model_surface& data)
{
	(void) data;
}

inline void validate(const gfx_static_model& data)
{
	for (const auto& name : data.flags)
	{
		if (std::ranges::none_of(static_model_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("gfx_static_model.flags names no flag");
		}
	}

}

inline void validate(const gfx_clutter_collection& data)
{
	(void) data;
}

inline void validate(const gfx_static_model_collection& data)
{
	for (const auto& name : data.flags)
	{
		if (std::ranges::none_of(static_model_collection_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("gfx_static_model_collection.flags names no flag");
		}
	}

}

inline void validate(const gfx_static_model_instance& data)
{
	(void) data;
}

inline void validate(const gfx_model_static_indirection& data)
{
	(void) data;
}

inline void validate(const gfx_lightmap_transform_packed& data)
{
	(void) data;
}

inline void validate(const gfx_base_surf_data& data)
{
	(void) data;
}

inline void validate(const gfx_base_s_model_surf_data& data)
{
	validate(data.unnamed);

}

inline void validate(const gfx_s_model_instance_data& data)
{
	(void) data;
}

inline void validate(const gfx_clutter_collection_gpu_data& data)
{
	(void) data;
}

inline void validate(const gfx_static_model_lod& data)
{
	(void) data;
}

inline void validate(const gfx_static_model_expansion& data)
{
	validate(data.model_bounds);

	for (const auto& item : data.lod_info)
	{
		validate(item);
	}

}

inline void validate(const gfx_s_model_surf_mat_indirection& data)
{
	(void) data;
}

inline void validate(const hlsl_gpu_splined_model_instance& data)
{
	(void) data;
}

inline void validate(const hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& data)
{
	(void) data;
}

inline void validate(const hlsl_gpu_spline_segment& data)
{
	validate(data.ent_to_aligned);

	validate(data.model_to_wld);

}

inline void validate(const gfx_reflection_probe& data)
{
	if (data.probe_instances && data.probe_instances->size() != data.probe_instance_count)
	{
		throw std::runtime_error("gfx_reflection_probe.probe_instances has a length that its count does not give");
	}

}

inline void validate(const gfx_reflection_probe_relighting_data& data)
{
	if (data.zones && data.zones->size() != data.zone_count)
	{
		throw std::runtime_error("gfx_reflection_probe_relighting_data.zones has a length that its count does not give");
	}

}

inline void validate(const gfx_reflection_probe_obb& data)
{
	(void) data;
}

inline void validate(const gfx_reflection_probe_instance& data)
{
	validate(data.volume_obb);

}

inline void validate(const gfx_sh3_vec& data)
{
	(void) data;
}

inline void validate(const gfx_world_lightmap_reindex_data_element& data)
{
	(void) data;
}

inline void validate(const gfx_world_packed_lightmap& data)
{
	(void) data;
}

inline void validate(const gfx_static_decal_volume_material& data)
{
	(void) data;
}

inline void validate(const gfx_static_decal_volume_mask& data)
{
	(void) data;
}

inline void validate(const gfx_decal_volume_collection& data)
{
	(void) data;
}

inline void validate(const gfx_oriented_bounding_box& data)
{
	(void) data;
}

inline void validate(const gfx_volumetric_mask& data)
{
	if (std::ranges::none_of(volumetric_mask_type_names, [&](const auto& entry) { return entry.second == data.type; }))
	{
		throw std::runtime_error("gfx_volumetric_mask.type names no value");
	}

	if (std::ranges::none_of(volumetric_axis_names, [&](const auto& entry) { return entry.second == data.axis; }))
	{
		throw std::runtime_error("gfx_volumetric_mask.axis names no value");
	}

}

inline void validate(const gfx_volumetric& data)
{
	validate(data.obb);

	for (const auto& item : data.masks)
	{
		validate(item);
	}

}

inline void validate(const gfx_dynamic_lightset_bf_data& data)
{
	if (std::ranges::none_of(gfx_dynamic_lightset_bf_type_names, [&](const auto& entry) { return entry.second == data.bf_type; }))
	{
		throw std::runtime_error("gfx_dynamic_lightset_bf_data.bf_type names no value");
	}

}

inline void validate(const gfx_dynamic_lightset_light_info& data)
{
	(void) data;
}

inline void validate(const gfx_dynamic_lightset_portal_gi_data& data)
{
	(void) data;
}

inline void validate(const gfx_dynamic_lightset_product_gi_data& data)
{
	(void) data;
}

inline void validate(const gfx_dynamic_lightset_g_chunk_info& data)
{
	(void) data;
}

inline void validate(const gfx_dynamic_lightset_g_chunk& data)
{
	validate(data.chunk_info);

	for (const auto& name : data.flags)
	{
		if (std::ranges::none_of(gfx_dynamic_lightset_g_chunk_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("gfx_dynamic_lightset_g_chunk.flags names no flag");
		}
	}

}

inline void validate(const gfx_sh_compressed_probe_data& data)
{
	(void) data;
}

inline void validate(const gfx_light_view_frustum& data)
{
	if (data.planes && static_cast<std::int64_t>(data.plane_count) != static_cast<std::int64_t>(data.planes->size()))
	{
		throw std::runtime_error("gfx_light_view_frustum.planes has a length that its count does not give");
	}

	if (data.indices && data.indices->size() != data.index_count)
	{
		throw std::runtime_error("gfx_light_view_frustum.indices has a length that its count does not give");
	}

	if (data.vertices && data.vertices->size() != data.vertex_count)
	{
		throw std::runtime_error("gfx_light_view_frustum.vertices has a length that its count does not give");
	}

}

inline void validate(const gfx_light& data)
{
	(void) data;
}

inline void validate(const gfx_voxel_tree_header& data)
{
	(void) data;
}

inline void validate(const replay_voxel_runtime_state& data)
{
	(void) data;
}

inline void validate(const gfx_voxel_tree& data)
{
	validate(data.zone_bound);

	if (data.voxel_tree_header)
	{
		validate(*data.voxel_tree_header);
	}

	if (data.voxel_top_down_view_node_array)
	{
		for (const auto& item : *data.voxel_top_down_view_node_array)
		{
			validate(item);
		}
	}

	if (data.voxel_internal_node_array)
	{
		for (const auto& item : *data.voxel_internal_node_array)
		{
			validate(item);
		}
	}

	if (data.voxel_leaf_node_array)
	{
		for (const auto& item : *data.voxel_leaf_node_array)
		{
			validate(item);
		}
	}

	validate(data.runtime_state);

	if (data.voxel_top_down_view_node_array && data.voxel_top_down_view_node_array->size() != data.voxel_top_down_view_node_count)
	{
		throw std::runtime_error("gfx_voxel_tree.voxel_top_down_view_node_array has a length that its count does not give");
	}

	if (data.voxel_internal_node_array && data.voxel_internal_node_array->size() != data.voxel_internal_node_count)
	{
		throw std::runtime_error("gfx_voxel_tree.voxel_internal_node_array has a length that its count does not give");
	}

	if (data.voxel_leaf_node_array && data.voxel_leaf_node_array->size() != data.voxel_leaf_node_count)
	{
		throw std::runtime_error("gfx_voxel_tree.voxel_leaf_node_array has a length that its count does not give");
	}

	if (data.light_list_array && data.light_list_array->size() != data.light_list_array_size)
	{
		throw std::runtime_error("gfx_voxel_tree.light_list_array has a length that its count does not give");
	}

	if (data.voxel_internal_node_dynamic_light_list_cpu && static_cast<std::int64_t>((data.voxel_internal_node_count + data.voxel_internal_node_count)) != static_cast<std::int64_t>(data.voxel_internal_node_dynamic_light_list_cpu->size()))
	{
		throw std::runtime_error("gfx_voxel_tree.voxel_internal_node_dynamic_light_list_cpu has a length that its count does not give");
	}

}

inline void validate(const gfx_voxel_top_down_view_node& data)
{
	(void) data;
}

inline void validate(const gfx_voxel_internal_node& data)
{
	(void) data;
}

inline void validate(const gfx_voxel_leaf_node& data)
{
	(void) data;
}

inline void validate(const gfx_heightfield& data)
{
	validate(data.bounds);

}

inline void validate(const gfx_brush_model_writable& data)
{
	validate(data.bounds);

}

inline void validate(const gfx_brush_model& data)
{
	validate(data.writable);

	validate(data.bounds);

}

inline void validate(const gfx_model_lightmap_inst& data)
{
	(void) data;
}

inline void validate(const gfx_map_ent_lookup& data)
{
	(void) data;
}

inline void validate(const x_model_draw_info& data)
{
	(void) data;
}

inline void validate(const gfx_scene_dyn_model& data)
{
	validate(data.info);

}

inline void validate(const b_model_draw_info& data)
{
	(void) data;
}

inline void validate(const gfx_scene_dyn_brush& data)
{
	validate(data.info);

}

inline void validate(const gfx_shadow_geometry& data)
{
	if (data.sorted_surf_index && static_cast<std::int64_t>(data.surface_count) != static_cast<std::int64_t>(data.sorted_surf_index->size()))
	{
		throw std::runtime_error("gfx_shadow_geometry.sorted_surf_index has a length that its count does not give");
	}

	if (data.smodel_index && data.smodel_index->size() != data.smodel_count)
	{
		throw std::runtime_error("gfx_shadow_geometry.smodel_index has a length that its count does not give");
	}

}

inline void validate(const gfx_light_aabb_node& data)
{
	validate(data.bound);

}

inline void validate(const umbra_gate& data)
{
	validate(data.bounds);

}

inline void validate(const mdao_volume& data)
{
	validate(data.bounds);

}

inline void validate(const stream_tree_node& data)
{
	if (data.infos)
	{
		for (const auto& item : *data.infos)
		{
			validate(item);
		}
	}

	validate(data.aabb);

	if (data.infos && static_cast<std::int64_t>((data.img_info_count + data.mtl_info_count)) != static_cast<std::int64_t>(data.infos->size()))
	{
		throw std::runtime_error("stream_tree_node.infos has a length that its count does not give");
	}

}

inline void validate(const stream_mtl_img_info& data)
{
	(void) data;
}

inline void validate(const gfx_index_buffer& data)
{
	(void) data;
}

inline void validate(const gfx_world_draw_verts& data)
{
	validate(data.pos_buffer);

	validate(data.aux_buffer);

	validate(data.index_buffer);

	if (data.pos_data && static_cast<std::int64_t>(data.pos_data_size) != static_cast<std::int64_t>(data.pos_data->size()))
	{
		throw std::runtime_error("gfx_world_draw_verts.pos_data has a length that its count does not give");
	}

	if (data.aux_data && data.aux_data->size() != data.aux_data_size)
	{
		throw std::runtime_error("gfx_world_draw_verts.aux_data has a length that its count does not give");
	}

	if (data.indices && data.indices->size() != data.index_count)
	{
		throw std::runtime_error("gfx_world_draw_verts.indices has a length that its count does not give");
	}

}

inline void validate(const gfx_world_draw_cells& data)
{
	if (data.aabb_tree_counts)
	{
		for (const auto& item : *data.aabb_tree_counts)
		{
			validate(item);
		}
	}

	if (data.aabb_trees)
	{
		for (const auto& item : *data.aabb_trees)
		{
			validate(item);
		}
	}

	if (data.aabb_tree_counts && static_cast<std::int64_t>(data.cell_count) != static_cast<std::int64_t>(data.aabb_tree_counts->size()))
	{
		throw std::runtime_error("gfx_world_draw_cells.aabb_tree_counts has a length that its count does not give");
	}

	if (data.aabb_trees && static_cast<std::int64_t>(data.cell_count) != static_cast<std::int64_t>(data.aabb_trees->size()))
	{
		throw std::runtime_error("gfx_world_draw_cells.aabb_trees has a length that its count does not give");
	}

}

inline void validate(const gfx_image_desc& data)
{
	if (std::ranges::none_of(gfx_pixel_format_names, [&](const auto& entry) { return entry.second == data.format; }))
	{
		throw std::runtime_error("gfx_image_desc.format names no value");
	}

	for (const auto& name : data.flags)
	{
		if (std::ranges::none_of(gfx_image_flags_names, [&](const auto& entry) { return entry.second == name; }))
		{
			throw std::runtime_error("gfx_image_desc.flags names no flag");
		}
	}

}

inline void validate(const gfx_image_raw& data)
{
	validate(data.desc);

	if (std::ranges::none_of(gfx_texture_id_names, [&](const auto& entry) { return entry.second == data.texture_id; }))
	{
		throw std::runtime_error("gfx_image_raw.texture_id names no value");
	}

	if (data.pixels && static_cast<std::int64_t>(data.desc.total_size) != static_cast<std::int64_t>(data.pixels->size()))
	{
		throw std::runtime_error("gfx_image_raw.pixels has a length that its count does not give");
	}

}

inline void validate(const gfx_lightmap_raw& data)
{
	for (const auto& item : data.textures)
	{
		validate(item);
	}

}

inline void validate(const gfx_lightmap_atlas_data_temp& data)
{
	if (std::ranges::none_of(gfx_lightmap_type_names, [&](const auto& entry) { return entry.second == data.lightmap_type; }))
	{
		throw std::runtime_error("gfx_lightmap_atlas_data_temp.lightmap_type names no value");
	}

	for (const auto& item : data.lightmaps)
	{
		validate(item);
	}

}

inline void validate(const gfx_gpu_light_grid& data)
{
	if (data.probes)
	{
		for (const auto& item : *data.probes)
		{
			validate(item);
		}
	}

	validate(data.probes_buffer);

	validate(data.probe_positions_buffer);

	if (data.zones)
	{
		for (const auto& item : *data.zones)
		{
			validate(item);
		}
	}

	if (data.zone_voxel_trees)
	{
		for (const auto& item : *data.zone_voxel_trees)
		{
			validate(item);
		}
	}

	if (data.tetrahedrons)
	{
		for (const auto& item : *data.tetrahedrons)
		{
			validate(item);
		}
	}

	validate(data.tetrahedron_buffer);

	if (data.tetrahedron_neighbors)
	{
		for (const auto& item : *data.tetrahedron_neighbors)
		{
			validate(item);
		}
	}

	validate(data.tetrahedron_neighbors_buffer);

	if (data.tetrahedron_visibility)
	{
		for (const auto& item : *data.tetrahedron_visibility)
		{
			validate(item);
		}
	}

	validate(data.tetrahedron_visibility_buffer);

	if (data.voxel_start_tetrahedron)
	{
		for (const auto& item : *data.voxel_start_tetrahedron)
		{
			validate(item);
		}
	}

	validate(data.voxel_start_tetrahedron_buffer);

	if (data.probes && static_cast<std::int64_t>(data.probe_count) != static_cast<std::int64_t>(data.probes->size()))
	{
		throw std::runtime_error("gfx_gpu_light_grid.probes has a length that its count does not give");
	}

	if (data.probe_positions && static_cast<std::int64_t>(data.probe_count) != static_cast<std::int64_t>(data.probe_positions->size()))
	{
		throw std::runtime_error("gfx_gpu_light_grid.probe_positions has a length that its count does not give");
	}

	if (data.zones && data.zones->size() != data.zone_count)
	{
		throw std::runtime_error("gfx_gpu_light_grid.zones has a length that its count does not give");
	}

	if (data.zone_voxel_trees && data.zone_voxel_trees->size() != data.zone_count)
	{
		throw std::runtime_error("gfx_gpu_light_grid.zone_voxel_trees has a length that its count does not give");
	}

	if (data.tetrahedrons && data.tetrahedrons->size() != data.tetrahedron_count)
	{
		throw std::runtime_error("gfx_gpu_light_grid.tetrahedrons has a length that its count does not give");
	}

	if (data.tetrahedron_neighbors && data.tetrahedron_neighbors->size() != data.tetrahedron_count)
	{
		throw std::runtime_error("gfx_gpu_light_grid.tetrahedron_neighbors has a length that its count does not give");
	}

	if (data.tetrahedron_visibility && data.tetrahedron_visibility->size() != data.tetrahedron_count_visible)
	{
		throw std::runtime_error("gfx_gpu_light_grid.tetrahedron_visibility has a length that its count does not give");
	}

	if (data.voxel_start_tetrahedron && data.voxel_start_tetrahedron->size() != data.voxel_start_tetrahedron_count)
	{
		throw std::runtime_error("gfx_gpu_light_grid.voxel_start_tetrahedron has a length that its count does not give");
	}

}

inline void validate(const gfx_dynamic_lightset_l_gpp_zone_info& data)
{
	if (data.lgv_references)
	{
		for (const auto& item : *data.lgv_references)
		{
			validate(item);
		}
	}

	validate(data.lgv_references_buffer);

	validate(data.lgv_probe_data);

	if (data.lgv_references && static_cast<std::int64_t>(data.num_orig_sml_gpp_probes) != static_cast<std::int64_t>(data.lgv_references->size()))
	{
		throw std::runtime_error("gfx_dynamic_lightset_l_gpp_zone_info.lgv_references has a length that its count does not give");
	}

}

inline void validate(const gfx_light_grid_volume_atlas_data_temp& data)
{
	for (const auto& item : data.m_textures)
	{
		validate(item);
	}

	if (data.m_packing_params)
	{
		for (const auto& item : *data.m_packing_params)
		{
			validate(item);
		}
	}

	validate(data.m_packing_params_buffer);

	if (data.m_transform_params)
	{
		for (const auto& item : *data.m_transform_params)
		{
			validate(item);
		}
	}

	validate(data.m_transform_params_buffer);

	if (data.m_packing_params && data.m_packing_params->size() != data.m_num_volumes)
	{
		throw std::runtime_error("gfx_light_grid_volume_atlas_data_temp.m_packing_params has a length that its count does not give");
	}

	if (data.m_transform_params && data.m_transform_params->size() != data.m_num_volumes)
	{
		throw std::runtime_error("gfx_light_grid_volume_atlas_data_temp.m_transform_params has a length that its count does not give");
	}

}

inline void validate(const gfx_gpu_light_grid_transient_data& data)
{
	if (data.gpu_light_grid)
	{
		validate(*data.gpu_light_grid);
	}

	if (data.static_model_volumes)
	{
		for (const auto& item : *data.static_model_volumes)
		{
			validate(item);
		}
	}

	if (data.sml_gpp_zone)
	{
		validate(*data.sml_gpp_zone);
	}

	if (data.temp_light_grid_volume_data)
	{
		validate(*data.temp_light_grid_volume_data);
	}

	if (data.static_model_volumes && data.static_model_volumes->size() != data.static_model_volume_count)
	{
		throw std::runtime_error("gfx_gpu_light_grid_transient_data.static_model_volumes has a length that its count does not give");
	}

}

inline void validate(const gfx_reflection_probe_transient_data& data)
{
	if (data.exclusive_probe_buckets)
	{
		for (const auto& item : *data.exclusive_probe_buckets)
		{
			validate(item);
		}
	}

	if (data.reflection_probe_instances && static_cast<std::int64_t>(data.reflection_probe_instance_count) != static_cast<std::int64_t>(data.reflection_probe_instances->size()))
	{
		throw std::runtime_error("gfx_reflection_probe_transient_data.reflection_probe_instances has a length that its count does not give");
	}

	if (data.exclusive_probe_buckets && data.exclusive_probe_buckets->size() != data.exclusive_probe_bucket_count)
	{
		throw std::runtime_error("gfx_reflection_probe_transient_data.exclusive_probe_buckets has a length that its count does not give");
	}

}

inline void validate(const gfx_decal_volume_transient_data& data)
{
	if (data.decal_volumes)
	{
		for (const auto& item : *data.decal_volumes)
		{
			validate(item);
		}
	}

	validate(data.decal_volumes_buffer);

	if (data.collection_bounds)
	{
		for (const auto& item : *data.collection_bounds)
		{
			validate(item);
		}
	}

	if (data.decal_volume_uids)
	{
		for (const auto& item : *data.decal_volume_uids)
		{
			validate(item);
		}
	}

	if (data.decal_volumes && static_cast<std::int64_t>(data.decal_volume_count) != static_cast<std::int64_t>(data.decal_volumes->size()))
	{
		throw std::runtime_error("gfx_decal_volume_transient_data.decal_volumes has a length that its count does not give");
	}

	if (data.collection_bounds && data.collection_bounds->size() != data.collection_count)
	{
		throw std::runtime_error("gfx_decal_volume_transient_data.collection_bounds has a length that its count does not give");
	}

	if (data.decal_volume_uids && static_cast<std::int64_t>(data.decal_volume_count) != static_cast<std::int64_t>(data.decal_volume_uids->size()))
	{
		throw std::runtime_error("gfx_decal_volume_transient_data.decal_volume_uids has a length that its count does not give");
	}

}

inline void validate(const gfx_compressed_sun_shadow_data_temp& data)
{
	validate(data.tiles_data_buffer);

	if (data.tiles_indices && data.tiles_indices->size() != data.tiles_count)
	{
		throw std::runtime_error("gfx_compressed_sun_shadow_data_temp.tiles_indices has a length that its count does not give");
	}

	if (data.tiles_offsets && data.tiles_offsets->size() != data.tiles_count)
	{
		throw std::runtime_error("gfx_compressed_sun_shadow_data_temp.tiles_offsets has a length that its count does not give");
	}

	if (data.tiles_data && data.tiles_data->size() != data.tiles_data_size)
	{
		throw std::runtime_error("gfx_compressed_sun_shadow_data_temp.tiles_data has a length that its count does not give");
	}

}

inline void validate(const gfx_world_transient_zone& data)
{
	if (!data.name)
	{
		throw std::runtime_error("gfx_world_transient_zone has no name");
	}

	detail::name(*data.name, "gfx_world_transient_zone");

	validate(data.draw_verts);

	validate(data.draw_cells);

	if (data.temp_lightmap_data)
	{
		validate(*data.temp_lightmap_data);
	}

	validate(data.gpu_light_grid);

	if (data.reflection_probes)
	{
		validate(*data.reflection_probes);
	}

	if (data.decal_volumes)
	{
		validate(*data.decal_volumes);
	}

	if (data.compressed_sun_shadow)
	{
		validate(*data.compressed_sun_shadow);
	}

	if (data.collections && data.collections->size() != data.collection_count)
	{
		throw std::runtime_error("gfx_world_transient_zone.collections has a length that its count does not give");
	}

}

inline void validate(const gfx_cell_tree_count& data)
{
	(void) data;
}

inline void validate(const gfx_cell_tree& data)
{
	if (data.aabb_tree)
	{
		for (const auto& item : *data.aabb_tree)
		{
			validate(item);
		}
	}

}

inline void validate(const gfx_aabb_tree& data)
{
	validate(data.bounds);

	if (data.smodel_indexes && data.smodel_indexes->size() != data.smodel_index_count)
	{
		throw std::runtime_error("gfx_aabb_tree.smodel_indexes has a length that its count does not give");
	}

	if (data.smodel_indexes && data.smodel_indexes_id == 0)
	{
		throw std::runtime_error("gfx_aabb_tree has an array without an id");
	}

}

inline void validate(const gfx_sh_probe_data& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_zone& data)
{
	validate(data.fallback_probe_data);

}

inline void validate(const gfx_gpu_light_grid_voxel_tree& data)
{
	if (data.tree_header)
	{
		validate(*data.tree_header);
	}

	if (data.top_down_view_nodes)
	{
		for (const auto& item : *data.top_down_view_nodes)
		{
			validate(item);
		}
	}

	if (data.internal_nodes)
	{
		for (const auto& item : *data.internal_nodes)
		{
			validate(item);
		}
	}

	validate(data.tree_header_buffer);

	validate(data.top_down_view_node_buffer);

	validate(data.internal_node_buffer);

	if (data.top_down_view_nodes && static_cast<std::int64_t>(data.top_down_view_node_count) != static_cast<std::int64_t>(data.top_down_view_nodes->size()))
	{
		throw std::runtime_error("gfx_gpu_light_grid_voxel_tree.top_down_view_nodes has a length that its count does not give");
	}

	if (data.internal_nodes && data.internal_nodes->size() != data.internal_node_count)
	{
		throw std::runtime_error("gfx_gpu_light_grid_voxel_tree.internal_nodes has a length that its count does not give");
	}

}

inline void validate(const gfx_gpu_light_grid_tetrahedron& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_tetrahedron_neighbors& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_tetrahedron_visibility& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_light_grid_voxel_start_tetrahedron& data)
{
	(void) data;
}

inline void validate(const gfx_static_model_light_grid_volume& data)
{
	(void) data;
}

inline void validate(const gfx_gpu_dyn_lightsets_lgv_reference& data)
{
	(void) data;
}

inline void validate(const light_grid_volume_packing_params& data)
{
	(void) data;
}

inline void validate(const light_grid_volume_transform_params& data)
{
	(void) data;
}

inline void validate(const gfx_image_reflection_probe& data)
{
	if (std::ranges::none_of(gfx_texture_id_names, [&](const auto& entry) { return entry.second == data.texture_id; }))
	{
		throw std::runtime_error("gfx_image_reflection_probe.texture_id names no value");
	}

	if (data.pixels && static_cast<std::int64_t>(data.total_size) != static_cast<std::int64_t>(data.pixels->size()))
	{
		throw std::runtime_error("gfx_image_reflection_probe.pixels has a length that its count does not give");
	}

}

inline void validate(const gfx_static_decal_volume_packed& data)
{
	(void) data;
}

inline void validate(const gfx_static_decal_volume_uid& data)
{
	(void) data;
}

} // namespace gfxworld
} // namespace zonetool::asset_data
