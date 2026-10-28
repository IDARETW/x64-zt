// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/gfxworld.json: do not edit.
#include <std_include.hpp>
#include "gfxworld.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::gfxworld::gfx_world read_gfx_world(const GfxWorld& native);
		void write_gfx_world(GfxWorld& native, const asset_data::gfxworld::gfx_world& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world(const asset_data::gfxworld::gfx_world& data);
		asset_data::gfxworld::bounds read_bounds(const Bounds& native);
		void write_bounds(Bounds& native, const asset_data::gfxworld::bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bounds(const asset_data::gfxworld::bounds& data);
		asset_data::gfxworld::gfx_world_dpvs_planes read_gfx_world_dpvs_planes(const GfxWorldDpvsPlanes& native);
		void write_gfx_world_dpvs_planes(GfxWorldDpvsPlanes& native, const asset_data::gfxworld::gfx_world_dpvs_planes& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_dpvs_planes(const asset_data::gfxworld::gfx_world_dpvs_planes& data);
		asset_data::gfxworld::gfx_world_dpvs_plane read_gfx_world_dpvs_plane(const GfxWorldDpvsPlane& native);
		void write_gfx_world_dpvs_plane(GfxWorldDpvsPlane& native, const asset_data::gfxworld::gfx_world_dpvs_plane& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_dpvs_plane(const asset_data::gfxworld::gfx_world_dpvs_plane& data);
		asset_data::gfxworld::gfx_cell read_gfx_cell(const GfxCell& native);
		void write_gfx_cell(GfxCell& native, const asset_data::gfxworld::gfx_cell& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_cell(const asset_data::gfxworld::gfx_cell& data);
		asset_data::gfxworld::gfx_portal read_gfx_portal(const GfxPortal& native);
		void write_gfx_portal(GfxPortal& native, const asset_data::gfxworld::gfx_portal& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_portal(const asset_data::gfxworld::gfx_portal& data);
		asset_data::gfxworld::gfx_portal_writable read_gfx_portal_writable(const GfxPortalWritable& native);
		void write_gfx_portal_writable(GfxPortalWritable& native, const asset_data::gfxworld::gfx_portal_writable& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_portal_writable(const asset_data::gfxworld::gfx_portal_writable& data);
		asset_data::gfxworld::dpvs_plane read_dpvs_plane(const DpvsPlane& native);
		void write_dpvs_plane(DpvsPlane& native, const asset_data::gfxworld::dpvs_plane& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_dpvs_plane(const asset_data::gfxworld::dpvs_plane& data);
		asset_data::gfxworld::gfx_cell_transient_info read_gfx_cell_transient_info(const GfxCellTransientInfo& native);
		void write_gfx_cell_transient_info(GfxCellTransientInfo& native, const asset_data::gfxworld::gfx_cell_transient_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_cell_transient_info(const asset_data::gfxworld::gfx_cell_transient_info& data);
		asset_data::gfxworld::gfx_world_surfaces read_gfx_world_surfaces(const GfxWorldSurfaces& native);
		void write_gfx_world_surfaces(GfxWorldSurfaces& native, const asset_data::gfxworld::gfx_world_surfaces& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_surfaces(const asset_data::gfxworld::gfx_world_surfaces& data);
		asset_data::gfxworld::gfx_surface read_gfx_surface(const GfxSurface& native);
		void write_gfx_surface(GfxSurface& native, const asset_data::gfxworld::gfx_surface& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_surface(const asset_data::gfxworld::gfx_surface& data);
		asset_data::gfxworld::srf_triangles_t read_srf_triangles_t(const srfTriangles_t& native);
		void write_srf_triangles_t(srfTriangles_t& native, const asset_data::gfxworld::srf_triangles_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_srf_triangles_t(const asset_data::gfxworld::srf_triangles_t& data);
		asset_data::gfxworld::gfx_surface_bounds read_gfx_surface_bounds(const GfxSurfaceBounds& native);
		void write_gfx_surface_bounds(GfxSurfaceBounds& native, const asset_data::gfxworld::gfx_surface_bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_surface_bounds(const asset_data::gfxworld::gfx_surface_bounds& data);
		asset_data::gfxworld::replay_surface_bounds_extra read_replay_surface_bounds_extra(const Replay_SurfaceBoundsExtra& native);
		void write_replay_surface_bounds_extra(Replay_SurfaceBoundsExtra& native, const asset_data::gfxworld::replay_surface_bounds_extra& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_replay_surface_bounds_extra(const asset_data::gfxworld::replay_surface_bounds_extra& data);
		asset_data::gfxworld::gfx_world_surf_data read_gfx_world_surf_data(const GfxWorldSurfData& native);
		void write_gfx_world_surf_data(GfxWorldSurfData& native, const asset_data::gfxworld::gfx_world_surf_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_surf_data(const asset_data::gfxworld::gfx_world_surf_data& data);
		asset_data::gfxworld::gfx_wrapped_buffer read_gfx_wrapped_buffer(const GfxWrappedBuffer& native);
		void write_gfx_wrapped_buffer(GfxWrappedBuffer& native, const asset_data::gfxworld::gfx_wrapped_buffer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_wrapped_buffer(const asset_data::gfxworld::gfx_wrapped_buffer& data);
		asset_data::gfxworld::gfx_wrapped_base_buffer read_gfx_wrapped_base_buffer(const GfxWrappedBaseBuffer& native);
		void write_gfx_wrapped_base_buffer(GfxWrappedBaseBuffer& native, const asset_data::gfxworld::gfx_wrapped_base_buffer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_wrapped_base_buffer(const asset_data::gfxworld::gfx_wrapped_base_buffer& data);
		asset_data::gfxworld::gfx_buffer_resource read_gfx_buffer_resource(const GfxBufferResource& native);
		void write_gfx_buffer_resource(GfxBufferResource& native, const asset_data::gfxworld::gfx_buffer_resource& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_buffer_resource(const asset_data::gfxworld::gfx_buffer_resource& data);
		asset_data::gfxworld::gfx_shader_buffer_view read_gfx_shader_buffer_view(const GfxShaderBufferView& native);
		void write_gfx_shader_buffer_view(GfxShaderBufferView& native, const asset_data::gfxworld::gfx_shader_buffer_view& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_shader_buffer_view(const asset_data::gfxworld::gfx_shader_buffer_view& data);
		asset_data::gfxworld::gfx_world_static_models read_gfx_world_static_models(const GfxWorldStaticModels& native);
		void write_gfx_world_static_models(GfxWorldStaticModels& native, const asset_data::gfxworld::gfx_world_static_models& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_static_models(const asset_data::gfxworld::gfx_world_static_models& data);
		asset_data::gfxworld::gfx_clutter_instance_pool read_gfx_clutter_instance_pool(const GfxClutterInstancePool& native);
		void write_gfx_clutter_instance_pool(GfxClutterInstancePool& native, const asset_data::gfxworld::gfx_clutter_instance_pool& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_clutter_instance_pool(const asset_data::gfxworld::gfx_clutter_instance_pool& data);
		asset_data::gfxworld::gfx_static_model_surface read_gfx_static_model_surface(const GfxStaticModelSurface& native);
		void write_gfx_static_model_surface(GfxStaticModelSurface& native, const asset_data::gfxworld::gfx_static_model_surface& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_model_surface(const asset_data::gfxworld::gfx_static_model_surface& data);
		asset_data::gfxworld::gfx_static_model read_gfx_static_model(const GfxStaticModel& native);
		void write_gfx_static_model(GfxStaticModel& native, const asset_data::gfxworld::gfx_static_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_model(const asset_data::gfxworld::gfx_static_model& data);
		asset_data::gfxworld::gfx_clutter_collection read_gfx_clutter_collection(const GfxClutterCollection& native);
		void write_gfx_clutter_collection(GfxClutterCollection& native, const asset_data::gfxworld::gfx_clutter_collection& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_clutter_collection(const asset_data::gfxworld::gfx_clutter_collection& data);
		asset_data::gfxworld::gfx_static_model_collection read_gfx_static_model_collection(const GfxStaticModelCollection& native);
		void write_gfx_static_model_collection(GfxStaticModelCollection& native, const asset_data::gfxworld::gfx_static_model_collection& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_model_collection(const asset_data::gfxworld::gfx_static_model_collection& data);
		asset_data::gfxworld::gfx_static_model_instance read_gfx_static_model_instance(const GfxStaticModelInstance& native);
		void write_gfx_static_model_instance(GfxStaticModelInstance& native, const asset_data::gfxworld::gfx_static_model_instance& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_model_instance(const asset_data::gfxworld::gfx_static_model_instance& data);
		asset_data::gfxworld::gfx_model_static_indirection read_gfx_model_static_indirection(const GfxModelStaticIndirection& native);
		void write_gfx_model_static_indirection(GfxModelStaticIndirection& native, const asset_data::gfxworld::gfx_model_static_indirection& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_model_static_indirection(const asset_data::gfxworld::gfx_model_static_indirection& data);
		asset_data::gfxworld::gfx_lightmap_transform_packed read_gfx_lightmap_transform_packed(const GfxLightmapTransformPacked& native);
		void write_gfx_lightmap_transform_packed(GfxLightmapTransformPacked& native, const asset_data::gfxworld::gfx_lightmap_transform_packed& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_lightmap_transform_packed(const asset_data::gfxworld::gfx_lightmap_transform_packed& data);
		asset_data::gfxworld::gfx_base_s_model_surf_data read_gfx_base_s_model_surf_data(const GfxBaseSModelSurfData& native);
		void write_gfx_base_s_model_surf_data(GfxBaseSModelSurfData& native, const asset_data::gfxworld::gfx_base_s_model_surf_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_base_s_model_surf_data(const asset_data::gfxworld::gfx_base_s_model_surf_data& data);
		asset_data::gfxworld::gfx_base_surf_data read_gfx_base_surf_data(const GfxBaseSurfData& native);
		void write_gfx_base_surf_data(GfxBaseSurfData& native, const asset_data::gfxworld::gfx_base_surf_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_base_surf_data(const asset_data::gfxworld::gfx_base_surf_data& data);
		asset_data::gfxworld::gfx_s_model_instance_data read_gfx_s_model_instance_data(const GfxSModelInstanceData& native);
		void write_gfx_s_model_instance_data(GfxSModelInstanceData& native, const asset_data::gfxworld::gfx_s_model_instance_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_s_model_instance_data(const asset_data::gfxworld::gfx_s_model_instance_data& data);
		asset_data::gfxworld::gfx_wrapped_rw_buffer read_gfx_wrapped_rw_buffer(const GfxWrappedRWBuffer& native);
		void write_gfx_wrapped_rw_buffer(GfxWrappedRWBuffer& native, const asset_data::gfxworld::gfx_wrapped_rw_buffer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_wrapped_rw_buffer(const asset_data::gfxworld::gfx_wrapped_rw_buffer& data);
		asset_data::gfxworld::gfx_shader_buffer_rw_view read_gfx_shader_buffer_rw_view(const GfxShaderBufferRWView& native);
		void write_gfx_shader_buffer_rw_view(GfxShaderBufferRWView& native, const asset_data::gfxworld::gfx_shader_buffer_rw_view& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_shader_buffer_rw_view(const asset_data::gfxworld::gfx_shader_buffer_rw_view& data);
		asset_data::gfxworld::gfx_clutter_collection_gpu_data read_gfx_clutter_collection_gpu_data(const GfxClutterCollectionGPUData& native);
		void write_gfx_clutter_collection_gpu_data(GfxClutterCollectionGPUData& native, const asset_data::gfxworld::gfx_clutter_collection_gpu_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_clutter_collection_gpu_data(const asset_data::gfxworld::gfx_clutter_collection_gpu_data& data);
		asset_data::gfxworld::gfx_static_model_expansion read_gfx_static_model_expansion(const GfxStaticModelExpansion& native);
		void write_gfx_static_model_expansion(GfxStaticModelExpansion& native, const asset_data::gfxworld::gfx_static_model_expansion& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_model_expansion(const asset_data::gfxworld::gfx_static_model_expansion& data);
		asset_data::gfxworld::gfx_static_model_lod read_gfx_static_model_lod(const GfxStaticModelLOD& native);
		void write_gfx_static_model_lod(GfxStaticModelLOD& native, const asset_data::gfxworld::gfx_static_model_lod& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_model_lod(const asset_data::gfxworld::gfx_static_model_lod& data);
		asset_data::gfxworld::gfx_s_model_surf_mat_indirection read_gfx_s_model_surf_mat_indirection(const GfxSModelSurfMatIndirection& native);
		void write_gfx_s_model_surf_mat_indirection(GfxSModelSurfMatIndirection& native, const asset_data::gfxworld::gfx_s_model_surf_mat_indirection& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_s_model_surf_mat_indirection(const asset_data::gfxworld::gfx_s_model_surf_mat_indirection& data);
		asset_data::gfxworld::hlsl_gpu_splined_model_instance read_hlsl_gpu_splined_model_instance(const HLSL_GpuSplinedModelInstance& native);
		void write_hlsl_gpu_splined_model_instance(HLSL_GpuSplinedModelInstance& native, const asset_data::gfxworld::hlsl_gpu_splined_model_instance& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_hlsl_gpu_splined_model_instance(const asset_data::gfxworld::hlsl_gpu_splined_model_instance& data);
		asset_data::gfxworld::hlsl_gpu_spline_segment read_hlsl_gpu_spline_segment(const HLSL_GpuSplineSegment& native);
		void write_hlsl_gpu_spline_segment(HLSL_GpuSplineSegment& native, const asset_data::gfxworld::hlsl_gpu_spline_segment& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_hlsl_gpu_spline_segment(const asset_data::gfxworld::hlsl_gpu_spline_segment& data);
		asset_data::gfxworld::hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t read_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(const HLSL_hlsl_3x3_t_float_HLSL_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& native);
		void write_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(HLSL_hlsl_3x3_t_float_HLSL_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& native, const asset_data::gfxworld::hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(const asset_data::gfxworld::hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& data);
		asset_data::gfxworld::bitarray_dynamic read_bitarray_dynamic(const bitarray_dynamic& native);
		void write_bitarray_dynamic(bitarray_dynamic& native, const asset_data::gfxworld::bitarray_dynamic& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_bitarray_dynamic(const asset_data::gfxworld::bitarray_dynamic& data);
		asset_data::gfxworld::gfx_world_draw read_gfx_world_draw(const GfxWorldDraw& native);
		void write_gfx_world_draw(GfxWorldDraw& native, const asset_data::gfxworld::gfx_world_draw& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_draw(const asset_data::gfxworld::gfx_world_draw& data);
		asset_data::gfxworld::gfx_world_reflection_probe_data read_gfx_world_reflection_probe_data(const GfxWorldReflectionProbeData& native);
		void write_gfx_world_reflection_probe_data(GfxWorldReflectionProbeData& native, const asset_data::gfxworld::gfx_world_reflection_probe_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_reflection_probe_data(const asset_data::gfxworld::gfx_world_reflection_probe_data& data);
		asset_data::gfxworld::gfx_reflection_probe read_gfx_reflection_probe(const GfxReflectionProbe& native);
		void write_gfx_reflection_probe(GfxReflectionProbe& native, const asset_data::gfxworld::gfx_reflection_probe& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_reflection_probe(const asset_data::gfxworld::gfx_reflection_probe& data);
		asset_data::gfxworld::gfx_reflection_probe_relighting_data read_gfx_reflection_probe_relighting_data(const GfxReflectionProbeRelightingData& native);
		void write_gfx_reflection_probe_relighting_data(GfxReflectionProbeRelightingData& native, const asset_data::gfxworld::gfx_reflection_probe_relighting_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_reflection_probe_relighting_data(const asset_data::gfxworld::gfx_reflection_probe_relighting_data& data);
		asset_data::gfxworld::gfx_reflection_probe_instance read_gfx_reflection_probe_instance(const GfxReflectionProbeInstance& native);
		void write_gfx_reflection_probe_instance(GfxReflectionProbeInstance& native, const asset_data::gfxworld::gfx_reflection_probe_instance& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_reflection_probe_instance(const asset_data::gfxworld::gfx_reflection_probe_instance& data);
		asset_data::gfxworld::gfx_reflection_probe_obb read_gfx_reflection_probe_obb(const GfxReflectionProbeObb& native);
		void write_gfx_reflection_probe_obb(GfxReflectionProbeObb& native, const asset_data::gfxworld::gfx_reflection_probe_obb& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_reflection_probe_obb(const asset_data::gfxworld::gfx_reflection_probe_obb& data);
		asset_data::gfxworld::gfx_sh3_vec read_gfx_sh3_vec(const GfxSH3Vec& native);
		void write_gfx_sh3_vec(GfxSH3Vec& native, const asset_data::gfxworld::gfx_sh3_vec& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_sh3_vec(const asset_data::gfxworld::gfx_sh3_vec& data);
		asset_data::gfxworld::gfx_world_lightmap_reindex_data read_gfx_world_lightmap_reindex_data(const GfxWorldLightmapReindexData& native);
		void write_gfx_world_lightmap_reindex_data(GfxWorldLightmapReindexData& native, const asset_data::gfxworld::gfx_world_lightmap_reindex_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_lightmap_reindex_data(const asset_data::gfxworld::gfx_world_lightmap_reindex_data& data);
		asset_data::gfxworld::gfx_world_lightmap_reindex_data_element read_gfx_world_lightmap_reindex_data_element(const GfxWorldLightmapReindexDataElement& native);
		void write_gfx_world_lightmap_reindex_data_element(GfxWorldLightmapReindexDataElement& native, const asset_data::gfxworld::gfx_world_lightmap_reindex_data_element& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_lightmap_reindex_data_element(const asset_data::gfxworld::gfx_world_lightmap_reindex_data_element& data);
		asset_data::gfxworld::gfx_world_packed_lightmap read_gfx_world_packed_lightmap(const GfxWorldPackedLightmap& native);
		void write_gfx_world_packed_lightmap(GfxWorldPackedLightmap& native, const asset_data::gfxworld::gfx_world_packed_lightmap& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_packed_lightmap(const asset_data::gfxworld::gfx_world_packed_lightmap& data);
		asset_data::gfxworld::gfx_static_decal_volume_material read_gfx_static_decal_volume_material(const GfxStaticDecalVolumeMaterial& native);
		void write_gfx_static_decal_volume_material(GfxStaticDecalVolumeMaterial& native, const asset_data::gfxworld::gfx_static_decal_volume_material& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_decal_volume_material(const asset_data::gfxworld::gfx_static_decal_volume_material& data);
		asset_data::gfxworld::gfx_static_decal_volume_mask read_gfx_static_decal_volume_mask(const GfxStaticDecalVolumeMask& native);
		void write_gfx_static_decal_volume_mask(GfxStaticDecalVolumeMask& native, const asset_data::gfxworld::gfx_static_decal_volume_mask& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_decal_volume_mask(const asset_data::gfxworld::gfx_static_decal_volume_mask& data);
		asset_data::gfxworld::gfx_decal_volume_collection read_gfx_decal_volume_collection(const GfxDecalVolumeCollection& native);
		void write_gfx_decal_volume_collection(GfxDecalVolumeCollection& native, const asset_data::gfxworld::gfx_decal_volume_collection& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_decal_volume_collection(const asset_data::gfxworld::gfx_decal_volume_collection& data);
		asset_data::gfxworld::gfx_world_volumetrics read_gfx_world_volumetrics(const GfxWorldVolumetrics& native);
		void write_gfx_world_volumetrics(GfxWorldVolumetrics& native, const asset_data::gfxworld::gfx_world_volumetrics& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_volumetrics(const asset_data::gfxworld::gfx_world_volumetrics& data);
		asset_data::gfxworld::gfx_volumetric read_gfx_volumetric(const GfxVolumetric& native);
		void write_gfx_volumetric(GfxVolumetric& native, const asset_data::gfxworld::gfx_volumetric& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_volumetric(const asset_data::gfxworld::gfx_volumetric& data);
		asset_data::gfxworld::gfx_oriented_bounding_box read_gfx_oriented_bounding_box(const GfxOrientedBoundingBox& native);
		void write_gfx_oriented_bounding_box(GfxOrientedBoundingBox& native, const asset_data::gfxworld::gfx_oriented_bounding_box& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_oriented_bounding_box(const asset_data::gfxworld::gfx_oriented_bounding_box& data);
		asset_data::gfxworld::gfx_volumetric_mask read_gfx_volumetric_mask(const GfxVolumetricMask& native);
		void write_gfx_volumetric_mask(GfxVolumetricMask& native, const asset_data::gfxworld::gfx_volumetric_mask& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_volumetric_mask(const asset_data::gfxworld::gfx_volumetric_mask& data);
		asset_data::gfxworld::gfx_compressed_sun_shadow_params read_gfx_compressed_sun_shadow_params(const GfxCompressedSunShadowParams& native);
		void write_gfx_compressed_sun_shadow_params(GfxCompressedSunShadowParams& native, const asset_data::gfxworld::gfx_compressed_sun_shadow_params& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_compressed_sun_shadow_params(const asset_data::gfxworld::gfx_compressed_sun_shadow_params& data);
		asset_data::gfxworld::gfx_dynamic_lightset read_gfx_dynamic_lightset(const GfxDynamicLightset& native);
		void write_gfx_dynamic_lightset(GfxDynamicLightset& native, const asset_data::gfxworld::gfx_dynamic_lightset& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset(const asset_data::gfxworld::gfx_dynamic_lightset& data);
		asset_data::gfxworld::gfx_dynamic_lightset_bf_data read_gfx_dynamic_lightset_bf_data(const GfxDynamicLightsetBFData& native);
		void write_gfx_dynamic_lightset_bf_data(GfxDynamicLightsetBFData& native, const asset_data::gfxworld::gfx_dynamic_lightset_bf_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_bf_data(const asset_data::gfxworld::gfx_dynamic_lightset_bf_data& data);
		asset_data::gfxworld::gfx_dynamic_lightset_light_info read_gfx_dynamic_lightset_light_info(const GfxDynamicLightsetLightInfo& native);
		void write_gfx_dynamic_lightset_light_info(GfxDynamicLightsetLightInfo& native, const asset_data::gfxworld::gfx_dynamic_lightset_light_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_light_info(const asset_data::gfxworld::gfx_dynamic_lightset_light_info& data);
		asset_data::gfxworld::gfx_dynamic_lightset_portal_gi_data read_gfx_dynamic_lightset_portal_gi_data(const GfxDynamicLightsetPortalGIData& native);
		void write_gfx_dynamic_lightset_portal_gi_data(GfxDynamicLightsetPortalGIData& native, const asset_data::gfxworld::gfx_dynamic_lightset_portal_gi_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_portal_gi_data(const asset_data::gfxworld::gfx_dynamic_lightset_portal_gi_data& data);
		asset_data::gfxworld::gfx_dynamic_lightset_product_gi_data read_gfx_dynamic_lightset_product_gi_data(const GfxDynamicLightsetProductGIData& native);
		void write_gfx_dynamic_lightset_product_gi_data(GfxDynamicLightsetProductGIData& native, const asset_data::gfxworld::gfx_dynamic_lightset_product_gi_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_product_gi_data(const asset_data::gfxworld::gfx_dynamic_lightset_product_gi_data& data);
		asset_data::gfxworld::gfx_dynamic_lightset_lm_data read_gfx_dynamic_lightset_lm_data(const GfxDynamicLightsetLMData& native);
		void write_gfx_dynamic_lightset_lm_data(GfxDynamicLightsetLMData& native, const asset_data::gfxworld::gfx_dynamic_lightset_lm_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_lm_data(const asset_data::gfxworld::gfx_dynamic_lightset_lm_data& data);
		asset_data::gfxworld::gfx_dynamic_lightset_g_chunk read_gfx_dynamic_lightset_g_chunk(const GfxDynamicLightsetGChunk& native);
		void write_gfx_dynamic_lightset_g_chunk(GfxDynamicLightsetGChunk& native, const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_g_chunk(const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk& data);
		asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_info read_gfx_dynamic_lightset_g_chunk_info(const GfxDynamicLightsetGChunkInfo& native);
		void write_gfx_dynamic_lightset_g_chunk_info(GfxDynamicLightsetGChunkInfo& native, const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_g_chunk_info(const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_info& data);
		asset_data::gfxworld::gfx_dynamic_lightset_probe_data read_gfx_dynamic_lightset_probe_data(const GfxDynamicLightsetProbeData& native);
		void write_gfx_dynamic_lightset_probe_data(GfxDynamicLightsetProbeData& native, const asset_data::gfxworld::gfx_dynamic_lightset_probe_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_probe_data(const asset_data::gfxworld::gfx_dynamic_lightset_probe_data& data);
		asset_data::gfxworld::gfx_sh_compressed_probe_data read_gfx_sh_compressed_probe_data(const GfxSHCompressedProbeData& native);
		void write_gfx_sh_compressed_probe_data(GfxSHCompressedProbeData& native, const asset_data::gfxworld::gfx_sh_compressed_probe_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_sh_compressed_probe_data(const asset_data::gfxworld::gfx_sh_compressed_probe_data& data);
		asset_data::gfxworld::gfx_mayhem_self_vis read_gfx_mayhem_self_vis(const GfxMayhemSelfVis& native);
		void write_gfx_mayhem_self_vis(GfxMayhemSelfVis& native, const asset_data::gfxworld::gfx_mayhem_self_vis& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_mayhem_self_vis(const asset_data::gfxworld::gfx_mayhem_self_vis& data);
		asset_data::gfxworld::gfx_frustum_lights read_gfx_frustum_lights(const GfxFrustumLights& native);
		void write_gfx_frustum_lights(GfxFrustumLights& native, const asset_data::gfxworld::gfx_frustum_lights& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_frustum_lights(const asset_data::gfxworld::gfx_frustum_lights& data);
		asset_data::gfxworld::gfx_light_view_frustum read_gfx_light_view_frustum(const GfxLightViewFrustum& native);
		void write_gfx_light_view_frustum(GfxLightViewFrustum& native, const asset_data::gfxworld::gfx_light_view_frustum& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_light_view_frustum(const asset_data::gfxworld::gfx_light_view_frustum& data);
		asset_data::gfxworld::gfx_light read_gfx_light(const GfxLight& native);
		void write_gfx_light(GfxLight& native, const asset_data::gfxworld::gfx_light& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_light(const asset_data::gfxworld::gfx_light& data);
		asset_data::gfxworld::gfx_voxel_tree read_gfx_voxel_tree(const GfxVoxelTree& native);
		void write_gfx_voxel_tree(GfxVoxelTree& native, const asset_data::gfxworld::gfx_voxel_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_voxel_tree(const asset_data::gfxworld::gfx_voxel_tree& data);
		asset_data::gfxworld::gfx_voxel_tree_header read_gfx_voxel_tree_header(const GfxVoxelTreeHeader& native);
		void write_gfx_voxel_tree_header(GfxVoxelTreeHeader& native, const asset_data::gfxworld::gfx_voxel_tree_header& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_voxel_tree_header(const asset_data::gfxworld::gfx_voxel_tree_header& data);
		asset_data::gfxworld::gfx_voxel_top_down_view_node read_gfx_voxel_top_down_view_node(const GfxVoxelTopDownViewNode& native);
		void write_gfx_voxel_top_down_view_node(GfxVoxelTopDownViewNode& native, const asset_data::gfxworld::gfx_voxel_top_down_view_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_voxel_top_down_view_node(const asset_data::gfxworld::gfx_voxel_top_down_view_node& data);
		asset_data::gfxworld::gfx_voxel_internal_node read_gfx_voxel_internal_node(const GfxVoxelInternalNode& native);
		void write_gfx_voxel_internal_node(GfxVoxelInternalNode& native, const asset_data::gfxworld::gfx_voxel_internal_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_voxel_internal_node(const asset_data::gfxworld::gfx_voxel_internal_node& data);
		asset_data::gfxworld::gfx_voxel_leaf_node read_gfx_voxel_leaf_node(const GfxVoxelLeafNode& native);
		void write_gfx_voxel_leaf_node(GfxVoxelLeafNode& native, const asset_data::gfxworld::gfx_voxel_leaf_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_voxel_leaf_node(const asset_data::gfxworld::gfx_voxel_leaf_node& data);
		asset_data::gfxworld::replay_voxel_runtime_state read_replay_voxel_runtime_state(const Replay_VoxelRuntimeState& native);
		void write_replay_voxel_runtime_state(Replay_VoxelRuntimeState& native, const asset_data::gfxworld::replay_voxel_runtime_state& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_replay_voxel_runtime_state(const asset_data::gfxworld::replay_voxel_runtime_state& data);
		asset_data::gfxworld::gfx_heightfield read_gfx_heightfield(const GfxHeightfield& native);
		void write_gfx_heightfield(GfxHeightfield& native, const asset_data::gfxworld::gfx_heightfield& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_heightfield(const asset_data::gfxworld::gfx_heightfield& data);
		asset_data::gfxworld::gfx_precomputed_sky_illumination read_gfx_precomputed_sky_illumination(const GfxPrecomputedSkyIllumination& native);
		void write_gfx_precomputed_sky_illumination(GfxPrecomputedSkyIllumination& native, const asset_data::gfxworld::gfx_precomputed_sky_illumination& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_precomputed_sky_illumination(const asset_data::gfxworld::gfx_precomputed_sky_illumination& data);
		asset_data::gfxworld::gfx_brush_model read_gfx_brush_model(const GfxBrushModel& native);
		void write_gfx_brush_model(GfxBrushModel& native, const asset_data::gfxworld::gfx_brush_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_brush_model(const asset_data::gfxworld::gfx_brush_model& data);
		asset_data::gfxworld::gfx_brush_model_writable read_gfx_brush_model_writable(const GfxBrushModelWritable& native);
		void write_gfx_brush_model_writable(GfxBrushModelWritable& native, const asset_data::gfxworld::gfx_brush_model_writable& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_brush_model_writable(const asset_data::gfxworld::gfx_brush_model_writable& data);
		asset_data::gfxworld::gfx_model_lightmap_inst read_gfx_model_lightmap_inst(const GfxModelLightmapInst& native);
		void write_gfx_model_lightmap_inst(GfxModelLightmapInst& native, const asset_data::gfxworld::gfx_model_lightmap_inst& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_model_lightmap_inst(const asset_data::gfxworld::gfx_model_lightmap_inst& data);
		asset_data::gfxworld::gfx_map_ent_lookup read_gfx_map_ent_lookup(const GfxMapEntLookup& native);
		void write_gfx_map_ent_lookup(GfxMapEntLookup& native, const asset_data::gfxworld::gfx_map_ent_lookup& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_map_ent_lookup(const asset_data::gfxworld::gfx_map_ent_lookup& data);
		asset_data::gfxworld::sunflare_t read_sunflare_t(const sunflare_t& native);
		void write_sunflare_t(sunflare_t& native, const asset_data::gfxworld::sunflare_t& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_sunflare_t(const asset_data::gfxworld::sunflare_t& data);
		asset_data::gfxworld::gfx_scene_dyn_model read_gfx_scene_dyn_model(const GfxSceneDynModel& native);
		void write_gfx_scene_dyn_model(GfxSceneDynModel& native, const asset_data::gfxworld::gfx_scene_dyn_model& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_scene_dyn_model(const asset_data::gfxworld::gfx_scene_dyn_model& data);
		asset_data::gfxworld::x_model_draw_info read_x_model_draw_info(const XModelDrawInfo& native);
		void write_x_model_draw_info(XModelDrawInfo& native, const asset_data::gfxworld::x_model_draw_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_x_model_draw_info(const asset_data::gfxworld::x_model_draw_info& data);
		asset_data::gfxworld::gfx_scene_dyn_brush read_gfx_scene_dyn_brush(const GfxSceneDynBrush& native);
		void write_gfx_scene_dyn_brush(GfxSceneDynBrush& native, const asset_data::gfxworld::gfx_scene_dyn_brush& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_scene_dyn_brush(const asset_data::gfxworld::gfx_scene_dyn_brush& data);
		asset_data::gfxworld::b_model_draw_info read_b_model_draw_info(const BModelDrawInfo& native);
		void write_b_model_draw_info(BModelDrawInfo& native, const asset_data::gfxworld::b_model_draw_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_b_model_draw_info(const asset_data::gfxworld::b_model_draw_info& data);
		asset_data::gfxworld::gfx_shadow_geometry read_gfx_shadow_geometry(const GfxShadowGeometry& native);
		void write_gfx_shadow_geometry(GfxShadowGeometry& native, const asset_data::gfxworld::gfx_shadow_geometry& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_shadow_geometry(const asset_data::gfxworld::gfx_shadow_geometry& data);
		asset_data::gfxworld::gfx_light_aabb read_gfx_light_aabb(const GfxLightAABB& native);
		void write_gfx_light_aabb(GfxLightAABB& native, const asset_data::gfxworld::gfx_light_aabb& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_light_aabb(const asset_data::gfxworld::gfx_light_aabb& data);
		asset_data::gfxworld::gfx_light_aabb_node read_gfx_light_aabb_node(const GfxLightAABBNode& native);
		void write_gfx_light_aabb_node(GfxLightAABBNode& native, const asset_data::gfxworld::gfx_light_aabb_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_light_aabb_node(const asset_data::gfxworld::gfx_light_aabb_node& data);
		asset_data::gfxworld::gfx_world_dpvs_static read_gfx_world_dpvs_static(const GfxWorldDpvsStatic& native);
		void write_gfx_world_dpvs_static(GfxWorldDpvsStatic& native, const asset_data::gfxworld::gfx_world_dpvs_static& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_dpvs_static(const asset_data::gfxworld::gfx_world_dpvs_static& data);
		asset_data::gfxworld::gfx_world_dpvs_dynamic read_gfx_world_dpvs_dynamic(const GfxWorldDpvsDynamic& native, const GfxWorld& outer);
		void write_gfx_world_dpvs_dynamic(GfxWorldDpvsDynamic& native, const asset_data::gfxworld::gfx_world_dpvs_dynamic& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_dpvs_dynamic(const asset_data::gfxworld::gfx_world_dpvs_dynamic& data);
		asset_data::gfxworld::umbra_gate read_umbra_gate(const UmbraGate& native);
		void write_umbra_gate(UmbraGate& native, const asset_data::gfxworld::umbra_gate& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_umbra_gate(const asset_data::gfxworld::umbra_gate& data);
		asset_data::gfxworld::mdao_volume read_mdao_volume(const MdaoVolume& native);
		void write_mdao_volume(MdaoVolume& native, const asset_data::gfxworld::mdao_volume& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_mdao_volume(const asset_data::gfxworld::mdao_volume& data);
		asset_data::gfxworld::extent_bounds read_extent_bounds(const ExtentBounds& native);
		void write_extent_bounds(ExtentBounds& native, const asset_data::gfxworld::extent_bounds& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_extent_bounds(const asset_data::gfxworld::extent_bounds& data);
		asset_data::gfxworld::stream_tree read_stream_tree(const StreamTree& native);
		void write_stream_tree(StreamTree& native, const asset_data::gfxworld::stream_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_stream_tree(const asset_data::gfxworld::stream_tree& data);
		asset_data::gfxworld::stream_tree_node read_stream_tree_node(const StreamTreeNode& native);
		void write_stream_tree_node(StreamTreeNode& native, const asset_data::gfxworld::stream_tree_node& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_stream_tree_node(const asset_data::gfxworld::stream_tree_node& data);
		asset_data::gfxworld::stream_mtl_img_info read_stream_mtl_img_info(const StreamMtlImgInfo& native);
		void write_stream_mtl_img_info(StreamMtlImgInfo& native, const asset_data::gfxworld::stream_mtl_img_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_stream_mtl_img_info(const asset_data::gfxworld::stream_mtl_img_info& data);
		asset_data::gfxworld::stream_mtl_group read_stream_mtl_group(const StreamMtlGroup& native);
		void write_stream_mtl_group(StreamMtlGroup& native, const asset_data::gfxworld::stream_mtl_group& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_stream_mtl_group(const asset_data::gfxworld::stream_mtl_group& data);
		asset_data::gfxworld::stream_tree_grid read_stream_tree_grid(const StreamTreeGrid& native);
		void write_stream_tree_grid(StreamTreeGrid& native, const asset_data::gfxworld::stream_tree_grid& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_stream_tree_grid(const asset_data::gfxworld::stream_tree_grid& data);
		asset_data::gfxworld::gfx_world_transient_zone read_gfx_world_transient_zone(const GfxWorldTransientZone& native);
		void write_gfx_world_transient_zone(GfxWorldTransientZone& native, const asset_data::gfxworld::gfx_world_transient_zone& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_transient_zone(const asset_data::gfxworld::gfx_world_transient_zone& data);
		asset_data::gfxworld::gfx_world_draw_verts read_gfx_world_draw_verts(const GfxWorldDrawVerts& native);
		void write_gfx_world_draw_verts(GfxWorldDrawVerts& native, const asset_data::gfxworld::gfx_world_draw_verts& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_draw_verts(const asset_data::gfxworld::gfx_world_draw_verts& data);
		asset_data::gfxworld::gfx_index_buffer read_gfx_index_buffer(const GfxIndexBuffer& native);
		void write_gfx_index_buffer(GfxIndexBuffer& native, const asset_data::gfxworld::gfx_index_buffer& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_index_buffer(const asset_data::gfxworld::gfx_index_buffer& data);
		asset_data::gfxworld::gfx_world_draw_cells read_gfx_world_draw_cells(const GfxWorldDrawCells& native);
		void write_gfx_world_draw_cells(GfxWorldDrawCells& native, const asset_data::gfxworld::gfx_world_draw_cells& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_world_draw_cells(const asset_data::gfxworld::gfx_world_draw_cells& data);
		asset_data::gfxworld::gfx_cell_tree_count read_gfx_cell_tree_count(const GfxCellTreeCount& native);
		void write_gfx_cell_tree_count(GfxCellTreeCount& native, const asset_data::gfxworld::gfx_cell_tree_count& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_cell_tree_count(const asset_data::gfxworld::gfx_cell_tree_count& data);
		asset_data::gfxworld::gfx_cell_tree read_gfx_cell_tree(const GfxCellTree& native, const GfxWorldDrawCells& outer);
		void write_gfx_cell_tree(GfxCellTree& native, const asset_data::gfxworld::gfx_cell_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_cell_tree(const asset_data::gfxworld::gfx_cell_tree& data);
		asset_data::gfxworld::gfx_aabb_tree read_gfx_aabb_tree(const GfxAabbTree& native);
		void write_gfx_aabb_tree(GfxAabbTree& native, const asset_data::gfxworld::gfx_aabb_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_aabb_tree(const asset_data::gfxworld::gfx_aabb_tree& data);
		asset_data::gfxworld::gfx_lightmap_atlas_data_temp read_gfx_lightmap_atlas_data_temp(const GfxLightmapAtlasDataTemp& native);
		void write_gfx_lightmap_atlas_data_temp(GfxLightmapAtlasDataTemp& native, const asset_data::gfxworld::gfx_lightmap_atlas_data_temp& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_lightmap_atlas_data_temp(const asset_data::gfxworld::gfx_lightmap_atlas_data_temp& data);
		asset_data::gfxworld::gfx_lightmap_raw read_gfx_lightmap_raw(const GfxLightmapRaw& native);
		void write_gfx_lightmap_raw(GfxLightmapRaw& native, const asset_data::gfxworld::gfx_lightmap_raw& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_lightmap_raw(const asset_data::gfxworld::gfx_lightmap_raw& data);
		asset_data::gfxworld::gfx_image_raw read_gfx_image_raw(const GfxImageRaw& native);
		void write_gfx_image_raw(GfxImageRaw& native, const asset_data::gfxworld::gfx_image_raw& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_image_raw(const asset_data::gfxworld::gfx_image_raw& data);
		asset_data::gfxworld::gfx_image_desc read_gfx_image_desc(const GfxImageDesc& native);
		void write_gfx_image_desc(GfxImageDesc& native, const asset_data::gfxworld::gfx_image_desc& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_image_desc(const asset_data::gfxworld::gfx_image_desc& data);
		asset_data::gfxworld::gfx_gpu_light_grid_transient_data read_gfx_gpu_light_grid_transient_data(const GfxGpuLightGridTransientData& native);
		void write_gfx_gpu_light_grid_transient_data(GfxGpuLightGridTransientData& native, const asset_data::gfxworld::gfx_gpu_light_grid_transient_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid_transient_data(const asset_data::gfxworld::gfx_gpu_light_grid_transient_data& data);
		asset_data::gfxworld::gfx_gpu_light_grid read_gfx_gpu_light_grid(const GfxGpuLightGrid& native);
		void write_gfx_gpu_light_grid(GfxGpuLightGrid& native, const asset_data::gfxworld::gfx_gpu_light_grid& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid(const asset_data::gfxworld::gfx_gpu_light_grid& data);
		asset_data::gfxworld::gfx_gpu_light_grid_zone read_gfx_gpu_light_grid_zone(const GfxGpuLightGridZone& native);
		void write_gfx_gpu_light_grid_zone(GfxGpuLightGridZone& native, const asset_data::gfxworld::gfx_gpu_light_grid_zone& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid_zone(const asset_data::gfxworld::gfx_gpu_light_grid_zone& data);
		asset_data::gfxworld::gfx_sh_probe_data read_gfx_sh_probe_data(const GfxSHProbeData& native);
		void write_gfx_sh_probe_data(GfxSHProbeData& native, const asset_data::gfxworld::gfx_sh_probe_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_sh_probe_data(const asset_data::gfxworld::gfx_sh_probe_data& data);
		asset_data::gfxworld::gfx_gpu_light_grid_voxel_tree read_gfx_gpu_light_grid_voxel_tree(const GfxGpuLightGridVoxelTree& native);
		void write_gfx_gpu_light_grid_voxel_tree(GfxGpuLightGridVoxelTree& native, const asset_data::gfxworld::gfx_gpu_light_grid_voxel_tree& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid_voxel_tree(const asset_data::gfxworld::gfx_gpu_light_grid_voxel_tree& data);
		asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron read_gfx_gpu_light_grid_tetrahedron(const GfxGpuLightGridTetrahedron& native);
		void write_gfx_gpu_light_grid_tetrahedron(GfxGpuLightGridTetrahedron& native, const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid_tetrahedron(const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron& data);
		asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_neighbors read_gfx_gpu_light_grid_tetrahedron_neighbors(const GfxGpuLightGridTetrahedronNeighbors& native);
		void write_gfx_gpu_light_grid_tetrahedron_neighbors(GfxGpuLightGridTetrahedronNeighbors& native, const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_neighbors& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid_tetrahedron_neighbors(const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_neighbors& data);
		asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_visibility read_gfx_gpu_light_grid_tetrahedron_visibility(const GfxGpuLightGridTetrahedronVisibility& native);
		void write_gfx_gpu_light_grid_tetrahedron_visibility(GfxGpuLightGridTetrahedronVisibility& native, const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_visibility& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid_tetrahedron_visibility(const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_visibility& data);
		asset_data::gfxworld::gfx_gpu_light_grid_voxel_start_tetrahedron read_gfx_gpu_light_grid_voxel_start_tetrahedron(const GfxGpuLightGridVoxelStartTetrahedron& native);
		void write_gfx_gpu_light_grid_voxel_start_tetrahedron(GfxGpuLightGridVoxelStartTetrahedron& native, const asset_data::gfxworld::gfx_gpu_light_grid_voxel_start_tetrahedron& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_light_grid_voxel_start_tetrahedron(const asset_data::gfxworld::gfx_gpu_light_grid_voxel_start_tetrahedron& data);
		asset_data::gfxworld::gfx_static_model_light_grid_volume read_gfx_static_model_light_grid_volume(const GfxStaticModelLightGridVolume& native);
		void write_gfx_static_model_light_grid_volume(GfxStaticModelLightGridVolume& native, const asset_data::gfxworld::gfx_static_model_light_grid_volume& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_model_light_grid_volume(const asset_data::gfxworld::gfx_static_model_light_grid_volume& data);
		asset_data::gfxworld::gfx_dynamic_lightset_l_gpp_zone_info read_gfx_dynamic_lightset_l_gpp_zone_info(const GfxDynamicLightsetLGppZoneInfo& native);
		void write_gfx_dynamic_lightset_l_gpp_zone_info(GfxDynamicLightsetLGppZoneInfo& native, const asset_data::gfxworld::gfx_dynamic_lightset_l_gpp_zone_info& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_dynamic_lightset_l_gpp_zone_info(const asset_data::gfxworld::gfx_dynamic_lightset_l_gpp_zone_info& data);
		asset_data::gfxworld::gfx_gpu_dyn_lightsets_lgv_reference read_gfx_gpu_dyn_lightsets_lgv_reference(const GfxGpuDynLightsetsLGVReference& native);
		void write_gfx_gpu_dyn_lightsets_lgv_reference(GfxGpuDynLightsetsLGVReference& native, const asset_data::gfxworld::gfx_gpu_dyn_lightsets_lgv_reference& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_gpu_dyn_lightsets_lgv_reference(const asset_data::gfxworld::gfx_gpu_dyn_lightsets_lgv_reference& data);
		asset_data::gfxworld::gfx_light_grid_volume_atlas_data_temp read_gfx_light_grid_volume_atlas_data_temp(const GfxLightGridVolumeAtlasDataTemp& native);
		void write_gfx_light_grid_volume_atlas_data_temp(GfxLightGridVolumeAtlasDataTemp& native, const asset_data::gfxworld::gfx_light_grid_volume_atlas_data_temp& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_light_grid_volume_atlas_data_temp(const asset_data::gfxworld::gfx_light_grid_volume_atlas_data_temp& data);
		asset_data::gfxworld::light_grid_volume_packing_params read_light_grid_volume_packing_params(const LightGridVolumePackingParams& native);
		void write_light_grid_volume_packing_params(LightGridVolumePackingParams& native, const asset_data::gfxworld::light_grid_volume_packing_params& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_light_grid_volume_packing_params(const asset_data::gfxworld::light_grid_volume_packing_params& data);
		asset_data::gfxworld::light_grid_volume_transform_params read_light_grid_volume_transform_params(const LightGridVolumeTransformParams& native);
		void write_light_grid_volume_transform_params(LightGridVolumeTransformParams& native, const asset_data::gfxworld::light_grid_volume_transform_params& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_light_grid_volume_transform_params(const asset_data::gfxworld::light_grid_volume_transform_params& data);
		asset_data::gfxworld::gfx_reflection_probe_transient_data read_gfx_reflection_probe_transient_data(const GfxReflectionProbeTransientData& native);
		void write_gfx_reflection_probe_transient_data(GfxReflectionProbeTransientData& native, const asset_data::gfxworld::gfx_reflection_probe_transient_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_reflection_probe_transient_data(const asset_data::gfxworld::gfx_reflection_probe_transient_data& data);
		asset_data::gfxworld::gfx_image_reflection_probe read_gfx_image_reflection_probe(const GfxImageReflectionProbe& native);
		void write_gfx_image_reflection_probe(GfxImageReflectionProbe& native, const asset_data::gfxworld::gfx_image_reflection_probe& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_image_reflection_probe(const asset_data::gfxworld::gfx_image_reflection_probe& data);
		asset_data::gfxworld::gfx_decal_volume_transient_data read_gfx_decal_volume_transient_data(const GfxDecalVolumeTransientData& native);
		void write_gfx_decal_volume_transient_data(GfxDecalVolumeTransientData& native, const asset_data::gfxworld::gfx_decal_volume_transient_data& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_decal_volume_transient_data(const asset_data::gfxworld::gfx_decal_volume_transient_data& data);
		asset_data::gfxworld::gfx_static_decal_volume_packed read_gfx_static_decal_volume_packed(const GfxStaticDecalVolumePacked& native);
		void write_gfx_static_decal_volume_packed(GfxStaticDecalVolumePacked& native, const asset_data::gfxworld::gfx_static_decal_volume_packed& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_decal_volume_packed(const asset_data::gfxworld::gfx_static_decal_volume_packed& data);
		asset_data::gfxworld::gfx_static_decal_volume_uid read_gfx_static_decal_volume_uid(const GfxStaticDecalVolumeUid& native);
		void write_gfx_static_decal_volume_uid(GfxStaticDecalVolumeUid& native, const asset_data::gfxworld::gfx_static_decal_volume_uid& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_static_decal_volume_uid(const asset_data::gfxworld::gfx_static_decal_volume_uid& data);
		asset_data::gfxworld::gfx_compressed_sun_shadow_data_temp read_gfx_compressed_sun_shadow_data_temp(const GfxCompressedSunShadowDataTemp& native);
		void write_gfx_compressed_sun_shadow_data_temp(GfxCompressedSunShadowDataTemp& native, const asset_data::gfxworld::gfx_compressed_sun_shadow_data_temp& data, const asset_resolver& resolve, plain::context& memory);
		std::size_t size_gfx_compressed_sun_shadow_data_temp(const asset_data::gfxworld::gfx_compressed_sun_shadow_data_temp& data);

		asset_data::gfxworld::bounds read_bounds(const Bounds& native)
		{
			plain::trace("read_bounds");
			asset_data::gfxworld::bounds data;
			data.mid_point = plain::read_scalars<float, 3>(&native.midPoint);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_bounds(Bounds& native, const asset_data::gfxworld::bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.midPoint, data.mid_point);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_bounds([[maybe_unused]] const asset_data::gfxworld::bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_world_dpvs_planes read_gfx_world_dpvs_planes(const GfxWorldDpvsPlanes& native)
		{
			plain::trace("read_gfx_world_dpvs_planes");
			asset_data::gfxworld::gfx_world_dpvs_planes data;
			data.cell_count = native.cellCount;
			data.plane_count = native.planeCount;
			data.planes = plain::read_array_if_present(native.planes, native.planeCount, "GfxWorldDpvsPlanes.planes", [&](const auto& item) { return read_gfx_world_dpvs_plane(item); });
			data.node_count = native.nodeCount;
			data.nodes = plain::read_array_if_present(native.nodes, native.nodeCount, "GfxWorldDpvsPlanes.nodes", [&](const auto& item) { return item; });
			data.scene_ent_cell_bits = plain::read_array_if_present(native.sceneEntCellBits, ((native.cellCount) * 512), "GfxWorldDpvsPlanes.sceneEntCellBits", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_world_dpvs_planes(GfxWorldDpvsPlanes& native, const asset_data::gfxworld::gfx_world_dpvs_planes& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.cellCount = data.cell_count;
			native.planeCount = data.plane_count;
			native.planes = plain::write_array<GfxWorldDpvsPlane>(data.planes, memory, [&](auto& out, const auto& in) { write_gfx_world_dpvs_plane(out, in, resolve, memory); });
			native.nodeCount = data.node_count;
			native.nodes = plain::write_array<std::uint16_t>(data.nodes, memory, [&](auto& out, const auto& in) { out = in; });
			native.sceneEntCellBits = plain::write_array<std::uint32_t>(data.scene_ent_cell_bits, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_world_dpvs_planes([[maybe_unused]] const asset_data::gfxworld::gfx_world_dpvs_planes& data)
		{
			std::size_t size = 0;
			if (data.planes)
			{
				size += plain::sized(std::max<std::size_t>(data.planes->size(), 1) * sizeof(GfxWorldDpvsPlane));
				for (const auto& item : *data.planes)
				{
					size += size_gfx_world_dpvs_plane(item);
				}
			}
			if (data.nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.nodes->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.scene_ent_cell_bits)
			{
				size += plain::sized(std::max<std::size_t>(data.scene_ent_cell_bits->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_buffer_resource read_gfx_buffer_resource(const GfxBufferResource& native)
		{
			plain::trace("read_gfx_buffer_resource");
			asset_data::gfxworld::gfx_buffer_resource data;
			plain::require_zero(native.resource, "GfxBufferResource.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.size = native.size;
			return data;
		}

		void write_gfx_buffer_resource(GfxBufferResource& native, const asset_data::gfxworld::gfx_buffer_resource& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.size = data.size;
		}

		std::size_t size_gfx_buffer_resource([[maybe_unused]] const asset_data::gfxworld::gfx_buffer_resource& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_shader_buffer_view read_gfx_shader_buffer_view(const GfxShaderBufferView& native)
		{
			plain::trace("read_gfx_shader_buffer_view");
			asset_data::gfxworld::gfx_shader_buffer_view data;
			plain::require_zero(native.resource, "GfxShaderBufferView.resource");
			data.view = native.view;
			return data;
		}

		void write_gfx_shader_buffer_view(GfxShaderBufferView& native, const asset_data::gfxworld::gfx_shader_buffer_view& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.view = data.view;
		}

		std::size_t size_gfx_shader_buffer_view([[maybe_unused]] const asset_data::gfxworld::gfx_shader_buffer_view& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_wrapped_base_buffer read_gfx_wrapped_base_buffer(const GfxWrappedBaseBuffer& native)
		{
			plain::trace("read_gfx_wrapped_base_buffer");
			asset_data::gfxworld::gfx_wrapped_base_buffer data;
			data.buffer = read_gfx_buffer_resource(native.buffer);
			data.view = read_gfx_shader_buffer_view(native.view);
			return data;
		}

		void write_gfx_wrapped_base_buffer(GfxWrappedBaseBuffer& native, const asset_data::gfxworld::gfx_wrapped_base_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_buffer_resource(native.buffer, data.buffer, resolve, memory);
			write_gfx_shader_buffer_view(native.view, data.view, resolve, memory);
		}

		std::size_t size_gfx_wrapped_base_buffer([[maybe_unused]] const asset_data::gfxworld::gfx_wrapped_base_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_buffer_resource(data.buffer);
			size += size_gfx_shader_buffer_view(data.view);
			return size;
		}

		asset_data::gfxworld::gfx_wrapped_buffer read_gfx_wrapped_buffer(const GfxWrappedBuffer& native)
		{
			plain::trace("read_gfx_wrapped_buffer");
			asset_data::gfxworld::gfx_wrapped_buffer data;
			data.base = read_gfx_wrapped_base_buffer(native.base);
			plain::require_zero(native.data, "GfxWrappedBuffer.data");
			data.flags = native.flags;
			return data;
		}

		void write_gfx_wrapped_buffer(GfxWrappedBuffer& native, const asset_data::gfxworld::gfx_wrapped_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_wrapped_base_buffer(native.base, data.base, resolve, memory);
			native.flags = data.flags;
		}

		std::size_t size_gfx_wrapped_buffer([[maybe_unused]] const asset_data::gfxworld::gfx_wrapped_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_wrapped_base_buffer(data.base);
			return size;
		}

		asset_data::gfxworld::gfx_world_surfaces read_gfx_world_surfaces(const GfxWorldSurfaces& native)
		{
			plain::trace("read_gfx_world_surfaces");
			asset_data::gfxworld::gfx_world_surfaces data;
			data.count = native.count;
			data.surf_data_count = native.surfDataCount;
			data.lit_opaque_surfs_begin = native.litOpaqueSurfsBegin;
			data.lit_opaque_surfs_end = native.litOpaqueSurfsEnd;
			data.lit_decal_surfs_begin = native.litDecalSurfsBegin;
			data.lit_decal_surfs_end = native.litDecalSurfsEnd;
			data.lit_trans_surfs_begin = native.litTransSurfsBegin;
			data.lit_trans_surfs_end = native.litTransSurfsEnd;
			data.emissive_surfs_begin = native.emissiveSurfsBegin;
			data.emissive_surfs_end = native.emissiveSurfsEnd;
			data.surfaces = plain::read_array_if_present(native.surfaces, native.count, "GfxWorldSurfaces.surfaces", [&](const auto& item) { return read_gfx_surface(item); });
			data.surface_bounds = plain::read_array_if_present(native.surfaceBounds, native.count, "GfxWorldSurfaces.surfaceBounds", [&](const auto& item) { return read_gfx_surface_bounds(item); });
			data.surface_materials = plain::read_array_if_present(native.surfaceMaterials, native.count, "GfxWorldSurfaces.surfaceMaterials", [&](const auto& item) { return plain::read_block<std::array<std::uint64_t, 2>>(&item); });
			data.surf_data = plain::read_array_if_present(native.surfData, native.surfDataCount, "GfxWorldSurfaces.surfData", [&](const auto& item) { return read_gfx_world_surf_data(item); });
			data.surf_data_buffer = read_gfx_wrapped_buffer(native.surfDataBuffer);
			return data;
		}

		void write_gfx_world_surfaces(GfxWorldSurfaces& native, const asset_data::gfxworld::gfx_world_surfaces& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.count = data.count;
			native.surfDataCount = data.surf_data_count;
			native.litOpaqueSurfsBegin = data.lit_opaque_surfs_begin;
			native.litOpaqueSurfsEnd = data.lit_opaque_surfs_end;
			native.litDecalSurfsBegin = data.lit_decal_surfs_begin;
			native.litDecalSurfsEnd = data.lit_decal_surfs_end;
			native.litTransSurfsBegin = data.lit_trans_surfs_begin;
			native.litTransSurfsEnd = data.lit_trans_surfs_end;
			native.emissiveSurfsBegin = data.emissive_surfs_begin;
			native.emissiveSurfsEnd = data.emissive_surfs_end;
			native.surfaces = plain::write_array<GfxSurface>(data.surfaces, memory, [&](auto& out, const auto& in) { write_gfx_surface(out, in, resolve, memory); });
			native.surfaceBounds = plain::write_array<GfxSurfaceBounds>(data.surface_bounds, memory, [&](auto& out, const auto& in) { write_gfx_surface_bounds(out, in, resolve, memory); });
			native.surfaceMaterials = plain::write_array<GfxDrawSurf>(data.surface_materials, memory, [&](auto& out, const auto& in) { plain::write_block(&out, in); });
			native.surfData = plain::write_array<GfxWorldSurfData>(data.surf_data, memory, [&](auto& out, const auto& in) { write_gfx_world_surf_data(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.surfDataBuffer, data.surf_data_buffer, resolve, memory);
		}

		std::size_t size_gfx_world_surfaces([[maybe_unused]] const asset_data::gfxworld::gfx_world_surfaces& data)
		{
			std::size_t size = 0;
			if (data.surfaces)
			{
				size += plain::sized(std::max<std::size_t>(data.surfaces->size(), 1) * sizeof(GfxSurface));
				for (const auto& item : *data.surfaces)
				{
					size += size_gfx_surface(item);
				}
			}
			if (data.surface_bounds)
			{
				size += plain::sized(std::max<std::size_t>(data.surface_bounds->size(), 1) * sizeof(GfxSurfaceBounds));
				for (const auto& item : *data.surface_bounds)
				{
					size += size_gfx_surface_bounds(item);
				}
			}
			if (data.surface_materials)
			{
				size += plain::sized(std::max<std::size_t>(data.surface_materials->size(), 1) * sizeof(GfxDrawSurf));
			}
			if (data.surf_data)
			{
				size += plain::sized(std::max<std::size_t>(data.surf_data->size(), 1) * sizeof(GfxWorldSurfData));
				for (const auto& item : *data.surf_data)
				{
					size += size_gfx_world_surf_data(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.surf_data_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_clutter_instance_pool read_gfx_clutter_instance_pool(const GfxClutterInstancePool& native)
		{
			plain::trace("read_gfx_clutter_instance_pool");
			asset_data::gfxworld::gfx_clutter_instance_pool data;
			data.start = native.start;
			data.size = native.size;
			return data;
		}

		void write_gfx_clutter_instance_pool(GfxClutterInstancePool& native, const asset_data::gfxworld::gfx_clutter_instance_pool& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.start = data.start;
			native.size = data.size;
		}

		std::size_t size_gfx_clutter_instance_pool([[maybe_unused]] const asset_data::gfxworld::gfx_clutter_instance_pool& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_shader_buffer_rw_view read_gfx_shader_buffer_rw_view(const GfxShaderBufferRWView& native)
		{
			plain::trace("read_gfx_shader_buffer_rw_view");
			asset_data::gfxworld::gfx_shader_buffer_rw_view data;
			plain::require_zero(native.resource, "GfxShaderBufferRWView.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.view = native.view;
			return data;
		}

		void write_gfx_shader_buffer_rw_view(GfxShaderBufferRWView& native, const asset_data::gfxworld::gfx_shader_buffer_rw_view& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.view = data.view;
		}

		std::size_t size_gfx_shader_buffer_rw_view([[maybe_unused]] const asset_data::gfxworld::gfx_shader_buffer_rw_view& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_wrapped_rw_buffer read_gfx_wrapped_rw_buffer(const GfxWrappedRWBuffer& native)
		{
			plain::trace("read_gfx_wrapped_rw_buffer");
			asset_data::gfxworld::gfx_wrapped_rw_buffer data;
			data.buffer = read_gfx_wrapped_buffer(native.buffer);
			data.rw_view = read_gfx_shader_buffer_rw_view(native.rwView);
			return data;
		}

		void write_gfx_wrapped_rw_buffer(GfxWrappedRWBuffer& native, const asset_data::gfxworld::gfx_wrapped_rw_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_wrapped_buffer(native.buffer, data.buffer, resolve, memory);
			write_gfx_shader_buffer_rw_view(native.rwView, data.rw_view, resolve, memory);
		}

		std::size_t size_gfx_wrapped_rw_buffer([[maybe_unused]] const asset_data::gfxworld::gfx_wrapped_rw_buffer& data)
		{
			std::size_t size = 0;
			size += size_gfx_wrapped_buffer(data.buffer);
			size += size_gfx_shader_buffer_rw_view(data.rw_view);
			return size;
		}

		asset_data::gfxworld::bitarray_dynamic read_bitarray_dynamic(const bitarray_dynamic& native)
		{
			plain::trace("read_bitarray_dynamic");
			asset_data::gfxworld::bitarray_dynamic data;
			data.array = plain::read_array_if_present(native.array, native.wordCount, "bitarray_dynamic.array", [&](const auto& item) { return item; });
			data.word_count = native.wordCount;
			data.bit_count = native.bitCount;
			return data;
		}

		void write_bitarray_dynamic(bitarray_dynamic& native, const asset_data::gfxworld::bitarray_dynamic& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.array = plain::write_array<std::uint32_t>(data.array, memory, [&](auto& out, const auto& in) { out = in; });
			native.wordCount = data.word_count;
			native.bitCount = data.bit_count;
		}

		std::size_t size_bitarray_dynamic([[maybe_unused]] const asset_data::gfxworld::bitarray_dynamic& data)
		{
			std::size_t size = 0;
			if (data.array)
			{
				size += plain::sized(std::max<std::size_t>(data.array->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_world_static_models read_gfx_world_static_models(const GfxWorldStaticModels& native)
		{
			plain::trace("read_gfx_world_static_models");
			asset_data::gfxworld::gfx_world_static_models data;
			data.surface_count = native.surfaceCount;
			data.model_count = native.modelCount;
			data.collection_count = native.collectionCount;
			data.clutter_collection_count = native.clutterCollectionCount;
			data.instance_count = native.instanceCount;
			data.clutter_tint_count = native.clutterTintCount;
			data.lmap_transforms_count = native.lmapTransformsCount;
			data.material_count = native.materialCount;
			data.collection_vis_data_count = native.collectionVisDataCount;
			data.splined_model_instance_count = native.splinedModelInstanceCount;
			data.spline_segment_count = native.splineSegmentCount;
			data.clutter_instance_pool = read_gfx_clutter_instance_pool(native.clutterInstancePool);
			data.surfaces = plain::read_array_if_present(native.surfaces, native.surfaceCount, "GfxWorldStaticModels.surfaces", [&](const auto& item) { return read_gfx_static_model_surface(item); });
			data.models = plain::read_array_if_present(native.models, native.modelCount, "GfxWorldStaticModels.models", [&](const auto& item) { return read_gfx_static_model(item); });
			data.clutter_collections = plain::read_array_if_present(native.clutterCollections, native.clutterCollectionCount, "GfxWorldStaticModels.clutterCollections", [&](const auto& item) { return read_gfx_clutter_collection(item); });
			data.collections = plain::read_array_if_present(native.collections, native.collectionCount, "GfxWorldStaticModels.collections", [&](const auto& item) { return read_gfx_static_model_collection(item); });
			data.instances = plain::read_array_if_present(native.instances, native.instanceCount, "GfxWorldStaticModels.instances", [&](const auto& item) { return read_gfx_static_model_instance(item); });
			data.instance_flags = plain::read_array_if_present(native.instanceFlags, native.instanceCount, "GfxWorldStaticModels.instanceFlags", [&](const auto& item) { return item; });
			data.collection_bounds = plain::read_array_if_present(native.collectionBounds, native.collectionCount, "GfxWorldStaticModels.collectionBounds", [&](const auto& item) { return read_bounds(item); });
			data.materials = plain::read_array_if_present(native.materials, native.materialCount, "GfxWorldStaticModels.materials", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_MATERIAL, item); });
			data.model_static_indirection = plain::read_array_if_present(native.modelStaticIndirection, native.instanceCount, "GfxWorldStaticModels.modelStaticIndirection", [&](const auto& item) { return read_gfx_model_static_indirection(item); });
			data.model_static_indirection_buffer = read_gfx_wrapped_buffer(native.modelStaticIndirectionBuffer);
			data.lmap_transforms = plain::read_array_if_present(native.lmapTransforms, native.lmapTransformsCount, "GfxWorldStaticModels.lmapTransforms", [&](const auto& item) { return read_gfx_lightmap_transform_packed(item); });
			data.lmap_transforms_buffer = read_gfx_wrapped_buffer(native.lmapTransformsBuffer);
			data.smodel_surf_data = plain::read_array_if_present(native.smodelSurfData, native.surfaceCount, "GfxWorldStaticModels.smodelSurfData", [&](const auto& item) { return read_gfx_base_s_model_surf_data(item); });
			data.smodel_surf_data_buffer = read_gfx_wrapped_buffer(native.smodelSurfDataBuffer);
			data.smodel_instance_data = plain::read_array_if_present(native.smodelInstanceData, native.instanceCount, "GfxWorldStaticModels.smodelInstanceData", [&](const auto& item) { return read_gfx_s_model_instance_data(item); });
			data.smodel_instance_data_buffer = read_gfx_wrapped_rw_buffer(native.smodelInstanceDataBuffer);
			data.clutter_collection_gpu_data = plain::read_array_if_present(native.clutterCollectionGPUData, native.clutterCollectionCount, "GfxWorldStaticModels.clutterCollectionGPUData", [&](const auto& item) { return read_gfx_clutter_collection_gpu_data(item); });
			data.clutter_collection_gpu_data_buffer = read_gfx_wrapped_rw_buffer(native.clutterCollectionGPUDataBuffer);
			data.clutter_tint_data = plain::read_array_if_present(native.clutterTintData, native.clutterTintCount, "GfxWorldStaticModels.clutterTintData", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			data.clutter_tint_data_buffer = read_gfx_wrapped_rw_buffer(native.clutterTintDataBuffer);
			data.smodel_expansion_data = plain::read_array_if_present(native.smodelExpansionData, native.modelCount, "GfxWorldStaticModels.smodelExpansionData", [&](const auto& item) { return read_gfx_static_model_expansion(item); });
			data.smodel_expansion_data_buffer = read_gfx_wrapped_buffer(native.smodelExpansionDataBuffer);
			data.smodel_surf_mat_indirection = plain::read_array_if_present(native.smodelSurfMatIndirection, native.surfaceCount, "GfxWorldStaticModels.smodelSurfMatIndirection", [&](const auto& item) { return read_gfx_s_model_surf_mat_indirection(item); });
			data.smodel_surf_mat_indirection_buffer = read_gfx_wrapped_buffer(native.smodelSurfMatIndirectionBuffer);
			data.clutter_collections_data_buffer = read_gfx_wrapped_buffer(native.clutterCollectionsDataBuffer);
			data.smodel_collections_data_buffer = read_gfx_wrapped_buffer(native.smodelCollectionsDataBuffer);
			data.smodel_surf_ugb_indirection = plain::read_array_if_present(native.smodelSurfUGBIndirection, native.surfaceCount, "GfxWorldStaticModels.smodelSurfUGBIndirection", [&](const auto& item) { return item; });
			data.smodel_surf_ugb_indirection_buffer = read_gfx_wrapped_rw_buffer(native.smodelSurfUGBIndirectionBuffer);
			data.splined_model_instance_data = plain::read_array_if_present(native.splinedModelInstanceData, native.splinedModelInstanceCount, "GfxWorldStaticModels.splinedModelInstanceData", [&](const auto& item) { return read_hlsl_gpu_splined_model_instance(item); });
			data.splined_model_instance_buffer = read_gfx_wrapped_buffer(native.splinedModelInstanceBuffer);
			data.spline_segment_data = plain::read_array_if_present(native.splineSegmentData, native.splineSegmentCount, "GfxWorldStaticModels.splineSegmentData", [&](const auto& item) { return read_hlsl_gpu_spline_segment(item); });
			data.spline_segment_buffer = read_gfx_wrapped_buffer(native.splineSegmentBuffer);
			data.splined_model_instance_bounds = plain::read_array_if_present(native.splinedModelInstanceBounds, native.splinedModelInstanceCount, "GfxWorldStaticModels.splinedModelInstanceBounds", [&](const auto& item) { return read_bounds(item); });
			data.splined_model_instance_bounds_buffer = read_gfx_wrapped_buffer(native.splinedModelInstanceBoundsBuffer);
			data.collection_transient_zone_prev_frame_visibility = read_bitarray_dynamic(native.collectionTransientZonePrevFrameVisibility);
			data.collection_vis_data = plain::read_array_if_present(native.collectionVisData, native.collectionVisDataCount, "GfxWorldStaticModels.collectionVisData", [&](const auto& item) { return item; });
			data.collection_vis_ref_count = plain::read_array_if_present(native.collectionVisRefCount, native.collectionCount, "GfxWorldStaticModels.collectionVisRefCount", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_world_static_models(GfxWorldStaticModels& native, const asset_data::gfxworld::gfx_world_static_models& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.surfaceCount = data.surface_count;
			native.modelCount = data.model_count;
			native.collectionCount = data.collection_count;
			native.clutterCollectionCount = data.clutter_collection_count;
			native.instanceCount = data.instance_count;
			native.clutterTintCount = data.clutter_tint_count;
			native.lmapTransformsCount = data.lmap_transforms_count;
			native.materialCount = data.material_count;
			native.collectionVisDataCount = data.collection_vis_data_count;
			native.splinedModelInstanceCount = data.splined_model_instance_count;
			native.splineSegmentCount = data.spline_segment_count;
			write_gfx_clutter_instance_pool(native.clutterInstancePool, data.clutter_instance_pool, resolve, memory);
			native.surfaces = plain::write_array<GfxStaticModelSurface>(data.surfaces, memory, [&](auto& out, const auto& in) { write_gfx_static_model_surface(out, in, resolve, memory); });
			native.models = plain::write_array<GfxStaticModel>(data.models, memory, [&](auto& out, const auto& in) { write_gfx_static_model(out, in, resolve, memory); });
			native.clutterCollections = plain::write_array<GfxClutterCollection>(data.clutter_collections, memory, [&](auto& out, const auto& in) { write_gfx_clutter_collection(out, in, resolve, memory); });
			native.collections = plain::write_array<GfxStaticModelCollection>(data.collections, memory, [&](auto& out, const auto& in) { write_gfx_static_model_collection(out, in, resolve, memory); });
			native.instances = plain::write_array<GfxStaticModelInstance>(data.instances, memory, [&](auto& out, const auto& in) { write_gfx_static_model_instance(out, in, resolve, memory); });
			native.instanceFlags = plain::write_array<std::uint8_t>(data.instance_flags, memory, [&](auto& out, const auto& in) { out = in; });
			native.collectionBounds = plain::write_array<Bounds>(data.collection_bounds, memory, [&](auto& out, const auto& in) { write_bounds(out, in, resolve, memory); });
			native.materials = plain::write_array<Material*>(data.materials, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, in); });
			native.modelStaticIndirection = plain::write_array<GfxModelStaticIndirection>(data.model_static_indirection, memory, [&](auto& out, const auto& in) { write_gfx_model_static_indirection(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.modelStaticIndirectionBuffer, data.model_static_indirection_buffer, resolve, memory);
			native.lmapTransforms = plain::write_array<GfxLightmapTransformPacked>(data.lmap_transforms, memory, [&](auto& out, const auto& in) { write_gfx_lightmap_transform_packed(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.lmapTransformsBuffer, data.lmap_transforms_buffer, resolve, memory);
			native.smodelSurfData = plain::write_array<GfxBaseSModelSurfData>(data.smodel_surf_data, memory, [&](auto& out, const auto& in) { write_gfx_base_s_model_surf_data(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.smodelSurfDataBuffer, data.smodel_surf_data_buffer, resolve, memory);
			native.smodelInstanceData = plain::write_array<GfxSModelInstanceData>(data.smodel_instance_data, memory, [&](auto& out, const auto& in) { write_gfx_s_model_instance_data(out, in, resolve, memory); });
			write_gfx_wrapped_rw_buffer(native.smodelInstanceDataBuffer, data.smodel_instance_data_buffer, resolve, memory);
			native.clutterCollectionGPUData = plain::write_array<GfxClutterCollectionGPUData>(data.clutter_collection_gpu_data, memory, [&](auto& out, const auto& in) { write_gfx_clutter_collection_gpu_data(out, in, resolve, memory); });
			write_gfx_wrapped_rw_buffer(native.clutterCollectionGPUDataBuffer, data.clutter_collection_gpu_data_buffer, resolve, memory);
			native.clutterTintData = plain::write_array<vec3_t>(data.clutter_tint_data, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			write_gfx_wrapped_rw_buffer(native.clutterTintDataBuffer, data.clutter_tint_data_buffer, resolve, memory);
			native.smodelExpansionData = plain::write_array<GfxStaticModelExpansion>(data.smodel_expansion_data, memory, [&](auto& out, const auto& in) { write_gfx_static_model_expansion(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.smodelExpansionDataBuffer, data.smodel_expansion_data_buffer, resolve, memory);
			native.smodelSurfMatIndirection = plain::write_array<GfxSModelSurfMatIndirection>(data.smodel_surf_mat_indirection, memory, [&](auto& out, const auto& in) { write_gfx_s_model_surf_mat_indirection(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.smodelSurfMatIndirectionBuffer, data.smodel_surf_mat_indirection_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.clutterCollectionsDataBuffer, data.clutter_collections_data_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.smodelCollectionsDataBuffer, data.smodel_collections_data_buffer, resolve, memory);
			native.smodelSurfUGBIndirection = plain::write_array<std::uint32_t>(data.smodel_surf_ugb_indirection, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_rw_buffer(native.smodelSurfUGBIndirectionBuffer, data.smodel_surf_ugb_indirection_buffer, resolve, memory);
			native.splinedModelInstanceData = plain::write_array<HLSL_GpuSplinedModelInstance>(data.splined_model_instance_data, memory, [&](auto& out, const auto& in) { write_hlsl_gpu_splined_model_instance(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.splinedModelInstanceBuffer, data.splined_model_instance_buffer, resolve, memory);
			native.splineSegmentData = plain::write_array<HLSL_GpuSplineSegment>(data.spline_segment_data, memory, [&](auto& out, const auto& in) { write_hlsl_gpu_spline_segment(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.splineSegmentBuffer, data.spline_segment_buffer, resolve, memory);
			native.splinedModelInstanceBounds = plain::write_array<Bounds>(data.splined_model_instance_bounds, memory, [&](auto& out, const auto& in) { write_bounds(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.splinedModelInstanceBoundsBuffer, data.splined_model_instance_bounds_buffer, resolve, memory);
			write_bitarray_dynamic(native.collectionTransientZonePrevFrameVisibility, data.collection_transient_zone_prev_frame_visibility, resolve, memory);
			native.collectionVisData = plain::write_array<std::uint32_t>(data.collection_vis_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.collectionVisRefCount = plain::write_array<std::uint8_t>(data.collection_vis_ref_count, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_world_static_models([[maybe_unused]] const asset_data::gfxworld::gfx_world_static_models& data)
		{
			std::size_t size = 0;
			size += size_gfx_clutter_instance_pool(data.clutter_instance_pool);
			if (data.surfaces)
			{
				size += plain::sized(std::max<std::size_t>(data.surfaces->size(), 1) * sizeof(GfxStaticModelSurface));
				for (const auto& item : *data.surfaces)
				{
					size += size_gfx_static_model_surface(item);
				}
			}
			if (data.models)
			{
				size += plain::sized(std::max<std::size_t>(data.models->size(), 1) * sizeof(GfxStaticModel));
				for (const auto& item : *data.models)
				{
					size += size_gfx_static_model(item);
				}
			}
			if (data.clutter_collections)
			{
				size += plain::sized(std::max<std::size_t>(data.clutter_collections->size(), 1) * sizeof(GfxClutterCollection));
				for (const auto& item : *data.clutter_collections)
				{
					size += size_gfx_clutter_collection(item);
				}
			}
			if (data.collections)
			{
				size += plain::sized(std::max<std::size_t>(data.collections->size(), 1) * sizeof(GfxStaticModelCollection));
				for (const auto& item : *data.collections)
				{
					size += size_gfx_static_model_collection(item);
				}
			}
			if (data.instances)
			{
				size += plain::sized(std::max<std::size_t>(data.instances->size(), 1) * sizeof(GfxStaticModelInstance));
				for (const auto& item : *data.instances)
				{
					size += size_gfx_static_model_instance(item);
				}
			}
			if (data.instance_flags)
			{
				size += plain::sized(std::max<std::size_t>(data.instance_flags->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.collection_bounds)
			{
				size += plain::sized(std::max<std::size_t>(data.collection_bounds->size(), 1) * sizeof(Bounds));
				for (const auto& item : *data.collection_bounds)
				{
					size += size_bounds(item);
				}
			}
			if (data.materials)
			{
				size += plain::sized(std::max<std::size_t>(data.materials->size(), 1) * sizeof(Material*));
			}
			if (data.model_static_indirection)
			{
				size += plain::sized(std::max<std::size_t>(data.model_static_indirection->size(), 1) * sizeof(GfxModelStaticIndirection));
				for (const auto& item : *data.model_static_indirection)
				{
					size += size_gfx_model_static_indirection(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.model_static_indirection_buffer);
			if (data.lmap_transforms)
			{
				size += plain::sized(std::max<std::size_t>(data.lmap_transforms->size(), 1) * sizeof(GfxLightmapTransformPacked));
				for (const auto& item : *data.lmap_transforms)
				{
					size += size_gfx_lightmap_transform_packed(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.lmap_transforms_buffer);
			if (data.smodel_surf_data)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_surf_data->size(), 1) * sizeof(GfxBaseSModelSurfData));
				for (const auto& item : *data.smodel_surf_data)
				{
					size += size_gfx_base_s_model_surf_data(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.smodel_surf_data_buffer);
			if (data.smodel_instance_data)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_instance_data->size(), 1) * sizeof(GfxSModelInstanceData));
				for (const auto& item : *data.smodel_instance_data)
				{
					size += size_gfx_s_model_instance_data(item);
				}
			}
			size += size_gfx_wrapped_rw_buffer(data.smodel_instance_data_buffer);
			if (data.clutter_collection_gpu_data)
			{
				size += plain::sized(std::max<std::size_t>(data.clutter_collection_gpu_data->size(), 1) * sizeof(GfxClutterCollectionGPUData));
				for (const auto& item : *data.clutter_collection_gpu_data)
				{
					size += size_gfx_clutter_collection_gpu_data(item);
				}
			}
			size += size_gfx_wrapped_rw_buffer(data.clutter_collection_gpu_data_buffer);
			if (data.clutter_tint_data)
			{
				size += plain::sized(std::max<std::size_t>(data.clutter_tint_data->size(), 1) * sizeof(vec3_t));
			}
			size += size_gfx_wrapped_rw_buffer(data.clutter_tint_data_buffer);
			if (data.smodel_expansion_data)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_expansion_data->size(), 1) * sizeof(GfxStaticModelExpansion));
				for (const auto& item : *data.smodel_expansion_data)
				{
					size += size_gfx_static_model_expansion(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.smodel_expansion_data_buffer);
			if (data.smodel_surf_mat_indirection)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_surf_mat_indirection->size(), 1) * sizeof(GfxSModelSurfMatIndirection));
				for (const auto& item : *data.smodel_surf_mat_indirection)
				{
					size += size_gfx_s_model_surf_mat_indirection(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.smodel_surf_mat_indirection_buffer);
			size += size_gfx_wrapped_buffer(data.clutter_collections_data_buffer);
			size += size_gfx_wrapped_buffer(data.smodel_collections_data_buffer);
			if (data.smodel_surf_ugb_indirection)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_surf_ugb_indirection->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_rw_buffer(data.smodel_surf_ugb_indirection_buffer);
			if (data.splined_model_instance_data)
			{
				size += plain::sized(std::max<std::size_t>(data.splined_model_instance_data->size(), 1) * sizeof(HLSL_GpuSplinedModelInstance));
				for (const auto& item : *data.splined_model_instance_data)
				{
					size += size_hlsl_gpu_splined_model_instance(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.splined_model_instance_buffer);
			if (data.spline_segment_data)
			{
				size += plain::sized(std::max<std::size_t>(data.spline_segment_data->size(), 1) * sizeof(HLSL_GpuSplineSegment));
				for (const auto& item : *data.spline_segment_data)
				{
					size += size_hlsl_gpu_spline_segment(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.spline_segment_buffer);
			if (data.splined_model_instance_bounds)
			{
				size += plain::sized(std::max<std::size_t>(data.splined_model_instance_bounds->size(), 1) * sizeof(Bounds));
				for (const auto& item : *data.splined_model_instance_bounds)
				{
					size += size_bounds(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.splined_model_instance_bounds_buffer);
			size += size_bitarray_dynamic(data.collection_transient_zone_prev_frame_visibility);
			if (data.collection_vis_data)
			{
				size += plain::sized(std::max<std::size_t>(data.collection_vis_data->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.collection_vis_ref_count)
			{
				size += plain::sized(std::max<std::size_t>(data.collection_vis_ref_count->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_world_reflection_probe_data read_gfx_world_reflection_probe_data(const GfxWorldReflectionProbeData& native)
		{
			plain::trace("read_gfx_world_reflection_probe_data");
			asset_data::gfxworld::gfx_world_reflection_probe_data data;
			data.reflection_probe_count = native.reflectionProbeCount;
			data.shared_reflection_probe_count = native.sharedReflectionProbeCount;
			data.reflection_probe_images = plain::read_array_if_present(native.reflectionProbeImages, native.reflectionProbeCount, "GfxWorldReflectionProbeData.reflectionProbeImages", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.reflection_probe_images_octahedron = plain::read_array_if_present(native.reflectionProbeImages_octahedron, native.sharedReflectionProbeCount, "GfxWorldReflectionProbeData.reflectionProbeImages_octahedron", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.reflection_probes = plain::read_array_if_present(native.reflectionProbes, native.reflectionProbeCount, "GfxWorldReflectionProbeData.reflectionProbes", [&](const auto& item) { return read_gfx_reflection_probe(item); });
			data.reflection_probe_array_image = plain::read_asset(ASSET_TYPE_IMAGE, native.reflectionProbeArrayImage);
			data.reflection_probe_array_image_octahedron = plain::read_asset(ASSET_TYPE_IMAGE, native.reflectionProbeArrayImage_octahedron);
			data.probe_relighting_count = native.probeRelightingCount;
			data.probe_relighting_data = plain::read_array_if_present(native.probeRelightingData, native.probeRelightingCount, "GfxWorldReflectionProbeData.probeRelightingData", [&](const auto& item) { return read_gfx_reflection_probe_relighting_data(item); });
			data.reflection_probe_g_buffer_image_count = native.reflectionProbeGBufferImageCount;
			data.reflection_probe_g_buffer_images = plain::read_array_if_present(native.reflectionProbeGBufferImages, native.reflectionProbeGBufferImageCount, "GfxWorldReflectionProbeData.reflectionProbeGBufferImages", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.reflection_probe_g_buffer_images_octahedron = plain::read_array_if_present(native.reflectionProbeGBufferImages_octahedron, native.reflectionProbeGBufferImageCount, "GfxWorldReflectionProbeData.reflectionProbeGBufferImages_octahedron", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.reflection_probe_instance_count = native.reflectionProbeInstanceCount;
			data.reflection_probe_instances = plain::read_array_if_present(native.reflectionProbeInstances, native.reflectionProbeInstanceCount, "GfxWorldReflectionProbeData.reflectionProbeInstances", [&](const auto& item) { return read_gfx_reflection_probe_instance(item); });
			data.reflection_probe_sh = plain::read_array_if_present(native.reflectionProbeSH, ((native.reflectionProbeCount) * 4), "GfxWorldReflectionProbeData.reflectionProbeSH", [&](const auto& item) { return read_gfx_sh3_vec(item); });
			data.reflection_probe_sh_buffer = read_gfx_wrapped_rw_buffer(native.reflectionProbeSHBuffer);
			return data;
		}

		void write_gfx_world_reflection_probe_data(GfxWorldReflectionProbeData& native, const asset_data::gfxworld::gfx_world_reflection_probe_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.reflectionProbeCount = data.reflection_probe_count;
			native.sharedReflectionProbeCount = data.shared_reflection_probe_count;
			native.reflectionProbeImages = plain::write_array<GfxImage*>(data.reflection_probe_images, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			native.reflectionProbeImages_octahedron = plain::write_array<GfxImage*>(data.reflection_probe_images_octahedron, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			native.reflectionProbes = plain::write_array<GfxReflectionProbe>(data.reflection_probes, memory, [&](auto& out, const auto& in) { write_gfx_reflection_probe(out, in, resolve, memory); });
			native.reflectionProbeArrayImage = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.reflection_probe_array_image);
			native.reflectionProbeArrayImage_octahedron = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.reflection_probe_array_image_octahedron);
			native.probeRelightingCount = data.probe_relighting_count;
			native.probeRelightingData = plain::write_array<GfxReflectionProbeRelightingData>(data.probe_relighting_data, memory, [&](auto& out, const auto& in) { write_gfx_reflection_probe_relighting_data(out, in, resolve, memory); });
			native.reflectionProbeGBufferImageCount = data.reflection_probe_g_buffer_image_count;
			native.reflectionProbeGBufferImages = plain::write_array<GfxImage*>(data.reflection_probe_g_buffer_images, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			native.reflectionProbeGBufferImages_octahedron = plain::write_array<GfxImage*>(data.reflection_probe_g_buffer_images_octahedron, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			native.reflectionProbeInstanceCount = data.reflection_probe_instance_count;
			native.reflectionProbeInstances = plain::write_array<GfxReflectionProbeInstance>(data.reflection_probe_instances, memory, [&](auto& out, const auto& in) { write_gfx_reflection_probe_instance(out, in, resolve, memory); });
			native.reflectionProbeSH = plain::write_array<GfxSH3Vec>(data.reflection_probe_sh, memory, [&](auto& out, const auto& in) { write_gfx_sh3_vec(out, in, resolve, memory); });
			write_gfx_wrapped_rw_buffer(native.reflectionProbeSHBuffer, data.reflection_probe_sh_buffer, resolve, memory);
		}

		std::size_t size_gfx_world_reflection_probe_data([[maybe_unused]] const asset_data::gfxworld::gfx_world_reflection_probe_data& data)
		{
			std::size_t size = 0;
			if (data.reflection_probe_images)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_images->size(), 1) * sizeof(GfxImage*));
			}
			if (data.reflection_probe_images_octahedron)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_images_octahedron->size(), 1) * sizeof(GfxImage*));
			}
			if (data.reflection_probes)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probes->size(), 1) * sizeof(GfxReflectionProbe));
				for (const auto& item : *data.reflection_probes)
				{
					size += size_gfx_reflection_probe(item);
				}
			}
			if (data.probe_relighting_data)
			{
				size += plain::sized(std::max<std::size_t>(data.probe_relighting_data->size(), 1) * sizeof(GfxReflectionProbeRelightingData));
				for (const auto& item : *data.probe_relighting_data)
				{
					size += size_gfx_reflection_probe_relighting_data(item);
				}
			}
			if (data.reflection_probe_g_buffer_images)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_g_buffer_images->size(), 1) * sizeof(GfxImage*));
			}
			if (data.reflection_probe_g_buffer_images_octahedron)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_g_buffer_images_octahedron->size(), 1) * sizeof(GfxImage*));
			}
			if (data.reflection_probe_instances)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_instances->size(), 1) * sizeof(GfxReflectionProbeInstance));
				for (const auto& item : *data.reflection_probe_instances)
				{
					size += size_gfx_reflection_probe_instance(item);
				}
			}
			if (data.reflection_probe_sh)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_sh->size(), 1) * sizeof(GfxSH3Vec));
				for (const auto& item : *data.reflection_probe_sh)
				{
					size += size_gfx_sh3_vec(item);
				}
			}
			size += size_gfx_wrapped_rw_buffer(data.reflection_probe_sh_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_world_lightmap_reindex_data read_gfx_world_lightmap_reindex_data(const GfxWorldLightmapReindexData& native)
		{
			plain::trace("read_gfx_world_lightmap_reindex_data");
			asset_data::gfxworld::gfx_world_lightmap_reindex_data data;
			data.image_pixel_size = native.imagePixelSize;
			data.reindex_count = native.reindexCount;
			data.reindex_element = plain::read_array_if_present(native.reindexElement, native.reindexCount, "GfxWorldLightmapReindexData.reindexElement", [&](const auto& item) { return read_gfx_world_lightmap_reindex_data_element(item); });
			data.packed_lightmap_count = native.packedLightmapCount;
			data.packed_lightmap = plain::read_array_if_present(native.packedLightmap, native.packedLightmapCount, "GfxWorldLightmapReindexData.packedLightmap", [&](const auto& item) { return read_gfx_world_packed_lightmap(item); });
			return data;
		}

		void write_gfx_world_lightmap_reindex_data(GfxWorldLightmapReindexData& native, const asset_data::gfxworld::gfx_world_lightmap_reindex_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.imagePixelSize = data.image_pixel_size;
			native.reindexCount = data.reindex_count;
			native.reindexElement = plain::write_array<GfxWorldLightmapReindexDataElement>(data.reindex_element, memory, [&](auto& out, const auto& in) { write_gfx_world_lightmap_reindex_data_element(out, in, resolve, memory); });
			native.packedLightmapCount = data.packed_lightmap_count;
			native.packedLightmap = plain::write_array<GfxWorldPackedLightmap>(data.packed_lightmap, memory, [&](auto& out, const auto& in) { write_gfx_world_packed_lightmap(out, in, resolve, memory); });
		}

		std::size_t size_gfx_world_lightmap_reindex_data([[maybe_unused]] const asset_data::gfxworld::gfx_world_lightmap_reindex_data& data)
		{
			std::size_t size = 0;
			if (data.reindex_element)
			{
				size += plain::sized(std::max<std::size_t>(data.reindex_element->size(), 1) * sizeof(GfxWorldLightmapReindexDataElement));
				for (const auto& item : *data.reindex_element)
				{
					size += size_gfx_world_lightmap_reindex_data_element(item);
				}
			}
			if (data.packed_lightmap)
			{
				size += plain::sized(std::max<std::size_t>(data.packed_lightmap->size(), 1) * sizeof(GfxWorldPackedLightmap));
				for (const auto& item : *data.packed_lightmap)
				{
					size += size_gfx_world_packed_lightmap(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::gfx_world_volumetrics read_gfx_world_volumetrics(const GfxWorldVolumetrics& native)
		{
			plain::trace("read_gfx_world_volumetrics");
			asset_data::gfxworld::gfx_world_volumetrics data;
			data.volumetric_count = native.volumetricCount;
			data.volumetrics = plain::read_array_if_present(native.volumetrics, native.volumetricCount, "GfxWorldVolumetrics.volumetrics", [&](const auto& item) { return read_gfx_volumetric(item); });
			data.volumetric_mask_atlas_image = plain::read_asset(ASSET_TYPE_IMAGE, native.volumetricMaskAtlasImage);
			return data;
		}

		void write_gfx_world_volumetrics(GfxWorldVolumetrics& native, const asset_data::gfxworld::gfx_world_volumetrics& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.volumetricCount = data.volumetric_count;
			native.volumetrics = plain::write_array<GfxVolumetric>(data.volumetrics, memory, [&](auto& out, const auto& in) { write_gfx_volumetric(out, in, resolve, memory); });
			native.volumetricMaskAtlasImage = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.volumetric_mask_atlas_image);
		}

		std::size_t size_gfx_world_volumetrics([[maybe_unused]] const asset_data::gfxworld::gfx_world_volumetrics& data)
		{
			std::size_t size = 0;
			if (data.volumetrics)
			{
				size += plain::sized(std::max<std::size_t>(data.volumetrics->size(), 1) * sizeof(GfxVolumetric));
				for (const auto& item : *data.volumetrics)
				{
					size += size_gfx_volumetric(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::gfx_compressed_sun_shadow_params read_gfx_compressed_sun_shadow_params(const GfxCompressedSunShadowParams& native)
		{
			plain::trace("read_gfx_compressed_sun_shadow_params");
			asset_data::gfxworld::gfx_compressed_sun_shadow_params data;
			data.sun_direction = plain::read_scalars<float, 3>(&native.sunDirection);
			data.resolution = native.resolution;
			data.center_x = native.centerX;
			data.center_y = native.centerY;
			data.sample_size = native.sampleSize;
			data.near_plane = native.nearPlane;
			data.far_plane = native.farPlane;
			data.forest_size = native.forestSize;
			data.flags = native.flags;
			data.reserved = native.reserved;
			return data;
		}

		void write_gfx_compressed_sun_shadow_params(GfxCompressedSunShadowParams& native, const asset_data::gfxworld::gfx_compressed_sun_shadow_params& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.sunDirection, data.sun_direction);
			native.resolution = data.resolution;
			native.centerX = data.center_x;
			native.centerY = data.center_y;
			native.sampleSize = data.sample_size;
			native.nearPlane = data.near_plane;
			native.farPlane = data.far_plane;
			native.forestSize = data.forest_size;
			native.flags = data.flags;
			native.reserved = data.reserved;
		}

		std::size_t size_gfx_compressed_sun_shadow_params([[maybe_unused]] const asset_data::gfxworld::gfx_compressed_sun_shadow_params& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_world_draw read_gfx_world_draw(const GfxWorldDraw& native)
		{
			plain::trace("read_gfx_world_draw");
			asset_data::gfxworld::gfx_world_draw data;
			data.reflection_probe_data = read_gfx_world_reflection_probe_data(native.reflectionProbeData);
			data.lightmap_reindex_data = read_gfx_world_lightmap_reindex_data(native.lightmapReindexData);
			data.ies_lookup_texture = plain::read_asset(ASSET_TYPE_IMAGE, native.iesLookupTexture);
			data.decal_volume_version = native.decalVolumeVersion;
			data.decal_volume_reserved0 = native.decalVolumeReserved0;
			data.decal_volume_reserved1 = native.decalVolumeReserved1;
			data.decal_volume_reserved2 = native.decalVolumeReserved2;
			data.decal_volume_material_count = native.decalVolumeMaterialCount;
			data.decal_volume_mask_count = native.decalVolumeMaskCount;
			data.map_decal_volume_count = native.mapDecalVolumeCount;
			data.decal_volume_collection_count = native.decalVolumeCollectionCount;
			data.decal_volume_materials = plain::read_array_if_present(native.decalVolumeMaterials, native.decalVolumeMaterialCount, "GfxWorldDraw.decalVolumeMaterials", [&](const auto& item) { return read_gfx_static_decal_volume_material(item); });
			data.decal_volume_masks = plain::read_array_if_present(native.decalVolumeMasks, native.decalVolumeMaskCount, "GfxWorldDraw.decalVolumeMasks", [&](const auto& item) { return read_gfx_static_decal_volume_mask(item); });
			data.decal_volume_collections = plain::read_array_if_present(native.decalVolumeCollections, native.decalVolumeCollectionCount, "GfxWorldDraw.decalVolumeCollections", [&](const auto& item) { return read_gfx_decal_volume_collection(item); });
			data.material_and_mask_indirection = plain::read_array_if_present(native.materialAndMaskIndirection, (native.decalVolumeMaskCount + native.decalVolumeMaterialCount), "GfxWorldDraw.materialAndMaskIndirection", [&](const auto& item) { return item; });
			data.material_and_mask_indirection_buffer = read_gfx_wrapped_buffer(native.materialAndMaskIndirectionBuffer);
			data.light_grid_type = plain::enum_name(asset_data::gfxworld::gfx_light_grid_type_names, static_cast<std::int64_t>(native.lightGridType), "GfxWorldDraw.lightGridType");
			data.lightmap_type = plain::enum_name(asset_data::gfxworld::gfx_lightmap_type_names, static_cast<std::int64_t>(native.lightmapType), "GfxWorldDraw.lightmapType");
			data.lightmap_count = native.lightmapCount;
			data.transient_zone_count = native.transientZoneCount;
			data.transient_zones = plain::read_prefix(native.transientZones, native.transientZoneCount, "GfxWorldDraw.transientZones", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, item); });
			data.lightmap_transient_index = plain::read_array_if_present(native.lightmapTransientIndex, native.lightmapCount, "GfxWorldDraw.lightmapTransientIndex", [&](const auto& item) { return item; });
			data.volumetrics = read_gfx_world_volumetrics(native.volumetrics);
			data.baked_light_scale = native.bakedLightScale;
			data.compressed_sun_shadow_size = native.compressedSunShadowSize;
			data.compressed_sun_shadow = plain::read_array_if_present(native.compressedSunShadow, native.compressedSunShadowSize, "GfxWorldDraw.compressedSunShadow", [&](const auto& item) { return item; });
			data.compressed_sun_shadow_buffer = read_gfx_wrapped_rw_buffer(native.compressedSunShadowBuffer);
			data.compressed_sun_shadow_params = read_gfx_compressed_sun_shadow_params(native.compressedSunShadowParams);
			return data;
		}

		void write_gfx_world_draw(GfxWorldDraw& native, const asset_data::gfxworld::gfx_world_draw& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_world_reflection_probe_data(native.reflectionProbeData, data.reflection_probe_data, resolve, memory);
			write_gfx_world_lightmap_reindex_data(native.lightmapReindexData, data.lightmap_reindex_data, resolve, memory);
			native.iesLookupTexture = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.ies_lookup_texture);
			native.decalVolumeVersion = data.decal_volume_version;
			native.decalVolumeReserved0 = data.decal_volume_reserved0;
			native.decalVolumeReserved1 = data.decal_volume_reserved1;
			native.decalVolumeReserved2 = data.decal_volume_reserved2;
			native.decalVolumeMaterialCount = data.decal_volume_material_count;
			native.decalVolumeMaskCount = data.decal_volume_mask_count;
			native.mapDecalVolumeCount = data.map_decal_volume_count;
			native.decalVolumeCollectionCount = data.decal_volume_collection_count;
			native.decalVolumeMaterials = plain::write_array<GfxStaticDecalVolumeMaterial>(data.decal_volume_materials, memory, [&](auto& out, const auto& in) { write_gfx_static_decal_volume_material(out, in, resolve, memory); });
			native.decalVolumeMasks = plain::write_array<GfxStaticDecalVolumeMask>(data.decal_volume_masks, memory, [&](auto& out, const auto& in) { write_gfx_static_decal_volume_mask(out, in, resolve, memory); });
			native.decalVolumeCollections = plain::write_array<GfxDecalVolumeCollection>(data.decal_volume_collections, memory, [&](auto& out, const auto& in) { write_gfx_decal_volume_collection(out, in, resolve, memory); });
			native.materialAndMaskIndirection = plain::write_array<std::uint32_t>(data.material_and_mask_indirection, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.materialAndMaskIndirectionBuffer, data.material_and_mask_indirection_buffer, resolve, memory);
			native.lightGridType = static_cast<GfxLightGridType>(plain::enum_value(asset_data::gfxworld::gfx_light_grid_type_names, data.light_grid_type, "GfxWorldDraw.lightGridType"));
			native.lightmapType = static_cast<GfxLightmapType>(plain::enum_value(asset_data::gfxworld::gfx_lightmap_type_names, data.lightmap_type, "GfxWorldDraw.lightmapType"));
			native.lightmapCount = data.lightmap_count;
			native.transientZoneCount = data.transient_zone_count;
			plain::write_prefix(native.transientZones, data.transient_zones, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxWorldTransientZone>(resolve, ASSET_TYPE_GFXWORLD_TRANSIENT_ZONE, in); });
			native.lightmapTransientIndex = plain::write_array<std::uint32_t>(data.lightmap_transient_index, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_world_volumetrics(native.volumetrics, data.volumetrics, resolve, memory);
			native.bakedLightScale = data.baked_light_scale;
			native.compressedSunShadowSize = data.compressed_sun_shadow_size;
			native.compressedSunShadow = plain::write_array<std::uint8_t>(data.compressed_sun_shadow, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_rw_buffer(native.compressedSunShadowBuffer, data.compressed_sun_shadow_buffer, resolve, memory);
			write_gfx_compressed_sun_shadow_params(native.compressedSunShadowParams, data.compressed_sun_shadow_params, resolve, memory);
		}

		std::size_t size_gfx_world_draw([[maybe_unused]] const asset_data::gfxworld::gfx_world_draw& data)
		{
			std::size_t size = 0;
			size += size_gfx_world_reflection_probe_data(data.reflection_probe_data);
			size += size_gfx_world_lightmap_reindex_data(data.lightmap_reindex_data);
			if (data.decal_volume_materials)
			{
				size += plain::sized(std::max<std::size_t>(data.decal_volume_materials->size(), 1) * sizeof(GfxStaticDecalVolumeMaterial));
				for (const auto& item : *data.decal_volume_materials)
				{
					size += size_gfx_static_decal_volume_material(item);
				}
			}
			if (data.decal_volume_masks)
			{
				size += plain::sized(std::max<std::size_t>(data.decal_volume_masks->size(), 1) * sizeof(GfxStaticDecalVolumeMask));
				for (const auto& item : *data.decal_volume_masks)
				{
					size += size_gfx_static_decal_volume_mask(item);
				}
			}
			if (data.decal_volume_collections)
			{
				size += plain::sized(std::max<std::size_t>(data.decal_volume_collections->size(), 1) * sizeof(GfxDecalVolumeCollection));
				for (const auto& item : *data.decal_volume_collections)
				{
					size += size_gfx_decal_volume_collection(item);
				}
			}
			if (data.material_and_mask_indirection)
			{
				size += plain::sized(std::max<std::size_t>(data.material_and_mask_indirection->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.material_and_mask_indirection_buffer);
			if (data.transient_zones)
			{
				size += plain::sized(std::max<std::size_t>(data.transient_zones->size(), 1) * sizeof(GfxWorldTransientZone*));
			}
			if (data.lightmap_transient_index)
			{
				size += plain::sized(std::max<std::size_t>(data.lightmap_transient_index->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_world_volumetrics(data.volumetrics);
			if (data.compressed_sun_shadow)
			{
				size += plain::sized(std::max<std::size_t>(data.compressed_sun_shadow->size(), 1) * sizeof(std::uint8_t));
			}
			size += size_gfx_wrapped_rw_buffer(data.compressed_sun_shadow_buffer);
			size += size_gfx_compressed_sun_shadow_params(data.compressed_sun_shadow_params);
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_lm_data read_gfx_dynamic_lightset_lm_data(const GfxDynamicLightsetLMData& native)
		{
			plain::trace("read_gfx_dynamic_lightset_lm_data");
			asset_data::gfxworld::gfx_dynamic_lightset_lm_data data;
			data.num_g_chunk_lm = native.numGChunkLM;
			data.chunk_glm = plain::read_array_if_present(native.chunkGLM, native.numGChunkLM, "GfxDynamicLightsetLMData.chunkGLM", [&](const auto& item) { return read_gfx_dynamic_lightset_g_chunk(item); });
			data.num_g_chunk_lmbf = native.numGChunkLMBF;
			data.chunk_glmbf = plain::read_array_if_present(native.chunkGLMBF, native.numGChunkLMBF, "GfxDynamicLightsetLMData.chunkGLMBF", [&](const auto& item) { return item; });
			data.num_lm_have = native.numLMHave;
			data.chunk_which_lm = plain::read_array_if_present(native.chunkWhichLM, native.numLMHave, "GfxDynamicLightsetLMData.chunkWhichLM", [&](const auto& item) { return item; });
			data.chunk_lm_start = plain::read_array_if_present(native.chunkLMStart, (native.numLMHave + (native.numLMHave != 0 ? 1 : 0)), "GfxDynamicLightsetLMData.chunkLMStart", [&](const auto& item) { return item; });
			data.num_orig_texels = native.numOrigTexels;
			data.orig_texels = plain::read_array_if_present(native.origTexels, (native.numOrigTexels + native.numOrigTexels), "GfxDynamicLightsetLMData.origTexels", [&](const auto& item) { return item; });
			data.orig_texels_buffer = read_gfx_wrapped_buffer(native.origTexelsBuffer);
			data.orig_records = plain::read_array_if_present(native.origRecords, native.numOrigTexels, "GfxDynamicLightsetLMData.origRecords", [&](const auto& item) { return item; });
			data.orig_records_buffer = read_gfx_wrapped_buffer(native.origRecordsBuffer);
			data.num_bflm_texels = native.numBFLMTexels;
			data.bf_texels = plain::read_array_if_present(native.bfTexels, (native.numBFLMTexels + native.numBFLMTexels), "GfxDynamicLightsetLMData.bfTexels", [&](const auto& item) { return item; });
			data.bf_texels_buffer = read_gfx_wrapped_buffer(native.bfTexelsBuffer);
			data.num_skip_records = native.numSkipRecords;
			data.skip_records = plain::read_array_if_present(native.skipRecords, native.numSkipRecords, "GfxDynamicLightsetLMData.skipRecords", [&](const auto& item) { return item; });
			data.skip_records_buffer = read_gfx_wrapped_buffer(native.skipRecordsBuffer);
			data.pallette_records = plain::read_array_if_present(native.palletteRecords, native.numSkipRecords, "GfxDynamicLightsetLMData.palletteRecords", [&](const auto& item) { return item; });
			data.pallette_records_buffer = read_gfx_wrapped_buffer(native.palletteRecordsBuffer);
			return data;
		}

		void write_gfx_dynamic_lightset_lm_data(GfxDynamicLightsetLMData& native, const asset_data::gfxworld::gfx_dynamic_lightset_lm_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numGChunkLM = data.num_g_chunk_lm;
			native.chunkGLM = plain::write_array<GfxDynamicLightsetGChunk>(data.chunk_glm, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_g_chunk(out, in, resolve, memory); });
			native.numGChunkLMBF = data.num_g_chunk_lmbf;
			native.chunkGLMBF = plain::write_array<std::uint16_t>(data.chunk_glmbf, memory, [&](auto& out, const auto& in) { out = in; });
			native.numLMHave = data.num_lm_have;
			native.chunkWhichLM = plain::write_array<std::uint16_t>(data.chunk_which_lm, memory, [&](auto& out, const auto& in) { out = in; });
			native.chunkLMStart = plain::write_array<std::uint32_t>(data.chunk_lm_start, memory, [&](auto& out, const auto& in) { out = in; });
			native.numOrigTexels = data.num_orig_texels;
			native.origTexels = plain::write_array<std::uint32_t>(data.orig_texels, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.origTexelsBuffer, data.orig_texels_buffer, resolve, memory);
			native.origRecords = plain::write_array<std::uint32_t>(data.orig_records, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.origRecordsBuffer, data.orig_records_buffer, resolve, memory);
			native.numBFLMTexels = data.num_bflm_texels;
			native.bfTexels = plain::write_array<std::uint32_t>(data.bf_texels, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.bfTexelsBuffer, data.bf_texels_buffer, resolve, memory);
			native.numSkipRecords = data.num_skip_records;
			native.skipRecords = plain::write_array<std::uint16_t>(data.skip_records, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.skipRecordsBuffer, data.skip_records_buffer, resolve, memory);
			native.palletteRecords = plain::write_array<std::uint32_t>(data.pallette_records, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.palletteRecordsBuffer, data.pallette_records_buffer, resolve, memory);
		}

		std::size_t size_gfx_dynamic_lightset_lm_data([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_lm_data& data)
		{
			std::size_t size = 0;
			if (data.chunk_glm)
			{
				size += plain::sized(std::max<std::size_t>(data.chunk_glm->size(), 1) * sizeof(GfxDynamicLightsetGChunk));
				for (const auto& item : *data.chunk_glm)
				{
					size += size_gfx_dynamic_lightset_g_chunk(item);
				}
			}
			if (data.chunk_glmbf)
			{
				size += plain::sized(std::max<std::size_t>(data.chunk_glmbf->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.chunk_which_lm)
			{
				size += plain::sized(std::max<std::size_t>(data.chunk_which_lm->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.chunk_lm_start)
			{
				size += plain::sized(std::max<std::size_t>(data.chunk_lm_start->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.orig_texels)
			{
				size += plain::sized(std::max<std::size_t>(data.orig_texels->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.orig_texels_buffer);
			if (data.orig_records)
			{
				size += plain::sized(std::max<std::size_t>(data.orig_records->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.orig_records_buffer);
			if (data.bf_texels)
			{
				size += plain::sized(std::max<std::size_t>(data.bf_texels->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.bf_texels_buffer);
			if (data.skip_records)
			{
				size += plain::sized(std::max<std::size_t>(data.skip_records->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_gfx_wrapped_buffer(data.skip_records_buffer);
			if (data.pallette_records)
			{
				size += plain::sized(std::max<std::size_t>(data.pallette_records->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.pallette_records_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_probe_data read_gfx_dynamic_lightset_probe_data(const GfxDynamicLightsetProbeData& native)
		{
			plain::trace("read_gfx_dynamic_lightset_probe_data");
			asset_data::gfxworld::gfx_dynamic_lightset_probe_data data;
			data.num_g_chunk = native.numGChunk;
			data.chunk_g = plain::read_array_if_present(native.chunkG, native.numGChunk, "GfxDynamicLightsetProbeData.chunkG", [&](const auto& item) { return read_gfx_dynamic_lightset_g_chunk(item); });
			data.num_g_chunk_bf = native.numGChunkBF;
			data.chunk_gbf = plain::read_array_if_present(native.chunkGBF, native.numGChunkBF, "GfxDynamicLightsetProbeData.chunkGBF", [&](const auto& item) { return item; });
			data.orig_start = native.origStart;
			data.num_orig_probes = native.numOrigProbes;
			data.orig_probes = plain::read_array_if_present(native.origProbes, native.numOrigProbes, "GfxDynamicLightsetProbeData.origProbes", [&](const auto& item) { return read_gfx_sh_compressed_probe_data(item); });
			data.orig_probes_buffer = read_gfx_wrapped_buffer(native.origProbesBuffer);
			data.num_bf_probes = native.numBFProbes;
			data.bf_probes = plain::read_array_if_present(native.bfProbes, native.numBFProbes, "GfxDynamicLightsetProbeData.bfProbes", [&](const auto& item) { return read_gfx_sh_compressed_probe_data(item); });
			data.bf_probes_buffer = read_gfx_wrapped_buffer(native.bfProbesBuffer);
			data.num_skip_records = native.numSkipRecords;
			data.skip_records = plain::read_array_if_present(native.skipRecords, native.numSkipRecords, "GfxDynamicLightsetProbeData.skipRecords", [&](const auto& item) { return item; });
			data.skip_records_buffer = read_gfx_wrapped_buffer(native.skipRecordsBuffer);
			data.pallette_records = plain::read_array_if_present(native.palletteRecords, native.numSkipRecords, "GfxDynamicLightsetProbeData.palletteRecords", [&](const auto& item) { return item; });
			data.pallette_records_buffer = read_gfx_wrapped_buffer(native.palletteRecordsBuffer);
			data.temp_remap = plain::read_array_if_present(native.tempRemap, native.numOrigProbes, "GfxDynamicLightsetProbeData.tempRemap", [&](const auto& item) { return item; });
			data.temp_remap_buffer = read_gfx_wrapped_buffer(native.tempRemapBuffer);
			return data;
		}

		void write_gfx_dynamic_lightset_probe_data(GfxDynamicLightsetProbeData& native, const asset_data::gfxworld::gfx_dynamic_lightset_probe_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numGChunk = data.num_g_chunk;
			native.chunkG = plain::write_array<GfxDynamicLightsetGChunk>(data.chunk_g, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_g_chunk(out, in, resolve, memory); });
			native.numGChunkBF = data.num_g_chunk_bf;
			native.chunkGBF = plain::write_array<std::uint16_t>(data.chunk_gbf, memory, [&](auto& out, const auto& in) { out = in; });
			native.origStart = data.orig_start;
			native.numOrigProbes = data.num_orig_probes;
			native.origProbes = plain::write_array<GfxSHCompressedProbeData>(data.orig_probes, memory, [&](auto& out, const auto& in) { write_gfx_sh_compressed_probe_data(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.origProbesBuffer, data.orig_probes_buffer, resolve, memory);
			native.numBFProbes = data.num_bf_probes;
			native.bfProbes = plain::write_array<GfxSHCompressedProbeData>(data.bf_probes, memory, [&](auto& out, const auto& in) { write_gfx_sh_compressed_probe_data(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.bfProbesBuffer, data.bf_probes_buffer, resolve, memory);
			native.numSkipRecords = data.num_skip_records;
			native.skipRecords = plain::write_array<std::uint16_t>(data.skip_records, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.skipRecordsBuffer, data.skip_records_buffer, resolve, memory);
			native.palletteRecords = plain::write_array<std::uint32_t>(data.pallette_records, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.palletteRecordsBuffer, data.pallette_records_buffer, resolve, memory);
			native.tempRemap = plain::write_array<std::uint32_t>(data.temp_remap, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.tempRemapBuffer, data.temp_remap_buffer, resolve, memory);
		}

		std::size_t size_gfx_dynamic_lightset_probe_data([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_probe_data& data)
		{
			std::size_t size = 0;
			if (data.chunk_g)
			{
				size += plain::sized(std::max<std::size_t>(data.chunk_g->size(), 1) * sizeof(GfxDynamicLightsetGChunk));
				for (const auto& item : *data.chunk_g)
				{
					size += size_gfx_dynamic_lightset_g_chunk(item);
				}
			}
			if (data.chunk_gbf)
			{
				size += plain::sized(std::max<std::size_t>(data.chunk_gbf->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.orig_probes)
			{
				size += plain::sized(std::max<std::size_t>(data.orig_probes->size(), 1) * sizeof(GfxSHCompressedProbeData));
				for (const auto& item : *data.orig_probes)
				{
					size += size_gfx_sh_compressed_probe_data(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.orig_probes_buffer);
			if (data.bf_probes)
			{
				size += plain::sized(std::max<std::size_t>(data.bf_probes->size(), 1) * sizeof(GfxSHCompressedProbeData));
				for (const auto& item : *data.bf_probes)
				{
					size += size_gfx_sh_compressed_probe_data(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.bf_probes_buffer);
			if (data.skip_records)
			{
				size += plain::sized(std::max<std::size_t>(data.skip_records->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_gfx_wrapped_buffer(data.skip_records_buffer);
			if (data.pallette_records)
			{
				size += plain::sized(std::max<std::size_t>(data.pallette_records->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.pallette_records_buffer);
			if (data.temp_remap)
			{
				size += plain::sized(std::max<std::size_t>(data.temp_remap->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_gfx_wrapped_buffer(data.temp_remap_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset read_gfx_dynamic_lightset(const GfxDynamicLightset& native)
		{
			plain::trace("read_gfx_dynamic_lightset");
			asset_data::gfxworld::gfx_dynamic_lightset data;
			data.num_basis_functions = native.numBasisFunctions;
			data.basis_functions = plain::read_array_if_present(native.basisFunctions, native.numBasisFunctions, "GfxDynamicLightset.basisFunctions", [&](const auto& item) { return read_gfx_dynamic_lightset_bf_data(item); });
			data.num_basis_function_primary_lights = native.numBasisFunctionPrimaryLights;
			data.basis_function_lights = plain::read_array_if_present(native.basisFunctionLights, native.numBasisFunctionPrimaryLights, "GfxDynamicLightset.basisFunctionLights", [&](const auto& item) { return read_gfx_dynamic_lightset_light_info(item); });
			data.num_portal_gi = native.numPortalGI;
			data.portal_gi_data = plain::read_array_if_present(native.portalGIData, native.numPortalGI, "GfxDynamicLightset.portalGIData", [&](const auto& item) { return read_gfx_dynamic_lightset_portal_gi_data(item); });
			data.num_product_gi = native.numProductGI;
			data.product_gi_data = plain::read_array_if_present(native.productGIData, native.numProductGI, "GfxDynamicLightset.productGIData", [&](const auto& item) { return read_gfx_dynamic_lightset_product_gi_data(item); });
			data.lm_data = read_gfx_dynamic_lightset_lm_data(native.lmData);
			data.lg_probe_data = read_gfx_dynamic_lightset_probe_data(native.lgProbeData);
			data.num_dls_basis_functions = native.numDLSBasisFunctions;
			return data;
		}

		void write_gfx_dynamic_lightset(GfxDynamicLightset& native, const asset_data::gfxworld::gfx_dynamic_lightset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numBasisFunctions = data.num_basis_functions;
			native.basisFunctions = plain::write_array<GfxDynamicLightsetBFData>(data.basis_functions, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_bf_data(out, in, resolve, memory); });
			native.numBasisFunctionPrimaryLights = data.num_basis_function_primary_lights;
			native.basisFunctionLights = plain::write_array<GfxDynamicLightsetLightInfo>(data.basis_function_lights, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_light_info(out, in, resolve, memory); });
			native.numPortalGI = data.num_portal_gi;
			native.portalGIData = plain::write_array<GfxDynamicLightsetPortalGIData>(data.portal_gi_data, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_portal_gi_data(out, in, resolve, memory); });
			native.numProductGI = data.num_product_gi;
			native.productGIData = plain::write_array<GfxDynamicLightsetProductGIData>(data.product_gi_data, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_product_gi_data(out, in, resolve, memory); });
			write_gfx_dynamic_lightset_lm_data(native.lmData, data.lm_data, resolve, memory);
			write_gfx_dynamic_lightset_probe_data(native.lgProbeData, data.lg_probe_data, resolve, memory);
			native.numDLSBasisFunctions = data.num_dls_basis_functions;
		}

		std::size_t size_gfx_dynamic_lightset([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset& data)
		{
			std::size_t size = 0;
			if (data.basis_functions)
			{
				size += plain::sized(std::max<std::size_t>(data.basis_functions->size(), 1) * sizeof(GfxDynamicLightsetBFData));
				for (const auto& item : *data.basis_functions)
				{
					size += size_gfx_dynamic_lightset_bf_data(item);
				}
			}
			if (data.basis_function_lights)
			{
				size += plain::sized(std::max<std::size_t>(data.basis_function_lights->size(), 1) * sizeof(GfxDynamicLightsetLightInfo));
				for (const auto& item : *data.basis_function_lights)
				{
					size += size_gfx_dynamic_lightset_light_info(item);
				}
			}
			if (data.portal_gi_data)
			{
				size += plain::sized(std::max<std::size_t>(data.portal_gi_data->size(), 1) * sizeof(GfxDynamicLightsetPortalGIData));
				for (const auto& item : *data.portal_gi_data)
				{
					size += size_gfx_dynamic_lightset_portal_gi_data(item);
				}
			}
			if (data.product_gi_data)
			{
				size += plain::sized(std::max<std::size_t>(data.product_gi_data->size(), 1) * sizeof(GfxDynamicLightsetProductGIData));
				for (const auto& item : *data.product_gi_data)
				{
					size += size_gfx_dynamic_lightset_product_gi_data(item);
				}
			}
			size += size_gfx_dynamic_lightset_lm_data(data.lm_data);
			size += size_gfx_dynamic_lightset_probe_data(data.lg_probe_data);
			return size;
		}

		asset_data::gfxworld::gfx_mayhem_self_vis read_gfx_mayhem_self_vis(const GfxMayhemSelfVis& native)
		{
			plain::trace("read_gfx_mayhem_self_vis");
			asset_data::gfxworld::gfx_mayhem_self_vis data;
			data.self_vis_buffer_size = native.selfVisBufferSize;
			data.self_vis = plain::read_array_if_present(native.selfVis, native.selfVisBufferSize, "GfxMayhemSelfVis.selfVis", [&](const auto& item) { return item; });
			data.self_vis_buffer = read_gfx_wrapped_rw_buffer(native.selfVisBuffer);
			return data;
		}

		void write_gfx_mayhem_self_vis(GfxMayhemSelfVis& native, const asset_data::gfxworld::gfx_mayhem_self_vis& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.selfVisBufferSize = data.self_vis_buffer_size;
			native.selfVis = plain::write_array<std::uint8_t>(data.self_vis, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_rw_buffer(native.selfVisBuffer, data.self_vis_buffer, resolve, memory);
		}

		std::size_t size_gfx_mayhem_self_vis([[maybe_unused]] const asset_data::gfxworld::gfx_mayhem_self_vis& data)
		{
			std::size_t size = 0;
			if (data.self_vis)
			{
				size += plain::sized(std::max<std::size_t>(data.self_vis->size(), 1) * sizeof(std::uint8_t));
			}
			size += size_gfx_wrapped_rw_buffer(data.self_vis_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_frustum_lights read_gfx_frustum_lights(const GfxFrustumLights& native)
		{
			plain::trace("read_gfx_frustum_lights");
			asset_data::gfxworld::gfx_frustum_lights data;
			data.light_count = native.lightCount;
			data.total_indices_count = native.totalIndicesCount;
			data.total_vertices_count = native.totalVerticesCount;
			data.indices_offsets = plain::read_array_if_present(native.indicesOffsets, native.lightCount, "GfxFrustumLights.indicesOffsets", [&](const auto& item) { return item; });
			data.vertices_offsets = plain::read_array_if_present(native.verticesOffsets, native.lightCount, "GfxFrustumLights.verticesOffsets", [&](const auto& item) { return item; });
			data.indices_counts = plain::read_array_if_present(native.indicesCounts, native.lightCount, "GfxFrustumLights.indicesCounts", [&](const auto& item) { return item; });
			data.vertices_counts = plain::read_array_if_present(native.verticesCounts, native.lightCount, "GfxFrustumLights.verticesCounts", [&](const auto& item) { return item; });
			data.indices = plain::read_array_if_present(native.indices, native.totalIndicesCount, "GfxFrustumLights.indices", [&](const auto& item) { return item; });
			data.vertices = plain::read_array_if_present(native.vertices, (native.totalVerticesCount * 16), "GfxFrustumLights.vertices", [&](const auto& item) { return item; });
			data.index_buffer = read_gfx_wrapped_buffer(native.indexBuffer);
			data.vertex_buffer = read_gfx_wrapped_buffer(native.vertexBuffer);
			return data;
		}

		void write_gfx_frustum_lights(GfxFrustumLights& native, const asset_data::gfxworld::gfx_frustum_lights& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lightCount = data.light_count;
			native.totalIndicesCount = data.total_indices_count;
			native.totalVerticesCount = data.total_vertices_count;
			native.indicesOffsets = plain::write_array<std::uint32_t>(data.indices_offsets, memory, [&](auto& out, const auto& in) { out = in; });
			native.verticesOffsets = plain::write_array<std::uint32_t>(data.vertices_offsets, memory, [&](auto& out, const auto& in) { out = in; });
			native.indicesCounts = plain::write_array<std::uint16_t>(data.indices_counts, memory, [&](auto& out, const auto& in) { out = in; });
			native.verticesCounts = plain::write_array<std::uint16_t>(data.vertices_counts, memory, [&](auto& out, const auto& in) { out = in; });
			native.indices = plain::write_array<std::uint16_t>(data.indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.vertices = plain::write_array<std::uint8_t>(data.vertices, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.indexBuffer, data.index_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.vertexBuffer, data.vertex_buffer, resolve, memory);
		}

		std::size_t size_gfx_frustum_lights([[maybe_unused]] const asset_data::gfxworld::gfx_frustum_lights& data)
		{
			std::size_t size = 0;
			if (data.indices_offsets)
			{
				size += plain::sized(std::max<std::size_t>(data.indices_offsets->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.vertices_offsets)
			{
				size += plain::sized(std::max<std::size_t>(data.vertices_offsets->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.indices_counts)
			{
				size += plain::sized(std::max<std::size_t>(data.indices_counts->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.vertices_counts)
			{
				size += plain::sized(std::max<std::size_t>(data.vertices_counts->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.indices)
			{
				size += plain::sized(std::max<std::size_t>(data.indices->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.vertices)
			{
				size += plain::sized(std::max<std::size_t>(data.vertices->size(), 1) * sizeof(std::uint8_t));
			}
			size += size_gfx_wrapped_buffer(data.index_buffer);
			size += size_gfx_wrapped_buffer(data.vertex_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_precomputed_sky_illumination read_gfx_precomputed_sky_illumination(const GfxPrecomputedSkyIllumination& native)
		{
			plain::trace("read_gfx_precomputed_sky_illumination");
			asset_data::gfxworld::gfx_precomputed_sky_illumination data;
			data.detail_mask = plain::read_asset(ASSET_TYPE_IMAGE, native.detailMask);
			data.height_map = plain::read_asset(ASSET_TYPE_IMAGE, native.heightMap);
			data.svd_coefficient = plain::read_asset(ASSET_TYPE_IMAGE, native.SVDCoefficient);
			data.svd_basis = plain::read_asset(ASSET_TYPE_IMAGE, native.SVDBasis);
			data.box_min = plain::read_scalars<float, 4>(&native.boxMin);
			data.box_max = plain::read_scalars<float, 4>(&native.boxMax);
			data.svd_coefficient_min_max = plain::read_scalars<float, 4>(&native.SVDCoefficientMinMax);
			data.height_consts = plain::read_scalars<float, 4>(&native.heightConsts);
			return data;
		}

		void write_gfx_precomputed_sky_illumination(GfxPrecomputedSkyIllumination& native, const asset_data::gfxworld::gfx_precomputed_sky_illumination& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.detailMask = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.detail_mask);
			native.heightMap = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.height_map);
			native.SVDCoefficient = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.svd_coefficient);
			native.SVDBasis = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.svd_basis);
			plain::write_scalars(&native.boxMin, data.box_min);
			plain::write_scalars(&native.boxMax, data.box_max);
			plain::write_scalars(&native.SVDCoefficientMinMax, data.svd_coefficient_min_max);
			plain::write_scalars(&native.heightConsts, data.height_consts);
		}

		std::size_t size_gfx_precomputed_sky_illumination([[maybe_unused]] const asset_data::gfxworld::gfx_precomputed_sky_illumination& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::sunflare_t read_sunflare_t(const sunflare_t& native)
		{
			plain::trace("read_sunflare_t");
			asset_data::gfxworld::sunflare_t data;
			data.has_valid_data = native.hasValidData;
			data.sprite_material = plain::read_asset(ASSET_TYPE_MATERIAL, native.spriteMaterial);
			data.flare_material = plain::read_asset(ASSET_TYPE_MATERIAL, native.flareMaterial);
			data.sprite_size = native.spriteSize;
			data.flare_min_size = native.flareMinSize;
			data.flare_min_dot = native.flareMinDot;
			data.flare_max_size = native.flareMaxSize;
			data.flare_max_dot = native.flareMaxDot;
			data.flare_max_alpha = native.flareMaxAlpha;
			data.flare_fade_in_time = native.flareFadeInTime;
			data.flare_fade_out_time = native.flareFadeOutTime;
			data.blind_min_dot = native.blindMinDot;
			data.blind_max_dot = native.blindMaxDot;
			data.blind_max_darken = native.blindMaxDarken;
			data.blind_fade_in_time = native.blindFadeInTime;
			data.blind_fade_out_time = native.blindFadeOutTime;
			data.glare_min_dot = native.glareMinDot;
			data.glare_max_dot = native.glareMaxDot;
			data.glare_max_lighten = native.glareMaxLighten;
			data.glare_fade_in_time = native.glareFadeInTime;
			data.glare_fade_out_time = native.glareFadeOutTime;
			data.sun_fx_position = plain::read_scalars<float, 3>(&native.sunFxPosition);
			return data;
		}

		void write_sunflare_t(sunflare_t& native, const asset_data::gfxworld::sunflare_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hasValidData = data.has_valid_data;
			native.spriteMaterial = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.sprite_material);
			native.flareMaterial = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.flare_material);
			native.spriteSize = data.sprite_size;
			native.flareMinSize = data.flare_min_size;
			native.flareMinDot = data.flare_min_dot;
			native.flareMaxSize = data.flare_max_size;
			native.flareMaxDot = data.flare_max_dot;
			native.flareMaxAlpha = data.flare_max_alpha;
			native.flareFadeInTime = data.flare_fade_in_time;
			native.flareFadeOutTime = data.flare_fade_out_time;
			native.blindMinDot = data.blind_min_dot;
			native.blindMaxDot = data.blind_max_dot;
			native.blindMaxDarken = data.blind_max_darken;
			native.blindFadeInTime = data.blind_fade_in_time;
			native.blindFadeOutTime = data.blind_fade_out_time;
			native.glareMinDot = data.glare_min_dot;
			native.glareMaxDot = data.glare_max_dot;
			native.glareMaxLighten = data.glare_max_lighten;
			native.glareFadeInTime = data.glare_fade_in_time;
			native.glareFadeOutTime = data.glare_fade_out_time;
			plain::write_scalars(&native.sunFxPosition, data.sun_fx_position);
		}

		std::size_t size_sunflare_t([[maybe_unused]] const asset_data::gfxworld::sunflare_t& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_light_aabb read_gfx_light_aabb(const GfxLightAABB& native)
		{
			plain::trace("read_gfx_light_aabb");
			asset_data::gfxworld::gfx_light_aabb data;
			data.node_count = native.nodeCount;
			data.light_count = native.lightCount;
			data.node_array = plain::read_array_if_present(native.nodeArray, native.nodeCount, "GfxLightAABB.nodeArray", [&](const auto& item) { return read_gfx_light_aabb_node(item); });
			data.light_array = plain::read_array_if_present(native.lightArray, native.lightCount, "GfxLightAABB.lightArray", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_light_aabb(GfxLightAABB& native, const asset_data::gfxworld::gfx_light_aabb& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.nodeCount = data.node_count;
			native.lightCount = data.light_count;
			native.nodeArray = plain::write_array<GfxLightAABBNode>(data.node_array, memory, [&](auto& out, const auto& in) { write_gfx_light_aabb_node(out, in, resolve, memory); });
			native.lightArray = plain::write_array<std::uint16_t>(data.light_array, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_light_aabb([[maybe_unused]] const asset_data::gfxworld::gfx_light_aabb& data)
		{
			std::size_t size = 0;
			if (data.node_array)
			{
				size += plain::sized(std::max<std::size_t>(data.node_array->size(), 1) * sizeof(GfxLightAABBNode));
				for (const auto& item : *data.node_array)
				{
					size += size_gfx_light_aabb_node(item);
				}
			}
			if (data.light_array)
			{
				size += plain::sized(std::max<std::size_t>(data.light_array->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_world_dpvs_static read_gfx_world_dpvs_static(const GfxWorldDpvsStatic& native)
		{
			plain::trace("read_gfx_world_dpvs_static");
			asset_data::gfxworld::gfx_world_dpvs_static data;
			data.smodel_vis_data_count = native.smodelVisDataCount;
			data.surface_vis_data_count = native.surfaceVisDataCount;
			data.primary_light_vis_data_count = native.primaryLightVisDataCount;
			data.reflection_probe_vis_data_count = native.reflectionProbeVisDataCount;
			data.volumetric_vis_data_count = native.volumetricVisDataCount;
			data.decal_vis_data_count = native.decalVisDataCount;
			data.smodel_vis_data = [&] { const std::size_t counts[] = {native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, 0, 0, 0, 0, 0, 0, 0, 0, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, native.smodelVisDataCount, 0}; std::array<std::optional<std::vector<std::uint32_t>>, 33> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_array_if_present(native.smodelVisData[i], counts[i], "GfxWorldDpvsStatic.smodelVisData", [&](const auto& item) { return item; }); } return out; }();
			data.surface_vis_data = [&] { const std::size_t counts[] = {native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, 0, 0, 0, 0, 0, 0, 0, 0, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, native.surfaceVisDataCount, 0}; std::array<std::optional<std::vector<std::uint32_t>>, 33> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_array_if_present(native.surfaceVisData[i], counts[i], "GfxWorldDpvsStatic.surfaceVisData", [&](const auto& item) { return item; }); } return out; }();
			data.primary_light_vis_data = plain::read_array_if_present(native.primaryLightVisData, native.primaryLightVisDataCount, "GfxWorldDpvsStatic.primaryLightVisData", [&](const auto& item) { return item; });
			data.reflection_probe_vis_data = plain::read_array_if_present(native.reflectionProbeVisData, native.reflectionProbeVisDataCount, "GfxWorldDpvsStatic.reflectionProbeVisData", [&](const auto& item) { return item; });
			data.volumetric_vis_data = plain::read_array_if_present(native.volumetricVisData, native.volumetricVisDataCount, "GfxWorldDpvsStatic.volumetricVisData", [&](const auto& item) { return item; });
			data.decal_vis_data = plain::read_array_if_present(native.decalVisData, native.decalVisDataCount, "GfxWorldDpvsStatic.decalVisData", [&](const auto& item) { return item; });
			data.sorted_surf_index = plain::read_array_if_present(native.sortedSurfIndex, (native.surfaceVisDataCount * 32), "GfxWorldDpvsStatic.sortedSurfIndex", [&](const auto& item) { return item; });
			data.static_model_id_count = native.staticModelIDCount;
			data.sorted_smodel_indices = plain::read_array_if_present(native.sortedSmodelIndices, (((native.smodelVisDataCount) * 32) + 1), "GfxWorldDpvsStatic.sortedSmodelIndices", [&](const auto& item) { return item; });
			data.surface_casts_sun_shadow = plain::read_array_if_present(native.surfaceCastsSunShadow, native.surfaceVisDataCount, "GfxWorldDpvsStatic.surfaceCastsSunShadow", [&](const auto& item) { return item; });
			data.sun_shadow_opt_count = native.sunShadowOptCount;
			data.sun_surf_vis_data_count = native.sunSurfVisDataCount;
			data.surface_casts_sun_shadow_opt = plain::read_array_if_present(native.surfaceCastsSunShadowOpt, (native.sunSurfVisDataCount * native.sunShadowOptCount), "GfxWorldDpvsStatic.surfaceCastsSunShadowOpt", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_world_dpvs_static(GfxWorldDpvsStatic& native, const asset_data::gfxworld::gfx_world_dpvs_static& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.smodelVisDataCount = data.smodel_vis_data_count;
			native.surfaceVisDataCount = data.surface_vis_data_count;
			native.primaryLightVisDataCount = data.primary_light_vis_data_count;
			native.reflectionProbeVisDataCount = data.reflection_probe_vis_data_count;
			native.volumetricVisDataCount = data.volumetric_vis_data_count;
			native.decalVisDataCount = data.decal_vis_data_count;
			for (std::size_t i = 0; i < data.smodel_vis_data.size(); ++i)
			{
				native.smodelVisData[i] = plain::write_array<std::uint32_t>(data.smodel_vis_data[i], memory, [&](auto& out, const auto& in) { out = in; });
			}
			for (std::size_t i = 0; i < data.surface_vis_data.size(); ++i)
			{
				native.surfaceVisData[i] = plain::write_array<std::uint32_t>(data.surface_vis_data[i], memory, [&](auto& out, const auto& in) { out = in; });
			}
			native.primaryLightVisData = plain::write_array<std::uint32_t>(data.primary_light_vis_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.reflectionProbeVisData = plain::write_array<std::uint32_t>(data.reflection_probe_vis_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.volumetricVisData = plain::write_array<std::uint32_t>(data.volumetric_vis_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.decalVisData = plain::write_array<std::uint32_t>(data.decal_vis_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.sortedSurfIndex = plain::write_array<std::uint32_t>(data.sorted_surf_index, memory, [&](auto& out, const auto& in) { out = in; });
			native.staticModelIDCount = data.static_model_id_count;
			native.sortedSmodelIndices = plain::write_array<std::uint16_t>(data.sorted_smodel_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.surfaceCastsSunShadow = plain::write_array<std::uint32_t>(data.surface_casts_sun_shadow, memory, [&](auto& out, const auto& in) { out = in; });
			native.sunShadowOptCount = data.sun_shadow_opt_count;
			native.sunSurfVisDataCount = data.sun_surf_vis_data_count;
			native.surfaceCastsSunShadowOpt = plain::write_array<std::uint32_t>(data.surface_casts_sun_shadow_opt, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_world_dpvs_static([[maybe_unused]] const asset_data::gfxworld::gfx_world_dpvs_static& data)
		{
			std::size_t size = 0;
			for (std::size_t i = 0; i < data.smodel_vis_data.size(); ++i)
			{
				if (data.smodel_vis_data[i])
				{
					size += plain::sized(std::max<std::size_t>(data.smodel_vis_data[i]->size(), 1) * sizeof(std::uint32_t));
				}
			}
			for (std::size_t i = 0; i < data.surface_vis_data.size(); ++i)
			{
				if (data.surface_vis_data[i])
				{
					size += plain::sized(std::max<std::size_t>(data.surface_vis_data[i]->size(), 1) * sizeof(std::uint32_t));
				}
			}
			if (data.primary_light_vis_data)
			{
				size += plain::sized(std::max<std::size_t>(data.primary_light_vis_data->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.reflection_probe_vis_data)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_vis_data->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.volumetric_vis_data)
			{
				size += plain::sized(std::max<std::size_t>(data.volumetric_vis_data->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.decal_vis_data)
			{
				size += plain::sized(std::max<std::size_t>(data.decal_vis_data->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.sorted_surf_index)
			{
				size += plain::sized(std::max<std::size_t>(data.sorted_surf_index->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.sorted_smodel_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.sorted_smodel_indices->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.surface_casts_sun_shadow)
			{
				size += plain::sized(std::max<std::size_t>(data.surface_casts_sun_shadow->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.surface_casts_sun_shadow_opt)
			{
				size += plain::sized(std::max<std::size_t>(data.surface_casts_sun_shadow_opt->size(), 1) * sizeof(std::uint32_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_world_dpvs_dynamic read_gfx_world_dpvs_dynamic(const GfxWorldDpvsDynamic& native, [[maybe_unused]] const GfxWorld& outer)
		{
			plain::trace("read_gfx_world_dpvs_dynamic");
			asset_data::gfxworld::gfx_world_dpvs_dynamic data;
			data.dyn_ent_client_word_count = std::to_array(native.dynEntClientWordCount);
			data.dyn_ent_client_count = std::to_array(native.dynEntClientCount);
			data.dyn_ent_cell_bits_0 = plain::read_array_if_present(native.dynEntCellBits[0], (outer.dpvsPlanes.cellCount * native.dynEntClientWordCount[0]), "GfxWorldDpvsDynamic.dynEntCellBits[0]", [&](const auto& item) { return item; });
			data.dyn_ent_cell_bits_1 = plain::read_array_if_present(native.dynEntCellBits[1], (outer.dpvsPlanes.cellCount * native.dynEntClientWordCount[1]), "GfxWorldDpvsDynamic.dynEntCellBits[1]", [&](const auto& item) { return item; });
			data.dyn_ent_vis_data = [&] { const std::size_t counts[][33] = {{((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), 0, ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), ((native.dynEntClientWordCount[0]) * 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {(native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), 0, (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), (native.dynEntClientWordCount[1] * 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}; std::array<std::array<std::optional<std::vector<std::uint8_t>>, 33>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { for (std::size_t j = 0; j < out[i].size(); ++j) { out[i][j] = plain::read_array_if_present(native.dynEntVisData[i][j], counts[i][j], "GfxWorldDpvsDynamic.dynEntVisData", [&](const auto& item) { return item; }); } } return out; }();
			return data;
		}

		void write_gfx_world_dpvs_dynamic(GfxWorldDpvsDynamic& native, const asset_data::gfxworld::gfx_world_dpvs_dynamic& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.dyn_ent_client_word_count, native.dynEntClientWordCount);
			std::ranges::copy(data.dyn_ent_client_count, native.dynEntClientCount);
			native.dynEntCellBits[0] = plain::write_array<std::uint32_t>(data.dyn_ent_cell_bits_0, memory, [&](auto& out, const auto& in) { out = in; });
			native.dynEntCellBits[1] = plain::write_array<std::uint32_t>(data.dyn_ent_cell_bits_1, memory, [&](auto& out, const auto& in) { out = in; });
			for (std::size_t i = 0; i < data.dyn_ent_vis_data.size(); ++i)
			{
				for (std::size_t j = 0; j < data.dyn_ent_vis_data[i].size(); ++j)
				{
					native.dynEntVisData[i][j] = plain::write_array<std::uint8_t>(data.dyn_ent_vis_data[i][j], memory, [&](auto& out, const auto& in) { out = in; });
				}
			}
		}

		std::size_t size_gfx_world_dpvs_dynamic([[maybe_unused]] const asset_data::gfxworld::gfx_world_dpvs_dynamic& data)
		{
			std::size_t size = 0;
			if (data.dyn_ent_cell_bits_0)
			{
				size += plain::sized(std::max<std::size_t>(data.dyn_ent_cell_bits_0->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.dyn_ent_cell_bits_1)
			{
				size += plain::sized(std::max<std::size_t>(data.dyn_ent_cell_bits_1->size(), 1) * sizeof(std::uint32_t));
			}
			for (const auto& row : data.dyn_ent_vis_data)
			{
				for (const auto& entry : row)
				{
					if (entry)
					{
						size += plain::sized(std::max<std::size_t>(entry->size(), 1) * sizeof(std::uint8_t));
					}
				}
			}
			return size;
		}

		asset_data::gfxworld::stream_mtl_group read_stream_mtl_group(const StreamMtlGroup& native)
		{
			plain::trace("read_stream_mtl_group");
			asset_data::gfxworld::stream_mtl_group data;
			data.volumes_key = plain::read_asset(ASSET_TYPE_STREAM_KEY, native.volumesKey);
			data.infos = plain::read_array_if_present(native.infos, (native.imgInfoCount + native.mtlInfoCount), "StreamMtlGroup.infos", [&](const auto& item) { return read_stream_mtl_img_info(item); });
			data.mtl_info_count = native.mtlInfoCount;
			data.img_info_count = native.imgInfoCount;
			data.transient_zone = native.transientZone;
			plain::require_zero(native.unused, "StreamMtlGroup.unused");
			return data;
		}

		void write_stream_mtl_group(StreamMtlGroup& native, const asset_data::gfxworld::stream_mtl_group& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.volumesKey = plain::resolve_asset<StreamKey>(resolve, ASSET_TYPE_STREAM_KEY, data.volumes_key);
			native.infos = plain::write_array<StreamMtlImgInfo>(data.infos, memory, [&](auto& out, const auto& in) { write_stream_mtl_img_info(out, in, resolve, memory); });
			native.mtlInfoCount = data.mtl_info_count;
			native.imgInfoCount = data.img_info_count;
			native.transientZone = data.transient_zone;
		}

		std::size_t size_stream_mtl_group([[maybe_unused]] const asset_data::gfxworld::stream_mtl_group& data)
		{
			std::size_t size = 0;
			if (data.infos)
			{
				size += plain::sized(std::max<std::size_t>(data.infos->size(), 1) * sizeof(StreamMtlImgInfo));
				for (const auto& item : *data.infos)
				{
					size += size_stream_mtl_img_info(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::stream_tree read_stream_tree(const StreamTree& native)
		{
			plain::trace("read_stream_tree");
			asset_data::gfxworld::stream_tree data;
			data.nodes = plain::read_array_if_present(native.nodes, native.nodeCount, "StreamTree.nodes", [&](const auto& item) { return read_stream_tree_node(item); });
			data.materials = plain::read_array_if_present(native.materials, native.materialCount, "StreamTree.materials", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_MATERIAL, item); });
			data.images = plain::read_array_if_present(native.images, native.imageCount, "StreamTree.images", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.node_count = native.nodeCount;
			data.material_count = native.materialCount;
			data.image_count = native.imageCount;
			data.large_mtl_group = read_stream_mtl_group(native.largeMtlGroup);
			data.memory_size = native.memorySize;
			data.mins = plain::read_scalars<float, 3>(&native.mins);
			data.maxs = plain::read_scalars<float, 3>(&native.maxs);
			return data;
		}

		void write_stream_tree(StreamTree& native, const asset_data::gfxworld::stream_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.nodes = plain::write_array<StreamTreeNode>(data.nodes, memory, [&](auto& out, const auto& in) { write_stream_tree_node(out, in, resolve, memory); });
			native.materials = plain::write_array<Material*>(data.materials, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, in); });
			native.images = plain::write_array<GfxImage*>(data.images, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			native.nodeCount = data.node_count;
			native.materialCount = data.material_count;
			native.imageCount = data.image_count;
			write_stream_mtl_group(native.largeMtlGroup, data.large_mtl_group, resolve, memory);
			native.memorySize = data.memory_size;
			plain::write_scalars(&native.mins, data.mins);
			plain::write_scalars(&native.maxs, data.maxs);
		}

		std::size_t size_stream_tree([[maybe_unused]] const asset_data::gfxworld::stream_tree& data)
		{
			std::size_t size = 0;
			if (data.nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.nodes->size(), 1) * sizeof(StreamTreeNode));
				for (const auto& item : *data.nodes)
				{
					size += size_stream_tree_node(item);
				}
			}
			if (data.materials)
			{
				size += plain::sized(std::max<std::size_t>(data.materials->size(), 1) * sizeof(Material*));
			}
			if (data.images)
			{
				size += plain::sized(std::max<std::size_t>(data.images->size(), 1) * sizeof(GfxImage*));
			}
			size += size_stream_mtl_group(data.large_mtl_group);
			return size;
		}

		asset_data::gfxworld::extent_bounds read_extent_bounds(const ExtentBounds& native)
		{
			plain::trace("read_extent_bounds");
			asset_data::gfxworld::extent_bounds data;
			data.mins = plain::read_scalars<float, 3>(&native.mins);
			data.maxs = plain::read_scalars<float, 3>(&native.maxs);
			return data;
		}

		void write_extent_bounds(ExtentBounds& native, const asset_data::gfxworld::extent_bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.mins, data.mins);
			plain::write_scalars(&native.maxs, data.maxs);
		}

		std::size_t size_extent_bounds([[maybe_unused]] const asset_data::gfxworld::extent_bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::stream_tree_grid read_stream_tree_grid(const StreamTreeGrid& native)
		{
			plain::trace("read_stream_tree_grid");
			asset_data::gfxworld::stream_tree_grid data;
			data.cell_keys = plain::read_array_if_present(native.cellKeys, (native.columnCount * native.rowCount), "StreamTreeGrid.cellKeys", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_STREAM_KEY, item); });
			data.materials = plain::read_array_if_present(native.materials, native.materialCount, "StreamTreeGrid.materials", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_MATERIAL, item); });
			data.images = plain::read_array_if_present(native.images, native.imageCount, "StreamTreeGrid.images", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_IMAGE, item); });
			data.xmodels = plain::read_array_if_present(native.xmodels, native.xmodelCount, "StreamTreeGrid.xmodels", [&](const auto& item) { return plain::read_asset(ASSET_TYPE_XMODEL, item); });
			data.smodel_collection_indices = plain::read_array_if_present(native.smodelCollectionIndices, native.smodelCollectionIndexCount, "StreamTreeGrid.smodelCollectionIndices", [&](const auto& item) { return item; });
			data.bounds = read_extent_bounds(native.bounds);
			data.version = native.version;
			data.row_count = native.rowCount;
			data.column_count = native.columnCount;
			data.material_count = native.materialCount;
			data.image_count = native.imageCount;
			data.xmodel_count = native.xmodelCount;
			data.smodel_collection_index_count = native.smodelCollectionIndexCount;
			return data;
		}

		void write_stream_tree_grid(StreamTreeGrid& native, const asset_data::gfxworld::stream_tree_grid& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.cellKeys = plain::write_array<StreamKey*>(data.cell_keys, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<StreamKey>(resolve, ASSET_TYPE_STREAM_KEY, in); });
			native.materials = plain::write_array<Material*>(data.materials, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, in); });
			native.images = plain::write_array<GfxImage*>(data.images, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, in); });
			native.xmodels = plain::write_array<XModel*>(data.xmodels, memory, [&](auto& out, const auto& in) { out = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, in); });
			native.smodelCollectionIndices = plain::write_array<std::uint16_t>(data.smodel_collection_indices, memory, [&](auto& out, const auto& in) { out = in; });
			write_extent_bounds(native.bounds, data.bounds, resolve, memory);
			native.version = data.version;
			native.rowCount = data.row_count;
			native.columnCount = data.column_count;
			native.materialCount = data.material_count;
			native.imageCount = data.image_count;
			native.xmodelCount = data.xmodel_count;
			native.smodelCollectionIndexCount = data.smodel_collection_index_count;
		}

		std::size_t size_stream_tree_grid([[maybe_unused]] const asset_data::gfxworld::stream_tree_grid& data)
		{
			std::size_t size = 0;
			if (data.cell_keys)
			{
				size += plain::sized(std::max<std::size_t>(data.cell_keys->size(), 1) * sizeof(StreamKey*));
			}
			if (data.materials)
			{
				size += plain::sized(std::max<std::size_t>(data.materials->size(), 1) * sizeof(Material*));
			}
			if (data.images)
			{
				size += plain::sized(std::max<std::size_t>(data.images->size(), 1) * sizeof(GfxImage*));
			}
			if (data.xmodels)
			{
				size += plain::sized(std::max<std::size_t>(data.xmodels->size(), 1) * sizeof(XModel*));
			}
			if (data.smodel_collection_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_collection_indices->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_extent_bounds(data.bounds);
			return size;
		}

		asset_data::gfxworld::gfx_world read_gfx_world(const GfxWorld& native)
		{
			plain::trace("read_gfx_world");
			asset_data::gfxworld::gfx_world data;
			data.name = plain::read_string(native.name);
			data.base_name = plain::read_string(native.baseName);
			data.bsp_version = plain::enum_name(asset_data::gfxworld::bsp_version_names, static_cast<std::int64_t>(native.bspVersion), "GfxWorld.bspVersion");
			data.last_sun_primary_light_index = native.lastSunPrimaryLightIndex;
			data.primary_light_count = native.primaryLightCount;
			data.first_mutable_primary_light = native.firstMutablePrimaryLight;
			data.mutable_primary_light_count = native.mutablePrimaryLightCount;
			data.first_static_scriptables_primary_light = native.firstStaticScriptablesPrimaryLight;
			data.static_scriptables_primary_light_count = native.staticScriptablesPrimaryLightCount;
			data.first_static_scriptable_primary_light = native.firstStaticScriptablePrimaryLight;
			data.static_scriptable_primary_light_count = native.staticScriptablePrimaryLightCount;
			data.first_moving_scriptable_primary_light = native.firstMovingScriptablePrimaryLight;
			data.moving_scriptable_primary_light_count = native.movingScriptablePrimaryLightCount;
			data.sort_key_lit_decal = native.sortKeyLitDecal;
			data.sort_key_effect_decal = native.sortKeyEffectDecal;
			data.sort_key_top_decal = native.sortKeyTopDecal;
			data.sort_key_effect_auto = native.sortKeyEffectAuto;
			data.sort_key_distortion = native.sortKeyDistortion;
			data.sort_key_effect_distortion = native.sortKeyEffectDistortion;
			data.sort_key2_d = native.sortKey2D;
			data.sort_key_opaque_begin = native.sortKeyOpaqueBegin;
			data.sort_key_opaque_end = native.sortKeyOpaqueEnd;
			data.sort_key_decal_begin = native.sortKeyDecalBegin;
			data.sort_key_decal_end = native.sortKeyDecalEnd;
			data.sort_key_trans_begin = native.sortKeyTransBegin;
			data.sort_key_trans_end = native.sortKeyTransEnd;
			data.sort_key_emissive_begin = native.sortKeyEmissiveBegin;
			data.sort_key_emissive_end = native.sortKeyEmissiveEnd;
			data.bounds = read_bounds(native.bounds);
			data.dpvs_planes = read_gfx_world_dpvs_planes(native.dpvsPlanes);
			data.cells = plain::read_array_if_present(native.cells, native.dpvsPlanes.cellCount, "GfxWorld.cells", [&](const auto& item) { return read_gfx_cell(item); });
			data.cell_transient_infos = plain::read_array_if_present(native.cellTransientInfos, native.dpvsPlanes.cellCount, "GfxWorld.cellTransientInfos", [&](const auto& item) { return read_gfx_cell_transient_info(item); });
			data.surfaces = read_gfx_world_surfaces(native.surfaces);
			data.smodels = read_gfx_world_static_models(native.smodels);
			data.draw = read_gfx_world_draw(native.draw);
			data.dynamic_lightset = read_gfx_dynamic_lightset(native.dynamicLightset);
			data.mayhem_self_vis = read_gfx_mayhem_self_vis(native.mayhemSelfVis);
			data.frustum_lights = read_gfx_frustum_lights(native.frustumLights);
			data.light_view_frustums = plain::read_array_if_present(native.lightViewFrustums, native.primaryLightCount, "GfxWorld.lightViewFrustums", [&](const auto& item) { return read_gfx_light_view_frustum(item); });
			data.primary_lights = plain::read_array_if_present(native.primaryLights, native.primaryLightCount, "GfxWorld.primaryLights", [&](const auto& item) { return read_gfx_light(item); });
			data.voxel_tree_count = native.voxelTreeCount;
			data.voxel_tree = plain::read_array_if_present(native.voxelTree, native.voxelTreeCount, "GfxWorld.voxelTree", [&](const auto& item) { return read_gfx_voxel_tree(item); });
			data.heightfield_count = native.heightfieldCount;
			data.heightfields = plain::read_array_if_present(native.heightfields, native.heightfieldCount, "GfxWorld.heightfields", [&](const auto& item) { return read_gfx_heightfield(item); });
			data.precomputed_sky_illumination = read_gfx_precomputed_sky_illumination(native.precomputedSkyIllumination);
			data.model_count = native.modelCount;
			data.models = plain::read_array_if_present(native.models, native.modelCount, "GfxWorld.models", [&](const auto& item) { return read_gfx_brush_model(item); });
			data.lmap_lookup_count = native.lmapLookupCount;
			data.lmap_lookup = plain::read_array_if_present(native.lmapLookup, native.lmapLookupCount, "GfxWorld.lmapLookup", [&](const auto& item) { return read_gfx_model_lightmap_inst(item); });
			data.gfx_map_ent_lookup_count = native.gfxMapEntLookupCount;
			data.gfx_map_ent_lookup = plain::read_array_if_present(native.gfxMapEntLookup, native.gfxMapEntLookupCount, "GfxWorld.gfxMapEntLookup", [&](const auto& item) { return read_gfx_map_ent_lookup(item); });
			data.checksum = native.checksum;
			data.sun = read_sunflare_t(native.sun);
			data.outdoor_lookup_matrix = plain::read_block<std::array<std::array<float, 4>, 4>>(&native.outdoorLookupMatrix);
			data.outdoor_image = plain::read_asset(ASSET_TYPE_IMAGE, native.outdoorImage);
			data.dust_material = plain::read_asset(ASSET_TYPE_MATERIAL, native.dustMaterial);
			data.material_lod0_size_threshold = native.materialLod0SizeThreshold;
			data.cell_caster_bits = plain::read_array_if_present(native.cellCasterBits, ((((native.dpvsPlanes.cellCount) + 31) >> 5) * (native.dpvsPlanes.cellCount)), "GfxWorld.cellCasterBits", [&](const auto& item) { return item; });
			data.cell_has_sun_lit_surfs_bits = plain::read_array_if_present(native.cellHasSunLitSurfsBits, (((native.dpvsPlanes.cellCount) + 31) >> 5), "GfxWorld.cellHasSunLitSurfsBits", [&](const auto& item) { return item; });
			data.scene_dyn_model = plain::read_array_if_present(native.sceneDynModel, native.dpvsDyn.dynEntClientCount[0], "GfxWorld.sceneDynModel", [&](const auto& item) { return read_gfx_scene_dyn_model(item); });
			data.scene_dyn_brush = plain::read_array_if_present(native.sceneDynBrush, native.dpvsDyn.dynEntClientCount[1], "GfxWorld.sceneDynBrush", [&](const auto& item) { return read_gfx_scene_dyn_brush(item); });
			data.primary_light_motion_detect_bits_entries = native.primaryLightMotionDetectBitsEntries;
			data.primary_light_motion_detect_bits = plain::read_array_if_present(native.primaryLightMotionDetectBits, native.primaryLightMotionDetectBitsEntries, "GfxWorld.primaryLightMotionDetectBits", [&](const auto& item) { return item; });
			data.entity_motion_bits_entries = native.entityMotionBitsEntries;
			data.entity_motion_bits = plain::read_array_if_present(native.entityMotionBits, native.entityMotionBitsEntries, "GfxWorld.entityMotionBits", [&](const auto& item) { return item; });
			data.static_spot_omni_primary_light_count_aligned = native.staticSpotOmniPrimaryLightCountAligned;
			data.shadow_bits_array_pitch = native.shadowBitsArrayPitch;
			data.num_primary_light_entity_shadow_vis_entries = native.numPrimaryLightEntityShadowVisEntries;
			data.primary_light_entity_shadow_vis = plain::read_array_if_present(native.primaryLightEntityShadowVis, native.numPrimaryLightEntityShadowVisEntries, "GfxWorld.primaryLightEntityShadowVis", [&](const auto& item) { return item; });
			data.dyn_ent_motion_bits_entries = std::to_array(native.dynEntMotionBitsEntries);
			data.dyn_ent_motion_bits_0 = plain::read_array_if_present(native.dynEntMotionBits[0], native.dynEntMotionBitsEntries[0], "GfxWorld.dynEntMotionBits[0]", [&](const auto& item) { return item; });
			data.dyn_ent_motion_bits_1 = plain::read_array_if_present(native.dynEntMotionBits[1], native.dynEntMotionBitsEntries[1], "GfxWorld.dynEntMotionBits[1]", [&](const auto& item) { return item; });
			data.num_primary_light_dyn_ent_shadow_vis_entries = std::to_array(native.numPrimaryLightDynEntShadowVisEntries);
			data.primary_light_dyn_ent_shadow_vis_0 = plain::read_array_if_present(native.primaryLightDynEntShadowVis[0], native.numPrimaryLightDynEntShadowVisEntries[0], "GfxWorld.primaryLightDynEntShadowVis[0]", [&](const auto& item) { return item; });
			data.primary_light_dyn_ent_shadow_vis_1 = plain::read_array_if_present(native.primaryLightDynEntShadowVis[1], native.numPrimaryLightDynEntShadowVisEntries[1], "GfxWorld.primaryLightDynEntShadowVis[1]", [&](const auto& item) { return item; });
			data.shadow_geom_optimized = plain::read_array_if_present(native.shadowGeomOptimized, native.primaryLightCount, "GfxWorld.shadowGeomOptimized", [&](const auto& item) { return read_gfx_shadow_geometry(item); });
			data.light_aabb = read_gfx_light_aabb(native.lightAABB);
			data.dpvs = read_gfx_world_dpvs_static(native.dpvs);
			data.dpvs_dyn = read_gfx_world_dpvs_dynamic(native.dpvsDyn, native);
			data.num_umbra_gates = native.numUmbraGates;
			data.umbra_gates = plain::read_array_if_present(native.umbraGates, native.numUmbraGates, "GfxWorld.umbraGates", [&](const auto& item) { return read_umbra_gate(item); });
			data.umbra_tome_size = native.umbraTomeSize;
			data.umbra_tome_data = plain::read_array_if_present(native.umbraTomeData, native.umbraTomeSize, "GfxWorld.umbraTomeData", [&](const auto& item) { return item; });
			plain::require_zero(native.umbraTome, "GfxWorld.umbraTome");
			data.mdao_volume_count = native.mdaoVolumeCount;
			data.mdao_volumes = plain::read_array_if_present(native.mdaoVolumes, native.mdaoVolumeCount, "GfxWorld.mdaoVolumes", [&](const auto& item) { return read_mdao_volume(item); });
			data.enable_mdao = native.enableMdao;
			data.stream_tree = read_stream_tree(native.streamTree);
			data.material_stream_tree_grid = read_stream_tree_grid(native.materialStreamTreeGrid);
			data.xmodel_stream_tree_grid = read_stream_tree_grid(native.xmodelStreamTreeGrid);
			return data;
		}

		void write_gfx_world(GfxWorld& native, const asset_data::gfxworld::gfx_world& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.baseName = plain::write_string(memory, data.base_name);
			native.bspVersion = static_cast<BspVersion>(plain::enum_value(asset_data::gfxworld::bsp_version_names, data.bsp_version, "GfxWorld.bspVersion"));
			native.lastSunPrimaryLightIndex = data.last_sun_primary_light_index;
			native.primaryLightCount = data.primary_light_count;
			native.firstMutablePrimaryLight = data.first_mutable_primary_light;
			native.mutablePrimaryLightCount = data.mutable_primary_light_count;
			native.firstStaticScriptablesPrimaryLight = data.first_static_scriptables_primary_light;
			native.staticScriptablesPrimaryLightCount = data.static_scriptables_primary_light_count;
			native.firstStaticScriptablePrimaryLight = data.first_static_scriptable_primary_light;
			native.staticScriptablePrimaryLightCount = data.static_scriptable_primary_light_count;
			native.firstMovingScriptablePrimaryLight = data.first_moving_scriptable_primary_light;
			native.movingScriptablePrimaryLightCount = data.moving_scriptable_primary_light_count;
			native.sortKeyLitDecal = data.sort_key_lit_decal;
			native.sortKeyEffectDecal = data.sort_key_effect_decal;
			native.sortKeyTopDecal = data.sort_key_top_decal;
			native.sortKeyEffectAuto = data.sort_key_effect_auto;
			native.sortKeyDistortion = data.sort_key_distortion;
			native.sortKeyEffectDistortion = data.sort_key_effect_distortion;
			native.sortKey2D = data.sort_key2_d;
			native.sortKeyOpaqueBegin = data.sort_key_opaque_begin;
			native.sortKeyOpaqueEnd = data.sort_key_opaque_end;
			native.sortKeyDecalBegin = data.sort_key_decal_begin;
			native.sortKeyDecalEnd = data.sort_key_decal_end;
			native.sortKeyTransBegin = data.sort_key_trans_begin;
			native.sortKeyTransEnd = data.sort_key_trans_end;
			native.sortKeyEmissiveBegin = data.sort_key_emissive_begin;
			native.sortKeyEmissiveEnd = data.sort_key_emissive_end;
			write_bounds(native.bounds, data.bounds, resolve, memory);
			write_gfx_world_dpvs_planes(native.dpvsPlanes, data.dpvs_planes, resolve, memory);
			native.cells = plain::write_array<GfxCell>(data.cells, memory, [&](auto& out, const auto& in) { write_gfx_cell(out, in, resolve, memory); });
			native.cellTransientInfos = plain::write_array<GfxCellTransientInfo>(data.cell_transient_infos, memory, [&](auto& out, const auto& in) { write_gfx_cell_transient_info(out, in, resolve, memory); });
			write_gfx_world_surfaces(native.surfaces, data.surfaces, resolve, memory);
			write_gfx_world_static_models(native.smodels, data.smodels, resolve, memory);
			write_gfx_world_draw(native.draw, data.draw, resolve, memory);
			write_gfx_dynamic_lightset(native.dynamicLightset, data.dynamic_lightset, resolve, memory);
			write_gfx_mayhem_self_vis(native.mayhemSelfVis, data.mayhem_self_vis, resolve, memory);
			write_gfx_frustum_lights(native.frustumLights, data.frustum_lights, resolve, memory);
			native.lightViewFrustums = plain::write_array<GfxLightViewFrustum>(data.light_view_frustums, memory, [&](auto& out, const auto& in) { write_gfx_light_view_frustum(out, in, resolve, memory); });
			native.primaryLights = plain::write_array<GfxLight>(data.primary_lights, memory, [&](auto& out, const auto& in) { write_gfx_light(out, in, resolve, memory); });
			native.voxelTreeCount = data.voxel_tree_count;
			native.voxelTree = plain::write_array<GfxVoxelTree>(data.voxel_tree, memory, [&](auto& out, const auto& in) { write_gfx_voxel_tree(out, in, resolve, memory); });
			native.heightfieldCount = data.heightfield_count;
			native.heightfields = plain::write_array<GfxHeightfield>(data.heightfields, memory, [&](auto& out, const auto& in) { write_gfx_heightfield(out, in, resolve, memory); });
			write_gfx_precomputed_sky_illumination(native.precomputedSkyIllumination, data.precomputed_sky_illumination, resolve, memory);
			native.modelCount = data.model_count;
			native.models = plain::write_array<GfxBrushModel>(data.models, memory, [&](auto& out, const auto& in) { write_gfx_brush_model(out, in, resolve, memory); });
			native.lmapLookupCount = data.lmap_lookup_count;
			native.lmapLookup = plain::write_array<GfxModelLightmapInst>(data.lmap_lookup, memory, [&](auto& out, const auto& in) { write_gfx_model_lightmap_inst(out, in, resolve, memory); });
			native.gfxMapEntLookupCount = data.gfx_map_ent_lookup_count;
			native.gfxMapEntLookup = plain::write_array<GfxMapEntLookup>(data.gfx_map_ent_lookup, memory, [&](auto& out, const auto& in) { write_gfx_map_ent_lookup(out, in, resolve, memory); });
			native.checksum = data.checksum;
			write_sunflare_t(native.sun, data.sun, resolve, memory);
			plain::write_block(&native.outdoorLookupMatrix, data.outdoor_lookup_matrix);
			native.outdoorImage = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.outdoor_image);
			native.dustMaterial = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.dust_material);
			native.materialLod0SizeThreshold = data.material_lod0_size_threshold;
			native.cellCasterBits = plain::write_array<std::uint32_t>(data.cell_caster_bits, memory, [&](auto& out, const auto& in) { out = in; });
			native.cellHasSunLitSurfsBits = plain::write_array<std::uint32_t>(data.cell_has_sun_lit_surfs_bits, memory, [&](auto& out, const auto& in) { out = in; });
			native.sceneDynModel = plain::write_array<GfxSceneDynModel>(data.scene_dyn_model, memory, [&](auto& out, const auto& in) { write_gfx_scene_dyn_model(out, in, resolve, memory); });
			native.sceneDynBrush = plain::write_array<GfxSceneDynBrush>(data.scene_dyn_brush, memory, [&](auto& out, const auto& in) { write_gfx_scene_dyn_brush(out, in, resolve, memory); });
			native.primaryLightMotionDetectBitsEntries = data.primary_light_motion_detect_bits_entries;
			native.primaryLightMotionDetectBits = plain::write_array<std::uint32_t>(data.primary_light_motion_detect_bits, memory, [&](auto& out, const auto& in) { out = in; });
			native.entityMotionBitsEntries = data.entity_motion_bits_entries;
			native.entityMotionBits = plain::write_array<std::uint32_t>(data.entity_motion_bits, memory, [&](auto& out, const auto& in) { out = in; });
			native.staticSpotOmniPrimaryLightCountAligned = data.static_spot_omni_primary_light_count_aligned;
			native.shadowBitsArrayPitch = data.shadow_bits_array_pitch;
			native.numPrimaryLightEntityShadowVisEntries = data.num_primary_light_entity_shadow_vis_entries;
			native.primaryLightEntityShadowVis = plain::write_array<std::uint32_t>(data.primary_light_entity_shadow_vis, memory, [&](auto& out, const auto& in) { out = in; });
			std::ranges::copy(data.dyn_ent_motion_bits_entries, native.dynEntMotionBitsEntries);
			native.dynEntMotionBits[0] = plain::write_array<std::uint32_t>(data.dyn_ent_motion_bits_0, memory, [&](auto& out, const auto& in) { out = in; });
			native.dynEntMotionBits[1] = plain::write_array<std::uint32_t>(data.dyn_ent_motion_bits_1, memory, [&](auto& out, const auto& in) { out = in; });
			std::ranges::copy(data.num_primary_light_dyn_ent_shadow_vis_entries, native.numPrimaryLightDynEntShadowVisEntries);
			native.primaryLightDynEntShadowVis[0] = plain::write_array<std::uint32_t>(data.primary_light_dyn_ent_shadow_vis_0, memory, [&](auto& out, const auto& in) { out = in; });
			native.primaryLightDynEntShadowVis[1] = plain::write_array<std::uint32_t>(data.primary_light_dyn_ent_shadow_vis_1, memory, [&](auto& out, const auto& in) { out = in; });
			native.shadowGeomOptimized = plain::write_array<GfxShadowGeometry>(data.shadow_geom_optimized, memory, [&](auto& out, const auto& in) { write_gfx_shadow_geometry(out, in, resolve, memory); });
			write_gfx_light_aabb(native.lightAABB, data.light_aabb, resolve, memory);
			write_gfx_world_dpvs_static(native.dpvs, data.dpvs, resolve, memory);
			write_gfx_world_dpvs_dynamic(native.dpvsDyn, data.dpvs_dyn, resolve, memory);
			native.numUmbraGates = data.num_umbra_gates;
			native.umbraGates = plain::write_array<UmbraGate>(data.umbra_gates, memory, [&](auto& out, const auto& in) { write_umbra_gate(out, in, resolve, memory); });
			native.umbraTomeSize = data.umbra_tome_size;
			native.umbraTomeData = plain::write_array<std::uint8_t>(data.umbra_tome_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.mdaoVolumeCount = data.mdao_volume_count;
			native.mdaoVolumes = plain::write_array<MdaoVolume>(data.mdao_volumes, memory, [&](auto& out, const auto& in) { write_mdao_volume(out, in, resolve, memory); });
			native.enableMdao = data.enable_mdao;
			write_stream_tree(native.streamTree, data.stream_tree, resolve, memory);
			write_stream_tree_grid(native.materialStreamTreeGrid, data.material_stream_tree_grid, resolve, memory);
			write_stream_tree_grid(native.xmodelStreamTreeGrid, data.xmodel_stream_tree_grid, resolve, memory);
		}

		std::size_t size_gfx_world([[maybe_unused]] const asset_data::gfxworld::gfx_world& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += plain::string_size(data.base_name);
			size += size_bounds(data.bounds);
			size += size_gfx_world_dpvs_planes(data.dpvs_planes);
			if (data.cells)
			{
				size += plain::sized(std::max<std::size_t>(data.cells->size(), 1) * sizeof(GfxCell));
				for (const auto& item : *data.cells)
				{
					size += size_gfx_cell(item);
				}
			}
			if (data.cell_transient_infos)
			{
				size += plain::sized(std::max<std::size_t>(data.cell_transient_infos->size(), 1) * sizeof(GfxCellTransientInfo));
				for (const auto& item : *data.cell_transient_infos)
				{
					size += size_gfx_cell_transient_info(item);
				}
			}
			size += size_gfx_world_surfaces(data.surfaces);
			size += size_gfx_world_static_models(data.smodels);
			size += size_gfx_world_draw(data.draw);
			size += size_gfx_dynamic_lightset(data.dynamic_lightset);
			size += size_gfx_mayhem_self_vis(data.mayhem_self_vis);
			size += size_gfx_frustum_lights(data.frustum_lights);
			if (data.light_view_frustums)
			{
				size += plain::sized(std::max<std::size_t>(data.light_view_frustums->size(), 1) * sizeof(GfxLightViewFrustum));
				for (const auto& item : *data.light_view_frustums)
				{
					size += size_gfx_light_view_frustum(item);
				}
			}
			if (data.primary_lights)
			{
				size += plain::sized(std::max<std::size_t>(data.primary_lights->size(), 1) * sizeof(GfxLight));
				for (const auto& item : *data.primary_lights)
				{
					size += size_gfx_light(item);
				}
			}
			if (data.voxel_tree)
			{
				size += plain::sized(std::max<std::size_t>(data.voxel_tree->size(), 1) * sizeof(GfxVoxelTree));
				for (const auto& item : *data.voxel_tree)
				{
					size += size_gfx_voxel_tree(item);
				}
			}
			if (data.heightfields)
			{
				size += plain::sized(std::max<std::size_t>(data.heightfields->size(), 1) * sizeof(GfxHeightfield));
				for (const auto& item : *data.heightfields)
				{
					size += size_gfx_heightfield(item);
				}
			}
			size += size_gfx_precomputed_sky_illumination(data.precomputed_sky_illumination);
			if (data.models)
			{
				size += plain::sized(std::max<std::size_t>(data.models->size(), 1) * sizeof(GfxBrushModel));
				for (const auto& item : *data.models)
				{
					size += size_gfx_brush_model(item);
				}
			}
			if (data.lmap_lookup)
			{
				size += plain::sized(std::max<std::size_t>(data.lmap_lookup->size(), 1) * sizeof(GfxModelLightmapInst));
				for (const auto& item : *data.lmap_lookup)
				{
					size += size_gfx_model_lightmap_inst(item);
				}
			}
			if (data.gfx_map_ent_lookup)
			{
				size += plain::sized(std::max<std::size_t>(data.gfx_map_ent_lookup->size(), 1) * sizeof(GfxMapEntLookup));
				for (const auto& item : *data.gfx_map_ent_lookup)
				{
					size += size_gfx_map_ent_lookup(item);
				}
			}
			size += size_sunflare_t(data.sun);
			if (data.cell_caster_bits)
			{
				size += plain::sized(std::max<std::size_t>(data.cell_caster_bits->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.cell_has_sun_lit_surfs_bits)
			{
				size += plain::sized(std::max<std::size_t>(data.cell_has_sun_lit_surfs_bits->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.scene_dyn_model)
			{
				size += plain::sized(std::max<std::size_t>(data.scene_dyn_model->size(), 1) * sizeof(GfxSceneDynModel));
				for (const auto& item : *data.scene_dyn_model)
				{
					size += size_gfx_scene_dyn_model(item);
				}
			}
			if (data.scene_dyn_brush)
			{
				size += plain::sized(std::max<std::size_t>(data.scene_dyn_brush->size(), 1) * sizeof(GfxSceneDynBrush));
				for (const auto& item : *data.scene_dyn_brush)
				{
					size += size_gfx_scene_dyn_brush(item);
				}
			}
			if (data.primary_light_motion_detect_bits)
			{
				size += plain::sized(std::max<std::size_t>(data.primary_light_motion_detect_bits->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.entity_motion_bits)
			{
				size += plain::sized(std::max<std::size_t>(data.entity_motion_bits->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.primary_light_entity_shadow_vis)
			{
				size += plain::sized(std::max<std::size_t>(data.primary_light_entity_shadow_vis->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.dyn_ent_motion_bits_0)
			{
				size += plain::sized(std::max<std::size_t>(data.dyn_ent_motion_bits_0->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.dyn_ent_motion_bits_1)
			{
				size += plain::sized(std::max<std::size_t>(data.dyn_ent_motion_bits_1->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.primary_light_dyn_ent_shadow_vis_0)
			{
				size += plain::sized(std::max<std::size_t>(data.primary_light_dyn_ent_shadow_vis_0->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.primary_light_dyn_ent_shadow_vis_1)
			{
				size += plain::sized(std::max<std::size_t>(data.primary_light_dyn_ent_shadow_vis_1->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.shadow_geom_optimized)
			{
				size += plain::sized(std::max<std::size_t>(data.shadow_geom_optimized->size(), 1) * sizeof(GfxShadowGeometry));
				for (const auto& item : *data.shadow_geom_optimized)
				{
					size += size_gfx_shadow_geometry(item);
				}
			}
			size += size_gfx_light_aabb(data.light_aabb);
			size += size_gfx_world_dpvs_static(data.dpvs);
			size += size_gfx_world_dpvs_dynamic(data.dpvs_dyn);
			if (data.umbra_gates)
			{
				size += plain::sized(std::max<std::size_t>(data.umbra_gates->size(), 1) * sizeof(UmbraGate));
				for (const auto& item : *data.umbra_gates)
				{
					size += size_umbra_gate(item);
				}
			}
			if (data.umbra_tome_data)
			{
				size += plain::sized(std::max<std::size_t>(data.umbra_tome_data->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.mdao_volumes)
			{
				size += plain::sized(std::max<std::size_t>(data.mdao_volumes->size(), 1) * sizeof(MdaoVolume));
				for (const auto& item : *data.mdao_volumes)
				{
					size += size_mdao_volume(item);
				}
			}
			size += size_stream_tree(data.stream_tree);
			size += size_stream_tree_grid(data.material_stream_tree_grid);
			size += size_stream_tree_grid(data.xmodel_stream_tree_grid);
			return size;
		}

		asset_data::gfxworld::gfx_world_dpvs_plane read_gfx_world_dpvs_plane(const GfxWorldDpvsPlane& native)
		{
			plain::trace("read_gfx_world_dpvs_plane");
			asset_data::gfxworld::gfx_world_dpvs_plane data;
			data.normal = plain::read_scalars<float, 3>(&native.normal);
			data.dist = native.dist;
			data.type = native.type;
			plain::require_zero(native.pad, "GfxWorldDpvsPlane.pad");
			return data;
		}

		void write_gfx_world_dpvs_plane(GfxWorldDpvsPlane& native, const asset_data::gfxworld::gfx_world_dpvs_plane& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.normal, data.normal);
			native.dist = data.dist;
			native.type = data.type;
		}

		std::size_t size_gfx_world_dpvs_plane([[maybe_unused]] const asset_data::gfxworld::gfx_world_dpvs_plane& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_cell read_gfx_cell(const GfxCell& native)
		{
			plain::trace("read_gfx_cell");
			asset_data::gfxworld::gfx_cell data;
			data.bounds = read_bounds(native.bounds);
			data.portal_count = native.portalCount;
			data.portals = plain::read_array_if_present(native.portals, native.portalCount, "GfxCell.portals", [&](const auto& item) { return read_gfx_portal(item); });
			return data;
		}

		void write_gfx_cell(GfxCell& native, const asset_data::gfxworld::gfx_cell& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.portalCount = data.portal_count;
			native.portals = plain::write_array<GfxPortal>(data.portals, memory, [&](auto& out, const auto& in) { write_gfx_portal(out, in, resolve, memory); });
		}

		std::size_t size_gfx_cell([[maybe_unused]] const asset_data::gfxworld::gfx_cell& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			if (data.portals)
			{
				size += plain::sized(std::max<std::size_t>(data.portals->size(), 1) * sizeof(GfxPortal));
				for (const auto& item : *data.portals)
				{
					size += size_gfx_portal(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::gfx_portal_writable read_gfx_portal_writable(const GfxPortalWritable& native)
		{
			plain::trace("read_gfx_portal_writable");
			asset_data::gfxworld::gfx_portal_writable data;
			data.is_queued = native.isQueued;
			data.is_ancestor = native.isAncestor;
			data.recursion_depth = native.recursionDepth;
			data.hull_point_count = native.hullPointCount;
			plain::require_zero(native.hullPoints, "GfxPortalWritable.hullPoints");
			plain::require_zero(native.queuedParent, "GfxPortalWritable.queuedParent");
			return data;
		}

		void write_gfx_portal_writable(GfxPortalWritable& native, const asset_data::gfxworld::gfx_portal_writable& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.isQueued = data.is_queued;
			native.isAncestor = data.is_ancestor;
			native.recursionDepth = data.recursion_depth;
			native.hullPointCount = data.hull_point_count;
		}

		std::size_t size_gfx_portal_writable([[maybe_unused]] const asset_data::gfxworld::gfx_portal_writable& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::dpvs_plane read_dpvs_plane(const DpvsPlane& native)
		{
			plain::trace("read_dpvs_plane");
			asset_data::gfxworld::dpvs_plane data;
			data.coeffs = plain::read_scalars<float, 4>(&native.coeffs);
			return data;
		}

		void write_dpvs_plane(DpvsPlane& native, const asset_data::gfxworld::dpvs_plane& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.coeffs, data.coeffs);
		}

		std::size_t size_dpvs_plane([[maybe_unused]] const asset_data::gfxworld::dpvs_plane& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_portal read_gfx_portal(const GfxPortal& native)
		{
			plain::trace("read_gfx_portal");
			asset_data::gfxworld::gfx_portal data;
			data.writable = read_gfx_portal_writable(native.writable);
			data.plane = read_dpvs_plane(native.plane);
			data.vertices = plain::read_array_if_present(native.vertices, native.vertexCount, "GfxPortal.vertices", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			data.cell_index = native.cellIndex;
			data.close_distance = native.closeDistance;
			data.vertex_count = native.vertexCount;
			data.hull_axis = [&] { std::array<std::array<float, 3>, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.hullAxis[i]); } return out; }();
			return data;
		}

		void write_gfx_portal(GfxPortal& native, const asset_data::gfxworld::gfx_portal& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_portal_writable(native.writable, data.writable, resolve, memory);
			write_dpvs_plane(native.plane, data.plane, resolve, memory);
			native.vertices = plain::write_array<vec3_t>(data.vertices, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			native.cellIndex = data.cell_index;
			native.closeDistance = data.close_distance;
			native.vertexCount = data.vertex_count;
			for (std::size_t i = 0; i < data.hull_axis.size(); ++i)
			{
				plain::write_scalars(&native.hullAxis[i], data.hull_axis[i]);
			}
		}

		std::size_t size_gfx_portal([[maybe_unused]] const asset_data::gfxworld::gfx_portal& data)
		{
			std::size_t size = 0;
			size += size_gfx_portal_writable(data.writable);
			size += size_dpvs_plane(data.plane);
			if (data.vertices)
			{
				size += plain::sized(std::max<std::size_t>(data.vertices->size(), 1) * sizeof(vec3_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_cell_transient_info read_gfx_cell_transient_info(const GfxCellTransientInfo& native)
		{
			plain::trace("read_gfx_cell_transient_info");
			asset_data::gfxworld::gfx_cell_transient_info data;
			data.aabb_tree_index = native.aabbTreeIndex;
			data.transient_zone = native.transientZone;
			return data;
		}

		void write_gfx_cell_transient_info(GfxCellTransientInfo& native, const asset_data::gfxworld::gfx_cell_transient_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aabbTreeIndex = data.aabb_tree_index;
			native.transientZone = data.transient_zone;
		}

		std::size_t size_gfx_cell_transient_info([[maybe_unused]] const asset_data::gfxworld::gfx_cell_transient_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::srf_triangles_t read_srf_triangles_t(const srfTriangles_t& native)
		{
			plain::trace("read_srf_triangles_t");
			asset_data::gfxworld::srf_triangles_t data;
			data.pos_offset = native.posOffset;
			data.displacement_parms_offset = native.displacementParmsOffset;
			data.vertex_count = native.vertexCount;
			data.tri_count = native.triCount;
			data.base_index = native.baseIndex;
			return data;
		}

		void write_srf_triangles_t(srfTriangles_t& native, const asset_data::gfxworld::srf_triangles_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.posOffset = data.pos_offset;
			native.displacementParmsOffset = data.displacement_parms_offset;
			native.vertexCount = data.vertex_count;
			native.triCount = data.tri_count;
			native.baseIndex = data.base_index;
		}

		std::size_t size_srf_triangles_t([[maybe_unused]] const asset_data::gfxworld::srf_triangles_t& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_surface read_gfx_surface(const GfxSurface& native)
		{
			plain::trace("read_gfx_surface");
			asset_data::gfxworld::gfx_surface data;
			data.tris = read_srf_triangles_t(native.tris);
			data.material = plain::read_asset(ASSET_TYPE_MATERIAL, native.material);
			data.surf_data_index = native.surfDataIndex;
			data.transient_zone = native.transientZone;
			data.lightmap_index = native.lightmapIndex;
			data.flags = plain::flag_names(asset_data::gfxworld::gfx_surface_flags_names, static_cast<std::uint64_t>(native.flags), "GfxSurface.flags");
			return data;
		}

		void write_gfx_surface(GfxSurface& native, const asset_data::gfxworld::gfx_surface& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_srf_triangles_t(native.tris, data.tris, resolve, memory);
			native.material = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.material);
			native.surfDataIndex = data.surf_data_index;
			native.transientZone = data.transient_zone;
			native.lightmapIndex = data.lightmap_index;
			native.flags = static_cast<GfxSurfaceFlags>(plain::flag_value(asset_data::gfxworld::gfx_surface_flags_names, data.flags, "GfxSurface.flags"));
		}

		std::size_t size_gfx_surface([[maybe_unused]] const asset_data::gfxworld::gfx_surface& data)
		{
			std::size_t size = 0;
			size += size_srf_triangles_t(data.tris);
			return size;
		}

		asset_data::gfxworld::replay_surface_bounds_extra read_replay_surface_bounds_extra(const Replay_SurfaceBoundsExtra& native)
		{
			plain::trace("read_replay_surface_bounds_extra");
			asset_data::gfxworld::replay_surface_bounds_extra data;
			data.scalar = native.scalar;
			data.reserved = std::to_array(native.reserved);
			return data;
		}

		void write_replay_surface_bounds_extra(Replay_SurfaceBoundsExtra& native, const asset_data::gfxworld::replay_surface_bounds_extra& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.scalar = data.scalar;
			std::ranges::copy(data.reserved, native.reserved);
		}

		std::size_t size_replay_surface_bounds_extra([[maybe_unused]] const asset_data::gfxworld::replay_surface_bounds_extra& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_surface_bounds read_gfx_surface_bounds(const GfxSurfaceBounds& native)
		{
			plain::trace("read_gfx_surface_bounds");
			asset_data::gfxworld::gfx_surface_bounds data;
			data.bounds = read_bounds(native.bounds);
			data.serialized_extra = read_replay_surface_bounds_extra(native.serializedExtra);
			return data;
		}

		void write_gfx_surface_bounds(GfxSurfaceBounds& native, const asset_data::gfxworld::gfx_surface_bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			write_replay_surface_bounds_extra(native.serializedExtra, data.serialized_extra, resolve, memory);
		}

		std::size_t size_gfx_surface_bounds([[maybe_unused]] const asset_data::gfxworld::gfx_surface_bounds& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			size += size_replay_surface_bounds_extra(data.serialized_extra);
			return size;
		}

		asset_data::gfxworld::gfx_world_surf_data read_gfx_world_surf_data(const GfxWorldSurfData& native)
		{
			plain::trace("read_gfx_world_surf_data");
			asset_data::gfxworld::gfx_world_surf_data data;
			data.transient_zone = native.transientZone;
			data.layer_count = native.layerCount;
			data.xyz_offset = native.xyzOffset;
			data.tangent_frame_offset = native.tangentFrameOffset;
			data.lmap_coord_offset = native.lmapCoordOffset;
			data.color_offset = native.colorOffset;
			data.tex_coord_offset = native.texCoordOffset;
			data.normal_transform_offset = std::to_array(native.normalTransformOffset);
			data.displacement_offset = std::to_array(native.displacementOffset);
			return data;
		}

		void write_gfx_world_surf_data(GfxWorldSurfData& native, const asset_data::gfxworld::gfx_world_surf_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.transientZone = data.transient_zone;
			native.layerCount = data.layer_count;
			native.xyzOffset = data.xyz_offset;
			native.tangentFrameOffset = data.tangent_frame_offset;
			native.lmapCoordOffset = data.lmap_coord_offset;
			native.colorOffset = data.color_offset;
			native.texCoordOffset = data.tex_coord_offset;
			std::ranges::copy(data.normal_transform_offset, native.normalTransformOffset);
			std::ranges::copy(data.displacement_offset, native.displacementOffset);
		}

		std::size_t size_gfx_world_surf_data([[maybe_unused]] const asset_data::gfxworld::gfx_world_surf_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_model_surface read_gfx_static_model_surface(const GfxStaticModelSurface& native)
		{
			plain::trace("read_gfx_static_model_surface");
			asset_data::gfxworld::gfx_static_model_surface data;
			data.smodel_index = native.smodelIndex;
			data.lod_index = native.lodIndex;
			data.surf_index = native.surfIndex;
			return data;
		}

		void write_gfx_static_model_surface(GfxStaticModelSurface& native, const asset_data::gfxworld::gfx_static_model_surface& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.smodelIndex = data.smodel_index;
			native.lodIndex = data.lod_index;
			native.surfIndex = data.surf_index;
		}

		std::size_t size_gfx_static_model_surface([[maybe_unused]] const asset_data::gfxworld::gfx_static_model_surface& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_model read_gfx_static_model(const GfxStaticModel& native)
		{
			plain::trace("read_gfx_static_model");
			asset_data::gfxworld::gfx_static_model data;
			data.model = plain::read_asset(ASSET_TYPE_XMODEL, native.model);
			data.flags = plain::flag_names(asset_data::gfxworld::static_model_flags_names, static_cast<std::uint64_t>(native.flags), "GfxStaticModel.flags");
			data.first_mtl_skin_index = native.firstMtlSkinIndex;
			data.first_static_model_surface_index = native.firstStaticModelSurfaceIndex;
			return data;
		}

		void write_gfx_static_model(GfxStaticModel& native, const asset_data::gfxworld::gfx_static_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.model);
			native.flags = static_cast<StaticModelFlags>(plain::flag_value(asset_data::gfxworld::static_model_flags_names, data.flags, "GfxStaticModel.flags"));
			native.firstMtlSkinIndex = data.first_mtl_skin_index;
			native.firstStaticModelSurfaceIndex = data.first_static_model_surface_index;
		}

		std::size_t size_gfx_static_model([[maybe_unused]] const asset_data::gfxworld::gfx_static_model& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_clutter_collection read_gfx_clutter_collection(const GfxClutterCollection& native)
		{
			plain::trace("read_gfx_clutter_collection");
			asset_data::gfxworld::gfx_clutter_collection data;
			data.max_instance_count = native.maxInstanceCount;
			data.smodel_collection_index = native.smodelCollectionIndex;
			data.terrain_surface_index = native.terrainSurfaceIndex;
			data.clutter_layer_index = native.clutterLayerIndex;
			plain::require_zero(native.pad, "GfxClutterCollection.pad");
			return data;
		}

		void write_gfx_clutter_collection(GfxClutterCollection& native, const asset_data::gfxworld::gfx_clutter_collection& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.maxInstanceCount = data.max_instance_count;
			native.smodelCollectionIndex = data.smodel_collection_index;
			native.terrainSurfaceIndex = data.terrain_surface_index;
			native.clutterLayerIndex = data.clutter_layer_index;
		}

		std::size_t size_gfx_clutter_collection([[maybe_unused]] const asset_data::gfxworld::gfx_clutter_collection& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_model_collection read_gfx_static_model_collection(const GfxStaticModelCollection& native)
		{
			plain::trace("read_gfx_static_model_collection");
			asset_data::gfxworld::gfx_static_model_collection data;
			data.first_instance = native.firstInstance;
			data.instance_count = native.instanceCount;
			data.smodel_index = native.smodelIndex;
			data.transient_gfx_world_placed = native.transientGfxWorldPlaced;
			data.clutter_index = native.clutterIndex;
			data.flags = plain::flag_names(asset_data::gfxworld::static_model_collection_flags_names, static_cast<std::uint64_t>(native.flags), "GfxStaticModelCollection.flags");
			plain::require_zero(native.pad, "GfxStaticModelCollection.pad");
			return data;
		}

		void write_gfx_static_model_collection(GfxStaticModelCollection& native, const asset_data::gfxworld::gfx_static_model_collection& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstInstance = data.first_instance;
			native.instanceCount = data.instance_count;
			native.smodelIndex = data.smodel_index;
			native.transientGfxWorldPlaced = data.transient_gfx_world_placed;
			native.clutterIndex = data.clutter_index;
			native.flags = static_cast<StaticModelCollectionFlags>(plain::flag_value(asset_data::gfxworld::static_model_collection_flags_names, data.flags, "GfxStaticModelCollection.flags"));
		}

		std::size_t size_gfx_static_model_collection([[maybe_unused]] const asset_data::gfxworld::gfx_static_model_collection& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_model_instance read_gfx_static_model_instance(const GfxStaticModelInstance& native)
		{
			plain::trace("read_gfx_static_model_instance");
			asset_data::gfxworld::gfx_static_model_instance data;
			data.lgv_index_transient_index = native.lgvIndex_transientIndex;
			return data;
		}

		void write_gfx_static_model_instance(GfxStaticModelInstance& native, const asset_data::gfxworld::gfx_static_model_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lgvIndex_transientIndex = data.lgv_index_transient_index;
		}

		std::size_t size_gfx_static_model_instance([[maybe_unused]] const asset_data::gfxworld::gfx_static_model_instance& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_model_static_indirection read_gfx_model_static_indirection(const GfxModelStaticIndirection& native)
		{
			plain::trace("read_gfx_model_static_indirection");
			asset_data::gfxworld::gfx_model_static_indirection data;
			data.lighting_instance_indirection = native.lightingInstanceIndirection;
			data.splined_model_instance_index = native.splinedModelInstanceIndex;
			data.world_transient = native.worldTransient;
			return data;
		}

		void write_gfx_model_static_indirection(GfxModelStaticIndirection& native, const asset_data::gfxworld::gfx_model_static_indirection& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lightingInstanceIndirection = data.lighting_instance_indirection;
			native.splinedModelInstanceIndex = data.splined_model_instance_index;
			native.worldTransient = data.world_transient;
		}

		std::size_t size_gfx_model_static_indirection([[maybe_unused]] const asset_data::gfxworld::gfx_model_static_indirection& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_lightmap_transform_packed read_gfx_lightmap_transform_packed(const GfxLightmapTransformPacked& native)
		{
			plain::trace("read_gfx_lightmap_transform_packed");
			asset_data::gfxworld::gfx_lightmap_transform_packed data;
			data.top_left_index_low_packed = native.topLeft_indexLow_packed;
			data.bottom_right_index_high_packed = native.bottomRight_indexHigh_packed;
			return data;
		}

		void write_gfx_lightmap_transform_packed(GfxLightmapTransformPacked& native, const asset_data::gfxworld::gfx_lightmap_transform_packed& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.topLeft_indexLow_packed = data.top_left_index_low_packed;
			native.bottomRight_indexHigh_packed = data.bottom_right_index_high_packed;
		}

		std::size_t size_gfx_lightmap_transform_packed([[maybe_unused]] const asset_data::gfxworld::gfx_lightmap_transform_packed& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_base_surf_data read_gfx_base_surf_data(const GfxBaseSurfData& native)
		{
			plain::trace("read_gfx_base_surf_data");
			asset_data::gfxworld::gfx_base_surf_data data;
			data.bounding_box_data = plain::read_scalars<float, 4>(&native.boundingBoxData);
			return data;
		}

		void write_gfx_base_surf_data(GfxBaseSurfData& native, const asset_data::gfxworld::gfx_base_surf_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.boundingBoxData, data.bounding_box_data);
		}

		std::size_t size_gfx_base_surf_data([[maybe_unused]] const asset_data::gfxworld::gfx_base_surf_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_base_s_model_surf_data read_gfx_base_s_model_surf_data(const GfxBaseSModelSurfData& native)
		{
			plain::trace("read_gfx_base_s_model_surf_data");
			asset_data::gfxworld::gfx_base_s_model_surf_data data;
			data.unnamed = read_gfx_base_surf_data(native.unnamed);
			data.bounding_box_half_size = plain::read_scalars<float, 3>(&native.boundingBoxHalfSize);
			data.draw_distance = native.drawDistance;
			return data;
		}

		void write_gfx_base_s_model_surf_data(GfxBaseSModelSurfData& native, const asset_data::gfxworld::gfx_base_s_model_surf_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_base_surf_data(native.unnamed, data.unnamed, resolve, memory);
			plain::write_scalars(&native.boundingBoxHalfSize, data.bounding_box_half_size);
			native.drawDistance = data.draw_distance;
		}

		std::size_t size_gfx_base_s_model_surf_data([[maybe_unused]] const asset_data::gfxworld::gfx_base_s_model_surf_data& data)
		{
			std::size_t size = 0;
			size += size_gfx_base_surf_data(data.unnamed);
			return size;
		}

		asset_data::gfxworld::gfx_s_model_instance_data read_gfx_s_model_instance_data(const GfxSModelInstanceData& native)
		{
			plain::trace("read_gfx_s_model_instance_data");
			asset_data::gfxworld::gfx_s_model_instance_data data;
			data.translation = std::to_array(native.translation);
			data.orientation = plain::read_scalars<std::uint32_t, 2>(&native.orientation);
			data.scale = native.scale;
			return data;
		}

		void write_gfx_s_model_instance_data(GfxSModelInstanceData& native, const asset_data::gfxworld::gfx_s_model_instance_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.translation, native.translation);
			plain::write_scalars(&native.orientation, data.orientation);
			native.scale = data.scale;
		}

		std::size_t size_gfx_s_model_instance_data([[maybe_unused]] const asset_data::gfxworld::gfx_s_model_instance_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_clutter_collection_gpu_data read_gfx_clutter_collection_gpu_data(const GfxClutterCollectionGPUData& native)
		{
			plain::trace("read_gfx_clutter_collection_gpu_data");
			asset_data::gfxworld::gfx_clutter_collection_gpu_data data;
			data.first_instance = native.firstInstance;
			data.instance_count = native.instanceCount;
			return data;
		}

		void write_gfx_clutter_collection_gpu_data(GfxClutterCollectionGPUData& native, const asset_data::gfxworld::gfx_clutter_collection_gpu_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstInstance = data.first_instance;
			native.instanceCount = data.instance_count;
		}

		std::size_t size_gfx_clutter_collection_gpu_data([[maybe_unused]] const asset_data::gfxworld::gfx_clutter_collection_gpu_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_model_lod read_gfx_static_model_lod(const GfxStaticModelLOD& native)
		{
			plain::trace("read_gfx_static_model_lod");
			asset_data::gfxworld::gfx_static_model_lod data;
			data.surf_offset = native.surfOffset;
			data.surf_count = native.surfCount;
			return data;
		}

		void write_gfx_static_model_lod(GfxStaticModelLOD& native, const asset_data::gfxworld::gfx_static_model_lod& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.surfOffset = data.surf_offset;
			native.surfCount = data.surf_count;
		}

		std::size_t size_gfx_static_model_lod([[maybe_unused]] const asset_data::gfxworld::gfx_static_model_lod& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_model_expansion read_gfx_static_model_expansion(const GfxStaticModelExpansion& native)
		{
			plain::trace("read_gfx_static_model_expansion");
			asset_data::gfxworld::gfx_static_model_expansion data;
			data.first_static_model_surface_index = native.firstStaticModelSurfaceIndex;
			data.num_surfs = native.numSurfs;
			data.num_lods = native.numLods;
			data.model_bounds = read_bounds(native.modelBounds);
			data.lod_info = [&] { std::array<asset_data::gfxworld::gfx_static_model_lod, 6> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_gfx_static_model_lod(native.lodInfo[i]); } return out; }();
			data.lod_dist = std::to_array(native.lodDist);
			return data;
		}

		void write_gfx_static_model_expansion(GfxStaticModelExpansion& native, const asset_data::gfxworld::gfx_static_model_expansion& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstStaticModelSurfaceIndex = data.first_static_model_surface_index;
			native.numSurfs = data.num_surfs;
			native.numLods = data.num_lods;
			write_bounds(native.modelBounds, data.model_bounds, resolve, memory);
			for (std::size_t i = 0; i < data.lod_info.size(); ++i)
			{
				write_gfx_static_model_lod(native.lodInfo[i], data.lod_info[i], resolve, memory);
			}
			std::ranges::copy(data.lod_dist, native.lodDist);
		}

		std::size_t size_gfx_static_model_expansion([[maybe_unused]] const asset_data::gfxworld::gfx_static_model_expansion& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.model_bounds);
			for (const auto& item : data.lod_info)
			{
				size += size_gfx_static_model_lod(item);
			}
			return size;
		}

		asset_data::gfxworld::gfx_s_model_surf_mat_indirection read_gfx_s_model_surf_mat_indirection(const GfxSModelSurfMatIndirection& native)
		{
			plain::trace("read_gfx_s_model_surf_mat_indirection");
			asset_data::gfxworld::gfx_s_model_surf_mat_indirection data;
			data.smodel_material_index = native.smodelMaterialIndex;
			data.cull_type = native.cullType;
			data.emit_type = native.emitType;
			return data;
		}

		void write_gfx_s_model_surf_mat_indirection(GfxSModelSurfMatIndirection& native, const asset_data::gfxworld::gfx_s_model_surf_mat_indirection& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.smodelMaterialIndex = data.smodel_material_index;
			native.cullType = data.cull_type;
			native.emitType = data.emit_type;
		}

		std::size_t size_gfx_s_model_surf_mat_indirection([[maybe_unused]] const asset_data::gfxworld::gfx_s_model_surf_mat_indirection& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::hlsl_gpu_splined_model_instance read_hlsl_gpu_splined_model_instance(const HLSL_GpuSplinedModelInstance& native)
		{
			plain::trace("read_hlsl_gpu_splined_model_instance");
			asset_data::gfxworld::hlsl_gpu_splined_model_instance data;
			data.model_x_extent = native.modelXExtent;
			data.instance_length = native.instanceLength;
			data.model_spline_origin = plain::read_scalars<float, 3>(&native.modelSplineOrigin);
			data.dist_from_start_node = native.distFromStartNode;
			data.model_scale = native.modelScale;
			data.up_down_offset = native.upDownOffset;
			data.left_right_offset = native.leftRightOffset;
			data.segment_begin = native.segmentBegin;
			data.segment_end = native.segmentEnd;
			data.splining_axis_type = native.spliningAxisType;
			return data;
		}

		void write_hlsl_gpu_splined_model_instance(HLSL_GpuSplinedModelInstance& native, const asset_data::gfxworld::hlsl_gpu_splined_model_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.modelXExtent = data.model_x_extent;
			native.instanceLength = data.instance_length;
			plain::write_scalars(&native.modelSplineOrigin, data.model_spline_origin);
			native.distFromStartNode = data.dist_from_start_node;
			native.modelScale = data.model_scale;
			native.upDownOffset = data.up_down_offset;
			native.leftRightOffset = data.left_right_offset;
			native.segmentBegin = data.segment_begin;
			native.segmentEnd = data.segment_end;
			native.spliningAxisType = data.splining_axis_type;
		}

		std::size_t size_hlsl_gpu_splined_model_instance([[maybe_unused]] const asset_data::gfxworld::hlsl_gpu_splined_model_instance& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t read_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(const HLSL_hlsl_3x3_t_float_HLSL_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& native)
		{
			plain::trace("read_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t");
			asset_data::gfxworld::hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t data;
			data.m = [&] { std::array<std::array<float, 3>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.m[i]); } return out; }();
			return data;
		}

		void write_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(HLSL_hlsl_3x3_t_float_HLSL_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& native, const asset_data::gfxworld::hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.m.size(); ++i)
			{
				plain::write_scalars(&native.m[i], data.m[i]);
			}
		}

		std::size_t size_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t([[maybe_unused]] const asset_data::gfxworld::hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::hlsl_gpu_spline_segment read_hlsl_gpu_spline_segment(const HLSL_GpuSplineSegment& native)
		{
			plain::trace("read_hlsl_gpu_spline_segment");
			asset_data::gfxworld::hlsl_gpu_spline_segment data;
			data.dist_from_start_node = native.distFromStartNode;
			data.length = native.length;
			data.bezier_eval = [&] { std::array<std::array<float, 3>, 4> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.bezierEval[i]); } return out; }();
			data.ent_bezier_der_eval = [&] { std::array<std::array<float, 3>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 3>(&native.entBezierDerEval[i]); } return out; }();
			data.ent_to_aligned = read_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(native.entToAligned);
			data.model_to_wld = read_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(native.modelToWld);
			data.model_to_wld_origin = plain::read_scalars<float, 3>(&native.modelToWld_origin);
			data.bank_angle_bezier_eval = plain::read_scalars<float, 4>(&native.bankAngleBezierEval);
			return data;
		}

		void write_hlsl_gpu_spline_segment(HLSL_GpuSplineSegment& native, const asset_data::gfxworld::hlsl_gpu_spline_segment& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.distFromStartNode = data.dist_from_start_node;
			native.length = data.length;
			for (std::size_t i = 0; i < data.bezier_eval.size(); ++i)
			{
				plain::write_scalars(&native.bezierEval[i], data.bezier_eval[i]);
			}
			for (std::size_t i = 0; i < data.ent_bezier_der_eval.size(); ++i)
			{
				plain::write_scalars(&native.entBezierDerEval[i], data.ent_bezier_der_eval[i]);
			}
			write_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(native.entToAligned, data.ent_to_aligned, resolve, memory);
			write_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(native.modelToWld, data.model_to_wld, resolve, memory);
			plain::write_scalars(&native.modelToWld_origin, data.model_to_wld_origin);
			plain::write_scalars(&native.bankAngleBezierEval, data.bank_angle_bezier_eval);
		}

		std::size_t size_hlsl_gpu_spline_segment([[maybe_unused]] const asset_data::gfxworld::hlsl_gpu_spline_segment& data)
		{
			std::size_t size = 0;
			size += size_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(data.ent_to_aligned);
			size += size_hlsl_hlsl_3x3_t_float_hlsl_hlsl_3_t_float_vec2_t_vec3_t_tmat33_t_vec3_t(data.model_to_wld);
			return size;
		}

		asset_data::gfxworld::gfx_reflection_probe read_gfx_reflection_probe(const GfxReflectionProbe& native)
		{
			plain::trace("read_gfx_reflection_probe");
			asset_data::gfxworld::gfx_reflection_probe data;
			data.live_path = plain::read_string(native.livePath);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.angles = plain::read_scalars<float, 3>(&native.angles);
			data.probe_instances = plain::read_array_if_present(native.probeInstances, native.probeInstanceCount, "GfxReflectionProbe.probeInstances", [&](const auto& item) { return item; });
			data.probe_instance_count = native.probeInstanceCount;
			data.probe_relighting_index = native.probeRelightingIndex;
			data.probe_image_indirection = native.probeImageIndirection;
			return data;
		}

		void write_gfx_reflection_probe(GfxReflectionProbe& native, const asset_data::gfxworld::gfx_reflection_probe& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.livePath = plain::write_string(memory, data.live_path);
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.angles, data.angles);
			native.probeInstances = plain::write_array<std::uint16_t>(data.probe_instances, memory, [&](auto& out, const auto& in) { out = in; });
			native.probeInstanceCount = data.probe_instance_count;
			native.probeRelightingIndex = data.probe_relighting_index;
			native.probeImageIndirection = data.probe_image_indirection;
		}

		std::size_t size_gfx_reflection_probe([[maybe_unused]] const asset_data::gfxworld::gfx_reflection_probe& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.live_path);
			if (data.probe_instances)
			{
				size += plain::sized(std::max<std::size_t>(data.probe_instances->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_reflection_probe_relighting_data read_gfx_reflection_probe_relighting_data(const GfxReflectionProbeRelightingData& native)
		{
			plain::trace("read_gfx_reflection_probe_relighting_data");
			asset_data::gfxworld::gfx_reflection_probe_relighting_data data;
			data.reflection_probe_index = native.reflectionProbeIndex;
			data.relighting_flags = native.relightingFlags;
			data.zone_count = native.zoneCount;
			data.g_buffer_albedo_image_index = native.gBufferAlbedoImageIndex;
			data.g_buffer_normal_image_index = native.gBufferNormalImageIndex;
			data.g_buffer_secondary_diffuse_image_index = native.gBufferSecondaryDiffuseImageIndex;
			data.relighting_scale = native.relightingScale;
			data.zones = plain::read_array_if_present(native.zones, native.zoneCount, "GfxReflectionProbeRelightingData.zones", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_reflection_probe_relighting_data(GfxReflectionProbeRelightingData& native, const asset_data::gfxworld::gfx_reflection_probe_relighting_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.reflectionProbeIndex = data.reflection_probe_index;
			native.relightingFlags = data.relighting_flags;
			native.zoneCount = data.zone_count;
			native.gBufferAlbedoImageIndex = data.g_buffer_albedo_image_index;
			native.gBufferNormalImageIndex = data.g_buffer_normal_image_index;
			native.gBufferSecondaryDiffuseImageIndex = data.g_buffer_secondary_diffuse_image_index;
			native.relightingScale = data.relighting_scale;
			native.zones = plain::write_array<std::uint16_t>(data.zones, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_reflection_probe_relighting_data([[maybe_unused]] const asset_data::gfxworld::gfx_reflection_probe_relighting_data& data)
		{
			std::size_t size = 0;
			if (data.zones)
			{
				size += plain::sized(std::max<std::size_t>(data.zones->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_reflection_probe_obb read_gfx_reflection_probe_obb(const GfxReflectionProbeObb& native)
		{
			plain::trace("read_gfx_reflection_probe_obb");
			asset_data::gfxworld::gfx_reflection_probe_obb data;
			data.center = plain::read_scalars<float, 3>(&native.center);
			data.x_axis = plain::read_scalars<float, 3>(&native.xAxis);
			data.y_axis = plain::read_scalars<float, 3>(&native.yAxis);
			data.z_axis = plain::read_scalars<float, 3>(&native.zAxis);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_gfx_reflection_probe_obb(GfxReflectionProbeObb& native, const asset_data::gfxworld::gfx_reflection_probe_obb& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.center, data.center);
			plain::write_scalars(&native.xAxis, data.x_axis);
			plain::write_scalars(&native.yAxis, data.y_axis);
			plain::write_scalars(&native.zAxis, data.z_axis);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_gfx_reflection_probe_obb([[maybe_unused]] const asset_data::gfxworld::gfx_reflection_probe_obb& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_reflection_probe_instance read_gfx_reflection_probe_instance(const GfxReflectionProbeInstance& native)
		{
			plain::trace("read_gfx_reflection_probe_instance");
			asset_data::gfxworld::gfx_reflection_probe_instance data;
			data.live_path = plain::read_string(native.livePath);
			data.probe_position = plain::read_scalars<float, 3>(&native.probePosition);
			data.probe_image_index = native.probeImageIndex;
			data.lod_level = native.lodLevel;
			data.flags = native.flags;
			data.probe_rotation = plain::read_scalars<float, 4>(&native.probeRotation);
			data.volume_obb = read_gfx_reflection_probe_obb(native.volumeObb);
			data.priority = native.priority;
			data.feather = plain::read_scalars<float, 3>(&native.feather);
			data.expand_projection_neg = plain::read_scalars<float, 3>(&native.expandProjectionNeg);
			data.expand_projection_pos = plain::read_scalars<float, 3>(&native.expandProjectionPos);
			return data;
		}

		void write_gfx_reflection_probe_instance(GfxReflectionProbeInstance& native, const asset_data::gfxworld::gfx_reflection_probe_instance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.livePath = plain::write_string(memory, data.live_path);
			plain::write_scalars(&native.probePosition, data.probe_position);
			native.probeImageIndex = data.probe_image_index;
			native.lodLevel = data.lod_level;
			native.flags = data.flags;
			plain::write_scalars(&native.probeRotation, data.probe_rotation);
			write_gfx_reflection_probe_obb(native.volumeObb, data.volume_obb, resolve, memory);
			native.priority = data.priority;
			plain::write_scalars(&native.feather, data.feather);
			plain::write_scalars(&native.expandProjectionNeg, data.expand_projection_neg);
			plain::write_scalars(&native.expandProjectionPos, data.expand_projection_pos);
		}

		std::size_t size_gfx_reflection_probe_instance([[maybe_unused]] const asset_data::gfxworld::gfx_reflection_probe_instance& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.live_path);
			size += size_gfx_reflection_probe_obb(data.volume_obb);
			return size;
		}

		asset_data::gfxworld::gfx_sh3_vec read_gfx_sh3_vec(const GfxSH3Vec& native)
		{
			plain::trace("read_gfx_sh3_vec");
			asset_data::gfxworld::gfx_sh3_vec data;
			data.coeffs = std::to_array(native.coeffs);
			return data;
		}

		void write_gfx_sh3_vec(GfxSH3Vec& native, const asset_data::gfxworld::gfx_sh3_vec& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.coeffs, native.coeffs);
		}

		std::size_t size_gfx_sh3_vec([[maybe_unused]] const asset_data::gfxworld::gfx_sh3_vec& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_world_lightmap_reindex_data_element read_gfx_world_lightmap_reindex_data_element(const GfxWorldLightmapReindexDataElement& native)
		{
			plain::trace("read_gfx_world_lightmap_reindex_data_element");
			asset_data::gfxworld::gfx_world_lightmap_reindex_data_element data;
			data.index = native.index;
			data.pos_x = native.posX;
			data.pos_y = native.posY;
			data.width = native.width;
			data.height = native.height;
			return data;
		}

		void write_gfx_world_lightmap_reindex_data_element(GfxWorldLightmapReindexDataElement& native, const asset_data::gfxworld::gfx_world_lightmap_reindex_data_element& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.index = data.index;
			native.posX = data.pos_x;
			native.posY = data.pos_y;
			native.width = data.width;
			native.height = data.height;
		}

		std::size_t size_gfx_world_lightmap_reindex_data_element([[maybe_unused]] const asset_data::gfxworld::gfx_world_lightmap_reindex_data_element& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_world_packed_lightmap read_gfx_world_packed_lightmap(const GfxWorldPackedLightmap& native)
		{
			plain::trace("read_gfx_world_packed_lightmap");
			asset_data::gfxworld::gfx_world_packed_lightmap data;
			data.image_width = native.imageWidth;
			data.image_height = native.imageHeight;
			return data;
		}

		void write_gfx_world_packed_lightmap(GfxWorldPackedLightmap& native, const asset_data::gfxworld::gfx_world_packed_lightmap& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.imageWidth = data.image_width;
			native.imageHeight = data.image_height;
		}

		std::size_t size_gfx_world_packed_lightmap([[maybe_unused]] const asset_data::gfxworld::gfx_world_packed_lightmap& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_decal_volume_material read_gfx_static_decal_volume_material(const GfxStaticDecalVolumeMaterial& native)
		{
			plain::trace("read_gfx_static_decal_volume_material");
			asset_data::gfxworld::gfx_static_decal_volume_material data;
			data.material = plain::read_asset(ASSET_TYPE_MATERIAL, native.material);
			return data;
		}

		void write_gfx_static_decal_volume_material(GfxStaticDecalVolumeMaterial& native, const asset_data::gfxworld::gfx_static_decal_volume_material& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.material = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.material);
		}

		std::size_t size_gfx_static_decal_volume_material([[maybe_unused]] const asset_data::gfxworld::gfx_static_decal_volume_material& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_decal_volume_mask read_gfx_static_decal_volume_mask(const GfxStaticDecalVolumeMask& native)
		{
			plain::trace("read_gfx_static_decal_volume_mask");
			asset_data::gfxworld::gfx_static_decal_volume_mask data;
			data.blend_map_override = plain::read_asset(ASSET_TYPE_DECAL_VOLUME_MASK, native.blendMapOverride);
			return data;
		}

		void write_gfx_static_decal_volume_mask(GfxStaticDecalVolumeMask& native, const asset_data::gfxworld::gfx_static_decal_volume_mask& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.blendMapOverride = plain::resolve_asset<GfxDecalVolumeMask>(resolve, ASSET_TYPE_DECAL_VOLUME_MASK, data.blend_map_override);
		}

		std::size_t size_gfx_static_decal_volume_mask([[maybe_unused]] const asset_data::gfxworld::gfx_static_decal_volume_mask& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_decal_volume_collection read_gfx_decal_volume_collection(const GfxDecalVolumeCollection& native)
		{
			plain::trace("read_gfx_decal_volume_collection");
			asset_data::gfxworld::gfx_decal_volume_collection data;
			data.transient_zone_index = native.transientZoneIndex;
			data.decal_count = native.decalCount;
			data.first_zone_decal = native.firstZoneDecal;
			return data;
		}

		void write_gfx_decal_volume_collection(GfxDecalVolumeCollection& native, const asset_data::gfxworld::gfx_decal_volume_collection& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.transientZoneIndex = data.transient_zone_index;
			native.decalCount = data.decal_count;
			native.firstZoneDecal = data.first_zone_decal;
		}

		std::size_t size_gfx_decal_volume_collection([[maybe_unused]] const asset_data::gfxworld::gfx_decal_volume_collection& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_oriented_bounding_box read_gfx_oriented_bounding_box(const GfxOrientedBoundingBox& native)
		{
			plain::trace("read_gfx_oriented_bounding_box");
			asset_data::gfxworld::gfx_oriented_bounding_box data;
			data.center = plain::read_scalars<float, 3>(&native.center);
			data.x_axis = plain::read_scalars<float, 3>(&native.xAxis);
			data.y_axis = plain::read_scalars<float, 3>(&native.yAxis);
			data.z_axis = plain::read_scalars<float, 3>(&native.zAxis);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_gfx_oriented_bounding_box(GfxOrientedBoundingBox& native, const asset_data::gfxworld::gfx_oriented_bounding_box& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.center, data.center);
			plain::write_scalars(&native.xAxis, data.x_axis);
			plain::write_scalars(&native.yAxis, data.y_axis);
			plain::write_scalars(&native.zAxis, data.z_axis);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_gfx_oriented_bounding_box([[maybe_unused]] const asset_data::gfxworld::gfx_oriented_bounding_box& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_volumetric_mask read_gfx_volumetric_mask(const GfxVolumetricMask& native)
		{
			plain::trace("read_gfx_volumetric_mask");
			asset_data::gfxworld::gfx_volumetric_mask data;
			data.type = plain::enum_name(asset_data::gfxworld::volumetric_mask_type_names, static_cast<std::int64_t>(native.type), "GfxVolumetricMask.type");
			data.axis = plain::enum_name(asset_data::gfxworld::volumetric_axis_names, static_cast<std::int64_t>(native.axis), "GfxVolumetricMask.axis");
			data.image = plain::read_asset(ASSET_TYPE_IMAGE, native.image);
			data.scale = plain::read_scalars<float, 2>(&native.scale);
			data.offset = plain::read_scalars<float, 2>(&native.offset);
			data.scroll = plain::read_scalars<float, 2>(&native.scroll);
			return data;
		}

		void write_gfx_volumetric_mask(GfxVolumetricMask& native, const asset_data::gfxworld::gfx_volumetric_mask& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.type = static_cast<VolumetricMaskType>(plain::enum_value(asset_data::gfxworld::volumetric_mask_type_names, data.type, "GfxVolumetricMask.type"));
			native.axis = static_cast<VolumetricAxis>(plain::enum_value(asset_data::gfxworld::volumetric_axis_names, data.axis, "GfxVolumetricMask.axis"));
			native.image = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.image);
			plain::write_scalars(&native.scale, data.scale);
			plain::write_scalars(&native.offset, data.offset);
			plain::write_scalars(&native.scroll, data.scroll);
		}

		std::size_t size_gfx_volumetric_mask([[maybe_unused]] const asset_data::gfxworld::gfx_volumetric_mask& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_volumetric read_gfx_volumetric(const GfxVolumetric& native)
		{
			plain::trace("read_gfx_volumetric");
			asset_data::gfxworld::gfx_volumetric data;
			data.live_path = plain::read_string(native.livePath);
			data.flags = native.flags;
			data.obb = read_gfx_oriented_bounding_box(native.obb);
			data.color = plain::read_scalars<float, 3>(&native.color);
			data.density = native.density;
			data.falloff = native.falloff;
			data.height_fade = native.heightFade;
			data.anisotropy = native.anisotropy;
			data.masks = [&] { std::array<asset_data::gfxworld::gfx_volumetric_mask, 4> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_gfx_volumetric_mask(native.masks[i]); } return out; }();
			return data;
		}

		void write_gfx_volumetric(GfxVolumetric& native, const asset_data::gfxworld::gfx_volumetric& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.livePath = plain::write_string(memory, data.live_path);
			native.flags = data.flags;
			write_gfx_oriented_bounding_box(native.obb, data.obb, resolve, memory);
			plain::write_scalars(&native.color, data.color);
			native.density = data.density;
			native.falloff = data.falloff;
			native.heightFade = data.height_fade;
			native.anisotropy = data.anisotropy;
			for (std::size_t i = 0; i < data.masks.size(); ++i)
			{
				write_gfx_volumetric_mask(native.masks[i], data.masks[i], resolve, memory);
			}
		}

		std::size_t size_gfx_volumetric([[maybe_unused]] const asset_data::gfxworld::gfx_volumetric& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.live_path);
			size += size_gfx_oriented_bounding_box(data.obb);
			for (const auto& item : data.masks)
			{
				size += size_gfx_volumetric_mask(item);
			}
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_bf_data read_gfx_dynamic_lightset_bf_data(const GfxDynamicLightsetBFData& native)
		{
			plain::trace("read_gfx_dynamic_lightset_bf_data");
			asset_data::gfxworld::gfx_dynamic_lightset_bf_data data;
			data.cur_value = std::to_array(native.curValue);
			data.last_value = std::to_array(native.lastValue);
			data.bf_type = plain::enum_name(asset_data::gfxworld::gfx_dynamic_lightset_bf_type_names, static_cast<std::int64_t>(native.bfType), "GfxDynamicLightsetBFData.bfType");
			data.num_rec = native.numRec;
			data.start_rec = native.startRec;
			return data;
		}

		void write_gfx_dynamic_lightset_bf_data(GfxDynamicLightsetBFData& native, const asset_data::gfxworld::gfx_dynamic_lightset_bf_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.cur_value, native.curValue);
			std::ranges::copy(data.last_value, native.lastValue);
			native.bfType = static_cast<GfxDynamicLightsetBFType>(plain::enum_value(asset_data::gfxworld::gfx_dynamic_lightset_bf_type_names, data.bf_type, "GfxDynamicLightsetBFData.bfType"));
			native.numRec = data.num_rec;
			native.startRec = data.start_rec;
		}

		std::size_t size_gfx_dynamic_lightset_bf_data([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_bf_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_light_info read_gfx_dynamic_lightset_light_info(const GfxDynamicLightsetLightInfo& native)
		{
			plain::trace("read_gfx_dynamic_lightset_light_info");
			asset_data::gfxworld::gfx_dynamic_lightset_light_info data;
			data.base_intensity = std::to_array(native.baseIntensity);
			data.last_intensity = std::to_array(native.lastIntensity);
			data.which_primary = native.whichPrimary;
			return data;
		}

		void write_gfx_dynamic_lightset_light_info(GfxDynamicLightsetLightInfo& native, const asset_data::gfxworld::gfx_dynamic_lightset_light_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.base_intensity, native.baseIntensity);
			std::ranges::copy(data.last_intensity, native.lastIntensity);
			native.whichPrimary = data.which_primary;
		}

		std::size_t size_gfx_dynamic_lightset_light_info([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_light_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_portal_gi_data read_gfx_dynamic_lightset_portal_gi_data(const GfxDynamicLightsetPortalGIData& native)
		{
			plain::trace("read_gfx_dynamic_lightset_portal_gi_data");
			asset_data::gfxworld::gfx_dynamic_lightset_portal_gi_data data;
			data.which_entity = native.whichEntity;
			data.base_quat = plain::read_scalars<float, 4>(&native.baseQuat);
			data.extra_info = plain::read_scalars<float, 4>(&native.extraInfo);
			return data;
		}

		void write_gfx_dynamic_lightset_portal_gi_data(GfxDynamicLightsetPortalGIData& native, const asset_data::gfxworld::gfx_dynamic_lightset_portal_gi_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.whichEntity = data.which_entity;
			plain::write_scalars(&native.baseQuat, data.base_quat);
			plain::write_scalars(&native.extraInfo, data.extra_info);
		}

		std::size_t size_gfx_dynamic_lightset_portal_gi_data([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_portal_gi_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_product_gi_data read_gfx_dynamic_lightset_product_gi_data(const GfxDynamicLightsetProductGIData& native)
		{
			plain::trace("read_gfx_dynamic_lightset_product_gi_data");
			asset_data::gfxworld::gfx_dynamic_lightset_product_gi_data data;
			data.which_dls = native.whichDLS;
			data.which_portal = native.whichPortal;
			return data;
		}

		void write_gfx_dynamic_lightset_product_gi_data(GfxDynamicLightsetProductGIData& native, const asset_data::gfxworld::gfx_dynamic_lightset_product_gi_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.whichDLS = data.which_dls;
			native.whichPortal = data.which_portal;
		}

		std::size_t size_gfx_dynamic_lightset_product_gi_data([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_product_gi_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_info read_gfx_dynamic_lightset_g_chunk_info(const GfxDynamicLightsetGChunkInfo& native)
		{
			plain::trace("read_gfx_dynamic_lightset_g_chunk_info");
			asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_info data;
			data.num_records = native.numRecords;
			data.bf_start = native.bfStart;
			data.org_start = native.orgStart;
			data.tmp_start = native.tmpStart;
			return data;
		}

		void write_gfx_dynamic_lightset_g_chunk_info(GfxDynamicLightsetGChunkInfo& native, const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numRecords = data.num_records;
			native.bfStart = data.bf_start;
			native.orgStart = data.org_start;
			native.tmpStart = data.tmp_start;
		}

		std::size_t size_gfx_dynamic_lightset_g_chunk_info([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_g_chunk read_gfx_dynamic_lightset_g_chunk(const GfxDynamicLightsetGChunk& native)
		{
			plain::trace("read_gfx_dynamic_lightset_g_chunk");
			asset_data::gfxworld::gfx_dynamic_lightset_g_chunk data;
			data.chunk_info = read_gfx_dynamic_lightset_g_chunk_info(native.chunkInfo);
			data.flags = plain::flag_names(asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_flags_names, static_cast<std::uint64_t>(native.flags), "GfxDynamicLightsetGChunk.flags");
			data.which_lm = native.whichLM;
			data.bf_chunk_start = native.bfChunkStart;
			return data;
		}

		void write_gfx_dynamic_lightset_g_chunk(GfxDynamicLightsetGChunk& native, const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_dynamic_lightset_g_chunk_info(native.chunkInfo, data.chunk_info, resolve, memory);
			native.flags = static_cast<GfxDynamicLightsetGChunkFlags>(plain::flag_value(asset_data::gfxworld::gfx_dynamic_lightset_g_chunk_flags_names, data.flags, "GfxDynamicLightsetGChunk.flags"));
			native.whichLM = data.which_lm;
			native.bfChunkStart = data.bf_chunk_start;
		}

		std::size_t size_gfx_dynamic_lightset_g_chunk([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_g_chunk& data)
		{
			std::size_t size = 0;
			size += size_gfx_dynamic_lightset_g_chunk_info(data.chunk_info);
			return size;
		}

		asset_data::gfxworld::gfx_sh_compressed_probe_data read_gfx_sh_compressed_probe_data(const GfxSHCompressedProbeData& native)
		{
			plain::trace("read_gfx_sh_compressed_probe_data");
			asset_data::gfxworld::gfx_sh_compressed_probe_data data;
			data.data = std::to_array(native.data);
			return data;
		}

		void write_gfx_sh_compressed_probe_data(GfxSHCompressedProbeData& native, const asset_data::gfxworld::gfx_sh_compressed_probe_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.data, native.data);
		}

		std::size_t size_gfx_sh_compressed_probe_data([[maybe_unused]] const asset_data::gfxworld::gfx_sh_compressed_probe_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_light_view_frustum read_gfx_light_view_frustum(const GfxLightViewFrustum& native)
		{
			plain::trace("read_gfx_light_view_frustum");
			asset_data::gfxworld::gfx_light_view_frustum data;
			data.plane_count = native.planeCount;
			data.index_count = native.indexCount;
			data.vertex_count = native.vertexCount;
			data.planes = plain::read_array_if_present(native.planes, native.planeCount, "GfxLightViewFrustum.planes", [&](const auto& item) { return plain::read_scalars<float, 4>(&item); });
			data.indices = plain::read_array_if_present(native.indices, native.indexCount, "GfxLightViewFrustum.indices", [&](const auto& item) { return item; });
			data.vertices = plain::read_array_if_present(native.vertices, native.vertexCount, "GfxLightViewFrustum.vertices", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			return data;
		}

		void write_gfx_light_view_frustum(GfxLightViewFrustum& native, const asset_data::gfxworld::gfx_light_view_frustum& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.planeCount = data.plane_count;
			native.indexCount = data.index_count;
			native.vertexCount = data.vertex_count;
			native.planes = plain::write_array<vec4_t>(data.planes, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			native.indices = plain::write_array<std::uint8_t>(data.indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.vertices = plain::write_array<vec3_t>(data.vertices, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
		}

		std::size_t size_gfx_light_view_frustum([[maybe_unused]] const asset_data::gfxworld::gfx_light_view_frustum& data)
		{
			std::size_t size = 0;
			if (data.planes)
			{
				size += plain::sized(std::max<std::size_t>(data.planes->size(), 1) * sizeof(vec4_t));
			}
			if (data.indices)
			{
				size += plain::sized(std::max<std::size_t>(data.indices->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.vertices)
			{
				size += plain::sized(std::max<std::size_t>(data.vertices->size(), 1) * sizeof(vec3_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_light read_gfx_light(const GfxLight& native)
		{
			plain::trace("read_gfx_light");
			asset_data::gfxworld::gfx_light data;
			data.type = native.type;
			data.can_use_shadow_map = native.canUseShadowMap;
			data.needs_dynamic_shadows = native.needsDynamicShadows;
			data.is_volumetric = native.isVolumetric;
			data.uv_intensity = native.uvIntensity;
			data.ir_intensity = native.irIntensity;
			data.heat_intensity = native.heatIntensity;
			data.intensity = native.intensity;
			data.color_linear_srgb = plain::read_scalars<float, 3>(&native.colorLinearSrgb);
			data.dir = plain::read_scalars<float, 3>(&native.dir);
			data.up = plain::read_scalars<float, 3>(&native.up);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.radius = native.radius;
			data.fade_offset_rt = plain::read_scalars<float, 2>(&native.fadeOffsetRt);
			data.bulb_radius = native.bulbRadius;
			data.bulb_length = plain::read_scalars<float, 3>(&native.bulbLength);
			data.cos_half_fov_outer = native.cosHalfFovOuter;
			data.cos_half_fov_inner = native.cosHalfFovInner;
			data.entity_id = native.entityId;
			data.flags = native.flags;
			data.tonemapping_scale_factor = native.tonemappingScaleFactor;
			data.shadow_near_plane_bias = native.shadowNearPlaneBias;
			data.shadow_softness = native.shadowSoftness;
			data.shadow_bias = native.shadowBias;
			data.shadow_area = native.shadowArea;
			data.distance_falloff = native.distanceFalloff;
			data.cos_half_fov_collimation = native.cosHalfFovCollimation;
			plain::require_zero(native.def, "GfxLight.def");
			return data;
		}

		void write_gfx_light(GfxLight& native, const asset_data::gfxworld::gfx_light& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.type = data.type;
			native.canUseShadowMap = data.can_use_shadow_map;
			native.needsDynamicShadows = data.needs_dynamic_shadows;
			native.isVolumetric = data.is_volumetric;
			native.uvIntensity = data.uv_intensity;
			native.irIntensity = data.ir_intensity;
			native.heatIntensity = data.heat_intensity;
			native.intensity = data.intensity;
			plain::write_scalars(&native.colorLinearSrgb, data.color_linear_srgb);
			plain::write_scalars(&native.dir, data.dir);
			plain::write_scalars(&native.up, data.up);
			plain::write_scalars(&native.origin, data.origin);
			native.radius = data.radius;
			plain::write_scalars(&native.fadeOffsetRt, data.fade_offset_rt);
			native.bulbRadius = data.bulb_radius;
			plain::write_scalars(&native.bulbLength, data.bulb_length);
			native.cosHalfFovOuter = data.cos_half_fov_outer;
			native.cosHalfFovInner = data.cos_half_fov_inner;
			native.entityId = data.entity_id;
			native.flags = data.flags;
			native.tonemappingScaleFactor = data.tonemapping_scale_factor;
			native.shadowNearPlaneBias = data.shadow_near_plane_bias;
			native.shadowSoftness = data.shadow_softness;
			native.shadowBias = data.shadow_bias;
			native.shadowArea = data.shadow_area;
			native.distanceFalloff = data.distance_falloff;
			native.cosHalfFovCollimation = data.cos_half_fov_collimation;
		}

		std::size_t size_gfx_light([[maybe_unused]] const asset_data::gfxworld::gfx_light& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_voxel_tree_header read_gfx_voxel_tree_header(const GfxVoxelTreeHeader& native)
		{
			plain::trace("read_gfx_voxel_tree_header");
			asset_data::gfxworld::gfx_voxel_tree_header data;
			data.root_node_dimension = std::to_array(native.rootNodeDimension);
			data.node_coord_bit_shift = std::to_array(native.nodeCoordBitShift);
			data.bound_min = plain::read_scalars<float, 4>(&native.boundMin);
			data.bound_max = plain::read_scalars<float, 4>(&native.boundMax);
			return data;
		}

		void write_gfx_voxel_tree_header(GfxVoxelTreeHeader& native, const asset_data::gfxworld::gfx_voxel_tree_header& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.root_node_dimension, native.rootNodeDimension);
			std::ranges::copy(data.node_coord_bit_shift, native.nodeCoordBitShift);
			plain::write_scalars(&native.boundMin, data.bound_min);
			plain::write_scalars(&native.boundMax, data.bound_max);
		}

		std::size_t size_gfx_voxel_tree_header([[maybe_unused]] const asset_data::gfxworld::gfx_voxel_tree_header& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::replay_voxel_runtime_state read_replay_voxel_runtime_state(const Replay_VoxelRuntimeState& native)
		{
			plain::trace("read_replay_voxel_runtime_state");
			asset_data::gfxworld::replay_voxel_runtime_state data;
			plain::require_zero(native.voxelInternalNodeDynamicLightList, "Replay_VoxelRuntimeState.voxelInternalNodeDynamicLightList");
			plain::require_zero(native.voxelTreeHeaderCPU, "Replay_VoxelRuntimeState.voxelTreeHeaderCPU");
			plain::require_zero(native.voxelTopDownViewNodeArrayCPU, "Replay_VoxelRuntimeState.voxelTopDownViewNodeArrayCPU");
			return data;
		}

		void write_replay_voxel_runtime_state(Replay_VoxelRuntimeState& native, const asset_data::gfxworld::replay_voxel_runtime_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
		}

		std::size_t size_replay_voxel_runtime_state([[maybe_unused]] const asset_data::gfxworld::replay_voxel_runtime_state& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_voxel_tree read_gfx_voxel_tree(const GfxVoxelTree& native)
		{
			plain::trace("read_gfx_voxel_tree");
			asset_data::gfxworld::gfx_voxel_tree data;
			data.zone_bound = read_bounds(native.zoneBound);
			data.voxel_top_down_view_node_count = native.voxelTopDownViewNodeCount;
			data.voxel_internal_node_count = native.voxelInternalNodeCount;
			data.voxel_leaf_node_count = native.voxelLeafNodeCount;
			data.light_list_array_size = native.lightListArraySize;
			data.voxel_tree_header = plain::read_single(native.voxelTreeHeader, [&](const auto& item) { return read_gfx_voxel_tree_header(item); });
			data.voxel_top_down_view_node_array = plain::read_array_if_present(native.voxelTopDownViewNodeArray, native.voxelTopDownViewNodeCount, "GfxVoxelTree.voxelTopDownViewNodeArray", [&](const auto& item) { return read_gfx_voxel_top_down_view_node(item); });
			data.voxel_internal_node_array = plain::read_array_if_present(native.voxelInternalNodeArray, native.voxelInternalNodeCount, "GfxVoxelTree.voxelInternalNodeArray", [&](const auto& item) { return read_gfx_voxel_internal_node(item); });
			data.voxel_leaf_node_array = plain::read_array_if_present(native.voxelLeafNodeArray, native.voxelLeafNodeCount, "GfxVoxelTree.voxelLeafNodeArray", [&](const auto& item) { return read_gfx_voxel_leaf_node(item); });
			data.light_list_array = plain::read_array_if_present(native.lightListArray, native.lightListArraySize, "GfxVoxelTree.lightListArray", [&](const auto& item) { return item; });
			data.voxel_internal_node_dynamic_light_list_cpu = plain::read_array_if_present(native.voxelInternalNodeDynamicLightListCPU, (native.voxelInternalNodeCount + native.voxelInternalNodeCount), "GfxVoxelTree.voxelInternalNodeDynamicLightListCPU", [&](const auto& item) { return item; });
			data.runtime_state = read_replay_voxel_runtime_state(native.runtimeState);
			return data;
		}

		void write_gfx_voxel_tree(GfxVoxelTree& native, const asset_data::gfxworld::gfx_voxel_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.zoneBound, data.zone_bound, resolve, memory);
			native.voxelTopDownViewNodeCount = data.voxel_top_down_view_node_count;
			native.voxelInternalNodeCount = data.voxel_internal_node_count;
			native.voxelLeafNodeCount = data.voxel_leaf_node_count;
			native.lightListArraySize = data.light_list_array_size;
			native.voxelTreeHeader = plain::write_single<GfxVoxelTreeHeader>(data.voxel_tree_header, memory, [&](auto& out, const auto& in) { write_gfx_voxel_tree_header(out, in, resolve, memory); });
			native.voxelTopDownViewNodeArray = plain::write_array<GfxVoxelTopDownViewNode>(data.voxel_top_down_view_node_array, memory, [&](auto& out, const auto& in) { write_gfx_voxel_top_down_view_node(out, in, resolve, memory); });
			native.voxelInternalNodeArray = plain::write_array<GfxVoxelInternalNode>(data.voxel_internal_node_array, memory, [&](auto& out, const auto& in) { write_gfx_voxel_internal_node(out, in, resolve, memory); });
			native.voxelLeafNodeArray = plain::write_array<GfxVoxelLeafNode>(data.voxel_leaf_node_array, memory, [&](auto& out, const auto& in) { write_gfx_voxel_leaf_node(out, in, resolve, memory); });
			native.lightListArray = plain::write_array<std::uint16_t>(data.light_list_array, memory, [&](auto& out, const auto& in) { out = in; });
			native.voxelInternalNodeDynamicLightListCPU = plain::write_array<std::uint32_t>(data.voxel_internal_node_dynamic_light_list_cpu, memory, [&](auto& out, const auto& in) { out = in; });
			write_replay_voxel_runtime_state(native.runtimeState, data.runtime_state, resolve, memory);
		}

		std::size_t size_gfx_voxel_tree([[maybe_unused]] const asset_data::gfxworld::gfx_voxel_tree& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.zone_bound);
			if (data.voxel_tree_header)
			{
				size += plain::sized(sizeof(GfxVoxelTreeHeader));
				size += size_gfx_voxel_tree_header(*data.voxel_tree_header);
			}
			if (data.voxel_top_down_view_node_array)
			{
				size += plain::sized(std::max<std::size_t>(data.voxel_top_down_view_node_array->size(), 1) * sizeof(GfxVoxelTopDownViewNode));
				for (const auto& item : *data.voxel_top_down_view_node_array)
				{
					size += size_gfx_voxel_top_down_view_node(item);
				}
			}
			if (data.voxel_internal_node_array)
			{
				size += plain::sized(std::max<std::size_t>(data.voxel_internal_node_array->size(), 1) * sizeof(GfxVoxelInternalNode));
				for (const auto& item : *data.voxel_internal_node_array)
				{
					size += size_gfx_voxel_internal_node(item);
				}
			}
			if (data.voxel_leaf_node_array)
			{
				size += plain::sized(std::max<std::size_t>(data.voxel_leaf_node_array->size(), 1) * sizeof(GfxVoxelLeafNode));
				for (const auto& item : *data.voxel_leaf_node_array)
				{
					size += size_gfx_voxel_leaf_node(item);
				}
			}
			if (data.light_list_array)
			{
				size += plain::sized(std::max<std::size_t>(data.light_list_array->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.voxel_internal_node_dynamic_light_list_cpu)
			{
				size += plain::sized(std::max<std::size_t>(data.voxel_internal_node_dynamic_light_list_cpu->size(), 1) * sizeof(std::uint32_t));
			}
			size += size_replay_voxel_runtime_state(data.runtime_state);
			return size;
		}

		asset_data::gfxworld::gfx_voxel_top_down_view_node read_gfx_voxel_top_down_view_node(const GfxVoxelTopDownViewNode& native)
		{
			plain::trace("read_gfx_voxel_top_down_view_node");
			asset_data::gfxworld::gfx_voxel_top_down_view_node data;
			data.first_node_index = native.firstNodeIndex;
			data.z_min = native.zMin;
			data.z_max = native.zMax;
			return data;
		}

		void write_gfx_voxel_top_down_view_node(GfxVoxelTopDownViewNode& native, const asset_data::gfxworld::gfx_voxel_top_down_view_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstNodeIndex = data.first_node_index;
			native.zMin = data.z_min;
			native.zMax = data.z_max;
		}

		std::size_t size_gfx_voxel_top_down_view_node([[maybe_unused]] const asset_data::gfxworld::gfx_voxel_top_down_view_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_voxel_internal_node read_gfx_voxel_internal_node(const GfxVoxelInternalNode& native)
		{
			plain::trace("read_gfx_voxel_internal_node");
			asset_data::gfxworld::gfx_voxel_internal_node data;
			data.first_node_index = std::to_array(native.firstNodeIndex);
			data.child_node_mask = std::to_array(native.childNodeMask);
			return data;
		}

		void write_gfx_voxel_internal_node(GfxVoxelInternalNode& native, const asset_data::gfxworld::gfx_voxel_internal_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.first_node_index, native.firstNodeIndex);
			std::ranges::copy(data.child_node_mask, native.childNodeMask);
		}

		std::size_t size_gfx_voxel_internal_node([[maybe_unused]] const asset_data::gfxworld::gfx_voxel_internal_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_voxel_leaf_node read_gfx_voxel_leaf_node(const GfxVoxelLeafNode& native)
		{
			plain::trace("read_gfx_voxel_leaf_node");
			asset_data::gfxworld::gfx_voxel_leaf_node data;
			data.light_list_address = native.lightListAddress;
			return data;
		}

		void write_gfx_voxel_leaf_node(GfxVoxelLeafNode& native, const asset_data::gfxworld::gfx_voxel_leaf_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lightListAddress = data.light_list_address;
		}

		std::size_t size_gfx_voxel_leaf_node([[maybe_unused]] const asset_data::gfxworld::gfx_voxel_leaf_node& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_heightfield read_gfx_heightfield(const GfxHeightfield& native)
		{
			plain::trace("read_gfx_heightfield");
			asset_data::gfxworld::gfx_heightfield data;
			data.bounds = read_bounds(native.bounds);
			data.lookup_matrix = plain::read_block<std::array<std::array<float, 4>, 4>>(&native.lookupMatrix);
			data.image = plain::read_asset(ASSET_TYPE_IMAGE, native.image);
			return data;
		}

		void write_gfx_heightfield(GfxHeightfield& native, const asset_data::gfxworld::gfx_heightfield& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			plain::write_block(&native.lookupMatrix, data.lookup_matrix);
			native.image = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.image);
		}

		std::size_t size_gfx_heightfield([[maybe_unused]] const asset_data::gfxworld::gfx_heightfield& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::gfxworld::gfx_brush_model_writable read_gfx_brush_model_writable(const GfxBrushModelWritable& native)
		{
			plain::trace("read_gfx_brush_model_writable");
			asset_data::gfxworld::gfx_brush_model_writable data;
			data.bounds = read_bounds(native.bounds);
			data.origin = plain::read_scalars<float, 3>(&native.origin);
			data.quat = plain::read_scalars<float, 4>(&native.quat);
			data.mdao_volume_processed = native.mdaoVolumeProcessed;
			return data;
		}

		void write_gfx_brush_model_writable(GfxBrushModelWritable& native, const asset_data::gfxworld::gfx_brush_model_writable& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			plain::write_scalars(&native.origin, data.origin);
			plain::write_scalars(&native.quat, data.quat);
			native.mdaoVolumeProcessed = data.mdao_volume_processed;
		}

		std::size_t size_gfx_brush_model_writable([[maybe_unused]] const asset_data::gfxworld::gfx_brush_model_writable& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::gfxworld::gfx_brush_model read_gfx_brush_model(const GfxBrushModel& native)
		{
			plain::trace("read_gfx_brush_model");
			asset_data::gfxworld::gfx_brush_model data;
			data.writable = read_gfx_brush_model_writable(native.writable);
			data.bounds = read_bounds(native.bounds);
			data.radius = native.radius;
			data.start_surf_index = native.startSurfIndex;
			data.surface_count = native.surfaceCount;
			data.mdao_volume_index = native.mdaoVolumeIndex;
			return data;
		}

		void write_gfx_brush_model(GfxBrushModel& native, const asset_data::gfxworld::gfx_brush_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_brush_model_writable(native.writable, data.writable, resolve, memory);
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.radius = data.radius;
			native.startSurfIndex = data.start_surf_index;
			native.surfaceCount = data.surface_count;
			native.mdaoVolumeIndex = data.mdao_volume_index;
		}

		std::size_t size_gfx_brush_model([[maybe_unused]] const asset_data::gfxworld::gfx_brush_model& data)
		{
			std::size_t size = 0;
			size += size_gfx_brush_model_writable(data.writable);
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::gfxworld::gfx_model_lightmap_inst read_gfx_model_lightmap_inst(const GfxModelLightmapInst& native)
		{
			plain::trace("read_gfx_model_lightmap_inst");
			asset_data::gfxworld::gfx_model_lightmap_inst data;
			data.lmap_index = native.lmapIndex;
			data.offset = plain::read_scalars<float, 2>(&native.offset);
			data.scale_width = native.scaleWidth;
			data.scale_height = native.scaleHeight;
			return data;
		}

		void write_gfx_model_lightmap_inst(GfxModelLightmapInst& native, const asset_data::gfxworld::gfx_model_lightmap_inst& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lmapIndex = data.lmap_index;
			plain::write_scalars(&native.offset, data.offset);
			native.scaleWidth = data.scale_width;
			native.scaleHeight = data.scale_height;
		}

		std::size_t size_gfx_model_lightmap_inst([[maybe_unused]] const asset_data::gfxworld::gfx_model_lightmap_inst& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_map_ent_lookup read_gfx_map_ent_lookup(const GfxMapEntLookup& native)
		{
			plain::trace("read_gfx_map_ent_lookup");
			asset_data::gfxworld::gfx_map_ent_lookup data;
			data.lmap_lookup = native.lmapLookup;
			data.probes_offset = plain::read_scalars<float, 3>(&native.probesOffset);
			data.flags = native.flags;
			data.lgv_data = native.lgvData;
			data.portal_gi_slot = native.portalGISlot;
			return data;
		}

		void write_gfx_map_ent_lookup(GfxMapEntLookup& native, const asset_data::gfxworld::gfx_map_ent_lookup& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.lmapLookup = data.lmap_lookup;
			plain::write_scalars(&native.probesOffset, data.probes_offset);
			native.flags = data.flags;
			native.lgvData = data.lgv_data;
			native.portalGISlot = data.portal_gi_slot;
		}

		std::size_t size_gfx_map_ent_lookup([[maybe_unused]] const asset_data::gfxworld::gfx_map_ent_lookup& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::x_model_draw_info read_x_model_draw_info(const XModelDrawInfo& native)
		{
			plain::trace("read_x_model_draw_info");
			asset_data::gfxworld::x_model_draw_info data;
			data.has_gfx_ent_index = native.hasGfxEntIndex;
			data.lod = native.lod;
			data.material_lod = native.materialLod;
			data.surf_buf_size = native.surfBufSize;
			data.surf_id = native.surfId;
			return data;
		}

		void write_x_model_draw_info(XModelDrawInfo& native, const asset_data::gfxworld::x_model_draw_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.hasGfxEntIndex = data.has_gfx_ent_index;
			native.lod = data.lod;
			native.materialLod = data.material_lod;
			native.surfBufSize = data.surf_buf_size;
			native.surfId = data.surf_id;
		}

		std::size_t size_x_model_draw_info([[maybe_unused]] const asset_data::gfxworld::x_model_draw_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_scene_dyn_model read_gfx_scene_dyn_model(const GfxSceneDynModel& native)
		{
			plain::trace("read_gfx_scene_dyn_model");
			asset_data::gfxworld::gfx_scene_dyn_model data;
			data.info = read_x_model_draw_info(native.info);
			data.dyn_ent_client_id = native.dynEntClientId;
			return data;
		}

		void write_gfx_scene_dyn_model(GfxSceneDynModel& native, const asset_data::gfxworld::gfx_scene_dyn_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_x_model_draw_info(native.info, data.info, resolve, memory);
			native.dynEntClientId = data.dyn_ent_client_id;
		}

		std::size_t size_gfx_scene_dyn_model([[maybe_unused]] const asset_data::gfxworld::gfx_scene_dyn_model& data)
		{
			std::size_t size = 0;
			size += size_x_model_draw_info(data.info);
			return size;
		}

		asset_data::gfxworld::b_model_draw_info read_b_model_draw_info(const BModelDrawInfo& native)
		{
			plain::trace("read_b_model_draw_info");
			asset_data::gfxworld::b_model_draw_info data;
			data.surf_id = native.surfId;
			data.surf_buf_size = native.surfBufSize;
			return data;
		}

		void write_b_model_draw_info(BModelDrawInfo& native, const asset_data::gfxworld::b_model_draw_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.surfId = data.surf_id;
			native.surfBufSize = data.surf_buf_size;
		}

		std::size_t size_b_model_draw_info([[maybe_unused]] const asset_data::gfxworld::b_model_draw_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_scene_dyn_brush read_gfx_scene_dyn_brush(const GfxSceneDynBrush& native)
		{
			plain::trace("read_gfx_scene_dyn_brush");
			asset_data::gfxworld::gfx_scene_dyn_brush data;
			data.info = read_b_model_draw_info(native.info);
			data.dyn_ent_client_id = native.dynEntClientId;
			return data;
		}

		void write_gfx_scene_dyn_brush(GfxSceneDynBrush& native, const asset_data::gfxworld::gfx_scene_dyn_brush& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_b_model_draw_info(native.info, data.info, resolve, memory);
			native.dynEntClientId = data.dyn_ent_client_id;
		}

		std::size_t size_gfx_scene_dyn_brush([[maybe_unused]] const asset_data::gfxworld::gfx_scene_dyn_brush& data)
		{
			std::size_t size = 0;
			size += size_b_model_draw_info(data.info);
			return size;
		}

		asset_data::gfxworld::gfx_shadow_geometry read_gfx_shadow_geometry(const GfxShadowGeometry& native)
		{
			plain::trace("read_gfx_shadow_geometry");
			asset_data::gfxworld::gfx_shadow_geometry data;
			data.surface_count = native.surfaceCount;
			data.smodel_count = native.smodelCount;
			data.sorted_surf_index = plain::read_array_if_present(native.sortedSurfIndex, native.surfaceCount, "GfxShadowGeometry.sortedSurfIndex", [&](const auto& item) { return item; });
			data.smodel_index = plain::read_array_if_present(native.smodelIndex, native.smodelCount, "GfxShadowGeometry.smodelIndex", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_shadow_geometry(GfxShadowGeometry& native, const asset_data::gfxworld::gfx_shadow_geometry& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.surfaceCount = data.surface_count;
			native.smodelCount = data.smodel_count;
			native.sortedSurfIndex = plain::write_array<std::uint32_t>(data.sorted_surf_index, memory, [&](auto& out, const auto& in) { out = in; });
			native.smodelIndex = plain::write_array<std::uint16_t>(data.smodel_index, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_shadow_geometry([[maybe_unused]] const asset_data::gfxworld::gfx_shadow_geometry& data)
		{
			std::size_t size = 0;
			if (data.sorted_surf_index)
			{
				size += plain::sized(std::max<std::size_t>(data.sorted_surf_index->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.smodel_index)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_index->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_light_aabb_node read_gfx_light_aabb_node(const GfxLightAABBNode& native)
		{
			plain::trace("read_gfx_light_aabb_node");
			asset_data::gfxworld::gfx_light_aabb_node data;
			data.bound = read_bounds(native.bound);
			data.first_child = native.firstChild;
			data.child_count = native.childCount;
			return data;
		}

		void write_gfx_light_aabb_node(GfxLightAABBNode& native, const asset_data::gfxworld::gfx_light_aabb_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bound, data.bound, resolve, memory);
			native.firstChild = data.first_child;
			native.childCount = data.child_count;
		}

		std::size_t size_gfx_light_aabb_node([[maybe_unused]] const asset_data::gfxworld::gfx_light_aabb_node& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bound);
			return size;
		}

		asset_data::gfxworld::umbra_gate read_umbra_gate(const UmbraGate& native)
		{
			plain::trace("read_umbra_gate");
			asset_data::gfxworld::umbra_gate data;
			data.obj_id = native.objID;
			data.bounds = read_bounds(native.bounds);
			data.close_distance = native.closeDistance;
			return data;
		}

		void write_umbra_gate(UmbraGate& native, const asset_data::gfxworld::umbra_gate& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.objID = data.obj_id;
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.closeDistance = data.close_distance;
		}

		std::size_t size_umbra_gate([[maybe_unused]] const asset_data::gfxworld::umbra_gate& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			return size;
		}

		asset_data::gfxworld::mdao_volume read_mdao_volume(const MdaoVolume& native)
		{
			plain::trace("read_mdao_volume");
			asset_data::gfxworld::mdao_volume data;
			data.bounds = read_extent_bounds(native.bounds);
			data.cell_count = std::to_array(native.cellCount);
			data.parent_bone_index = native.parentBoneIndex;
			data.volume_data = plain::read_asset(ASSET_TYPE_IMAGE, native.volumeData);
			return data;
		}

		void write_mdao_volume(MdaoVolume& native, const asset_data::gfxworld::mdao_volume& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_extent_bounds(native.bounds, data.bounds, resolve, memory);
			std::ranges::copy(data.cell_count, native.cellCount);
			native.parentBoneIndex = data.parent_bone_index;
			native.volumeData = plain::resolve_asset<GfxImage>(resolve, ASSET_TYPE_IMAGE, data.volume_data);
		}

		std::size_t size_mdao_volume([[maybe_unused]] const asset_data::gfxworld::mdao_volume& data)
		{
			std::size_t size = 0;
			size += size_extent_bounds(data.bounds);
			return size;
		}

		asset_data::gfxworld::stream_tree_node read_stream_tree_node(const StreamTreeNode& native)
		{
			plain::trace("read_stream_tree_node");
			asset_data::gfxworld::stream_tree_node data;
			data.volumes_key = plain::read_asset(ASSET_TYPE_STREAM_KEY, native.volumesKey);
			data.infos = plain::read_array_if_present(native.infos, (native.imgInfoCount + native.mtlInfoCount), "StreamTreeNode.infos", [&](const auto& item) { return read_stream_mtl_img_info(item); });
			data.child_nodes = std::to_array(native.childNodes);
			data.mtl_info_count = native.mtlInfoCount;
			data.img_info_count = native.imgInfoCount;
			data.transient_zone = native.transientZone;
			plain::require_zero(native.unused, "StreamTreeNode.unused");
			data.aabb = read_extent_bounds(native.aabb);
			data.min_lomip_inv_sq_radii_of_children = native.minLomipInvSqRadiiOfChildren;
			return data;
		}

		void write_stream_tree_node(StreamTreeNode& native, const asset_data::gfxworld::stream_tree_node& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.volumesKey = plain::resolve_asset<StreamKey>(resolve, ASSET_TYPE_STREAM_KEY, data.volumes_key);
			native.infos = plain::write_array<StreamMtlImgInfo>(data.infos, memory, [&](auto& out, const auto& in) { write_stream_mtl_img_info(out, in, resolve, memory); });
			std::ranges::copy(data.child_nodes, native.childNodes);
			native.mtlInfoCount = data.mtl_info_count;
			native.imgInfoCount = data.img_info_count;
			native.transientZone = data.transient_zone;
			write_extent_bounds(native.aabb, data.aabb, resolve, memory);
			native.minLomipInvSqRadiiOfChildren = data.min_lomip_inv_sq_radii_of_children;
		}

		std::size_t size_stream_tree_node([[maybe_unused]] const asset_data::gfxworld::stream_tree_node& data)
		{
			std::size_t size = 0;
			if (data.infos)
			{
				size += plain::sized(std::max<std::size_t>(data.infos->size(), 1) * sizeof(StreamMtlImgInfo));
				for (const auto& item : *data.infos)
				{
					size += size_stream_mtl_img_info(item);
				}
			}
			size += size_extent_bounds(data.aabb);
			return size;
		}

		asset_data::gfxworld::stream_mtl_img_info read_stream_mtl_img_info(const StreamMtlImgInfo& native)
		{
			plain::trace("read_stream_mtl_img_info");
			asset_data::gfxworld::stream_mtl_img_info data;
			data.min_himip_inv_sq_radii = native.minHimipInvSqRadii;
			data.asset_index = native.assetIndex;
			data.volume_count = native.volumeCount;
			return data;
		}

		void write_stream_mtl_img_info(StreamMtlImgInfo& native, const asset_data::gfxworld::stream_mtl_img_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.minHimipInvSqRadii = data.min_himip_inv_sq_radii;
			native.assetIndex = data.asset_index;
			native.volumeCount = data.volume_count;
		}

		std::size_t size_stream_mtl_img_info([[maybe_unused]] const asset_data::gfxworld::stream_mtl_img_info& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_index_buffer read_gfx_index_buffer(const GfxIndexBuffer& native)
		{
			plain::trace("read_gfx_index_buffer");
			asset_data::gfxworld::gfx_index_buffer data;
			plain::require_zero(native.resource, "GfxIndexBuffer.resource");
			data.gpu_address = native.gpuAddress;
			data.allocation = native.allocation;
			data.size = native.size;
			return data;
		}

		void write_gfx_index_buffer(GfxIndexBuffer& native, const asset_data::gfxworld::gfx_index_buffer& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuAddress = data.gpu_address;
			native.allocation = data.allocation;
			native.size = data.size;
		}

		std::size_t size_gfx_index_buffer([[maybe_unused]] const asset_data::gfxworld::gfx_index_buffer& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_world_draw_verts read_gfx_world_draw_verts(const GfxWorldDrawVerts& native)
		{
			plain::trace("read_gfx_world_draw_verts");
			asset_data::gfxworld::gfx_world_draw_verts data;
			data.pos_data_size = native.posDataSize;
			data.aux_data_size = native.auxDataSize;
			data.pos_data = plain::read_array_if_present(native.posData, native.posDataSize, "GfxWorldDrawVerts.posData", [&](const auto& item) { return item; });
			data.aux_data = plain::read_array_if_present(native.auxData, native.auxDataSize, "GfxWorldDrawVerts.auxData", [&](const auto& item) { return item; });
			data.pos_buffer = read_gfx_wrapped_buffer(native.posBuffer);
			data.aux_buffer = read_gfx_wrapped_buffer(native.auxBuffer);
			data.index_count = native.indexCount;
			data.indices = plain::read_array_if_present(native.indices, native.indexCount, "GfxWorldDrawVerts.indices", [&](const auto& item) { return item; });
			data.index_buffer = read_gfx_index_buffer(native.indexBuffer);
			return data;
		}

		void write_gfx_world_draw_verts(GfxWorldDrawVerts& native, const asset_data::gfxworld::gfx_world_draw_verts& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.posDataSize = data.pos_data_size;
			native.auxDataSize = data.aux_data_size;
			native.posData = plain::write_array<std::uint8_t>(data.pos_data, memory, [&](auto& out, const auto& in) { out = in; });
			native.auxData = plain::write_array<std::uint8_t>(data.aux_data, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.posBuffer, data.pos_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.auxBuffer, data.aux_buffer, resolve, memory);
			native.indexCount = data.index_count;
			native.indices = plain::write_array<std::uint16_t>(data.indices, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_index_buffer(native.indexBuffer, data.index_buffer, resolve, memory);
		}

		std::size_t size_gfx_world_draw_verts([[maybe_unused]] const asset_data::gfxworld::gfx_world_draw_verts& data)
		{
			std::size_t size = 0;
			if (data.pos_data)
			{
				size += plain::sized(std::max<std::size_t>(data.pos_data->size(), 1) * sizeof(std::uint8_t));
			}
			if (data.aux_data)
			{
				size += plain::sized(std::max<std::size_t>(data.aux_data->size(), 1) * sizeof(std::uint8_t));
			}
			size += size_gfx_wrapped_buffer(data.pos_buffer);
			size += size_gfx_wrapped_buffer(data.aux_buffer);
			if (data.indices)
			{
				size += plain::sized(std::max<std::size_t>(data.indices->size(), 1) * sizeof(std::uint16_t));
			}
			size += size_gfx_index_buffer(data.index_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_world_draw_cells read_gfx_world_draw_cells(const GfxWorldDrawCells& native)
		{
			plain::trace("read_gfx_world_draw_cells");
			asset_data::gfxworld::gfx_world_draw_cells data;
			data.cell_count = native.cellCount;
			data.aabb_tree_counts = plain::read_array_if_present(native.aabbTreeCounts, native.cellCount, "GfxWorldDrawCells.aabbTreeCounts", [&](const auto& item) { return read_gfx_cell_tree_count(item); });
			data.aabb_trees = plain::read_array_if_present(native.aabbTrees, native.cellCount, "GfxWorldDrawCells.aabbTrees", [&](const auto& item) { return read_gfx_cell_tree(item, native); });
			return data;
		}

		void write_gfx_world_draw_cells(GfxWorldDrawCells& native, const asset_data::gfxworld::gfx_world_draw_cells& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.cellCount = data.cell_count;
			native.aabbTreeCounts = plain::write_array<GfxCellTreeCount>(data.aabb_tree_counts, memory, [&](auto& out, const auto& in) { write_gfx_cell_tree_count(out, in, resolve, memory); });
			native.aabbTrees = plain::write_array<GfxCellTree>(data.aabb_trees, memory, [&](auto& out, const auto& in) { write_gfx_cell_tree(out, in, resolve, memory); });
		}

		std::size_t size_gfx_world_draw_cells([[maybe_unused]] const asset_data::gfxworld::gfx_world_draw_cells& data)
		{
			std::size_t size = 0;
			if (data.aabb_tree_counts)
			{
				size += plain::sized(std::max<std::size_t>(data.aabb_tree_counts->size(), 1) * sizeof(GfxCellTreeCount));
				for (const auto& item : *data.aabb_tree_counts)
				{
					size += size_gfx_cell_tree_count(item);
				}
			}
			if (data.aabb_trees)
			{
				size += plain::sized(std::max<std::size_t>(data.aabb_trees->size(), 1) * sizeof(GfxCellTree));
				for (const auto& item : *data.aabb_trees)
				{
					size += size_gfx_cell_tree(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::gfx_image_desc read_gfx_image_desc(const GfxImageDesc& native)
		{
			plain::trace("read_gfx_image_desc");
			asset_data::gfxworld::gfx_image_desc data;
			data.format = plain::enum_name(asset_data::gfxworld::gfx_pixel_format_names, static_cast<std::int64_t>(native.format), "GfxImageDesc.format");
			data.flags = plain::flag_names(asset_data::gfxworld::gfx_image_flags_names, static_cast<std::uint64_t>(native.flags), "GfxImageDesc.flags");
			data.total_size = native.totalSize;
			data.width = native.width;
			data.height = native.height;
			data.depth = native.depth;
			data.num_elements = native.numElements;
			data.level_count = native.levelCount;
			return data;
		}

		void write_gfx_image_desc(GfxImageDesc& native, const asset_data::gfxworld::gfx_image_desc& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.format = static_cast<GfxPixelFormat>(plain::enum_value(asset_data::gfxworld::gfx_pixel_format_names, data.format, "GfxImageDesc.format"));
			native.flags = static_cast<GfxImageFlags>(plain::flag_value(asset_data::gfxworld::gfx_image_flags_names, data.flags, "GfxImageDesc.flags"));
			native.totalSize = data.total_size;
			native.width = data.width;
			native.height = data.height;
			native.depth = data.depth;
			native.numElements = data.num_elements;
			native.levelCount = data.level_count;
		}

		std::size_t size_gfx_image_desc([[maybe_unused]] const asset_data::gfxworld::gfx_image_desc& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_image_raw read_gfx_image_raw(const GfxImageRaw& native)
		{
			plain::trace("read_gfx_image_raw");
			asset_data::gfxworld::gfx_image_raw data;
			data.desc = read_gfx_image_desc(native.desc);
			data.texture_id = plain::enum_name(asset_data::gfxworld::gfx_texture_id_names, static_cast<std::int64_t>(native.textureId), "GfxImageRaw.textureId");
			data.pixels = plain::read_array_if_present(native.pixels, native.desc.totalSize, "GfxImageRaw.pixels", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_image_raw(GfxImageRaw& native, const asset_data::gfxworld::gfx_image_raw& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_gfx_image_desc(native.desc, data.desc, resolve, memory);
			native.textureId = static_cast<GfxTextureId>(plain::enum_value(asset_data::gfxworld::gfx_texture_id_names, data.texture_id, "GfxImageRaw.textureId"));
			native.pixels = plain::write_array<std::uint8_t>(data.pixels, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_image_raw([[maybe_unused]] const asset_data::gfxworld::gfx_image_raw& data)
		{
			std::size_t size = 0;
			size += size_gfx_image_desc(data.desc);
			if (data.pixels)
			{
				size += plain::sized(std::max<std::size_t>(data.pixels->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_lightmap_raw read_gfx_lightmap_raw(const GfxLightmapRaw& native)
		{
			plain::trace("read_gfx_lightmap_raw");
			asset_data::gfxworld::gfx_lightmap_raw data;
			data.textures = [&] { std::array<asset_data::gfxworld::gfx_image_raw, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_gfx_image_raw(native.textures[i]); } return out; }();
			return data;
		}

		void write_gfx_lightmap_raw(GfxLightmapRaw& native, const asset_data::gfxworld::gfx_lightmap_raw& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.textures.size(); ++i)
			{
				write_gfx_image_raw(native.textures[i], data.textures[i], resolve, memory);
			}
		}

		std::size_t size_gfx_lightmap_raw([[maybe_unused]] const asset_data::gfxworld::gfx_lightmap_raw& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.textures)
			{
				size += size_gfx_image_raw(item);
			}
			return size;
		}

		asset_data::gfxworld::gfx_lightmap_atlas_data_temp read_gfx_lightmap_atlas_data_temp(const GfxLightmapAtlasDataTemp& native)
		{
			plain::trace("read_gfx_lightmap_atlas_data_temp");
			asset_data::gfxworld::gfx_lightmap_atlas_data_temp data;
			data.transient_zone = native.transientZone;
			data.num_lightmaps = native.numLightmaps;
			data.first_lightmap_index = native.firstLightmapIndex;
			data.lightmap_type = plain::enum_name(asset_data::gfxworld::gfx_lightmap_type_names, static_cast<std::int64_t>(native.lightmapType), "GfxLightmapAtlasDataTemp.lightmapType");
			data.lightmaps = [&] { std::array<asset_data::gfxworld::gfx_lightmap_raw, 32> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_gfx_lightmap_raw(native.lightmaps[i]); } return out; }();
			return data;
		}

		void write_gfx_lightmap_atlas_data_temp(GfxLightmapAtlasDataTemp& native, const asset_data::gfxworld::gfx_lightmap_atlas_data_temp& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.transientZone = data.transient_zone;
			native.numLightmaps = data.num_lightmaps;
			native.firstLightmapIndex = data.first_lightmap_index;
			native.lightmapType = static_cast<GfxLightmapType>(plain::enum_value(asset_data::gfxworld::gfx_lightmap_type_names, data.lightmap_type, "GfxLightmapAtlasDataTemp.lightmapType"));
			for (std::size_t i = 0; i < data.lightmaps.size(); ++i)
			{
				write_gfx_lightmap_raw(native.lightmaps[i], data.lightmaps[i], resolve, memory);
			}
		}

		std::size_t size_gfx_lightmap_atlas_data_temp([[maybe_unused]] const asset_data::gfxworld::gfx_lightmap_atlas_data_temp& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.lightmaps)
			{
				size += size_gfx_lightmap_raw(item);
			}
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid read_gfx_gpu_light_grid(const GfxGpuLightGrid& native)
		{
			plain::trace("read_gfx_gpu_light_grid");
			asset_data::gfxworld::gfx_gpu_light_grid data;
			data.probe_count = native.probeCount;
			data.probes = plain::read_array_if_present(native.probes, native.probeCount, "GfxGpuLightGrid.probes", [&](const auto& item) { return read_gfx_sh_compressed_probe_data(item); });
			data.probes_buffer = read_gfx_wrapped_rw_buffer(native.probesBuffer);
			data.probe_positions = plain::read_array_if_present(native.probePositions, native.probeCount, "GfxGpuLightGrid.probePositions", [&](const auto& item) { return plain::read_scalars<float, 3>(&item); });
			data.probe_positions_buffer = read_gfx_wrapped_buffer(native.probePositionsBuffer);
			data.fallback_probe_use_custom_sample = native.fallbackProbeUseCustomSample;
			data.fallback_probe_custom_sample_pos = plain::read_scalars<float, 3>(&native.fallbackProbeCustomSamplePos);
			data.zone_count = native.zoneCount;
			data.zones = plain::read_array_if_present(native.zones, native.zoneCount, "GfxGpuLightGrid.zones", [&](const auto& item) { return read_gfx_gpu_light_grid_zone(item); });
			data.zone_voxel_trees = plain::read_array_if_present(native.zoneVoxelTrees, native.zoneCount, "GfxGpuLightGrid.zoneVoxelTrees", [&](const auto& item) { return read_gfx_gpu_light_grid_voxel_tree(item); });
			data.tetrahedron_count = native.tetrahedronCount;
			data.tetrahedron_count_visible = native.tetrahedronCountVisible;
			data.tetrahedrons = plain::read_array_if_present(native.tetrahedrons, native.tetrahedronCount, "GfxGpuLightGrid.tetrahedrons", [&](const auto& item) { return read_gfx_gpu_light_grid_tetrahedron(item); });
			data.tetrahedron_buffer = read_gfx_wrapped_buffer(native.tetrahedronBuffer);
			data.tetrahedron_neighbors = plain::read_array_if_present(native.tetrahedronNeighbors, native.tetrahedronCount, "GfxGpuLightGrid.tetrahedronNeighbors", [&](const auto& item) { return read_gfx_gpu_light_grid_tetrahedron_neighbors(item); });
			data.tetrahedron_neighbors_buffer = read_gfx_wrapped_buffer(native.tetrahedronNeighborsBuffer);
			data.tetrahedron_visibility = plain::read_array_if_present(native.tetrahedronVisibility, native.tetrahedronCountVisible, "GfxGpuLightGrid.tetrahedronVisibility", [&](const auto& item) { return read_gfx_gpu_light_grid_tetrahedron_visibility(item); });
			data.tetrahedron_visibility_buffer = read_gfx_wrapped_buffer(native.tetrahedronVisibilityBuffer);
			data.voxel_start_tetrahedron_count = native.voxelStartTetrahedronCount;
			data.voxel_start_tetrahedron = plain::read_array_if_present(native.voxelStartTetrahedron, native.voxelStartTetrahedronCount, "GfxGpuLightGrid.voxelStartTetrahedron", [&](const auto& item) { return read_gfx_gpu_light_grid_voxel_start_tetrahedron(item); });
			data.voxel_start_tetrahedron_buffer = read_gfx_wrapped_buffer(native.voxelStartTetrahedronBuffer);
			data.bounding_volume = [&] { std::array<std::array<float, 4>, 7> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.boundingVolume[i]); } return out; }();
			return data;
		}

		void write_gfx_gpu_light_grid(GfxGpuLightGrid& native, const asset_data::gfxworld::gfx_gpu_light_grid& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.probeCount = data.probe_count;
			native.probes = plain::write_array<GfxSHCompressedProbeData>(data.probes, memory, [&](auto& out, const auto& in) { write_gfx_sh_compressed_probe_data(out, in, resolve, memory); });
			write_gfx_wrapped_rw_buffer(native.probesBuffer, data.probes_buffer, resolve, memory);
			native.probePositions = plain::write_array<vec3_t>(data.probe_positions, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
			write_gfx_wrapped_buffer(native.probePositionsBuffer, data.probe_positions_buffer, resolve, memory);
			native.fallbackProbeUseCustomSample = data.fallback_probe_use_custom_sample;
			plain::write_scalars(&native.fallbackProbeCustomSamplePos, data.fallback_probe_custom_sample_pos);
			native.zoneCount = data.zone_count;
			native.zones = plain::write_array<GfxGpuLightGridZone>(data.zones, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_zone(out, in, resolve, memory); });
			native.zoneVoxelTrees = plain::write_array<GfxGpuLightGridVoxelTree>(data.zone_voxel_trees, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_voxel_tree(out, in, resolve, memory); });
			native.tetrahedronCount = data.tetrahedron_count;
			native.tetrahedronCountVisible = data.tetrahedron_count_visible;
			native.tetrahedrons = plain::write_array<GfxGpuLightGridTetrahedron>(data.tetrahedrons, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_tetrahedron(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.tetrahedronBuffer, data.tetrahedron_buffer, resolve, memory);
			native.tetrahedronNeighbors = plain::write_array<GfxGpuLightGridTetrahedronNeighbors>(data.tetrahedron_neighbors, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_tetrahedron_neighbors(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.tetrahedronNeighborsBuffer, data.tetrahedron_neighbors_buffer, resolve, memory);
			native.tetrahedronVisibility = plain::write_array<GfxGpuLightGridTetrahedronVisibility>(data.tetrahedron_visibility, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_tetrahedron_visibility(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.tetrahedronVisibilityBuffer, data.tetrahedron_visibility_buffer, resolve, memory);
			native.voxelStartTetrahedronCount = data.voxel_start_tetrahedron_count;
			native.voxelStartTetrahedron = plain::write_array<GfxGpuLightGridVoxelStartTetrahedron>(data.voxel_start_tetrahedron, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid_voxel_start_tetrahedron(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.voxelStartTetrahedronBuffer, data.voxel_start_tetrahedron_buffer, resolve, memory);
			for (std::size_t i = 0; i < data.bounding_volume.size(); ++i)
			{
				plain::write_scalars(&native.boundingVolume[i], data.bounding_volume[i]);
			}
		}

		std::size_t size_gfx_gpu_light_grid([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid& data)
		{
			std::size_t size = 0;
			if (data.probes)
			{
				size += plain::sized(std::max<std::size_t>(data.probes->size(), 1) * sizeof(GfxSHCompressedProbeData));
				for (const auto& item : *data.probes)
				{
					size += size_gfx_sh_compressed_probe_data(item);
				}
			}
			size += size_gfx_wrapped_rw_buffer(data.probes_buffer);
			if (data.probe_positions)
			{
				size += plain::sized(std::max<std::size_t>(data.probe_positions->size(), 1) * sizeof(vec3_t));
			}
			size += size_gfx_wrapped_buffer(data.probe_positions_buffer);
			if (data.zones)
			{
				size += plain::sized(std::max<std::size_t>(data.zones->size(), 1) * sizeof(GfxGpuLightGridZone));
				for (const auto& item : *data.zones)
				{
					size += size_gfx_gpu_light_grid_zone(item);
				}
			}
			if (data.zone_voxel_trees)
			{
				size += plain::sized(std::max<std::size_t>(data.zone_voxel_trees->size(), 1) * sizeof(GfxGpuLightGridVoxelTree));
				for (const auto& item : *data.zone_voxel_trees)
				{
					size += size_gfx_gpu_light_grid_voxel_tree(item);
				}
			}
			if (data.tetrahedrons)
			{
				size += plain::sized(std::max<std::size_t>(data.tetrahedrons->size(), 1) * sizeof(GfxGpuLightGridTetrahedron));
				for (const auto& item : *data.tetrahedrons)
				{
					size += size_gfx_gpu_light_grid_tetrahedron(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tetrahedron_buffer);
			if (data.tetrahedron_neighbors)
			{
				size += plain::sized(std::max<std::size_t>(data.tetrahedron_neighbors->size(), 1) * sizeof(GfxGpuLightGridTetrahedronNeighbors));
				for (const auto& item : *data.tetrahedron_neighbors)
				{
					size += size_gfx_gpu_light_grid_tetrahedron_neighbors(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tetrahedron_neighbors_buffer);
			if (data.tetrahedron_visibility)
			{
				size += plain::sized(std::max<std::size_t>(data.tetrahedron_visibility->size(), 1) * sizeof(GfxGpuLightGridTetrahedronVisibility));
				for (const auto& item : *data.tetrahedron_visibility)
				{
					size += size_gfx_gpu_light_grid_tetrahedron_visibility(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tetrahedron_visibility_buffer);
			if (data.voxel_start_tetrahedron)
			{
				size += plain::sized(std::max<std::size_t>(data.voxel_start_tetrahedron->size(), 1) * sizeof(GfxGpuLightGridVoxelStartTetrahedron));
				for (const auto& item : *data.voxel_start_tetrahedron)
				{
					size += size_gfx_gpu_light_grid_voxel_start_tetrahedron(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.voxel_start_tetrahedron_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_dynamic_lightset_l_gpp_zone_info read_gfx_dynamic_lightset_l_gpp_zone_info(const GfxDynamicLightsetLGppZoneInfo& native)
		{
			plain::trace("read_gfx_dynamic_lightset_l_gpp_zone_info");
			asset_data::gfxworld::gfx_dynamic_lightset_l_gpp_zone_info data;
			data.num_orig_sml_gpp_probes = native.numOrigSMLGppProbes;
			data.lgv_references = plain::read_array_if_present(native.lgvReferences, native.numOrigSMLGppProbes, "GfxDynamicLightsetLGppZoneInfo.lgvReferences", [&](const auto& item) { return read_gfx_gpu_dyn_lightsets_lgv_reference(item); });
			data.lgv_references_buffer = read_gfx_wrapped_buffer(native.lgvReferencesBuffer);
			data.lgv_probe_data = read_gfx_dynamic_lightset_probe_data(native.lgvProbeData);
			return data;
		}

		void write_gfx_dynamic_lightset_l_gpp_zone_info(GfxDynamicLightsetLGppZoneInfo& native, const asset_data::gfxworld::gfx_dynamic_lightset_l_gpp_zone_info& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numOrigSMLGppProbes = data.num_orig_sml_gpp_probes;
			native.lgvReferences = plain::write_array<GfxGpuDynLightsetsLGVReference>(data.lgv_references, memory, [&](auto& out, const auto& in) { write_gfx_gpu_dyn_lightsets_lgv_reference(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.lgvReferencesBuffer, data.lgv_references_buffer, resolve, memory);
			write_gfx_dynamic_lightset_probe_data(native.lgvProbeData, data.lgv_probe_data, resolve, memory);
		}

		std::size_t size_gfx_dynamic_lightset_l_gpp_zone_info([[maybe_unused]] const asset_data::gfxworld::gfx_dynamic_lightset_l_gpp_zone_info& data)
		{
			std::size_t size = 0;
			if (data.lgv_references)
			{
				size += plain::sized(std::max<std::size_t>(data.lgv_references->size(), 1) * sizeof(GfxGpuDynLightsetsLGVReference));
				for (const auto& item : *data.lgv_references)
				{
					size += size_gfx_gpu_dyn_lightsets_lgv_reference(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.lgv_references_buffer);
			size += size_gfx_dynamic_lightset_probe_data(data.lgv_probe_data);
			return size;
		}

		asset_data::gfxworld::gfx_light_grid_volume_atlas_data_temp read_gfx_light_grid_volume_atlas_data_temp(const GfxLightGridVolumeAtlasDataTemp& native)
		{
			plain::trace("read_gfx_light_grid_volume_atlas_data_temp");
			asset_data::gfxworld::gfx_light_grid_volume_atlas_data_temp data;
			data.m_width_and_height = native.m_widthAndHeight;
			data.m_depth = native.m_depth;
			data.m_num_volumes = native.m_numVolumes;
			data.m_world_transient = native.m_worldTransient;
			data.m_textures = [&] { std::array<asset_data::gfxworld::gfx_image_raw, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_gfx_image_raw(native.m_textures[i]); } return out; }();
			data.m_packing_params = plain::read_array_if_present(native.m_packingParams, native.m_numVolumes, "GfxLightGridVolumeAtlasDataTemp.m_packingParams", [&](const auto& item) { return read_light_grid_volume_packing_params(item); });
			data.m_packing_params_buffer = read_gfx_wrapped_buffer(native.m_packingParamsBuffer);
			data.m_transform_params = plain::read_array_if_present(native.m_transformParams, native.m_numVolumes, "GfxLightGridVolumeAtlasDataTemp.m_transformParams", [&](const auto& item) { return read_light_grid_volume_transform_params(item); });
			data.m_transform_params_buffer = read_gfx_wrapped_buffer(native.m_transformParamsBuffer);
			return data;
		}

		void write_gfx_light_grid_volume_atlas_data_temp(GfxLightGridVolumeAtlasDataTemp& native, const asset_data::gfxworld::gfx_light_grid_volume_atlas_data_temp& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_widthAndHeight = data.m_width_and_height;
			native.m_depth = data.m_depth;
			native.m_numVolumes = data.m_num_volumes;
			native.m_worldTransient = data.m_world_transient;
			for (std::size_t i = 0; i < data.m_textures.size(); ++i)
			{
				write_gfx_image_raw(native.m_textures[i], data.m_textures[i], resolve, memory);
			}
			native.m_packingParams = plain::write_array<LightGridVolumePackingParams>(data.m_packing_params, memory, [&](auto& out, const auto& in) { write_light_grid_volume_packing_params(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.m_packingParamsBuffer, data.m_packing_params_buffer, resolve, memory);
			native.m_transformParams = plain::write_array<LightGridVolumeTransformParams>(data.m_transform_params, memory, [&](auto& out, const auto& in) { write_light_grid_volume_transform_params(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.m_transformParamsBuffer, data.m_transform_params_buffer, resolve, memory);
		}

		std::size_t size_gfx_light_grid_volume_atlas_data_temp([[maybe_unused]] const asset_data::gfxworld::gfx_light_grid_volume_atlas_data_temp& data)
		{
			std::size_t size = 0;
			for (const auto& item : data.m_textures)
			{
				size += size_gfx_image_raw(item);
			}
			if (data.m_packing_params)
			{
				size += plain::sized(std::max<std::size_t>(data.m_packing_params->size(), 1) * sizeof(LightGridVolumePackingParams));
				for (const auto& item : *data.m_packing_params)
				{
					size += size_light_grid_volume_packing_params(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.m_packing_params_buffer);
			if (data.m_transform_params)
			{
				size += plain::sized(std::max<std::size_t>(data.m_transform_params->size(), 1) * sizeof(LightGridVolumeTransformParams));
				for (const auto& item : *data.m_transform_params)
				{
					size += size_light_grid_volume_transform_params(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.m_transform_params_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid_transient_data read_gfx_gpu_light_grid_transient_data(const GfxGpuLightGridTransientData& native)
		{
			plain::trace("read_gfx_gpu_light_grid_transient_data");
			asset_data::gfxworld::gfx_gpu_light_grid_transient_data data;
			data.gpu_light_grid = plain::read_single(native.gpuLightGrid, [&](const auto& item) { return read_gfx_gpu_light_grid(item); });
			data.static_model_volume_count = native.staticModelVolumeCount;
			data.static_model_volumes = plain::read_array_if_present(native.staticModelVolumes, native.staticModelVolumeCount, "GfxGpuLightGridTransientData.staticModelVolumes", [&](const auto& item) { return read_gfx_static_model_light_grid_volume(item); });
			data.sml_gpp_zone = plain::read_single(native.SMLGppZone, [&](const auto& item) { return read_gfx_dynamic_lightset_l_gpp_zone_info(item); });
			data.temp_light_grid_volume_data = plain::read_single(native.tempLightGridVolumeData, [&](const auto& item) { return read_gfx_light_grid_volume_atlas_data_temp(item); });
			return data;
		}

		void write_gfx_gpu_light_grid_transient_data(GfxGpuLightGridTransientData& native, const asset_data::gfxworld::gfx_gpu_light_grid_transient_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.gpuLightGrid = plain::write_single<GfxGpuLightGrid>(data.gpu_light_grid, memory, [&](auto& out, const auto& in) { write_gfx_gpu_light_grid(out, in, resolve, memory); });
			native.staticModelVolumeCount = data.static_model_volume_count;
			native.staticModelVolumes = plain::write_array<GfxStaticModelLightGridVolume>(data.static_model_volumes, memory, [&](auto& out, const auto& in) { write_gfx_static_model_light_grid_volume(out, in, resolve, memory); });
			native.SMLGppZone = plain::write_single<GfxDynamicLightsetLGppZoneInfo>(data.sml_gpp_zone, memory, [&](auto& out, const auto& in) { write_gfx_dynamic_lightset_l_gpp_zone_info(out, in, resolve, memory); });
			native.tempLightGridVolumeData = plain::write_single<GfxLightGridVolumeAtlasDataTemp>(data.temp_light_grid_volume_data, memory, [&](auto& out, const auto& in) { write_gfx_light_grid_volume_atlas_data_temp(out, in, resolve, memory); });
		}

		std::size_t size_gfx_gpu_light_grid_transient_data([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid_transient_data& data)
		{
			std::size_t size = 0;
			if (data.gpu_light_grid)
			{
				size += plain::sized(sizeof(GfxGpuLightGrid));
				size += size_gfx_gpu_light_grid(*data.gpu_light_grid);
			}
			if (data.static_model_volumes)
			{
				size += plain::sized(std::max<std::size_t>(data.static_model_volumes->size(), 1) * sizeof(GfxStaticModelLightGridVolume));
				for (const auto& item : *data.static_model_volumes)
				{
					size += size_gfx_static_model_light_grid_volume(item);
				}
			}
			if (data.sml_gpp_zone)
			{
				size += plain::sized(sizeof(GfxDynamicLightsetLGppZoneInfo));
				size += size_gfx_dynamic_lightset_l_gpp_zone_info(*data.sml_gpp_zone);
			}
			if (data.temp_light_grid_volume_data)
			{
				size += plain::sized(sizeof(GfxLightGridVolumeAtlasDataTemp));
				size += size_gfx_light_grid_volume_atlas_data_temp(*data.temp_light_grid_volume_data);
			}
			return size;
		}

		asset_data::gfxworld::gfx_reflection_probe_transient_data read_gfx_reflection_probe_transient_data(const GfxReflectionProbeTransientData& native)
		{
			plain::trace("read_gfx_reflection_probe_transient_data");
			asset_data::gfxworld::gfx_reflection_probe_transient_data data;
			data.reflection_probe_instance_count = native.reflectionProbeInstanceCount;
			data.exclusive_probe_bucket_count = native.exclusiveProbeBucketCount;
			data.exclusive_probe_count = native.exclusiveProbeCount;
			data.reflection_probe_instances = plain::read_array_if_present(native.reflectionProbeInstances, native.reflectionProbeInstanceCount, "GfxReflectionProbeTransientData.reflectionProbeInstances", [&](const auto& item) { return item; });
			data.exclusive_probe_buckets = plain::read_array_if_present(native.exclusiveProbeBuckets, native.exclusiveProbeBucketCount, "GfxReflectionProbeTransientData.exclusiveProbeBuckets", [&](const auto& item) { return read_gfx_image_reflection_probe(item); });
			return data;
		}

		void write_gfx_reflection_probe_transient_data(GfxReflectionProbeTransientData& native, const asset_data::gfxworld::gfx_reflection_probe_transient_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.reflectionProbeInstanceCount = data.reflection_probe_instance_count;
			native.exclusiveProbeBucketCount = data.exclusive_probe_bucket_count;
			native.exclusiveProbeCount = data.exclusive_probe_count;
			native.reflectionProbeInstances = plain::write_array<std::uint16_t>(data.reflection_probe_instances, memory, [&](auto& out, const auto& in) { out = in; });
			native.exclusiveProbeBuckets = plain::write_array<GfxImageReflectionProbe>(data.exclusive_probe_buckets, memory, [&](auto& out, const auto& in) { write_gfx_image_reflection_probe(out, in, resolve, memory); });
		}

		std::size_t size_gfx_reflection_probe_transient_data([[maybe_unused]] const asset_data::gfxworld::gfx_reflection_probe_transient_data& data)
		{
			std::size_t size = 0;
			if (data.reflection_probe_instances)
			{
				size += plain::sized(std::max<std::size_t>(data.reflection_probe_instances->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.exclusive_probe_buckets)
			{
				size += plain::sized(std::max<std::size_t>(data.exclusive_probe_buckets->size(), 1) * sizeof(GfxImageReflectionProbe));
				for (const auto& item : *data.exclusive_probe_buckets)
				{
					size += size_gfx_image_reflection_probe(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::gfx_decal_volume_transient_data read_gfx_decal_volume_transient_data(const GfxDecalVolumeTransientData& native)
		{
			plain::trace("read_gfx_decal_volume_transient_data");
			asset_data::gfxworld::gfx_decal_volume_transient_data data;
			data.decal_volume_count = native.decalVolumeCount;
			data.first_collection = native.firstCollection;
			data.collection_count = native.collectionCount;
			data.decal_volumes = plain::read_array_if_present(native.decalVolumes, native.decalVolumeCount, "GfxDecalVolumeTransientData.decalVolumes", [&](const auto& item) { return read_gfx_static_decal_volume_packed(item); });
			data.decal_volumes_buffer = read_gfx_wrapped_buffer(native.decalVolumesBuffer);
			data.collection_bounds = plain::read_array_if_present(native.collectionBounds, native.collectionCount, "GfxDecalVolumeTransientData.collectionBounds", [&](const auto& item) { return read_bounds(item); });
			data.decal_volume_uids = plain::read_array_if_present(native.decalVolumeUids, native.decalVolumeCount, "GfxDecalVolumeTransientData.decalVolumeUids", [&](const auto& item) { return read_gfx_static_decal_volume_uid(item); });
			return data;
		}

		void write_gfx_decal_volume_transient_data(GfxDecalVolumeTransientData& native, const asset_data::gfxworld::gfx_decal_volume_transient_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.decalVolumeCount = data.decal_volume_count;
			native.firstCollection = data.first_collection;
			native.collectionCount = data.collection_count;
			native.decalVolumes = plain::write_array<GfxStaticDecalVolumePacked>(data.decal_volumes, memory, [&](auto& out, const auto& in) { write_gfx_static_decal_volume_packed(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.decalVolumesBuffer, data.decal_volumes_buffer, resolve, memory);
			native.collectionBounds = plain::write_array<Bounds>(data.collection_bounds, memory, [&](auto& out, const auto& in) { write_bounds(out, in, resolve, memory); });
			native.decalVolumeUids = plain::write_array<GfxStaticDecalVolumeUid>(data.decal_volume_uids, memory, [&](auto& out, const auto& in) { write_gfx_static_decal_volume_uid(out, in, resolve, memory); });
		}

		std::size_t size_gfx_decal_volume_transient_data([[maybe_unused]] const asset_data::gfxworld::gfx_decal_volume_transient_data& data)
		{
			std::size_t size = 0;
			if (data.decal_volumes)
			{
				size += plain::sized(std::max<std::size_t>(data.decal_volumes->size(), 1) * sizeof(GfxStaticDecalVolumePacked));
				for (const auto& item : *data.decal_volumes)
				{
					size += size_gfx_static_decal_volume_packed(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.decal_volumes_buffer);
			if (data.collection_bounds)
			{
				size += plain::sized(std::max<std::size_t>(data.collection_bounds->size(), 1) * sizeof(Bounds));
				for (const auto& item : *data.collection_bounds)
				{
					size += size_bounds(item);
				}
			}
			if (data.decal_volume_uids)
			{
				size += plain::sized(std::max<std::size_t>(data.decal_volume_uids->size(), 1) * sizeof(GfxStaticDecalVolumeUid));
				for (const auto& item : *data.decal_volume_uids)
				{
					size += size_gfx_static_decal_volume_uid(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::gfx_compressed_sun_shadow_data_temp read_gfx_compressed_sun_shadow_data_temp(const GfxCompressedSunShadowDataTemp& native)
		{
			plain::trace("read_gfx_compressed_sun_shadow_data_temp");
			asset_data::gfxworld::gfx_compressed_sun_shadow_data_temp data;
			data.zone_index = native.zoneIndex;
			data.tiles_count = native.tilesCount;
			data.tiles_indices = plain::read_array_if_present(native.tilesIndices, native.tilesCount, "GfxCompressedSunShadowDataTemp.tilesIndices", [&](const auto& item) { return item; });
			data.tiles_offsets = plain::read_array_if_present(native.tilesOffsets, native.tilesCount, "GfxCompressedSunShadowDataTemp.tilesOffsets", [&](const auto& item) { return item; });
			data.tiles_data_size = native.tilesDataSize;
			data.tiles_data = plain::read_array_if_present(native.tilesData, native.tilesDataSize, "GfxCompressedSunShadowDataTemp.tilesData", [&](const auto& item) { return item; });
			data.tiles_data_buffer = read_gfx_wrapped_buffer(native.tilesDataBuffer);
			return data;
		}

		void write_gfx_compressed_sun_shadow_data_temp(GfxCompressedSunShadowDataTemp& native, const asset_data::gfxworld::gfx_compressed_sun_shadow_data_temp& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.zoneIndex = data.zone_index;
			native.tilesCount = data.tiles_count;
			native.tilesIndices = plain::write_array<std::uint32_t>(data.tiles_indices, memory, [&](auto& out, const auto& in) { out = in; });
			native.tilesOffsets = plain::write_array<std::uint32_t>(data.tiles_offsets, memory, [&](auto& out, const auto& in) { out = in; });
			native.tilesDataSize = data.tiles_data_size;
			native.tilesData = plain::write_array<std::uint8_t>(data.tiles_data, memory, [&](auto& out, const auto& in) { out = in; });
			write_gfx_wrapped_buffer(native.tilesDataBuffer, data.tiles_data_buffer, resolve, memory);
		}

		std::size_t size_gfx_compressed_sun_shadow_data_temp([[maybe_unused]] const asset_data::gfxworld::gfx_compressed_sun_shadow_data_temp& data)
		{
			std::size_t size = 0;
			if (data.tiles_indices)
			{
				size += plain::sized(std::max<std::size_t>(data.tiles_indices->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.tiles_offsets)
			{
				size += plain::sized(std::max<std::size_t>(data.tiles_offsets->size(), 1) * sizeof(std::uint32_t));
			}
			if (data.tiles_data)
			{
				size += plain::sized(std::max<std::size_t>(data.tiles_data->size(), 1) * sizeof(std::uint8_t));
			}
			size += size_gfx_wrapped_buffer(data.tiles_data_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_world_transient_zone read_gfx_world_transient_zone(const GfxWorldTransientZone& native)
		{
			plain::trace("read_gfx_world_transient_zone");
			asset_data::gfxworld::gfx_world_transient_zone data;
			data.name = plain::read_string(native.name);
			data.transient_zone_index = native.transientZoneIndex;
			data.draw_verts = read_gfx_world_draw_verts(native.drawVerts);
			data.draw_cells = read_gfx_world_draw_cells(native.drawCells);
			data.temp_lightmap_data = plain::read_single(native.tempLightmapData, [&](const auto& item) { return read_gfx_lightmap_atlas_data_temp(item); });
			data.gpu_light_grid = read_gfx_gpu_light_grid_transient_data(native.gpuLightGrid);
			data.reflection_probes = plain::read_single(native.reflectionProbes, [&](const auto& item) { return read_gfx_reflection_probe_transient_data(item); });
			data.collection_count = native.collectionCount;
			data.collections = plain::read_array_if_present(native.collections, native.collectionCount, "GfxWorldTransientZone.collections", [&](const auto& item) { return item; });
			data.decal_volumes = plain::read_single(native.decalVolumes, [&](const auto& item) { return read_gfx_decal_volume_transient_data(item); });
			data.compressed_sun_shadow = plain::read_single(native.compressedSunShadow, [&](const auto& item) { return read_gfx_compressed_sun_shadow_data_temp(item); });
			return data;
		}

		void write_gfx_world_transient_zone(GfxWorldTransientZone& native, const asset_data::gfxworld::gfx_world_transient_zone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.transientZoneIndex = data.transient_zone_index;
			write_gfx_world_draw_verts(native.drawVerts, data.draw_verts, resolve, memory);
			write_gfx_world_draw_cells(native.drawCells, data.draw_cells, resolve, memory);
			native.tempLightmapData = plain::write_single<GfxLightmapAtlasDataTemp>(data.temp_lightmap_data, memory, [&](auto& out, const auto& in) { write_gfx_lightmap_atlas_data_temp(out, in, resolve, memory); });
			write_gfx_gpu_light_grid_transient_data(native.gpuLightGrid, data.gpu_light_grid, resolve, memory);
			native.reflectionProbes = plain::write_single<GfxReflectionProbeTransientData>(data.reflection_probes, memory, [&](auto& out, const auto& in) { write_gfx_reflection_probe_transient_data(out, in, resolve, memory); });
			native.collectionCount = data.collection_count;
			native.collections = plain::write_array<std::uint16_t>(data.collections, memory, [&](auto& out, const auto& in) { out = in; });
			native.decalVolumes = plain::write_single<GfxDecalVolumeTransientData>(data.decal_volumes, memory, [&](auto& out, const auto& in) { write_gfx_decal_volume_transient_data(out, in, resolve, memory); });
			native.compressedSunShadow = plain::write_single<GfxCompressedSunShadowDataTemp>(data.compressed_sun_shadow, memory, [&](auto& out, const auto& in) { write_gfx_compressed_sun_shadow_data_temp(out, in, resolve, memory); });
		}

		std::size_t size_gfx_world_transient_zone([[maybe_unused]] const asset_data::gfxworld::gfx_world_transient_zone& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_gfx_world_draw_verts(data.draw_verts);
			size += size_gfx_world_draw_cells(data.draw_cells);
			if (data.temp_lightmap_data)
			{
				size += plain::sized(sizeof(GfxLightmapAtlasDataTemp));
				size += size_gfx_lightmap_atlas_data_temp(*data.temp_lightmap_data);
			}
			size += size_gfx_gpu_light_grid_transient_data(data.gpu_light_grid);
			if (data.reflection_probes)
			{
				size += plain::sized(sizeof(GfxReflectionProbeTransientData));
				size += size_gfx_reflection_probe_transient_data(*data.reflection_probes);
			}
			if (data.collections)
			{
				size += plain::sized(std::max<std::size_t>(data.collections->size(), 1) * sizeof(std::uint16_t));
			}
			if (data.decal_volumes)
			{
				size += plain::sized(sizeof(GfxDecalVolumeTransientData));
				size += size_gfx_decal_volume_transient_data(*data.decal_volumes);
			}
			if (data.compressed_sun_shadow)
			{
				size += plain::sized(sizeof(GfxCompressedSunShadowDataTemp));
				size += size_gfx_compressed_sun_shadow_data_temp(*data.compressed_sun_shadow);
			}
			return size;
		}

		asset_data::gfxworld::gfx_cell_tree_count read_gfx_cell_tree_count(const GfxCellTreeCount& native)
		{
			plain::trace("read_gfx_cell_tree_count");
			asset_data::gfxworld::gfx_cell_tree_count data;
			data.aabb_tree_count = native.aabbTreeCount;
			return data;
		}

		void write_gfx_cell_tree_count(GfxCellTreeCount& native, const asset_data::gfxworld::gfx_cell_tree_count& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aabbTreeCount = data.aabb_tree_count;
		}

		std::size_t size_gfx_cell_tree_count([[maybe_unused]] const asset_data::gfxworld::gfx_cell_tree_count& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_cell_tree read_gfx_cell_tree(const GfxCellTree& native, [[maybe_unused]] const GfxWorldDrawCells& outer)
		{
			plain::trace("read_gfx_cell_tree");
			asset_data::gfxworld::gfx_cell_tree data;
			data.aabb_tree = plain::read_array_if_present(native.aabbTree, outer.aabbTreeCounts[&native - outer.aabbTrees].aabbTreeCount, "GfxCellTree.aabbTree", [&](const auto& item) { return read_gfx_aabb_tree(item); });
			return data;
		}

		void write_gfx_cell_tree(GfxCellTree& native, const asset_data::gfxworld::gfx_cell_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.aabbTree = plain::write_array<GfxAabbTree>(data.aabb_tree, memory, [&](auto& out, const auto& in) { write_gfx_aabb_tree(out, in, resolve, memory); });
		}

		std::size_t size_gfx_cell_tree([[maybe_unused]] const asset_data::gfxworld::gfx_cell_tree& data)
		{
			std::size_t size = 0;
			if (data.aabb_tree)
			{
				size += plain::sized(std::max<std::size_t>(data.aabb_tree->size(), 1) * sizeof(GfxAabbTree));
				for (const auto& item : *data.aabb_tree)
				{
					size += size_gfx_aabb_tree(item);
				}
			}
			return size;
		}

		asset_data::gfxworld::gfx_aabb_tree read_gfx_aabb_tree(const GfxAabbTree& native)
		{
			plain::trace("read_gfx_aabb_tree");
			asset_data::gfxworld::gfx_aabb_tree data;
			data.bounds = read_bounds(native.bounds);
			data.surface_count = native.surfaceCount;
			data.start_surf_index = native.startSurfIndex;
			data.children_offset = native.childrenOffset;
			data.child_count = native.childCount;
			data.smodel_index_count = native.smodelIndexCount;
			data.smodel_indexes = plain::read_borrowed(native.smodelIndexes, native.smodelIndexCount, "GfxAabbTree.smodelIndexes", [&](const auto& item) { return item; });
			data.smodel_indexes_id = plain::pointer_ids::of(native.smodelIndexes);
			return data;
		}

		void write_gfx_aabb_tree(GfxAabbTree& native, const asset_data::gfxworld::gfx_aabb_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_bounds(native.bounds, data.bounds, resolve, memory);
			native.surfaceCount = data.surface_count;
			native.startSurfIndex = data.start_surf_index;
			native.childrenOffset = data.children_offset;
			native.childCount = data.child_count;
			native.smodelIndexCount = data.smodel_index_count;
			native.smodelIndexes = plain::write_borrowed_array<std::uint16_t>(data.smodel_indexes_id, data.smodel_indexes, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_aabb_tree([[maybe_unused]] const asset_data::gfxworld::gfx_aabb_tree& data)
		{
			std::size_t size = 0;
			size += size_bounds(data.bounds);
			if (data.smodel_indexes)
			{
				size += plain::sized(std::max<std::size_t>(data.smodel_indexes->size(), 1) * sizeof(std::uint16_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_sh_probe_data read_gfx_sh_probe_data(const GfxSHProbeData& native)
		{
			plain::trace("read_gfx_sh_probe_data");
			asset_data::gfxworld::gfx_sh_probe_data data;
			data.coeffs = std::to_array(native.coeffs);
			plain::require_zero(native.pad, "GfxSHProbeData.pad");
			return data;
		}

		void write_gfx_sh_probe_data(GfxSHProbeData& native, const asset_data::gfxworld::gfx_sh_probe_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.coeffs, native.coeffs);
		}

		std::size_t size_gfx_sh_probe_data([[maybe_unused]] const asset_data::gfxworld::gfx_sh_probe_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid_zone read_gfx_gpu_light_grid_zone(const GfxGpuLightGridZone& native)
		{
			plain::trace("read_gfx_gpu_light_grid_zone");
			asset_data::gfxworld::gfx_gpu_light_grid_zone data;
			data.num_probes = native.numProbes;
			data.first_probe = native.firstProbe;
			data.num_tetrahedrons = native.numTetrahedrons;
			data.first_tetrahedron = native.firstTetrahedron;
			data.first_voxel_tetrahedron_index = native.firstVoxelTetrahedronIndex;
			data.voxel_tetrahedron_internal_node_shift = native.voxelTetrahedronInternalNodeShift;
			data.num_voxel_tetrahedron_indices = native.numVoxelTetrahedronIndices;
			data.fallback_probe_data = read_gfx_sh_probe_data(native.fallbackProbeData);
			return data;
		}

		void write_gfx_gpu_light_grid_zone(GfxGpuLightGridZone& native, const asset_data::gfxworld::gfx_gpu_light_grid_zone& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.numProbes = data.num_probes;
			native.firstProbe = data.first_probe;
			native.numTetrahedrons = data.num_tetrahedrons;
			native.firstTetrahedron = data.first_tetrahedron;
			native.firstVoxelTetrahedronIndex = data.first_voxel_tetrahedron_index;
			native.voxelTetrahedronInternalNodeShift = data.voxel_tetrahedron_internal_node_shift;
			native.numVoxelTetrahedronIndices = data.num_voxel_tetrahedron_indices;
			write_gfx_sh_probe_data(native.fallbackProbeData, data.fallback_probe_data, resolve, memory);
		}

		std::size_t size_gfx_gpu_light_grid_zone([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid_zone& data)
		{
			std::size_t size = 0;
			size += size_gfx_sh_probe_data(data.fallback_probe_data);
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid_voxel_tree read_gfx_gpu_light_grid_voxel_tree(const GfxGpuLightGridVoxelTree& native)
		{
			plain::trace("read_gfx_gpu_light_grid_voxel_tree");
			asset_data::gfxworld::gfx_gpu_light_grid_voxel_tree data;
			data.top_down_view_node_count = native.topDownViewNodeCount;
			data.internal_node_count = native.internalNodeCount;
			data.tree_header = plain::read_single(native.treeHeader, [&](const auto& item) { return read_gfx_voxel_tree_header(item); });
			data.top_down_view_nodes = plain::read_array_if_present(native.topDownViewNodes, native.topDownViewNodeCount, "GfxGpuLightGridVoxelTree.topDownViewNodes", [&](const auto& item) { return read_gfx_voxel_top_down_view_node(item); });
			data.internal_nodes = plain::read_array_if_present(native.internalNodes, native.internalNodeCount, "GfxGpuLightGridVoxelTree.internalNodes", [&](const auto& item) { return read_gfx_voxel_internal_node(item); });
			data.tree_header_buffer = read_gfx_wrapped_buffer(native.treeHeaderBuffer);
			data.top_down_view_node_buffer = read_gfx_wrapped_buffer(native.topDownViewNodeBuffer);
			data.internal_node_buffer = read_gfx_wrapped_buffer(native.internalNodeBuffer);
			return data;
		}

		void write_gfx_gpu_light_grid_voxel_tree(GfxGpuLightGridVoxelTree& native, const asset_data::gfxworld::gfx_gpu_light_grid_voxel_tree& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.topDownViewNodeCount = data.top_down_view_node_count;
			native.internalNodeCount = data.internal_node_count;
			native.treeHeader = plain::write_single<GfxVoxelTreeHeader>(data.tree_header, memory, [&](auto& out, const auto& in) { write_gfx_voxel_tree_header(out, in, resolve, memory); });
			native.topDownViewNodes = plain::write_array<GfxVoxelTopDownViewNode>(data.top_down_view_nodes, memory, [&](auto& out, const auto& in) { write_gfx_voxel_top_down_view_node(out, in, resolve, memory); });
			native.internalNodes = plain::write_array<GfxVoxelInternalNode>(data.internal_nodes, memory, [&](auto& out, const auto& in) { write_gfx_voxel_internal_node(out, in, resolve, memory); });
			write_gfx_wrapped_buffer(native.treeHeaderBuffer, data.tree_header_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.topDownViewNodeBuffer, data.top_down_view_node_buffer, resolve, memory);
			write_gfx_wrapped_buffer(native.internalNodeBuffer, data.internal_node_buffer, resolve, memory);
		}

		std::size_t size_gfx_gpu_light_grid_voxel_tree([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid_voxel_tree& data)
		{
			std::size_t size = 0;
			if (data.tree_header)
			{
				size += plain::sized(sizeof(GfxVoxelTreeHeader));
				size += size_gfx_voxel_tree_header(*data.tree_header);
			}
			if (data.top_down_view_nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.top_down_view_nodes->size(), 1) * sizeof(GfxVoxelTopDownViewNode));
				for (const auto& item : *data.top_down_view_nodes)
				{
					size += size_gfx_voxel_top_down_view_node(item);
				}
			}
			if (data.internal_nodes)
			{
				size += plain::sized(std::max<std::size_t>(data.internal_nodes->size(), 1) * sizeof(GfxVoxelInternalNode));
				for (const auto& item : *data.internal_nodes)
				{
					size += size_gfx_voxel_internal_node(item);
				}
			}
			size += size_gfx_wrapped_buffer(data.tree_header_buffer);
			size += size_gfx_wrapped_buffer(data.top_down_view_node_buffer);
			size += size_gfx_wrapped_buffer(data.internal_node_buffer);
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron read_gfx_gpu_light_grid_tetrahedron(const GfxGpuLightGridTetrahedron& native)
		{
			plain::trace("read_gfx_gpu_light_grid_tetrahedron");
			asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron data;
			data.index_flags = std::to_array(native.indexFlags);
			return data;
		}

		void write_gfx_gpu_light_grid_tetrahedron(GfxGpuLightGridTetrahedron& native, const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.index_flags, native.indexFlags);
		}

		std::size_t size_gfx_gpu_light_grid_tetrahedron([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_neighbors read_gfx_gpu_light_grid_tetrahedron_neighbors(const GfxGpuLightGridTetrahedronNeighbors& native)
		{
			plain::trace("read_gfx_gpu_light_grid_tetrahedron_neighbors");
			asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_neighbors data;
			data.neighbors = std::to_array(native.neighbors);
			return data;
		}

		void write_gfx_gpu_light_grid_tetrahedron_neighbors(GfxGpuLightGridTetrahedronNeighbors& native, const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_neighbors& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.neighbors, native.neighbors);
		}

		std::size_t size_gfx_gpu_light_grid_tetrahedron_neighbors([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_neighbors& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_visibility read_gfx_gpu_light_grid_tetrahedron_visibility(const GfxGpuLightGridTetrahedronVisibility& native)
		{
			plain::trace("read_gfx_gpu_light_grid_tetrahedron_visibility");
			asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_visibility data;
			data.visibility = std::to_array(native.visibility);
			return data;
		}

		void write_gfx_gpu_light_grid_tetrahedron_visibility(GfxGpuLightGridTetrahedronVisibility& native, const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_visibility& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::ranges::copy(data.visibility, native.visibility);
		}

		std::size_t size_gfx_gpu_light_grid_tetrahedron_visibility([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid_tetrahedron_visibility& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_gpu_light_grid_voxel_start_tetrahedron read_gfx_gpu_light_grid_voxel_start_tetrahedron(const GfxGpuLightGridVoxelStartTetrahedron& native)
		{
			plain::trace("read_gfx_gpu_light_grid_voxel_start_tetrahedron");
			asset_data::gfxworld::gfx_gpu_light_grid_voxel_start_tetrahedron data;
			data.index = native.index;
			return data;
		}

		void write_gfx_gpu_light_grid_voxel_start_tetrahedron(GfxGpuLightGridVoxelStartTetrahedron& native, const asset_data::gfxworld::gfx_gpu_light_grid_voxel_start_tetrahedron& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.index = data.index;
		}

		std::size_t size_gfx_gpu_light_grid_voxel_start_tetrahedron([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_light_grid_voxel_start_tetrahedron& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_model_light_grid_volume read_gfx_static_model_light_grid_volume(const GfxStaticModelLightGridVolume& native)
		{
			plain::trace("read_gfx_static_model_light_grid_volume");
			asset_data::gfxworld::gfx_static_model_light_grid_volume data;
			data.first_probe = native.firstProbe;
			data.m_volume_size = std::to_array(native.m_volumeSize);
			data.flags = static_cast<std::uint16_t>(native.flags);
			data.single_probe = native.singleProbe;
			data.volume_transform = [&] { std::array<std::array<float, 4>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.volumeTransform[i]); } return out; }();
			return data;
		}

		void write_gfx_static_model_light_grid_volume(GfxStaticModelLightGridVolume& native, const asset_data::gfxworld::gfx_static_model_light_grid_volume& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.firstProbe = data.first_probe;
			std::ranges::copy(data.m_volume_size, native.m_volumeSize);
			native.flags = static_cast<GfxStaticModelLightGridVolumeFlags>(data.flags);
			native.singleProbe = data.single_probe;
			for (std::size_t i = 0; i < data.volume_transform.size(); ++i)
			{
				plain::write_scalars(&native.volumeTransform[i], data.volume_transform[i]);
			}
		}

		std::size_t size_gfx_static_model_light_grid_volume([[maybe_unused]] const asset_data::gfxworld::gfx_static_model_light_grid_volume& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_gpu_dyn_lightsets_lgv_reference read_gfx_gpu_dyn_lightsets_lgv_reference(const GfxGpuDynLightsetsLGVReference& native)
		{
			plain::trace("read_gfx_gpu_dyn_lightsets_lgv_reference");
			asset_data::gfxworld::gfx_gpu_dyn_lightsets_lgv_reference data;
			data.volume_index = native.volumeIndex;
			data.volume_probe_index = native.volumeProbeIndex;
			return data;
		}

		void write_gfx_gpu_dyn_lightsets_lgv_reference(GfxGpuDynLightsetsLGVReference& native, const asset_data::gfxworld::gfx_gpu_dyn_lightsets_lgv_reference& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.volumeIndex = data.volume_index;
			native.volumeProbeIndex = data.volume_probe_index;
		}

		std::size_t size_gfx_gpu_dyn_lightsets_lgv_reference([[maybe_unused]] const asset_data::gfxworld::gfx_gpu_dyn_lightsets_lgv_reference& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::light_grid_volume_packing_params read_light_grid_volume_packing_params(const LightGridVolumePackingParams& native)
		{
			plain::trace("read_light_grid_volume_packing_params");
			asset_data::gfxworld::light_grid_volume_packing_params data;
			data.m_uv_scale = plain::read_scalars<float, 3>(&native.m_uvScale);
			data.m_volume_size = std::to_array(native.m_volumeSize);
			data.m_basis_type = native.m_basisType;
			data.m_uv_bias = plain::read_scalars<float, 3>(&native.m_uvBias);
			data.m_first_static_probe_index = native.m_firstStaticProbeIndex;
			return data;
		}

		void write_light_grid_volume_packing_params(LightGridVolumePackingParams& native, const asset_data::gfxworld::light_grid_volume_packing_params& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.m_uvScale, data.m_uv_scale);
			std::ranges::copy(data.m_volume_size, native.m_volumeSize);
			native.m_basisType = data.m_basis_type;
			plain::write_scalars(&native.m_uvBias, data.m_uv_bias);
			native.m_firstStaticProbeIndex = data.m_first_static_probe_index;
		}

		std::size_t size_light_grid_volume_packing_params([[maybe_unused]] const asset_data::gfxworld::light_grid_volume_packing_params& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::light_grid_volume_transform_params read_light_grid_volume_transform_params(const LightGridVolumeTransformParams& native)
		{
			plain::trace("read_light_grid_volume_transform_params");
			asset_data::gfxworld::light_grid_volume_transform_params data;
			data.m_volume_transform = [&] { std::array<std::array<float, 4>, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = plain::read_scalars<float, 4>(&native.m_volumeTransform[i]); } return out; }();
			data.m_history_entry_or_static_flags = native.m_historyEntryOrStaticFlags;
			data.m_last_frame_history_entry_and_setup = native.m_lastFrameHistoryEntryAndSetup;
			data.m_lod_and_sampling_offset_z = native.m_lodAndSamplingOffsetZ;
			data.m_sampling_offset_xy = native.m_samplingOffsetXY;
			return data;
		}

		void write_light_grid_volume_transform_params(LightGridVolumeTransformParams& native, const asset_data::gfxworld::light_grid_volume_transform_params& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			for (std::size_t i = 0; i < data.m_volume_transform.size(); ++i)
			{
				plain::write_scalars(&native.m_volumeTransform[i], data.m_volume_transform[i]);
			}
			native.m_historyEntryOrStaticFlags = data.m_history_entry_or_static_flags;
			native.m_lastFrameHistoryEntryAndSetup = data.m_last_frame_history_entry_and_setup;
			native.m_lodAndSamplingOffsetZ = data.m_lod_and_sampling_offset_z;
			native.m_samplingOffsetXY = data.m_sampling_offset_xy;
		}

		std::size_t size_light_grid_volume_transform_params([[maybe_unused]] const asset_data::gfxworld::light_grid_volume_transform_params& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_image_reflection_probe read_gfx_image_reflection_probe(const GfxImageReflectionProbe& native)
		{
			plain::trace("read_gfx_image_reflection_probe");
			asset_data::gfxworld::gfx_image_reflection_probe data;
			data.total_size = native.totalSize;
			data.element_count = native.elementCount;
			data.texture_id = plain::enum_name(asset_data::gfxworld::gfx_texture_id_names, static_cast<std::int64_t>(native.textureId), "GfxImageReflectionProbe.textureId");
			data.pixels = plain::read_array_if_present(native.pixels, native.totalSize, "GfxImageReflectionProbe.pixels", [&](const auto& item) { return item; });
			return data;
		}

		void write_gfx_image_reflection_probe(GfxImageReflectionProbe& native, const asset_data::gfxworld::gfx_image_reflection_probe& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.totalSize = data.total_size;
			native.elementCount = data.element_count;
			native.textureId = static_cast<GfxTextureId>(plain::enum_value(asset_data::gfxworld::gfx_texture_id_names, data.texture_id, "GfxImageReflectionProbe.textureId"));
			native.pixels = plain::write_array<std::uint8_t>(data.pixels, memory, [&](auto& out, const auto& in) { out = in; });
		}

		std::size_t size_gfx_image_reflection_probe([[maybe_unused]] const asset_data::gfxworld::gfx_image_reflection_probe& data)
		{
			std::size_t size = 0;
			if (data.pixels)
			{
				size += plain::sized(std::max<std::size_t>(data.pixels->size(), 1) * sizeof(std::uint8_t));
			}
			return size;
		}

		asset_data::gfxworld::gfx_static_decal_volume_packed read_gfx_static_decal_volume_packed(const GfxStaticDecalVolumePacked& native)
		{
			plain::trace("read_gfx_static_decal_volume_packed");
			asset_data::gfxworld::gfx_static_decal_volume_packed data;
			data.obb_center = plain::read_scalars<float, 3>(&native.obbCenter);
			data.obb_quat = native.obbQuat;
			data.obb_half_size = native.obbHalfSize;
			data.opacity_params = native.opacityParams;
			data.uv_matrix_and_offset = std::to_array(native.uvMatrixAndOffset);
			data.packed_material_info = native.packedMaterialInfo;
			data.reserved = native.reserved;
			return data;
		}

		void write_gfx_static_decal_volume_packed(GfxStaticDecalVolumePacked& native, const asset_data::gfxworld::gfx_static_decal_volume_packed& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.obbCenter, data.obb_center);
			native.obbQuat = data.obb_quat;
			native.obbHalfSize = data.obb_half_size;
			native.opacityParams = data.opacity_params;
			std::ranges::copy(data.uv_matrix_and_offset, native.uvMatrixAndOffset);
			native.packedMaterialInfo = data.packed_material_info;
			native.reserved = data.reserved;
		}

		std::size_t size_gfx_static_decal_volume_packed([[maybe_unused]] const asset_data::gfxworld::gfx_static_decal_volume_packed& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::gfxworld::gfx_static_decal_volume_uid read_gfx_static_decal_volume_uid(const GfxStaticDecalVolumeUid& native)
		{
			plain::trace("read_gfx_static_decal_volume_uid");
			asset_data::gfxworld::gfx_static_decal_volume_uid data;
			data.uid = native.uid;
			return data;
		}

		void write_gfx_static_decal_volume_uid(GfxStaticDecalVolumeUid& native, const asset_data::gfxworld::gfx_static_decal_volume_uid& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.uid = data.uid;
		}

		std::size_t size_gfx_static_decal_volume_uid([[maybe_unused]] const asset_data::gfxworld::gfx_static_decal_volume_uid& data)
		{
			std::size_t size = 0;
			return size;
		}

	}

	std::expected<asset_data::gfxworld::gfx_world, std::string> to_neutral(const GfxWorld& native)
	{
		try
		{
			plain::pointer_ids ids;
			auto data = read_gfx_world(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GfxWorld* from_neutral(const asset_data::gfxworld::gfx_world& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GfxWorld>();
		write_gfx_world(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gfxworld::gfx_world& data)
	{
		return plain::rounded(sizeof(GfxWorld)) + size_gfx_world(data) + 64;
	}

	std::expected<asset_data::gfxworld::gfx_world_transient_zone, std::string> to_neutral(const GfxWorldTransientZone& native)
	{
		try
		{
			plain::pointer_ids ids;
			auto data = read_gfx_world_transient_zone(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	GfxWorldTransientZone* from_neutral(const asset_data::gfxworld::gfx_world_transient_zone& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<GfxWorldTransientZone>();
		write_gfx_world_transient_zone(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::gfxworld::gfx_world_transient_zone& data)
	{
		return plain::rounded(sizeof(GfxWorldTransientZone)) + size_gfx_world_transient_zone(data) + 64;
	}

}
