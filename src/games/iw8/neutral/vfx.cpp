// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/vfx.json: do not edit.
#include <std_include.hpp>
#include "vfx.hpp"
#include "plain.hpp"
#include "particle.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::vfx::particle_module read_particle_module(const ParticleModule& native)
		{
			asset_data::vfx::particle_module data;
			data.m_type = plain::enum_name(asset_data::vfx::particle_module_type_names, static_cast<std::int64_t>(native.m_type), "ParticleModule.m_type");
			data.m_flags = native.m_flags;
			return data;
		}

		void write_particle_module(ParticleModule& native, const asset_data::vfx::particle_module& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_type = static_cast<ParticleModuleType>(plain::enum_value(asset_data::vfx::particle_module_type_names, data.m_type, "ParticleModule.m_type"));
			native.m_flags = data.m_flags;
		}

		std::size_t size_particle_module([[maybe_unused]] const asset_data::vfx::particle_module& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::vfx::particle_curve_control_point_def read_particle_curve_control_point_def(const ParticleCurveControlPointDef& native)
		{
			asset_data::vfx::particle_curve_control_point_def data;
			data.time = native.time;
			data.value = native.value;
			data.inv_time_delta = native.invTimeDelta;
			plain::require_zero(native.pad, "ParticleCurveControlPointDef.pad");
			return data;
		}

		void write_particle_curve_control_point_def(ParticleCurveControlPointDef& native, const asset_data::vfx::particle_curve_control_point_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.time = data.time;
			native.value = data.value;
			native.invTimeDelta = data.inv_time_delta;
		}

		std::size_t size_particle_curve_control_point_def([[maybe_unused]] const asset_data::vfx::particle_curve_control_point_def& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::vfx::particle_curve_def read_particle_curve_def(const ParticleCurveDef& native)
		{
			asset_data::vfx::particle_curve_def data;
			data.control_points = plain::read_array(native.controlPoints, native.numControlPoints, "ParticleCurveDef.controlPoints", [&](const auto& item) { return read_particle_curve_control_point_def(item); });
			data.scale = native.scale;
			return data;
		}

		void write_particle_curve_def(ParticleCurveDef& native, const asset_data::vfx::particle_curve_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.controlPoints = plain::write_array<ParticleCurveControlPointDef>(data.control_points, memory, [&](auto& out, const auto& in) { write_particle_curve_control_point_def(out, in, resolve, memory); });
			native.scale = data.scale;
			native.numControlPoints = plain::count_of<std::int32_t>(data.control_points);
		}

		std::size_t size_particle_curve_def([[maybe_unused]] const asset_data::vfx::particle_curve_def& data)
		{
			std::size_t size = 0;
			if (data.control_points)
			{
				size += plain::sized(std::max<std::size_t>(data.control_points->size(), 1) * sizeof(ParticleCurveControlPointDef));
				for (const auto& item : *data.control_points)
				{
					size += size_particle_curve_control_point_def(item);
				}
			}
			return size;
		}

		asset_data::vfx::particle_module_init_atlas read_particle_module_init_atlas(const ParticleModuleInitAtlas& native)
		{
			asset_data::vfx::particle_module_init_atlas data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_start_frame = native.m_startFrame;
			data.m_loop_count = native.m_loopCount;
			data.m_random_index = native.m_randomIndex;
			data.m_play_over_life = native.m_playOverLife;
			plain::require_zero(native.m_pad, "ParticleModuleInitAtlas.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_atlas(ParticleModuleInitAtlas& native, const asset_data::vfx::particle_module_init_atlas& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_startFrame = data.m_start_frame;
			native.m_loopCount = data.m_loop_count;
			native.m_randomIndex = data.m_random_index;
			native.m_playOverLife = data.m_play_over_life;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_atlas([[maybe_unused]] const asset_data::vfx::particle_module_init_atlas& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_attributes read_particle_module_init_attributes(const ParticleModuleInitAttributes& native)
		{
			asset_data::vfx::particle_module_init_attributes data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_use_non_uniform_interpolation_for_color = native.m_useNonUniformInterpolationForColor;
			data.m_use_non_uniform_interpolation_for_size = native.m_useNonUniformInterpolationForSize;
			plain::require_zero(native.m_pad, "ParticleModuleInitAttributes.m_pad");
			data.m_size_min = plain::read_scalars<float, 4>(&native.m_sizeMin);
			data.m_size_max = plain::read_scalars<float, 4>(&native.m_sizeMax);
			data.m_color_min = plain::read_scalars<float, 4>(&native.m_colorMin);
			data.m_color_max = plain::read_scalars<float, 4>(&native.m_colorMax);
			data.m_velocity_min = plain::read_scalars<float, 4>(&native.m_velocityMin);
			data.m_velocity_max = plain::read_scalars<float, 4>(&native.m_velocityMax);
			return data;
		}

		void write_particle_module_init_attributes(ParticleModuleInitAttributes& native, const asset_data::vfx::particle_module_init_attributes& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_useNonUniformInterpolationForColor = data.m_use_non_uniform_interpolation_for_color;
			native.m_useNonUniformInterpolationForSize = data.m_use_non_uniform_interpolation_for_size;
			plain::write_scalars(&native.m_sizeMin, data.m_size_min);
			plain::write_scalars(&native.m_sizeMax, data.m_size_max);
			plain::write_scalars(&native.m_colorMin, data.m_color_min);
			plain::write_scalars(&native.m_colorMax, data.m_color_max);
			plain::write_scalars(&native.m_velocityMin, data.m_velocity_min);
			plain::write_scalars(&native.m_velocityMax, data.m_velocity_max);
		}

		std::size_t size_particle_module_init_attributes([[maybe_unused]] const asset_data::vfx::particle_module_init_attributes& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_beam read_particle_module_init_beam(const ParticleModuleInitBeam& native)
		{
			asset_data::vfx::particle_module_init_beam data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_beam_flags = plain::enum_name(asset_data::vfx::particle_module_init_beam_flags_names, static_cast<std::int64_t>(native.m_beamFlags), "ParticleModuleInitBeam.m_beamFlags");
			data.m_split_distance = native.m_splitDistance;
			data.m_start_pos = plain::read_scalars<float, 4>(&native.m_startPos);
			data.m_end_pos = plain::read_scalars<float, 4>(&native.m_endPos);
			data.m_offset = plain::read_scalars<float, 4>(&native.m_offset);
			data.m_curve_point1 = plain::read_scalars<float, 4>(&native.m_curvePoint1);
			data.m_curve_point2 = plain::read_scalars<float, 4>(&native.m_curvePoint2);
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_scroll_rate_uv = native.m_scrollRateUV;
			data.m_scroll_rate_spiral_graph = native.m_scrollRateSpiralGraph;
			data.m_spiral_repeat_distance = native.m_spiralRepeatDistance;
			return data;
		}

		void write_particle_module_init_beam(ParticleModuleInitBeam& native, const asset_data::vfx::particle_module_init_beam& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_beamFlags = static_cast<ParticleModuleInitBeamFlags>(plain::enum_value(asset_data::vfx::particle_module_init_beam_flags_names, data.m_beam_flags, "ParticleModuleInitBeam.m_beamFlags"));
			native.m_splitDistance = data.m_split_distance;
			plain::write_scalars(&native.m_startPos, data.m_start_pos);
			plain::write_scalars(&native.m_endPos, data.m_end_pos);
			plain::write_scalars(&native.m_offset, data.m_offset);
			plain::write_scalars(&native.m_curvePoint1, data.m_curve_point1);
			plain::write_scalars(&native.m_curvePoint2, data.m_curve_point2);
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			native.m_scrollRateUV = data.m_scroll_rate_uv;
			native.m_scrollRateSpiralGraph = data.m_scroll_rate_spiral_graph;
			native.m_spiralRepeatDistance = data.m_spiral_repeat_distance;
		}

		std::size_t size_particle_module_init_beam([[maybe_unused]] const asset_data::vfx::particle_module_init_beam& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_module_init_camera_offset read_particle_module_init_camera_offset(const ParticleModuleInitCameraOffset& native)
		{
			asset_data::vfx::particle_module_init_camera_offset data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitCameraOffset.m_pad");
			return data;
		}

		void write_particle_module_init_camera_offset(ParticleModuleInitCameraOffset& native, const asset_data::vfx::particle_module_init_camera_offset& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
		}

		std::size_t size_particle_module_init_camera_offset([[maybe_unused]] const asset_data::vfx::particle_module_init_camera_offset& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_cloud read_particle_module_init_cloud(const ParticleModuleInitCloud& native)
		{
			asset_data::vfx::particle_module_init_cloud data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitCloud.m_pad");
			return data;
		}

		void write_particle_module_init_cloud(ParticleModuleInitCloud& native, const asset_data::vfx::particle_module_init_cloud& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
		}

		std::size_t size_particle_module_init_cloud([[maybe_unused]] const asset_data::vfx::particle_module_init_cloud& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_decal read_particle_module_init_decal(const ParticleModuleInitDecal& native)
		{
			asset_data::vfx::particle_module_init_decal data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_fade_in_time = native.m_fadeInTime;
			data.m_fade_out_time = native.m_fadeOutTime;
			data.m_stoppable_fade_out_time = native.m_stoppableFadeOutTime;
			data.m_lerp_wait_time = native.m_lerpWaitTime;
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_lerp_color = plain::read_scalars<float, 4>(&native.m_lerpColor);
			data.m_lerp_time = native.m_lerpTime;
			data.m_dynamic_decal = native.m_dynamicDecal;
			data.m_projection_axis = plain::enum_name(asset_data::vfx::fx_mark_projection_axis_names, static_cast<std::int64_t>(native.m_projectionAxis), "ParticleModuleInitDecal.m_projectionAxis");
			data.m_bypass_stacking_limiter = native.m_bypassStackingLimiter;
			plain::require_zero(native.m_pad, "ParticleModuleInitDecal.m_pad");
			return data;
		}

		void write_particle_module_init_decal(ParticleModuleInitDecal& native, const asset_data::vfx::particle_module_init_decal& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_fadeInTime = data.m_fade_in_time;
			native.m_fadeOutTime = data.m_fade_out_time;
			native.m_stoppableFadeOutTime = data.m_stoppable_fade_out_time;
			native.m_lerpWaitTime = data.m_lerp_wait_time;
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			plain::write_scalars(&native.m_lerpColor, data.m_lerp_color);
			native.m_lerpTime = data.m_lerp_time;
			native.m_dynamicDecal = data.m_dynamic_decal;
			native.m_projectionAxis = static_cast<FxMarkProjectionAxis>(plain::enum_value(asset_data::vfx::fx_mark_projection_axis_names, data.m_projection_axis, "ParticleModuleInitDecal.m_projectionAxis"));
			native.m_bypassStackingLimiter = data.m_bypass_stacking_limiter;
		}

		std::size_t size_particle_module_init_decal([[maybe_unused]] const asset_data::vfx::particle_module_init_decal& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_float_range read_particle_float_range(const ParticleFloatRange& native)
		{
			asset_data::vfx::particle_float_range data;
			data.min = native.min;
			data.max = native.max;
			return data;
		}

		void write_particle_float_range(ParticleFloatRange& native, const asset_data::vfx::particle_float_range& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.min = data.min;
			native.max = data.max;
		}

		std::size_t size_particle_float_range([[maybe_unused]] const asset_data::vfx::particle_float_range& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::vfx::particle_module_init_flare read_particle_module_init_flare(const ParticleModuleInitFlare& native)
		{
			asset_data::vfx::particle_module_init_flare data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitFlare.m_pad");
			data.m_position = native.m_position;
			data.m_direction = plain::read_scalars<float, 4>(&native.m_direction);
			data.m_angular_rot_count = native.m_angularRotCount;
			data.m_flare_flags = native.m_flareFlags;
			data.m_depth_scale_range = read_particle_float_range(native.m_depthScaleRange);
			data.m_depth_scale_value = read_particle_float_range(native.m_depthScaleValue);
			data.m_radial_rot = read_particle_float_range(native.m_radialRot);
			data.m_radial_scale_x = read_particle_float_range(native.m_radialScaleX);
			data.m_radial_scale_y = read_particle_float_range(native.m_radialScaleY);
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 4> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_flare(ParticleModuleInitFlare& native, const asset_data::vfx::particle_module_init_flare& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_position = data.m_position;
			plain::write_scalars(&native.m_direction, data.m_direction);
			native.m_angularRotCount = data.m_angular_rot_count;
			native.m_flareFlags = data.m_flare_flags;
			write_particle_float_range(native.m_depthScaleRange, data.m_depth_scale_range, resolve, memory);
			write_particle_float_range(native.m_depthScaleValue, data.m_depth_scale_value, resolve, memory);
			write_particle_float_range(native.m_radialRot, data.m_radial_rot, resolve, memory);
			write_particle_float_range(native.m_radialScaleX, data.m_radial_scale_x, resolve, memory);
			write_particle_float_range(native.m_radialScaleY, data.m_radial_scale_y, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_flare([[maybe_unused]] const asset_data::vfx::particle_module_init_flare& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_float_range(data.m_depth_scale_range);
			size += size_particle_float_range(data.m_depth_scale_value);
			size += size_particle_float_range(data.m_radial_rot);
			size += size_particle_float_range(data.m_radial_scale_x);
			size += size_particle_float_range(data.m_radial_scale_y);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_geo_trail read_particle_module_init_geo_trail(const ParticleModuleInitGeoTrail& native)
		{
			asset_data::vfx::particle_module_init_geo_trail data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_num_points_max = native.m_numPointsMax;
			data.m_split_distance = native.m_splitDistance;
			data.m_split_angle = native.m_splitAngle;
			data.m_center_offset = native.m_centerOffset;
			data.m_num_sheets = native.m_numSheets;
			data.m_fade_in_distance = native.m_fadeInDistance;
			data.m_fade_out_distance = native.m_fadeOutDistance;
			data.m_tile_distance = native.m_tileDistance;
			data.m_tile_offset = read_particle_float_range(native.m_tileOffset);
			data.m_scroll_time = native.m_scrollTime;
			data.m_use_local_velocity = native.m_useLocalVelocity;
			data.m_use_vertical_texture = native.m_useVerticalTexture;
			data.m_camera_facing = native.m_cameraFacing;
			data.m_fix_leading_edge = native.m_fixLeadingEdge;
			data.m_clamp_u_vs = native.m_clampUVs;
			plain::require_zero(native.m_pad, "ParticleModuleInitGeoTrail.m_pad");
			plain::require_zero(native.m_pad2, "ParticleModuleInitGeoTrail.m_pad2");
			return data;
		}

		void write_particle_module_init_geo_trail(ParticleModuleInitGeoTrail& native, const asset_data::vfx::particle_module_init_geo_trail& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_numPointsMax = data.m_num_points_max;
			native.m_splitDistance = data.m_split_distance;
			native.m_splitAngle = data.m_split_angle;
			native.m_centerOffset = data.m_center_offset;
			native.m_numSheets = data.m_num_sheets;
			native.m_fadeInDistance = data.m_fade_in_distance;
			native.m_fadeOutDistance = data.m_fade_out_distance;
			native.m_tileDistance = data.m_tile_distance;
			write_particle_float_range(native.m_tileOffset, data.m_tile_offset, resolve, memory);
			native.m_scrollTime = data.m_scroll_time;
			native.m_useLocalVelocity = data.m_use_local_velocity;
			native.m_useVerticalTexture = data.m_use_vertical_texture;
			native.m_cameraFacing = data.m_camera_facing;
			native.m_fixLeadingEdge = data.m_fix_leading_edge;
			native.m_clampUVs = data.m_clamp_u_vs;
		}

		std::size_t size_particle_module_init_geo_trail([[maybe_unused]] const asset_data::vfx::particle_module_init_geo_trail& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_float_range(data.m_tile_offset);
			return size;
		}

		asset_data::vfx::particle_module_init_kill_wrap_box read_particle_module_init_kill_wrap_box(const ParticleModuleInitKillWrapBox& native)
		{
			asset_data::vfx::particle_module_init_kill_wrap_box data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_planes_kill = plain::flag_names(asset_data::vfx::particle_module_axes_flags_names, static_cast<std::uint64_t>(native.m_planesKill), "ParticleModuleInitKillWrapBox.m_planesKill");
			plain::require_zero(native.m_pad, "ParticleModuleInitKillWrapBox.m_pad");
			data.m_bounds = plain::read_scalars<float, 4>(&native.m_bounds);
			data.m_offset = plain::read_scalars<float, 4>(&native.m_offset);
			return data;
		}

		void write_particle_module_init_kill_wrap_box(ParticleModuleInitKillWrapBox& native, const asset_data::vfx::particle_module_init_kill_wrap_box& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_planesKill = static_cast<ParticleModuleAxesFlags>(plain::flag_value(asset_data::vfx::particle_module_axes_flags_names, data.m_planes_kill, "ParticleModuleInitKillWrapBox.m_planesKill"));
			plain::write_scalars(&native.m_bounds, data.m_bounds);
			plain::write_scalars(&native.m_offset, data.m_offset);
		}

		std::size_t size_particle_module_init_kill_wrap_box([[maybe_unused]] const asset_data::vfx::particle_module_init_kill_wrap_box& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_light_omni read_particle_module_init_light_omni(const ParticleModuleInitLightOmni& native)
		{
			asset_data::vfx::particle_module_init_light_omni data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_fov_outer = native.m_fovOuter;
			data.m_fov_inner = native.m_fovInner;
			data.m_bulb_radius = native.m_bulbRadius;
			data.m_bulb_length = native.m_bulbLength;
			data.m_distance_falloff = native.m_distanceFalloff;
			data.m_brightness = native.m_brightness;
			data.m_intensity_uv = native.m_intensityUV;
			data.m_intensity_ir = native.m_intensityIR;
			data.m_intensity_heat = native.m_intensityHeat;
			data.m_shadow_softness = native.m_shadowSoftness;
			data.m_shadow_bias = native.m_shadowBias;
			data.m_shadow_area = native.m_shadowArea;
			data.m_tone_mapping_scale_factor = native.m_toneMappingScaleFactor;
			data.m_disable_volumetric = native.m_disableVolumetric;
			data.m_disable_shadow_map = native.m_disableShadowMap;
			data.m_disable_dynamic_shadows = native.m_disableDynamicShadows;
			data.m_script_scale = native.m_scriptScale;
			return data;
		}

		void write_particle_module_init_light_omni(ParticleModuleInitLightOmni& native, const asset_data::vfx::particle_module_init_light_omni& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			native.m_fovOuter = data.m_fov_outer;
			native.m_fovInner = data.m_fov_inner;
			native.m_bulbRadius = data.m_bulb_radius;
			native.m_bulbLength = data.m_bulb_length;
			native.m_distanceFalloff = data.m_distance_falloff;
			native.m_brightness = data.m_brightness;
			native.m_intensityUV = data.m_intensity_uv;
			native.m_intensityIR = data.m_intensity_ir;
			native.m_intensityHeat = data.m_intensity_heat;
			native.m_shadowSoftness = data.m_shadow_softness;
			native.m_shadowBias = data.m_shadow_bias;
			native.m_shadowArea = data.m_shadow_area;
			native.m_toneMappingScaleFactor = data.m_tone_mapping_scale_factor;
			native.m_disableVolumetric = data.m_disable_volumetric;
			native.m_disableShadowMap = data.m_disable_shadow_map;
			native.m_disableDynamicShadows = data.m_disable_dynamic_shadows;
			native.m_scriptScale = data.m_script_scale;
		}

		std::size_t size_particle_module_init_light_omni([[maybe_unused]] const asset_data::vfx::particle_module_init_light_omni& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_module_init_light_spot read_particle_module_init_light_spot(const ParticleModuleInitLightSpot& native)
		{
			asset_data::vfx::particle_module_init_light_spot data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_fov_outer = native.m_fovOuter;
			data.m_fov_inner = native.m_fovInner;
			data.m_bulb_radius = native.m_bulbRadius;
			data.m_bulb_length = native.m_bulbLength;
			data.m_distance_falloff = native.m_distanceFalloff;
			data.m_fov_collimation = native.m_fovCollimation;
			data.m_brightness = native.m_brightness;
			data.m_intensity_uv = native.m_intensityUV;
			data.m_intensity_ir = native.m_intensityIR;
			data.m_intensity_heat = native.m_intensityHeat;
			data.m_shadow_softness = native.m_shadowSoftness;
			data.m_shadow_bias = native.m_shadowBias;
			data.m_shadow_area = native.m_shadowArea;
			data.m_shadow_near_plane = native.m_shadowNearPlane;
			data.m_tone_mapping_scale_factor = native.m_toneMappingScaleFactor;
			data.m_disable_volumetric = native.m_disableVolumetric;
			data.m_disable_shadow_map = native.m_disableShadowMap;
			data.m_disable_dynamic_shadows = native.m_disableDynamicShadows;
			data.m_script_scale = native.m_scriptScale;
			plain::require_zero(native.m_pad, "ParticleModuleInitLightSpot.m_pad");
			return data;
		}

		void write_particle_module_init_light_spot(ParticleModuleInitLightSpot& native, const asset_data::vfx::particle_module_init_light_spot& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			native.m_fovOuter = data.m_fov_outer;
			native.m_fovInner = data.m_fov_inner;
			native.m_bulbRadius = data.m_bulb_radius;
			native.m_bulbLength = data.m_bulb_length;
			native.m_distanceFalloff = data.m_distance_falloff;
			native.m_fovCollimation = data.m_fov_collimation;
			native.m_brightness = data.m_brightness;
			native.m_intensityUV = data.m_intensity_uv;
			native.m_intensityIR = data.m_intensity_ir;
			native.m_intensityHeat = data.m_intensity_heat;
			native.m_shadowSoftness = data.m_shadow_softness;
			native.m_shadowBias = data.m_shadow_bias;
			native.m_shadowArea = data.m_shadow_area;
			native.m_shadowNearPlane = data.m_shadow_near_plane;
			native.m_toneMappingScaleFactor = data.m_tone_mapping_scale_factor;
			native.m_disableVolumetric = data.m_disable_volumetric;
			native.m_disableShadowMap = data.m_disable_shadow_map;
			native.m_disableDynamicShadows = data.m_disable_dynamic_shadows;
			native.m_scriptScale = data.m_script_scale;
		}

		std::size_t size_particle_module_init_light_spot([[maybe_unused]] const asset_data::vfx::particle_module_init_light_spot& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_material_data read_particle_material_data(const Particle_MaterialData& native)
		{
			asset_data::vfx::particle_material_data data;
			data.falloff_parms = plain::read_scalars<float, 4>(&native.falloffParms);
			data.opaque_color = plain::read_scalars<float, 4>(&native.opaqueColor);
			data.transparent_color = plain::read_scalars<float, 4>(&native.transparentColor);
			data.translucent_shadow_params = plain::read_scalars<float, 2>(&native.translucentShadowParams);
			data.beam_parms = plain::read_scalars<float, 2>(&native.beamParms);
			data.eye_offset_parms = plain::read_scalars<float, 4>(&native.eyeOffsetParms);
			data.alpha_dissolve_parms = plain::read_scalars<float, 4>(&native.alphaDissolveParms);
			data.feather_parms0 = plain::read_scalars<float, 4>(&native.featherParms0);
			data.feather_parms1 = plain::read_scalars<float, 4>(&native.featherParms1);
			data.region_highlight_parms = plain::read_scalars<float, 4>(&native.regionHighlightParms);
			data.depth_scan_color = plain::read_scalars<float, 4>(&native.depthScanColor);
			data.depth_scan_outline_color = plain::read_scalars<float, 4>(&native.depthScanOutlineColor);
			data.thermal_params = plain::read_scalars<float, 2>(&native.thermalParams);
			data.anisotropy = native.anisotropy;
			data.feature_enabled_flags = native.featureEnabledFlags;
			return data;
		}

		void write_particle_material_data(Particle_MaterialData& native, const asset_data::vfx::particle_material_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.falloffParms, data.falloff_parms);
			plain::write_scalars(&native.opaqueColor, data.opaque_color);
			plain::write_scalars(&native.transparentColor, data.transparent_color);
			plain::write_scalars(&native.translucentShadowParams, data.translucent_shadow_params);
			plain::write_scalars(&native.beamParms, data.beam_parms);
			plain::write_scalars(&native.eyeOffsetParms, data.eye_offset_parms);
			plain::write_scalars(&native.alphaDissolveParms, data.alpha_dissolve_parms);
			plain::write_scalars(&native.featherParms0, data.feather_parms0);
			plain::write_scalars(&native.featherParms1, data.feather_parms1);
			plain::write_scalars(&native.regionHighlightParms, data.region_highlight_parms);
			plain::write_scalars(&native.depthScanColor, data.depth_scan_color);
			plain::write_scalars(&native.depthScanOutlineColor, data.depth_scan_outline_color);
			plain::write_scalars(&native.thermalParams, data.thermal_params);
			native.anisotropy = data.anisotropy;
			native.featureEnabledFlags = data.feature_enabled_flags;
		}

		std::size_t size_particle_material_data([[maybe_unused]] const asset_data::vfx::particle_material_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::vfx::particle_module_init_material read_particle_module_init_material(const ParticleModuleInitMaterial& native)
		{
			asset_data::vfx::particle_module_init_material data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_render_options = plain::enum_name(asset_data::vfx::particle_render_options_names, static_cast<std::int64_t>(native.m_renderOptions), "ParticleModuleInitMaterial.m_renderOptions");
			data.m_shader_graph_options = plain::enum_name(asset_data::vfx::particle_shader_graph_options_names, static_cast<std::int64_t>(native.m_shaderGraphOptions), "ParticleModuleInitMaterial.m_shaderGraphOptions");
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_material_data = read_particle_material_data(native.m_materialData);
			return data;
		}

		void write_particle_module_init_material(ParticleModuleInitMaterial& native, const asset_data::vfx::particle_module_init_material& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_renderOptions = static_cast<ParticleRenderOptions>(plain::enum_value(asset_data::vfx::particle_render_options_names, data.m_render_options, "ParticleModuleInitMaterial.m_renderOptions"));
			native.m_shaderGraphOptions = static_cast<ParticleShaderGraphOptions>(plain::enum_value(asset_data::vfx::particle_shader_graph_options_names, data.m_shader_graph_options, "ParticleModuleInitMaterial.m_shaderGraphOptions"));
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			write_particle_material_data(native.m_materialData, data.m_material_data, resolve, memory);
		}

		std::size_t size_particle_module_init_material([[maybe_unused]] const asset_data::vfx::particle_module_init_material& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			size += size_particle_material_data(data.m_material_data);
			return size;
		}

		asset_data::vfx::particle_module_init_mirror_texture read_particle_module_init_mirror_texture(const ParticleModuleInitMirrorTexture& native)
		{
			asset_data::vfx::particle_module_init_mirror_texture data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_mirror_horizontal = plain::enum_name(asset_data::vfx::particle_mirror_texture_type_names, static_cast<std::int64_t>(native.m_mirrorHorizontal), "ParticleModuleInitMirrorTexture.m_mirrorHorizontal");
			data.m_mirror_vertical = plain::enum_name(asset_data::vfx::particle_mirror_texture_type_names, static_cast<std::int64_t>(native.m_mirrorVertical), "ParticleModuleInitMirrorTexture.m_mirrorVertical");
			return data;
		}

		void write_particle_module_init_mirror_texture(ParticleModuleInitMirrorTexture& native, const asset_data::vfx::particle_module_init_mirror_texture& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_mirrorHorizontal = static_cast<ParticleMirrorTextureType>(plain::enum_value(asset_data::vfx::particle_mirror_texture_type_names, data.m_mirror_horizontal, "ParticleModuleInitMirrorTexture.m_mirrorHorizontal"));
			native.m_mirrorVertical = static_cast<ParticleMirrorTextureType>(plain::enum_value(asset_data::vfx::particle_mirror_texture_type_names, data.m_mirror_vertical, "ParticleModuleInitMirrorTexture.m_mirrorVertical"));
		}

		std::size_t size_particle_module_init_mirror_texture([[maybe_unused]] const asset_data::vfx::particle_module_init_mirror_texture& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_model read_particle_module_init_model(const ParticleModuleInitModel& native)
		{
			asset_data::vfx::particle_module_init_model data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_use_physics = native.m_usePhysics;
			data.m_motion_blur_hq = native.m_motionBlurHQ;
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			return data;
		}

		void write_particle_module_init_model(ParticleModuleInitModel& native, const asset_data::vfx::particle_module_init_model& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_usePhysics = data.m_use_physics;
			native.m_motionBlurHQ = data.m_motion_blur_hq;
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
		}

		std::size_t size_particle_module_init_model([[maybe_unused]] const asset_data::vfx::particle_module_init_model& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_module_init_occlusion_query read_particle_module_init_occlusion_query(const ParticleModuleInitOcclusionQuery& native)
		{
			asset_data::vfx::particle_module_init_occlusion_query data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_depth_bias = native.m_depthBias;
			data.m_fade_in_time = native.m_fadeInTime;
			data.m_fade_out_time = native.m_fadeOutTime;
			data.m_scale = read_particle_float_range(native.m_scale);
			data.m_world_size = plain::read_scalars<float, 2>(&native.m_worldSize);
			data.m_offscreen_raycast = native.m_offscreenRaycast;
			plain::require_zero(native.m_pad, "ParticleModuleInitOcclusionQuery.m_pad");
			return data;
		}

		void write_particle_module_init_occlusion_query(ParticleModuleInitOcclusionQuery& native, const asset_data::vfx::particle_module_init_occlusion_query& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_depthBias = data.m_depth_bias;
			native.m_fadeInTime = data.m_fade_in_time;
			native.m_fadeOutTime = data.m_fade_out_time;
			write_particle_float_range(native.m_scale, data.m_scale, resolve, memory);
			plain::write_scalars(&native.m_worldSize, data.m_world_size);
			native.m_offscreenRaycast = data.m_offscreen_raycast;
		}

		std::size_t size_particle_module_init_occlusion_query([[maybe_unused]] const asset_data::vfx::particle_module_init_occlusion_query& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_float_range(data.m_scale);
			return size;
		}

		asset_data::vfx::particle_module_init_oriented_sprite read_particle_module_init_oriented_sprite(const ParticleModuleInitOrientedSprite& native)
		{
			asset_data::vfx::particle_module_init_oriented_sprite data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitOrientedSprite.m_pad");
			data.m_orientation_quat = plain::read_scalars<float, 4>(&native.m_orientationQuat);
			return data;
		}

		void write_particle_module_init_oriented_sprite(ParticleModuleInitOrientedSprite& native, const asset_data::vfx::particle_module_init_oriented_sprite& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			plain::write_scalars(&native.m_orientationQuat, data.m_orientation_quat);
		}

		std::size_t size_particle_module_init_oriented_sprite([[maybe_unused]] const asset_data::vfx::particle_module_init_oriented_sprite& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_particle_sim read_particle_module_init_particle_sim(const ParticleModuleInitParticleSim& native)
		{
			asset_data::vfx::particle_module_init_particle_sim data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_scale_factor = read_particle_float_range(native.m_scaleFactor);
			data.m_hold_last_frame = native.m_holdLastFrame;
			plain::require_zero(native.m_pad, "ParticleModuleInitParticleSim.m_pad");
			return data;
		}

		void write_particle_module_init_particle_sim(ParticleModuleInitParticleSim& native, const asset_data::vfx::particle_module_init_particle_sim& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			write_particle_float_range(native.m_scaleFactor, data.m_scale_factor, resolve, memory);
			native.m_holdLastFrame = data.m_hold_last_frame;
		}

		std::size_t size_particle_module_init_particle_sim([[maybe_unused]] const asset_data::vfx::particle_module_init_particle_sim& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			size += size_particle_float_range(data.m_scale_factor);
			return size;
		}

		asset_data::vfx::particle_module_init_player_facing read_particle_module_init_player_facing(const ParticleModuleInitPlayerFacing& native)
		{
			asset_data::vfx::particle_module_init_player_facing data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitPlayerFacing.m_pad");
			return data;
		}

		void write_particle_module_init_player_facing(ParticleModuleInitPlayerFacing& native, const asset_data::vfx::particle_module_init_player_facing& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
		}

		std::size_t size_particle_module_init_player_facing([[maybe_unused]] const asset_data::vfx::particle_module_init_player_facing& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_relative_velocity read_particle_module_init_relative_velocity(const ParticleModuleInitRelativeVelocity& native)
		{
			asset_data::vfx::particle_module_init_relative_velocity data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_velocity_type = plain::enum_name(asset_data::vfx::particle_relative_velocity_type_names, static_cast<std::int64_t>(native.m_velocityType), "ParticleModuleInitRelativeVelocity.m_velocityType");
			data.m_use_bolt_info = native.m_useBoltInfo;
			return data;
		}

		void write_particle_module_init_relative_velocity(ParticleModuleInitRelativeVelocity& native, const asset_data::vfx::particle_module_init_relative_velocity& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_velocityType = static_cast<ParticleRelativeVelocityType>(plain::enum_value(asset_data::vfx::particle_relative_velocity_type_names, data.m_velocity_type, "ParticleModuleInitRelativeVelocity.m_velocityType"));
			native.m_useBoltInfo = data.m_use_bolt_info;
		}

		std::size_t size_particle_module_init_relative_velocity([[maybe_unused]] const asset_data::vfx::particle_module_init_relative_velocity& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_rotation read_particle_module_init_rotation(const ParticleModuleInitRotation& native)
		{
			asset_data::vfx::particle_module_init_rotation data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitRotation.m_pad");
			data.m_rotation_angle = read_particle_float_range(native.m_rotationAngle);
			data.m_rotation_rate = read_particle_float_range(native.m_rotationRate);
			return data;
		}

		void write_particle_module_init_rotation(ParticleModuleInitRotation& native, const asset_data::vfx::particle_module_init_rotation& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_float_range(native.m_rotationAngle, data.m_rotation_angle, resolve, memory);
			write_particle_float_range(native.m_rotationRate, data.m_rotation_rate, resolve, memory);
		}

		std::size_t size_particle_module_init_rotation([[maybe_unused]] const asset_data::vfx::particle_module_init_rotation& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_float_range(data.m_rotation_angle);
			size += size_particle_float_range(data.m_rotation_rate);
			return size;
		}

		asset_data::vfx::particle_module_init_rotation3_d read_particle_module_init_rotation3_d(const ParticleModuleInitRotation3D& native)
		{
			asset_data::vfx::particle_module_init_rotation3_d data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitRotation3D.m_pad");
			data.m_rotation_angle_min = plain::read_scalars<float, 4>(&native.m_rotationAngleMin);
			data.m_rotation_angle_max = plain::read_scalars<float, 4>(&native.m_rotationAngleMax);
			data.m_rotation_rate_min = plain::read_scalars<float, 4>(&native.m_rotationRateMin);
			data.m_rotation_rate_max = plain::read_scalars<float, 4>(&native.m_rotationRateMax);
			return data;
		}

		void write_particle_module_init_rotation3_d(ParticleModuleInitRotation3D& native, const asset_data::vfx::particle_module_init_rotation3_d& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			plain::write_scalars(&native.m_rotationAngleMin, data.m_rotation_angle_min);
			plain::write_scalars(&native.m_rotationAngleMax, data.m_rotation_angle_max);
			plain::write_scalars(&native.m_rotationRateMin, data.m_rotation_rate_min);
			plain::write_scalars(&native.m_rotationRateMax, data.m_rotation_rate_max);
		}

		std::size_t size_particle_module_init_rotation3_d([[maybe_unused]] const asset_data::vfx::particle_module_init_rotation3_d& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_modifier read_particle_modifier(const ParticleModifier& native)
		{
			asset_data::vfx::particle_modifier data;
			data.min = plain::read_scalars<float, 4>(&native.min);
			data.max = plain::read_scalars<float, 4>(&native.max);
			return data;
		}

		void write_particle_modifier(ParticleModifier& native, const asset_data::vfx::particle_modifier& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.min, data.min);
			plain::write_scalars(&native.max, data.max);
		}

		std::size_t size_particle_modifier([[maybe_unused]] const asset_data::vfx::particle_modifier& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::vfx::particle_module_init_runner read_particle_module_init_runner(const ParticleModuleInitRunner& native)
		{
			asset_data::vfx::particle_module_init_runner data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.pad_8, "ParticleModuleInitRunner.pad_8");
			data.m_scale_modifier = read_particle_modifier(native.m_scaleModifier);
			data.m_velocity_modifier = read_particle_modifier(native.m_velocityModifier);
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_use_orientation_options = plain::enum_name(asset_data::vfx::particle_use_orientation_options_names, static_cast<std::int64_t>(native.m_useOrientationOptions), "ParticleModuleInitRunner.m_useOrientationOptions");
			data.m_use_scale_options = plain::enum_name(asset_data::vfx::particle_use_scale_options_names, static_cast<std::int64_t>(native.m_useScaleOptions), "ParticleModuleInitRunner.m_useScaleOptions");
			data.m_use_velocity_options = plain::enum_name(asset_data::vfx::particle_use_velocity_options_names, static_cast<std::int64_t>(native.m_useVelocityOptions), "ParticleModuleInitRunner.m_useVelocityOptions");
			data.m_attach_to_parent = native.m_attachToParent;
			data.m_stop_child_on_death = native.m_stopChildOnDeath;
			data.m_kill_child_on_death = native.m_killChildOnDeath;
			data.m_legacy_orientation_velocity = native.m_legacyOrientationVelocity;
			data.m_legacy_orientation_rotation = native.m_legacyOrientationRotation;
			plain::require_zero(native.m_pad, "ParticleModuleInitRunner.m_pad");
			return data;
		}

		void write_particle_module_init_runner(ParticleModuleInitRunner& native, const asset_data::vfx::particle_module_init_runner& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_modifier(native.m_scaleModifier, data.m_scale_modifier, resolve, memory);
			write_particle_modifier(native.m_velocityModifier, data.m_velocity_modifier, resolve, memory);
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			native.m_useOrientationOptions = static_cast<ParticleUseOrientationOptions>(plain::enum_value(asset_data::vfx::particle_use_orientation_options_names, data.m_use_orientation_options, "ParticleModuleInitRunner.m_useOrientationOptions"));
			native.m_useScaleOptions = static_cast<ParticleUseScaleOptions>(plain::enum_value(asset_data::vfx::particle_use_scale_options_names, data.m_use_scale_options, "ParticleModuleInitRunner.m_useScaleOptions"));
			native.m_useVelocityOptions = static_cast<ParticleUseVelocityOptions>(plain::enum_value(asset_data::vfx::particle_use_velocity_options_names, data.m_use_velocity_options, "ParticleModuleInitRunner.m_useVelocityOptions"));
			native.m_attachToParent = data.m_attach_to_parent;
			native.m_stopChildOnDeath = data.m_stop_child_on_death;
			native.m_killChildOnDeath = data.m_kill_child_on_death;
			native.m_legacyOrientationVelocity = data.m_legacy_orientation_velocity;
			native.m_legacyOrientationRotation = data.m_legacy_orientation_rotation;
		}

		std::size_t size_particle_module_init_runner([[maybe_unused]] const asset_data::vfx::particle_module_init_runner& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_modifier(data.m_scale_modifier);
			size += size_particle_modifier(data.m_velocity_modifier);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_module_init_sound read_particle_module_init_sound(const ParticleModuleInitSound& native)
		{
			asset_data::vfx::particle_module_init_sound data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitSound.m_pad");
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			return data;
		}

		void write_particle_module_init_sound(ParticleModuleInitSound& native, const asset_data::vfx::particle_module_init_sound& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
		}

		std::size_t size_particle_module_init_sound([[maybe_unused]] const asset_data::vfx::particle_module_init_sound& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_module_init_spawn read_particle_module_init_spawn(const ParticleModuleInitSpawn& native)
		{
			asset_data::vfx::particle_module_init_spawn data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitSpawn.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 1> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_spawn(ParticleModuleInitSpawn& native, const asset_data::vfx::particle_module_init_spawn& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_spawn([[maybe_unused]] const asset_data::vfx::particle_module_init_spawn& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_spawn_shape read_particle_module_init_spawn_shape(const ParticleModuleInitSpawnShape& native)
		{
			asset_data::vfx::particle_module_init_spawn_shape data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_axis_flags = plain::flag_names(asset_data::vfx::particle_module_axes_flags_names, static_cast<std::uint64_t>(native.m_axisFlags), "ParticleModuleInitSpawnShape.m_axisFlags");
			data.m_spawn_flags = plain::flag_names(asset_data::vfx::particle_spawn_flags_names, static_cast<std::uint64_t>(native.m_spawnFlags), "ParticleModuleInitSpawnShape.m_spawnFlags");
			data.m_normal_axis = plain::enum_name(asset_data::vfx::particle_module_axis_names, static_cast<std::int64_t>(native.m_normalAxis), "ParticleModuleInitSpawnShape.m_normalAxis");
			data.m_spawn_type = plain::enum_name(asset_data::vfx::particle_spawn_shape_type_names, static_cast<std::int64_t>(native.m_spawnType), "ParticleModuleInitSpawnShape.m_spawnType");
			data.m_volume_cube_root = native.m_volumeCubeRoot;
			data.m_calculation_offset = plain::read_scalars<float, 4>(&native.m_calculationOffset);
			data.m_offset = plain::read_scalars<float, 4>(&native.m_offset);
			return data;
		}

		void write_particle_module_init_spawn_shape(ParticleModuleInitSpawnShape& native, const asset_data::vfx::particle_module_init_spawn_shape& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_axisFlags = static_cast<ParticleModuleAxesFlags>(plain::flag_value(asset_data::vfx::particle_module_axes_flags_names, data.m_axis_flags, "ParticleModuleInitSpawnShape.m_axisFlags"));
			native.m_spawnFlags = static_cast<ParticleSpawnFlags>(plain::flag_value(asset_data::vfx::particle_spawn_flags_names, data.m_spawn_flags, "ParticleModuleInitSpawnShape.m_spawnFlags"));
			native.m_normalAxis = static_cast<ParticleModuleAxis>(plain::enum_value(asset_data::vfx::particle_module_axis_names, data.m_normal_axis, "ParticleModuleInitSpawnShape.m_normalAxis"));
			native.m_spawnType = static_cast<ParticleSpawnShapeType>(plain::enum_value(asset_data::vfx::particle_spawn_shape_type_names, data.m_spawn_type, "ParticleModuleInitSpawnShape.m_spawnType"));
			native.m_volumeCubeRoot = data.m_volume_cube_root;
			plain::write_scalars(&native.m_calculationOffset, data.m_calculation_offset);
			plain::write_scalars(&native.m_offset, data.m_offset);
		}

		std::size_t size_particle_module_init_spawn_shape([[maybe_unused]] const asset_data::vfx::particle_module_init_spawn_shape& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_spawn_shape_box read_particle_module_init_spawn_shape_box(const ParticleModuleInitSpawnShapeBox& native)
		{
			asset_data::vfx::particle_module_init_spawn_shape_box data;
			data.unnamed = read_particle_module_init_spawn_shape(native.unnamed);
			data.m_use_beam_info = native.m_useBeamInfo;
			plain::require_zero(native.m_pad, "ParticleModuleInitSpawnShapeBox.m_pad");
			data.m_dimensions_min = plain::read_scalars<float, 4>(&native.m_dimensionsMin);
			data.m_dimensions_max = plain::read_scalars<float, 4>(&native.m_dimensionsMax);
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 6> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_spawn_shape_box(ParticleModuleInitSpawnShapeBox& native, const asset_data::vfx::particle_module_init_spawn_shape_box& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_init_spawn_shape(native.unnamed, data.unnamed, resolve, memory);
			native.m_useBeamInfo = data.m_use_beam_info;
			plain::write_scalars(&native.m_dimensionsMin, data.m_dimensions_min);
			plain::write_scalars(&native.m_dimensionsMax, data.m_dimensions_max);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_spawn_shape_box([[maybe_unused]] const asset_data::vfx::particle_module_init_spawn_shape_box& data)
		{
			std::size_t size = 0;
			size += size_particle_module_init_spawn_shape(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_spawn_shape_cylinder read_particle_module_init_spawn_shape_cylinder(const ParticleModuleInitSpawnShapeCylinder& native)
		{
			asset_data::vfx::particle_module_init_spawn_shape_cylinder data;
			data.unnamed = read_particle_module_init_spawn_shape(native.unnamed);
			data.m_has_rotation = native.m_hasRotation;
			data.m_rotate_calculated_offset = native.m_rotateCalculatedOffset;
			data.m_half_height = native.m_halfHeight;
			data.m_radius = read_particle_float_range(native.m_radius);
			data.m_direction_quat = plain::read_scalars<float, 4>(&native.m_directionQuat);
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 5> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_spawn_shape_cylinder(ParticleModuleInitSpawnShapeCylinder& native, const asset_data::vfx::particle_module_init_spawn_shape_cylinder& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_init_spawn_shape(native.unnamed, data.unnamed, resolve, memory);
			native.m_hasRotation = data.m_has_rotation;
			native.m_rotateCalculatedOffset = data.m_rotate_calculated_offset;
			native.m_halfHeight = data.m_half_height;
			write_particle_float_range(native.m_radius, data.m_radius, resolve, memory);
			plain::write_scalars(&native.m_directionQuat, data.m_direction_quat);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_spawn_shape_cylinder([[maybe_unused]] const asset_data::vfx::particle_module_init_spawn_shape_cylinder& data)
		{
			std::size_t size = 0;
			size += size_particle_module_init_spawn_shape(data.unnamed);
			size += size_particle_float_range(data.m_radius);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_spawn_shape_ellipsoid read_particle_module_init_spawn_shape_ellipsoid(const ParticleModuleInitSpawnShapeEllipsoid& native)
		{
			asset_data::vfx::particle_module_init_spawn_shape_ellipsoid data;
			data.unnamed = read_particle_module_init_spawn_shape(native.unnamed);
			data.m_radius_min = plain::read_scalars<float, 4>(&native.m_radiusMin);
			data.m_radius_max = plain::read_scalars<float, 4>(&native.m_radiusMax);
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 6> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_spawn_shape_ellipsoid(ParticleModuleInitSpawnShapeEllipsoid& native, const asset_data::vfx::particle_module_init_spawn_shape_ellipsoid& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_init_spawn_shape(native.unnamed, data.unnamed, resolve, memory);
			plain::write_scalars(&native.m_radiusMin, data.m_radius_min);
			plain::write_scalars(&native.m_radiusMax, data.m_radius_max);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_spawn_shape_ellipsoid([[maybe_unused]] const asset_data::vfx::particle_module_init_spawn_shape_ellipsoid& data)
		{
			std::size_t size = 0;
			size += size_particle_module_init_spawn_shape(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_spawn_shape_mesh read_particle_module_init_spawn_shape_mesh(const ParticleModuleInitSpawnShapeMesh& native)
		{
			asset_data::vfx::particle_module_init_spawn_shape_mesh data;
			data.unnamed = read_particle_module_init_spawn_shape(native.unnamed);
			data.m_num_mesh_assets = native.m_numMeshAssets;
			data.m_use_faces = native.m_useFaces;
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			plain::require_zero(native.m_meshAssetData, "ParticleModuleInitSpawnShapeMesh.m_meshAssetData");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 3> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_spawn_shape_mesh(ParticleModuleInitSpawnShapeMesh& native, const asset_data::vfx::particle_module_init_spawn_shape_mesh& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_init_spawn_shape(native.unnamed, data.unnamed, resolve, memory);
			native.m_numMeshAssets = data.m_num_mesh_assets;
			native.m_useFaces = data.m_use_faces;
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_spawn_shape_mesh([[maybe_unused]] const asset_data::vfx::particle_module_init_spawn_shape_mesh& data)
		{
			std::size_t size = 0;
			size += size_particle_module_init_spawn_shape(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_spawn_shape_sphere read_particle_module_init_spawn_shape_sphere(const ParticleModuleInitSpawnShapeSphere& native)
		{
			asset_data::vfx::particle_module_init_spawn_shape_sphere data;
			data.unnamed = read_particle_module_init_spawn_shape(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleInitSpawnShapeSphere.m_pad");
			data.m_radius = read_particle_float_range(native.m_radius);
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 4> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_init_spawn_shape_sphere(ParticleModuleInitSpawnShapeSphere& native, const asset_data::vfx::particle_module_init_spawn_shape_sphere& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_init_spawn_shape(native.unnamed, data.unnamed, resolve, memory);
			write_particle_float_range(native.m_radius, data.m_radius, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_init_spawn_shape_sphere([[maybe_unused]] const asset_data::vfx::particle_module_init_spawn_shape_sphere& data)
		{
			std::size_t size = 0;
			size += size_particle_module_init_spawn_shape(data.unnamed);
			size += size_particle_float_range(data.m_radius);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_init_tail read_particle_module_init_tail(const ParticleModuleInitTail& native)
		{
			asset_data::vfx::particle_module_init_tail data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_average_past_velocities = native.m_averagePastVelocities;
			data.m_max_parent_speed = native.m_maxParentSpeed;
			data.m_tail_leading = native.m_tailLeading;
			data.m_scale_with_velocity = native.m_scaleWithVelocity;
			data.m_rotate_around_pivot = native.m_rotateAroundPivot;
			return data;
		}

		void write_particle_module_init_tail(ParticleModuleInitTail& native, const asset_data::vfx::particle_module_init_tail& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_averagePastVelocities = data.m_average_past_velocities;
			native.m_maxParentSpeed = data.m_max_parent_speed;
			native.m_tailLeading = data.m_tail_leading;
			native.m_scaleWithVelocity = data.m_scale_with_velocity;
			native.m_rotateAroundPivot = data.m_rotate_around_pivot;
		}

		std::size_t size_particle_module_init_tail([[maybe_unused]] const asset_data::vfx::particle_module_init_tail& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_vector_field read_particle_module_init_vector_field(const ParticleModuleInitVectorField& native)
		{
			asset_data::vfx::particle_module_init_vector_field data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_area_scale = native.m_areaScale;
			data.m_amplitude_scale = native.m_amplitudeScale;
			data.m_frequency_scale = native.m_frequencyScale;
			data.m_noise_max_rotation = native.m_noiseMaxRotation;
			data.m_noise_frequency_scale = native.m_noiseFrequencyScale;
			data.m_noise_scale = native.m_noiseScale;
			return data;
		}

		void write_particle_module_init_vector_field(ParticleModuleInitVectorField& native, const asset_data::vfx::particle_module_init_vector_field& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			native.m_areaScale = data.m_area_scale;
			native.m_amplitudeScale = data.m_amplitude_scale;
			native.m_frequencyScale = data.m_frequency_scale;
			native.m_noiseMaxRotation = data.m_noise_max_rotation;
			native.m_noiseFrequencyScale = data.m_noise_frequency_scale;
			native.m_noiseScale = data.m_noise_scale;
		}

		std::size_t size_particle_module_init_vector_field([[maybe_unused]] const asset_data::vfx::particle_module_init_vector_field& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_module_init_volumetric read_particle_module_init_volumetric(const ParticleModuleInitVolumetric& native)
		{
			asset_data::vfx::particle_module_init_volumetric data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_density = native.m_density;
			data.m_falloff = native.m_falloff;
			data.m_noise_amplitude = native.m_noiseAmplitude;
			data.m_noise_mode = native.m_noiseMode;
			plain::require_zero(native.m_pad, "ParticleModuleInitVolumetric.m_pad");
			return data;
		}

		void write_particle_module_init_volumetric(ParticleModuleInitVolumetric& native, const asset_data::vfx::particle_module_init_volumetric& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_density = data.m_density;
			native.m_falloff = data.m_falloff;
			native.m_noiseAmplitude = data.m_noise_amplitude;
			native.m_noiseMode = data.m_noise_mode;
		}

		std::size_t size_particle_module_init_volumetric([[maybe_unused]] const asset_data::vfx::particle_module_init_volumetric& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_init_dismember read_particle_module_init_dismember(const ParticleModuleInitDismember& native)
		{
			asset_data::vfx::particle_module_init_dismember data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_chance = native.m_chance;
			data.m_dismemberment_type = plain::enum_name(asset_data::vfx::dismemberment_point_type_names, static_cast<std::int64_t>(native.m_dismembermentType), "ParticleModuleInitDismember.m_dismembermentType");
			plain::require_zero(native.m_pad, "ParticleModuleInitDismember.m_pad");
			return data;
		}

		void write_particle_module_init_dismember(ParticleModuleInitDismember& native, const asset_data::vfx::particle_module_init_dismember& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_chance = data.m_chance;
			native.m_dismembermentType = static_cast<DismembermentPointType>(plain::enum_value(asset_data::vfx::dismemberment_point_type_names, data.m_dismemberment_type, "ParticleModuleInitDismember.m_dismembermentType"));
		}

		std::size_t size_particle_module_init_dismember([[maybe_unused]] const asset_data::vfx::particle_module_init_dismember& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_attractor read_particle_module_attractor(const ParticleModuleAttractor& native)
		{
			asset_data::vfx::particle_module_attractor data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleAttractor.m_pad");
			data.m_kill_when_near = native.m_killWhenNear;
			data.m_force_mag = native.m_forceMag;
			data.m_near_distance_sq = native.m_nearDistanceSq;
			data.m_attract_point = plain::read_scalars<float, 4>(&native.m_attractPoint);
			return data;
		}

		void write_particle_module_attractor(ParticleModuleAttractor& native, const asset_data::vfx::particle_module_attractor& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_killWhenNear = data.m_kill_when_near;
			native.m_forceMag = data.m_force_mag;
			native.m_nearDistanceSq = data.m_near_distance_sq;
			plain::write_scalars(&native.m_attractPoint, data.m_attract_point);
		}

		std::size_t size_particle_module_attractor([[maybe_unused]] const asset_data::vfx::particle_module_attractor& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_color_graph read_particle_module_color_graph(const ParticleModuleColorGraph& native)
		{
			asset_data::vfx::particle_module_color_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.first_curve = native.firstCurve;
			plain::require_zero(native.m_pad, "ParticleModuleColorGraph.m_pad");
			data.m_modulate_color_by_alpha = native.m_modulateColorByAlpha;
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 8> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_color_graph(ParticleModuleColorGraph& native, const asset_data::vfx::particle_module_color_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.firstCurve = data.first_curve;
			native.m_modulateColorByAlpha = data.m_modulate_color_by_alpha;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_color_graph([[maybe_unused]] const asset_data::vfx::particle_module_color_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_color_lerp read_particle_module_color_lerp(const ParticleModuleColorLerp& native)
		{
			asset_data::vfx::particle_module_color_lerp data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleColorLerp.m_pad");
			data.m_color_begin = plain::read_scalars<float, 4>(&native.m_colorBegin);
			data.m_color_end = plain::read_scalars<float, 4>(&native.m_colorEnd);
			return data;
		}

		void write_particle_module_color_lerp(ParticleModuleColorLerp& native, const asset_data::vfx::particle_module_color_lerp& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			plain::write_scalars(&native.m_colorBegin, data.m_color_begin);
			plain::write_scalars(&native.m_colorEnd, data.m_color_end);
		}

		std::size_t size_particle_module_color_lerp([[maybe_unused]] const asset_data::vfx::particle_module_color_lerp& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_emission_graph read_particle_module_emission_graph(const ParticleModuleEmissionGraph& native)
		{
			asset_data::vfx::particle_module_emission_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleEmissionGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_emission_graph(ParticleModuleEmissionGraph& native, const asset_data::vfx::particle_module_emission_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_emission_graph([[maybe_unused]] const asset_data::vfx::particle_module_emission_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_emissive_graph read_particle_module_emissive_graph(const ParticleModuleEmissiveGraph& native)
		{
			asset_data::vfx::particle_module_emissive_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.first_curve = native.firstCurve;
			plain::require_zero(native.m_pad, "ParticleModuleEmissiveGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 10> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_emissive_graph(ParticleModuleEmissiveGraph& native, const asset_data::vfx::particle_module_emissive_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.firstCurve = data.first_curve;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_emissive_graph([[maybe_unused]] const asset_data::vfx::particle_module_emissive_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_shader_graph read_particle_module_shader_graph(const ParticleModuleShaderGraph& native)
		{
			asset_data::vfx::particle_module_shader_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.first_curve = native.firstCurve;
			plain::require_zero(native.m_pad, "ParticleModuleShaderGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 12> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_shader_graph(ParticleModuleShaderGraph& native, const asset_data::vfx::particle_module_shader_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.firstCurve = data.first_curve;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_shader_graph([[maybe_unused]] const asset_data::vfx::particle_module_shader_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_force read_particle_module_force(const ParticleModuleForce& native)
		{
			asset_data::vfx::particle_module_force data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleForce.m_pad");
			data.m_force_min = plain::read_scalars<float, 4>(&native.m_forceMin);
			data.m_force_max = plain::read_scalars<float, 4>(&native.m_forceMax);
			return data;
		}

		void write_particle_module_force(ParticleModuleForce& native, const asset_data::vfx::particle_module_force& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			plain::write_scalars(&native.m_forceMin, data.m_force_min);
			plain::write_scalars(&native.m_forceMax, data.m_force_max);
		}

		std::size_t size_particle_module_force([[maybe_unused]] const asset_data::vfx::particle_module_force& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_force_drag_graph read_particle_module_force_drag_graph(const ParticleModuleForceDragGraph& native)
		{
			asset_data::vfx::particle_module_force_drag_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleForceDragGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_force_drag_graph(ParticleModuleForceDragGraph& native, const asset_data::vfx::particle_module_force_drag_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_force_drag_graph([[maybe_unused]] const asset_data::vfx::particle_module_force_drag_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_gravity read_particle_module_gravity(const ParticleModuleGravity& native)
		{
			asset_data::vfx::particle_module_gravity data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_gravity_percentage = read_particle_float_range(native.m_gravityPercentage);
			return data;
		}

		void write_particle_module_gravity(ParticleModuleGravity& native, const asset_data::vfx::particle_module_gravity& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_float_range(native.m_gravityPercentage, data.m_gravity_percentage, resolve, memory);
		}

		std::size_t size_particle_module_gravity([[maybe_unused]] const asset_data::vfx::particle_module_gravity& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_float_range(data.m_gravity_percentage);
			return size;
		}

		asset_data::vfx::particle_module_intensity_graph read_particle_module_intensity_graph(const ParticleModuleIntensityGraph& native)
		{
			asset_data::vfx::particle_module_intensity_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.first_curve = native.firstCurve;
			plain::require_zero(native.m_pad, "ParticleModuleIntensityGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_intensity_graph(ParticleModuleIntensityGraph& native, const asset_data::vfx::particle_module_intensity_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.firstCurve = data.first_curve;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_intensity_graph([[maybe_unused]] const asset_data::vfx::particle_module_intensity_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_temperature_graph read_particle_module_temperature_graph(const ParticleModuleTemperatureGraph& native)
		{
			asset_data::vfx::particle_module_temperature_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.first_curve = native.firstCurve;
			plain::require_zero(native.m_pad, "ParticleModuleTemperatureGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_temperature_graph(ParticleModuleTemperatureGraph& native, const asset_data::vfx::particle_module_temperature_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.firstCurve = data.first_curve;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_temperature_graph([[maybe_unused]] const asset_data::vfx::particle_module_temperature_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_parent_velocity_graph read_particle_module_parent_velocity_graph(const ParticleModuleParentVelocityGraph& native)
		{
			asset_data::vfx::particle_module_parent_velocity_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleParentVelocityGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_parent_velocity_graph(ParticleModuleParentVelocityGraph& native, const asset_data::vfx::particle_module_parent_velocity_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_parent_velocity_graph([[maybe_unused]] const asset_data::vfx::particle_module_parent_velocity_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_physics_light read_particle_module_physics_light(const ParticleModulePhysicsLight& native)
		{
			asset_data::vfx::particle_module_physics_light data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_ignore_emitter_orientation = native.m_ignoreEmitterOrientation;
			data.m_use_surface_type = native.m_useSurfaceType;
			plain::require_zero(native.m_pad, "ParticleModulePhysicsLight.m_pad");
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			return data;
		}

		void write_particle_module_physics_light(ParticleModulePhysicsLight& native, const asset_data::vfx::particle_module_physics_light& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_ignoreEmitterOrientation = data.m_ignore_emitter_orientation;
			native.m_useSurfaceType = data.m_use_surface_type;
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
		}

		std::size_t size_particle_module_physics_light([[maybe_unused]] const asset_data::vfx::particle_module_physics_light& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::bounds read_bounds(const Bounds& native)
		{
			asset_data::vfx::bounds data;
			data.mid_point = plain::read_scalars<float, 3>(&native.midPoint);
			data.half_size = plain::read_scalars<float, 3>(&native.halfSize);
			return data;
		}

		void write_bounds(Bounds& native, const asset_data::vfx::bounds& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			plain::write_scalars(&native.midPoint, data.mid_point);
			plain::write_scalars(&native.halfSize, data.half_size);
		}

		std::size_t size_bounds([[maybe_unused]] const asset_data::vfx::bounds& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::vfx::particle_module_physics_ray_cast read_particle_module_physics_ray_cast(const ParticleModulePhysicsRayCast& native)
		{
			asset_data::vfx::particle_module_physics_ray_cast data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_bounce = read_particle_float_range(native.m_bounce);
			data.m_bounds = read_bounds(native.m_bounds);
			data.m_use_item_clip = native.m_useItemClip;
			data.m_use_surface_type = native.m_useSurfaceType;
			data.m_collide_with_water = native.m_collideWithWater;
			data.m_ignore_content_item = native.m_ignoreContentItem;
			plain::require_zero(native.m_pad, "ParticleModulePhysicsRayCast.m_pad");
			return data;
		}

		void write_particle_module_physics_ray_cast(ParticleModulePhysicsRayCast& native, const asset_data::vfx::particle_module_physics_ray_cast& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			write_particle_float_range(native.m_bounce, data.m_bounce, resolve, memory);
			write_bounds(native.m_bounds, data.m_bounds, resolve, memory);
			native.m_useItemClip = data.m_use_item_clip;
			native.m_useSurfaceType = data.m_use_surface_type;
			native.m_collideWithWater = data.m_collide_with_water;
			native.m_ignoreContentItem = data.m_ignore_content_item;
		}

		std::size_t size_particle_module_physics_ray_cast([[maybe_unused]] const asset_data::vfx::particle_module_physics_ray_cast& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_float_range(data.m_bounce);
			size += size_bounds(data.m_bounds);
			return size;
		}

		asset_data::vfx::particle_module_position_graph read_particle_module_position_graph(const ParticleModulePositionGraph& native)
		{
			asset_data::vfx::particle_module_position_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModulePositionGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 6> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_position_graph(ParticleModulePositionGraph& native, const asset_data::vfx::particle_module_position_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_position_graph([[maybe_unused]] const asset_data::vfx::particle_module_position_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_rotation_graph read_particle_module_rotation_graph(const ParticleModuleRotationGraph& native)
		{
			asset_data::vfx::particle_module_rotation_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_use_rotation_rate = native.m_useRotationRate;
			plain::require_zero(native.m_pad, "ParticleModuleRotationGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_rotation_graph(ParticleModuleRotationGraph& native, const asset_data::vfx::particle_module_rotation_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_useRotationRate = data.m_use_rotation_rate;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_rotation_graph([[maybe_unused]] const asset_data::vfx::particle_module_rotation_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_rotation_graph3_d read_particle_module_rotation_graph3_d(const ParticleModuleRotationGraph3D& native)
		{
			asset_data::vfx::particle_module_rotation_graph3_d data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_use_rotation_rate = native.m_useRotationRate;
			plain::require_zero(native.m_pad, "ParticleModuleRotationGraph3D.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 6> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_rotation_graph3_d(ParticleModuleRotationGraph3D& native, const asset_data::vfx::particle_module_rotation_graph3_d& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_useRotationRate = data.m_use_rotation_rate;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_rotation_graph3_d([[maybe_unused]] const asset_data::vfx::particle_module_rotation_graph3_d& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_scale_by_distance read_particle_module_scale_by_distance(const ParticleModuleScaleByDistance& native)
		{
			asset_data::vfx::particle_module_scale_by_distance data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleScaleByDistance.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			data.m_max_distance = native.m_maxDistance;
			data.m_scale_begin = native.m_scaleBegin;
			data.m_scale_end = native.m_scaleEnd;
			plain::require_zero(native.m_pad2, "ParticleModuleScaleByDistance.m_pad2");
			return data;
		}

		void write_particle_module_scale_by_distance(ParticleModuleScaleByDistance& native, const asset_data::vfx::particle_module_scale_by_distance& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
			native.m_maxDistance = data.m_max_distance;
			native.m_scaleBegin = data.m_scale_begin;
			native.m_scaleEnd = data.m_scale_end;
		}

		std::size_t size_particle_module_scale_by_distance([[maybe_unused]] const asset_data::vfx::particle_module_scale_by_distance& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_size_graph read_particle_module_size_graph(const ParticleModuleSizeGraph& native)
		{
			asset_data::vfx::particle_module_size_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.first_curve = native.firstCurve;
			plain::require_zero(native.m_pad, "ParticleModuleSizeGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 6> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			data.m_size_begin = plain::read_scalars<float, 4>(&native.m_sizeBegin);
			data.m_size_end = plain::read_scalars<float, 4>(&native.m_sizeEnd);
			return data;
		}

		void write_particle_module_size_graph(ParticleModuleSizeGraph& native, const asset_data::vfx::particle_module_size_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.firstCurve = data.first_curve;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
			plain::write_scalars(&native.m_sizeBegin, data.m_size_begin);
			plain::write_scalars(&native.m_sizeEnd, data.m_size_end);
		}

		std::size_t size_particle_module_size_graph([[maybe_unused]] const asset_data::vfx::particle_module_size_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_size_lerp read_particle_module_size_lerp(const ParticleModuleSizeLerp& native)
		{
			asset_data::vfx::particle_module_size_lerp data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleSizeLerp.m_pad");
			data.m_size_begin = plain::read_scalars<float, 4>(&native.m_sizeBegin);
			data.m_size_end = plain::read_scalars<float, 4>(&native.m_sizeEnd);
			return data;
		}

		void write_particle_module_size_lerp(ParticleModuleSizeLerp& native, const asset_data::vfx::particle_module_size_lerp& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			plain::write_scalars(&native.m_sizeBegin, data.m_size_begin);
			plain::write_scalars(&native.m_sizeEnd, data.m_size_end);
		}

		std::size_t size_particle_module_size_lerp([[maybe_unused]] const asset_data::vfx::particle_module_size_lerp& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_velocity_graph read_particle_module_velocity_graph(const ParticleModuleVelocityGraph& native)
		{
			asset_data::vfx::particle_module_velocity_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			plain::require_zero(native.m_pad, "ParticleModuleVelocityGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 6> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			data.m_velocity_begin = plain::read_scalars<float, 4>(&native.m_velocityBegin);
			data.m_velocity_end = plain::read_scalars<float, 4>(&native.m_velocityEnd);
			return data;
		}

		void write_particle_module_velocity_graph(ParticleModuleVelocityGraph& native, const asset_data::vfx::particle_module_velocity_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
			plain::write_scalars(&native.m_velocityBegin, data.m_velocity_begin);
			plain::write_scalars(&native.m_velocityEnd, data.m_velocity_end);
		}

		std::size_t size_particle_module_velocity_graph([[maybe_unused]] const asset_data::vfx::particle_module_velocity_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_vector_field_graph read_particle_module_vector_field_graph(const ParticleModuleVectorFieldGraph& native)
		{
			asset_data::vfx::particle_module_vector_field_graph data;
			data.unnamed = read_particle_module(native.unnamed);
			data.first_curve = native.firstCurve;
			plain::require_zero(native.m_pad, "ParticleModuleVectorFieldGraph.m_pad");
			data.m_curves = [&] { std::array<asset_data::vfx::particle_curve_def, 2> out; for (std::size_t i = 0; i < out.size(); ++i) { out[i] = read_particle_curve_def(native.m_curves[i]); } return out; }();
			return data;
		}

		void write_particle_module_vector_field_graph(ParticleModuleVectorFieldGraph& native, const asset_data::vfx::particle_module_vector_field_graph& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.firstCurve = data.first_curve;
			for (std::size_t i = 0; i < data.m_curves.size(); ++i)
			{
				write_particle_curve_def(native.m_curves[i], data.m_curves[i], resolve, memory);
			}
		}

		std::size_t size_particle_module_vector_field_graph([[maybe_unused]] const asset_data::vfx::particle_module_vector_field_graph& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			for (const auto& item : data.m_curves)
			{
				size += size_particle_curve_def(item);
			}
			return size;
		}

		asset_data::vfx::particle_module_test_event_handler_data read_particle_module_test_event_handler_data(const ParticleModuleTestEventHandlerData& native)
		{
			asset_data::vfx::particle_module_test_event_handler_data data;
			data.m_next_state = native.m_nextState;
			data.m_linked_asset_list = read_particle_linked_asset_list(native.m_linkedAssetList);
			data.m_kill = native.m_kill;
			plain::require_zero(native.m_pad, "ParticleModuleTestEventHandlerData.m_pad");
			return data;
		}

		void write_particle_module_test_event_handler_data(ParticleModuleTestEventHandlerData& native, const asset_data::vfx::particle_module_test_event_handler_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.m_nextState = data.m_next_state;
			write_particle_linked_asset_list(native.m_linkedAssetList, data.m_linked_asset_list, resolve, memory);
			native.m_kill = data.m_kill;
		}

		std::size_t size_particle_module_test_event_handler_data([[maybe_unused]] const asset_data::vfx::particle_module_test_event_handler_data& data)
		{
			std::size_t size = 0;
			size += size_particle_linked_asset_list(data.m_linked_asset_list);
			return size;
		}

		asset_data::vfx::particle_module_test read_particle_module_test(const ParticleModuleTest& native)
		{
			asset_data::vfx::particle_module_test data;
			data.unnamed = read_particle_module(native.unnamed);
			data.m_module_index = native.m_moduleIndex;
			data.m_use_orientation_options = plain::enum_name(asset_data::vfx::particle_use_orientation_options_names, static_cast<std::int64_t>(native.m_useOrientationOptions), "ParticleModuleTest.m_useOrientationOptions");
			data.m_use_scale_options = plain::enum_name(asset_data::vfx::particle_use_scale_options_names, static_cast<std::int64_t>(native.m_useScaleOptions), "ParticleModuleTest.m_useScaleOptions");
			data.m_use_velocity_options = plain::enum_name(asset_data::vfx::particle_use_velocity_options_names, static_cast<std::int64_t>(native.m_useVelocityOptions), "ParticleModuleTest.m_useVelocityOptions");
			data.m_scale_modifier = read_particle_modifier(native.m_scaleModifier);
			data.m_velocity_modifier = read_particle_modifier(native.m_velocityModifier);
			data.m_event_handler_data = read_particle_module_test_event_handler_data(native.m_eventHandlerData);
			return data;
		}

		void write_particle_module_test(ParticleModuleTest& native, const asset_data::vfx::particle_module_test& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module(native.unnamed, data.unnamed, resolve, memory);
			native.m_moduleIndex = data.m_module_index;
			native.m_useOrientationOptions = static_cast<ParticleUseOrientationOptions>(plain::enum_value(asset_data::vfx::particle_use_orientation_options_names, data.m_use_orientation_options, "ParticleModuleTest.m_useOrientationOptions"));
			native.m_useScaleOptions = static_cast<ParticleUseScaleOptions>(plain::enum_value(asset_data::vfx::particle_use_scale_options_names, data.m_use_scale_options, "ParticleModuleTest.m_useScaleOptions"));
			native.m_useVelocityOptions = static_cast<ParticleUseVelocityOptions>(plain::enum_value(asset_data::vfx::particle_use_velocity_options_names, data.m_use_velocity_options, "ParticleModuleTest.m_useVelocityOptions"));
			write_particle_modifier(native.m_scaleModifier, data.m_scale_modifier, resolve, memory);
			write_particle_modifier(native.m_velocityModifier, data.m_velocity_modifier, resolve, memory);
			write_particle_module_test_event_handler_data(native.m_eventHandlerData, data.m_event_handler_data, resolve, memory);
		}

		std::size_t size_particle_module_test([[maybe_unused]] const asset_data::vfx::particle_module_test& data)
		{
			std::size_t size = 0;
			size += size_particle_module(data.unnamed);
			size += size_particle_modifier(data.m_scale_modifier);
			size += size_particle_modifier(data.m_velocity_modifier);
			size += size_particle_module_test_event_handler_data(data.m_event_handler_data);
			return size;
		}

		asset_data::vfx::particle_module_test_age read_particle_module_test_age(const ParticleModuleTestAge& native)
		{
			asset_data::vfx::particle_module_test_age data;
			data.unnamed = read_particle_module_test(native.unnamed);
			data.m_age = native.m_age;
			plain::require_zero(native.m_pad, "ParticleModuleTestAge.m_pad");
			return data;
		}

		void write_particle_module_test_age(ParticleModuleTestAge& native, const asset_data::vfx::particle_module_test_age& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_test(native.unnamed, data.unnamed, resolve, memory);
			native.m_age = data.m_age;
		}

		std::size_t size_particle_module_test_age([[maybe_unused]] const asset_data::vfx::particle_module_test_age& data)
		{
			std::size_t size = 0;
			size += size_particle_module_test(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_test_birth read_particle_module_test_birth(const ParticleModuleTestBirth& native)
		{
			asset_data::vfx::particle_module_test_birth data;
			data.unnamed = read_particle_module_test(native.unnamed);
			return data;
		}

		void write_particle_module_test_birth(ParticleModuleTestBirth& native, const asset_data::vfx::particle_module_test_birth& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_test(native.unnamed, data.unnamed, resolve, memory);
		}

		std::size_t size_particle_module_test_birth([[maybe_unused]] const asset_data::vfx::particle_module_test_birth& data)
		{
			std::size_t size = 0;
			size += size_particle_module_test(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_test_death read_particle_module_test_death(const ParticleModuleTestDeath& native)
		{
			asset_data::vfx::particle_module_test_death data;
			data.unnamed = read_particle_module_test(native.unnamed);
			return data;
		}

		void write_particle_module_test_death(ParticleModuleTestDeath& native, const asset_data::vfx::particle_module_test_death& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_test(native.unnamed, data.unnamed, resolve, memory);
		}

		std::size_t size_particle_module_test_death([[maybe_unused]] const asset_data::vfx::particle_module_test_death& data)
		{
			std::size_t size = 0;
			size += size_particle_module_test(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_test_gravity read_particle_module_test_gravity(const ParticleModuleTestGravity& native)
		{
			asset_data::vfx::particle_module_test_gravity data;
			data.unnamed = read_particle_module_test(native.unnamed);
			data.m_gravity_state = native.m_gravityState;
			plain::require_zero(native.m_pad, "ParticleModuleTestGravity.m_pad");
			return data;
		}

		void write_particle_module_test_gravity(ParticleModuleTestGravity& native, const asset_data::vfx::particle_module_test_gravity& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_test(native.unnamed, data.unnamed, resolve, memory);
			native.m_gravityState = data.m_gravity_state;
		}

		std::size_t size_particle_module_test_gravity([[maybe_unused]] const asset_data::vfx::particle_module_test_gravity& data)
		{
			std::size_t size = 0;
			size += size_particle_module_test(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_test_impact read_particle_module_test_impact(const ParticleModuleTestImpact& native)
		{
			asset_data::vfx::particle_module_test_impact data;
			data.unnamed = read_particle_module_test(native.unnamed);
			data.m_impact_direction = native.m_impactDirection;
			plain::require_zero(native.m_pad, "ParticleModuleTestImpact.m_pad");
			return data;
		}

		void write_particle_module_test_impact(ParticleModuleTestImpact& native, const asset_data::vfx::particle_module_test_impact& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_test(native.unnamed, data.unnamed, resolve, memory);
			native.m_impactDirection = data.m_impact_direction;
		}

		std::size_t size_particle_module_test_impact([[maybe_unused]] const asset_data::vfx::particle_module_test_impact& data)
		{
			std::size_t size = 0;
			size += size_particle_module_test(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_test_pos read_particle_module_test_pos(const ParticleModuleTestPos& native)
		{
			asset_data::vfx::particle_module_test_pos data;
			data.unnamed = read_particle_module_test(native.unnamed);
			data.m_pos_z = native.m_posZ;
			plain::require_zero(native.m_pad, "ParticleModuleTestPos.m_pad");
			return data;
		}

		void write_particle_module_test_pos(ParticleModuleTestPos& native, const asset_data::vfx::particle_module_test_pos& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_test(native.unnamed, data.unnamed, resolve, memory);
			native.m_posZ = data.m_pos_z;
		}

		std::size_t size_particle_module_test_pos([[maybe_unused]] const asset_data::vfx::particle_module_test_pos& data)
		{
			std::size_t size = 0;
			size += size_particle_module_test(data.unnamed);
			return size;
		}

		asset_data::vfx::particle_module_test_time_in_state read_particle_module_test_time_in_state(const ParticleModuleTestTimeInState& native)
		{
			asset_data::vfx::particle_module_test_time_in_state data;
			data.unnamed = read_particle_module_test(native.unnamed);
			data.m_time_in_state = read_particle_float_range(native.m_timeInState);
			plain::require_zero(native.m_pad, "ParticleModuleTestTimeInState.m_pad");
			return data;
		}

		void write_particle_module_test_time_in_state(ParticleModuleTestTimeInState& native, const asset_data::vfx::particle_module_test_time_in_state& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			write_particle_module_test(native.unnamed, data.unnamed, resolve, memory);
			write_particle_float_range(native.m_timeInState, data.m_time_in_state, resolve, memory);
		}

		std::size_t size_particle_module_test_time_in_state([[maybe_unused]] const asset_data::vfx::particle_module_test_time_in_state& data)
		{
			std::size_t size = 0;
			size += size_particle_module_test(data.unnamed);
			size += size_particle_float_range(data.m_time_in_state);
			return size;
		}

		asset_data::vfx::particle_module_def read_particle_module_def(const ParticleModuleDef& native)
		{
			asset_data::vfx::particle_module_def data;
			plain::require_zero(native.m_pad, "ParticleModuleDef.m_pad");
			data.module_data = [&]() -> std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state> { switch (static_cast<std::uint32_t>(native.moduleType)) { case 0: { plain::module_scope scope(0); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_atlas(native.moduleData.initAtlas)); } case 1: { plain::module_scope scope(1); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_attributes(native.moduleData.initAttributes)); } case 2: { plain::module_scope scope(2); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_beam(native.moduleData.initBeam)); } case 3: { plain::module_scope scope(3); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_camera_offset(native.moduleData.initCameraOffset)); } case 4: { plain::module_scope scope(4); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_cloud(native.moduleData.initCloud)); } case 5: { plain::module_scope scope(5); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_decal(native.moduleData.initDecal)); } case 6: { plain::module_scope scope(6); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_flare(native.moduleData.initFlare)); } case 7: { plain::module_scope scope(7); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_geo_trail(native.moduleData.initGeoTrail)); } case 8: { plain::module_scope scope(8); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_kill_wrap_box(native.moduleData.initKillWrapBox)); } case 9: { plain::module_scope scope(9); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_light_omni(native.moduleData.initLightOmni)); } case 10: { plain::module_scope scope(10); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_light_spot(native.moduleData.initLightSpot)); } case 11: { plain::module_scope scope(11); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_material(native.moduleData.initMaterial)); } case 12: { plain::module_scope scope(12); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_mirror_texture(native.moduleData.initMirrorTexture)); } case 13: { plain::module_scope scope(13); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_model(native.moduleData.initModel)); } case 14: { plain::module_scope scope(14); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_occlusion_query(native.moduleData.initOcclusionQuery)); } case 15: { plain::module_scope scope(15); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_oriented_sprite(native.moduleData.initOrientedSprite)); } case 16: { plain::module_scope scope(16); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_particle_sim(native.moduleData.initParticleSim)); } case 17: { plain::module_scope scope(17); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_player_facing(native.moduleData.initPlayerFacing)); } case 18: { plain::module_scope scope(18); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_relative_velocity(native.moduleData.initRelativeVelocity)); } case 19: { plain::module_scope scope(19); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_rotation(native.moduleData.initRotation)); } case 20: { plain::module_scope scope(20); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_rotation3_d(native.moduleData.initRotation3D)); } case 21: { plain::module_scope scope(21); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_runner(native.moduleData.initRunner)); } case 22: { plain::module_scope scope(22); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_sound(native.moduleData.initSound)); } case 23: { plain::module_scope scope(23); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_spawn(native.moduleData.initSpawn)); } case 24: { plain::module_scope scope(24); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_spawn_shape_box(native.moduleData.initSpawnShapeBox)); } case 25: { plain::module_scope scope(25); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_spawn_shape_cylinder(native.moduleData.initSpawnShapeCylinder)); } case 26: { plain::module_scope scope(26); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_spawn_shape_ellipsoid(native.moduleData.initSpawnShapeEllipsoid)); } case 27: { plain::module_scope scope(27); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_spawn_shape_mesh(native.moduleData.initSpawnShapeMesh)); } case 28: { plain::module_scope scope(28); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_spawn_shape_sphere(native.moduleData.initSpawnShapeSphere)); } case 29: { plain::module_scope scope(29); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_tail(native.moduleData.initTail)); } case 30: { plain::module_scope scope(30); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_vector_field(native.moduleData.initVectorField)); } case 31: { plain::module_scope scope(31); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_volumetric(native.moduleData.initVolumetric)); } case 32: { plain::module_scope scope(32); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_init_dismember(native.moduleData.initDismember)); } case 33: { plain::module_scope scope(33); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_attractor(native.moduleData.attractor)); } case 34: { plain::module_scope scope(34); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_color_graph(native.moduleData.colorGraph)); } case 35: { plain::module_scope scope(35); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_color_lerp(native.moduleData.colorLerp)); } case 36: { plain::module_scope scope(36); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_emission_graph(native.moduleData.emissionGraph)); } case 37: { plain::module_scope scope(37); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_emissive_graph(native.moduleData.emissiveGraph)); } case 38: { plain::module_scope scope(38); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_shader_graph(native.moduleData.shaderGraph)); } case 39: { plain::module_scope scope(39); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_force(native.moduleData.force)); } case 40: { plain::module_scope scope(40); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_force_drag_graph(native.moduleData.forceDragGraph)); } case 41: { plain::module_scope scope(41); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_gravity(native.moduleData.gravity)); } case 42: { plain::module_scope scope(42); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_intensity_graph(native.moduleData.intensityGraph)); } case 43: { plain::module_scope scope(43); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_temperature_graph(native.moduleData.temperatureGraph)); } case 44: { plain::module_scope scope(44); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_parent_velocity_graph(native.moduleData.parentVelocityGraph)); } case 45: { plain::module_scope scope(45); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_physics_light(native.moduleData.physicsLight)); } case 46: { plain::module_scope scope(46); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_physics_ray_cast(native.moduleData.physicsRayCast)); } case 47: { plain::module_scope scope(47); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_position_graph(native.moduleData.positionGraph)); } case 48: { plain::module_scope scope(48); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_rotation_graph(native.moduleData.rotationGraph)); } case 49: { plain::module_scope scope(49); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_rotation_graph3_d(native.moduleData.rotationGraph3D)); } case 50: { plain::module_scope scope(50); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_scale_by_distance(native.moduleData.scaleByDistance)); } case 51: { plain::module_scope scope(51); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_size_graph(native.moduleData.sizeGraph)); } case 52: { plain::module_scope scope(52); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_size_lerp(native.moduleData.sizeLerp)); } case 53: { plain::module_scope scope(53); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_velocity_graph(native.moduleData.velocityGraph)); } case 54: { plain::module_scope scope(54); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_vector_field_graph(native.moduleData.vectorFieldGraph)); } case 55: { plain::module_scope scope(55); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_test_age(native.moduleData.testAge)); } case 56: { plain::module_scope scope(56); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_test_birth(native.moduleData.testBirth)); } case 57: { plain::module_scope scope(57); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_test_death(native.moduleData.testDeath)); } case 58: { plain::module_scope scope(58); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_test_gravity(native.moduleData.testGravity)); } case 59: { plain::module_scope scope(59); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_test_impact(native.moduleData.testImpact)); } case 60: { plain::module_scope scope(60); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_test_pos(native.moduleData.testPos)); } case 61: { plain::module_scope scope(61); return std::variant<std::monostate, asset_data::vfx::particle_module_init_atlas, asset_data::vfx::particle_module_init_attributes, asset_data::vfx::particle_module_init_beam, asset_data::vfx::particle_module_init_camera_offset, asset_data::vfx::particle_module_init_cloud, asset_data::vfx::particle_module_init_decal, asset_data::vfx::particle_module_init_flare, asset_data::vfx::particle_module_init_geo_trail, asset_data::vfx::particle_module_init_kill_wrap_box, asset_data::vfx::particle_module_init_light_omni, asset_data::vfx::particle_module_init_light_spot, asset_data::vfx::particle_module_init_material, asset_data::vfx::particle_module_init_mirror_texture, asset_data::vfx::particle_module_init_model, asset_data::vfx::particle_module_init_occlusion_query, asset_data::vfx::particle_module_init_oriented_sprite, asset_data::vfx::particle_module_init_particle_sim, asset_data::vfx::particle_module_init_player_facing, asset_data::vfx::particle_module_init_relative_velocity, asset_data::vfx::particle_module_init_rotation, asset_data::vfx::particle_module_init_rotation3_d, asset_data::vfx::particle_module_init_runner, asset_data::vfx::particle_module_init_sound, asset_data::vfx::particle_module_init_spawn, asset_data::vfx::particle_module_init_spawn_shape_box, asset_data::vfx::particle_module_init_spawn_shape_cylinder, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid, asset_data::vfx::particle_module_init_spawn_shape_mesh, asset_data::vfx::particle_module_init_spawn_shape_sphere, asset_data::vfx::particle_module_init_tail, asset_data::vfx::particle_module_init_vector_field, asset_data::vfx::particle_module_init_volumetric, asset_data::vfx::particle_module_init_dismember, asset_data::vfx::particle_module_attractor, asset_data::vfx::particle_module_color_graph, asset_data::vfx::particle_module_color_lerp, asset_data::vfx::particle_module_emission_graph, asset_data::vfx::particle_module_emissive_graph, asset_data::vfx::particle_module_shader_graph, asset_data::vfx::particle_module_force, asset_data::vfx::particle_module_force_drag_graph, asset_data::vfx::particle_module_gravity, asset_data::vfx::particle_module_intensity_graph, asset_data::vfx::particle_module_temperature_graph, asset_data::vfx::particle_module_parent_velocity_graph, asset_data::vfx::particle_module_physics_light, asset_data::vfx::particle_module_physics_ray_cast, asset_data::vfx::particle_module_position_graph, asset_data::vfx::particle_module_rotation_graph, asset_data::vfx::particle_module_rotation_graph3_d, asset_data::vfx::particle_module_scale_by_distance, asset_data::vfx::particle_module_size_graph, asset_data::vfx::particle_module_size_lerp, asset_data::vfx::particle_module_velocity_graph, asset_data::vfx::particle_module_vector_field_graph, asset_data::vfx::particle_module_test_age, asset_data::vfx::particle_module_test_birth, asset_data::vfx::particle_module_test_death, asset_data::vfx::particle_module_test_gravity, asset_data::vfx::particle_module_test_impact, asset_data::vfx::particle_module_test_pos, asset_data::vfx::particle_module_test_time_in_state>(read_particle_module_test_time_in_state(native.moduleData.testTimeInState)); } default: plain::fail("ParticleModuleDef.moduleData", "has a tag value without a case"); } }();
			return data;
		}

		void write_particle_module_def(ParticleModuleDef& native, const asset_data::vfx::particle_module_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
					plain::fail("ParticleModuleDef.moduleData", "holds no module");
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_atlas>)
				{
					native.moduleType = static_cast<ParticleModuleType>(0);
					plain::module_scope scope(0);
					write_particle_module_init_atlas(native.moduleData.initAtlas, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_attributes>)
				{
					native.moduleType = static_cast<ParticleModuleType>(1);
					plain::module_scope scope(1);
					write_particle_module_init_attributes(native.moduleData.initAttributes, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_beam>)
				{
					native.moduleType = static_cast<ParticleModuleType>(2);
					plain::module_scope scope(2);
					write_particle_module_init_beam(native.moduleData.initBeam, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_camera_offset>)
				{
					native.moduleType = static_cast<ParticleModuleType>(3);
					plain::module_scope scope(3);
					write_particle_module_init_camera_offset(native.moduleData.initCameraOffset, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_cloud>)
				{
					native.moduleType = static_cast<ParticleModuleType>(4);
					plain::module_scope scope(4);
					write_particle_module_init_cloud(native.moduleData.initCloud, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_decal>)
				{
					native.moduleType = static_cast<ParticleModuleType>(5);
					plain::module_scope scope(5);
					write_particle_module_init_decal(native.moduleData.initDecal, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_flare>)
				{
					native.moduleType = static_cast<ParticleModuleType>(6);
					plain::module_scope scope(6);
					write_particle_module_init_flare(native.moduleData.initFlare, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_geo_trail>)
				{
					native.moduleType = static_cast<ParticleModuleType>(7);
					plain::module_scope scope(7);
					write_particle_module_init_geo_trail(native.moduleData.initGeoTrail, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_kill_wrap_box>)
				{
					native.moduleType = static_cast<ParticleModuleType>(8);
					plain::module_scope scope(8);
					write_particle_module_init_kill_wrap_box(native.moduleData.initKillWrapBox, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_light_omni>)
				{
					native.moduleType = static_cast<ParticleModuleType>(9);
					plain::module_scope scope(9);
					write_particle_module_init_light_omni(native.moduleData.initLightOmni, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_light_spot>)
				{
					native.moduleType = static_cast<ParticleModuleType>(10);
					plain::module_scope scope(10);
					write_particle_module_init_light_spot(native.moduleData.initLightSpot, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_material>)
				{
					native.moduleType = static_cast<ParticleModuleType>(11);
					plain::module_scope scope(11);
					write_particle_module_init_material(native.moduleData.initMaterial, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_mirror_texture>)
				{
					native.moduleType = static_cast<ParticleModuleType>(12);
					plain::module_scope scope(12);
					write_particle_module_init_mirror_texture(native.moduleData.initMirrorTexture, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_model>)
				{
					native.moduleType = static_cast<ParticleModuleType>(13);
					plain::module_scope scope(13);
					write_particle_module_init_model(native.moduleData.initModel, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_occlusion_query>)
				{
					native.moduleType = static_cast<ParticleModuleType>(14);
					plain::module_scope scope(14);
					write_particle_module_init_occlusion_query(native.moduleData.initOcclusionQuery, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_oriented_sprite>)
				{
					native.moduleType = static_cast<ParticleModuleType>(15);
					plain::module_scope scope(15);
					write_particle_module_init_oriented_sprite(native.moduleData.initOrientedSprite, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_particle_sim>)
				{
					native.moduleType = static_cast<ParticleModuleType>(16);
					plain::module_scope scope(16);
					write_particle_module_init_particle_sim(native.moduleData.initParticleSim, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_player_facing>)
				{
					native.moduleType = static_cast<ParticleModuleType>(17);
					plain::module_scope scope(17);
					write_particle_module_init_player_facing(native.moduleData.initPlayerFacing, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_relative_velocity>)
				{
					native.moduleType = static_cast<ParticleModuleType>(18);
					plain::module_scope scope(18);
					write_particle_module_init_relative_velocity(native.moduleData.initRelativeVelocity, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_rotation>)
				{
					native.moduleType = static_cast<ParticleModuleType>(19);
					plain::module_scope scope(19);
					write_particle_module_init_rotation(native.moduleData.initRotation, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_rotation3_d>)
				{
					native.moduleType = static_cast<ParticleModuleType>(20);
					plain::module_scope scope(20);
					write_particle_module_init_rotation3_d(native.moduleData.initRotation3D, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_runner>)
				{
					native.moduleType = static_cast<ParticleModuleType>(21);
					plain::module_scope scope(21);
					write_particle_module_init_runner(native.moduleData.initRunner, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_sound>)
				{
					native.moduleType = static_cast<ParticleModuleType>(22);
					plain::module_scope scope(22);
					write_particle_module_init_sound(native.moduleData.initSound, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn>)
				{
					native.moduleType = static_cast<ParticleModuleType>(23);
					plain::module_scope scope(23);
					write_particle_module_init_spawn(native.moduleData.initSpawn, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_box>)
				{
					native.moduleType = static_cast<ParticleModuleType>(24);
					plain::module_scope scope(24);
					write_particle_module_init_spawn_shape_box(native.moduleData.initSpawnShapeBox, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_cylinder>)
				{
					native.moduleType = static_cast<ParticleModuleType>(25);
					plain::module_scope scope(25);
					write_particle_module_init_spawn_shape_cylinder(native.moduleData.initSpawnShapeCylinder, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid>)
				{
					native.moduleType = static_cast<ParticleModuleType>(26);
					plain::module_scope scope(26);
					write_particle_module_init_spawn_shape_ellipsoid(native.moduleData.initSpawnShapeEllipsoid, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_mesh>)
				{
					native.moduleType = static_cast<ParticleModuleType>(27);
					plain::module_scope scope(27);
					write_particle_module_init_spawn_shape_mesh(native.moduleData.initSpawnShapeMesh, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_sphere>)
				{
					native.moduleType = static_cast<ParticleModuleType>(28);
					plain::module_scope scope(28);
					write_particle_module_init_spawn_shape_sphere(native.moduleData.initSpawnShapeSphere, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_tail>)
				{
					native.moduleType = static_cast<ParticleModuleType>(29);
					plain::module_scope scope(29);
					write_particle_module_init_tail(native.moduleData.initTail, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_vector_field>)
				{
					native.moduleType = static_cast<ParticleModuleType>(30);
					plain::module_scope scope(30);
					write_particle_module_init_vector_field(native.moduleData.initVectorField, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_volumetric>)
				{
					native.moduleType = static_cast<ParticleModuleType>(31);
					plain::module_scope scope(31);
					write_particle_module_init_volumetric(native.moduleData.initVolumetric, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_dismember>)
				{
					native.moduleType = static_cast<ParticleModuleType>(32);
					plain::module_scope scope(32);
					write_particle_module_init_dismember(native.moduleData.initDismember, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_attractor>)
				{
					native.moduleType = static_cast<ParticleModuleType>(33);
					plain::module_scope scope(33);
					write_particle_module_attractor(native.moduleData.attractor, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_color_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(34);
					plain::module_scope scope(34);
					write_particle_module_color_graph(native.moduleData.colorGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_color_lerp>)
				{
					native.moduleType = static_cast<ParticleModuleType>(35);
					plain::module_scope scope(35);
					write_particle_module_color_lerp(native.moduleData.colorLerp, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_emission_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(36);
					plain::module_scope scope(36);
					write_particle_module_emission_graph(native.moduleData.emissionGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_emissive_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(37);
					plain::module_scope scope(37);
					write_particle_module_emissive_graph(native.moduleData.emissiveGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_shader_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(38);
					plain::module_scope scope(38);
					write_particle_module_shader_graph(native.moduleData.shaderGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_force>)
				{
					native.moduleType = static_cast<ParticleModuleType>(39);
					plain::module_scope scope(39);
					write_particle_module_force(native.moduleData.force, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_force_drag_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(40);
					plain::module_scope scope(40);
					write_particle_module_force_drag_graph(native.moduleData.forceDragGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_gravity>)
				{
					native.moduleType = static_cast<ParticleModuleType>(41);
					plain::module_scope scope(41);
					write_particle_module_gravity(native.moduleData.gravity, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_intensity_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(42);
					plain::module_scope scope(42);
					write_particle_module_intensity_graph(native.moduleData.intensityGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_temperature_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(43);
					plain::module_scope scope(43);
					write_particle_module_temperature_graph(native.moduleData.temperatureGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_parent_velocity_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(44);
					plain::module_scope scope(44);
					write_particle_module_parent_velocity_graph(native.moduleData.parentVelocityGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_physics_light>)
				{
					native.moduleType = static_cast<ParticleModuleType>(45);
					plain::module_scope scope(45);
					write_particle_module_physics_light(native.moduleData.physicsLight, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_physics_ray_cast>)
				{
					native.moduleType = static_cast<ParticleModuleType>(46);
					plain::module_scope scope(46);
					write_particle_module_physics_ray_cast(native.moduleData.physicsRayCast, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_position_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(47);
					plain::module_scope scope(47);
					write_particle_module_position_graph(native.moduleData.positionGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_rotation_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(48);
					plain::module_scope scope(48);
					write_particle_module_rotation_graph(native.moduleData.rotationGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_rotation_graph3_d>)
				{
					native.moduleType = static_cast<ParticleModuleType>(49);
					plain::module_scope scope(49);
					write_particle_module_rotation_graph3_d(native.moduleData.rotationGraph3D, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_scale_by_distance>)
				{
					native.moduleType = static_cast<ParticleModuleType>(50);
					plain::module_scope scope(50);
					write_particle_module_scale_by_distance(native.moduleData.scaleByDistance, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_size_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(51);
					plain::module_scope scope(51);
					write_particle_module_size_graph(native.moduleData.sizeGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_size_lerp>)
				{
					native.moduleType = static_cast<ParticleModuleType>(52);
					plain::module_scope scope(52);
					write_particle_module_size_lerp(native.moduleData.sizeLerp, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_velocity_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(53);
					plain::module_scope scope(53);
					write_particle_module_velocity_graph(native.moduleData.velocityGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_vector_field_graph>)
				{
					native.moduleType = static_cast<ParticleModuleType>(54);
					plain::module_scope scope(54);
					write_particle_module_vector_field_graph(native.moduleData.vectorFieldGraph, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_age>)
				{
					native.moduleType = static_cast<ParticleModuleType>(55);
					plain::module_scope scope(55);
					write_particle_module_test_age(native.moduleData.testAge, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_birth>)
				{
					native.moduleType = static_cast<ParticleModuleType>(56);
					plain::module_scope scope(56);
					write_particle_module_test_birth(native.moduleData.testBirth, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_death>)
				{
					native.moduleType = static_cast<ParticleModuleType>(57);
					plain::module_scope scope(57);
					write_particle_module_test_death(native.moduleData.testDeath, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_gravity>)
				{
					native.moduleType = static_cast<ParticleModuleType>(58);
					plain::module_scope scope(58);
					write_particle_module_test_gravity(native.moduleData.testGravity, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_impact>)
				{
					native.moduleType = static_cast<ParticleModuleType>(59);
					plain::module_scope scope(59);
					write_particle_module_test_impact(native.moduleData.testImpact, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_pos>)
				{
					native.moduleType = static_cast<ParticleModuleType>(60);
					plain::module_scope scope(60);
					write_particle_module_test_pos(native.moduleData.testPos, value, resolve, memory);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_time_in_state>)
				{
					native.moduleType = static_cast<ParticleModuleType>(61);
					plain::module_scope scope(61);
					write_particle_module_test_time_in_state(native.moduleData.testTimeInState, value, resolve, memory);
				}
			}, data.module_data);
		}

		std::size_t size_particle_module_def([[maybe_unused]] const asset_data::vfx::particle_module_def& data)
		{
			std::size_t size = 0;
			std::visit([&](const auto& value)
			{
				using value_type = std::decay_t<decltype(value)>;
				if constexpr (std::is_same_v<value_type, std::monostate>)
				{
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_atlas>)
				{
					size += size_particle_module_init_atlas(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_attributes>)
				{
					size += size_particle_module_init_attributes(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_beam>)
				{
					size += size_particle_module_init_beam(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_camera_offset>)
				{
					size += size_particle_module_init_camera_offset(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_cloud>)
				{
					size += size_particle_module_init_cloud(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_decal>)
				{
					size += size_particle_module_init_decal(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_flare>)
				{
					size += size_particle_module_init_flare(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_geo_trail>)
				{
					size += size_particle_module_init_geo_trail(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_kill_wrap_box>)
				{
					size += size_particle_module_init_kill_wrap_box(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_light_omni>)
				{
					size += size_particle_module_init_light_omni(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_light_spot>)
				{
					size += size_particle_module_init_light_spot(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_material>)
				{
					size += size_particle_module_init_material(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_mirror_texture>)
				{
					size += size_particle_module_init_mirror_texture(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_model>)
				{
					size += size_particle_module_init_model(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_occlusion_query>)
				{
					size += size_particle_module_init_occlusion_query(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_oriented_sprite>)
				{
					size += size_particle_module_init_oriented_sprite(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_particle_sim>)
				{
					size += size_particle_module_init_particle_sim(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_player_facing>)
				{
					size += size_particle_module_init_player_facing(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_relative_velocity>)
				{
					size += size_particle_module_init_relative_velocity(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_rotation>)
				{
					size += size_particle_module_init_rotation(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_rotation3_d>)
				{
					size += size_particle_module_init_rotation3_d(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_runner>)
				{
					size += size_particle_module_init_runner(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_sound>)
				{
					size += size_particle_module_init_sound(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn>)
				{
					size += size_particle_module_init_spawn(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_box>)
				{
					size += size_particle_module_init_spawn_shape_box(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_cylinder>)
				{
					size += size_particle_module_init_spawn_shape_cylinder(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_ellipsoid>)
				{
					size += size_particle_module_init_spawn_shape_ellipsoid(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_mesh>)
				{
					size += size_particle_module_init_spawn_shape_mesh(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_spawn_shape_sphere>)
				{
					size += size_particle_module_init_spawn_shape_sphere(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_tail>)
				{
					size += size_particle_module_init_tail(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_vector_field>)
				{
					size += size_particle_module_init_vector_field(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_volumetric>)
				{
					size += size_particle_module_init_volumetric(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_init_dismember>)
				{
					size += size_particle_module_init_dismember(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_attractor>)
				{
					size += size_particle_module_attractor(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_color_graph>)
				{
					size += size_particle_module_color_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_color_lerp>)
				{
					size += size_particle_module_color_lerp(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_emission_graph>)
				{
					size += size_particle_module_emission_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_emissive_graph>)
				{
					size += size_particle_module_emissive_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_shader_graph>)
				{
					size += size_particle_module_shader_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_force>)
				{
					size += size_particle_module_force(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_force_drag_graph>)
				{
					size += size_particle_module_force_drag_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_gravity>)
				{
					size += size_particle_module_gravity(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_intensity_graph>)
				{
					size += size_particle_module_intensity_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_temperature_graph>)
				{
					size += size_particle_module_temperature_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_parent_velocity_graph>)
				{
					size += size_particle_module_parent_velocity_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_physics_light>)
				{
					size += size_particle_module_physics_light(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_physics_ray_cast>)
				{
					size += size_particle_module_physics_ray_cast(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_position_graph>)
				{
					size += size_particle_module_position_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_rotation_graph>)
				{
					size += size_particle_module_rotation_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_rotation_graph3_d>)
				{
					size += size_particle_module_rotation_graph3_d(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_scale_by_distance>)
				{
					size += size_particle_module_scale_by_distance(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_size_graph>)
				{
					size += size_particle_module_size_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_size_lerp>)
				{
					size += size_particle_module_size_lerp(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_velocity_graph>)
				{
					size += size_particle_module_velocity_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_vector_field_graph>)
				{
					size += size_particle_module_vector_field_graph(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_age>)
				{
					size += size_particle_module_test_age(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_birth>)
				{
					size += size_particle_module_test_birth(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_death>)
				{
					size += size_particle_module_test_death(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_gravity>)
				{
					size += size_particle_module_test_gravity(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_impact>)
				{
					size += size_particle_module_test_impact(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_pos>)
				{
					size += size_particle_module_test_pos(value);
				}
				else if constexpr (std::is_same_v<value_type, asset_data::vfx::particle_module_test_time_in_state>)
				{
					size += size_particle_module_test_time_in_state(value);
				}
			}, data.module_data);
			return size;
		}

		asset_data::vfx::particle_module_group_def read_particle_module_group_def(const ParticleModuleGroupDef& native)
		{
			asset_data::vfx::particle_module_group_def data;
			data.module_defs = plain::read_array(native.moduleDefs, native.numModules, "ParticleModuleGroupDef.moduleDefs", [&](const auto& item) { return read_particle_module_def(item); });
			data.disabled = native.disabled;
			return data;
		}

		void write_particle_module_group_def(ParticleModuleGroupDef& native, const asset_data::vfx::particle_module_group_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.moduleDefs = plain::write_array<ParticleModuleDef>(data.module_defs, memory, [&](auto& out, const auto& in) { write_particle_module_def(out, in, resolve, memory); });
			native.disabled = data.disabled;
			native.numModules = plain::count_of<std::int32_t>(data.module_defs);
		}

		std::size_t size_particle_module_group_def([[maybe_unused]] const asset_data::vfx::particle_module_group_def& data)
		{
			std::size_t size = 0;
			if (data.module_defs)
			{
				size += plain::sized(std::max<std::size_t>(data.module_defs->size(), 1) * sizeof(ParticleModuleDef));
				for (const auto& item : *data.module_defs)
				{
					size += size_particle_module_def(item);
				}
			}
			return size;
		}

		asset_data::vfx::particle_state_def read_particle_state_def(const ParticleStateDef& native)
		{
			asset_data::vfx::particle_state_def data;
			data.module_group_defs = plain::read_fixed_with<3>(native.moduleGroupDefs, [&](const auto& item) { return read_particle_module_group_def(item); });
			data.element_type = native.elementType;
			data.flags = native.flags;
			plain::require_zero(native.pad, "ParticleStateDef.pad");
			return data;
		}

		void write_particle_state_def(ParticleStateDef& native, const asset_data::vfx::particle_state_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			if (data.module_group_defs)
			{
				native.moduleGroupDefs = memory.manual_allocate<ParticleModuleGroupDef>(plain::rounded(3 * sizeof(ParticleModuleGroupDef)));
				for (std::size_t i = 0; i < 3; ++i)
				{
					write_particle_module_group_def(native.moduleGroupDefs[i], (*data.module_group_defs)[i], resolve, memory);
				}
			}
			native.elementType = data.element_type;
			native.flags = data.flags;
		}

		std::size_t size_particle_state_def([[maybe_unused]] const asset_data::vfx::particle_state_def& data)
		{
			std::size_t size = 0;
			if (data.module_group_defs)
			{
				size += plain::sized(3 * sizeof(ParticleModuleGroupDef));
				for (const auto& item : *data.module_group_defs)
				{
					size += size_particle_module_group_def(item);
				}
			}
			return size;
		}

		asset_data::vfx::particle_int_range read_particle_int_range(const ParticleIntRange& native)
		{
			asset_data::vfx::particle_int_range data;
			data.min = native.min;
			data.max = native.max;
			return data;
		}

		void write_particle_int_range(ParticleIntRange& native, const asset_data::vfx::particle_int_range& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.min = data.min;
			native.max = data.max;
		}

		std::size_t size_particle_int_range([[maybe_unused]] const asset_data::vfx::particle_int_range& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::vfx::particle_emitter_def read_particle_emitter_def(const ParticleEmitterDef& native)
		{
			asset_data::vfx::particle_emitter_def data;
			data.state_defs = plain::read_array(native.stateDefs, native.numStates, "ParticleEmitterDef.stateDefs", [&](const auto& item) { return read_particle_state_def(item); });
			data.particle_spawn_rate = read_particle_float_range(native.particleSpawnRate);
			data.particle_life = read_particle_float_range(native.particleLife);
			data.particle_delay = read_particle_float_range(native.particleDelay);
			data.particle_count_max = native.particleCountMax;
			data.particle_burst_count = read_particle_int_range(native.particleBurstCount);
			data.emitter_life = read_particle_float_range(native.emitterLife);
			data.emitter_delay = read_particle_float_range(native.emitterDelay);
			data.random_seed = native.randomSeed;
			data.spawn_range_sq = read_particle_float_range(native.spawnRangeSq);
			data.fade_out_max_distance = native.fadeOutMaxDistance;
			data.fade_curve_def = read_particle_curve_def(native.fadeCurveDef);
			data.spawn_frustum_cull_radius = native.spawnFrustumCullRadius;
			data.flags = native.flags;
			data.gravity_options = plain::enum_name(asset_data::vfx::particle_gravity_options_names, static_cast<std::int64_t>(native.gravityOptions), "ParticleEmitterDef.gravityOptions");
			data.group_i_ds = plain::read_block<std::array<std::uint32_t, 4>>(&native.groupIDs);
			data.emit_by_distance_density = read_particle_float_range(native.emitByDistanceDensity);
			data.instance_pool = native.instancePool;
			data.solo_instance_max = native.soloInstanceMax;
			data.instance_action = plain::enum_name(asset_data::vfx::particle_instance_pool_action_names, static_cast<std::int64_t>(native.instanceAction), "ParticleEmitterDef.instanceAction");
			data.m_data_flags = plain::flag_names(asset_data::vfx::particle_data_flags_names, static_cast<std::uint64_t>(native.m_dataFlags), "ParticleEmitterDef.m_dataFlags");
			data.particle_spawn_shape_range = read_particle_float_range(native.particleSpawnShapeRange);
			plain::require_zero(native.pad, "ParticleEmitterDef.pad");
			return data;
		}

		void write_particle_emitter_def(ParticleEmitterDef& native, const asset_data::vfx::particle_emitter_def& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.stateDefs = plain::write_array<ParticleStateDef>(data.state_defs, memory, [&](auto& out, const auto& in) { write_particle_state_def(out, in, resolve, memory); });
			write_particle_float_range(native.particleSpawnRate, data.particle_spawn_rate, resolve, memory);
			write_particle_float_range(native.particleLife, data.particle_life, resolve, memory);
			write_particle_float_range(native.particleDelay, data.particle_delay, resolve, memory);
			native.particleCountMax = data.particle_count_max;
			write_particle_int_range(native.particleBurstCount, data.particle_burst_count, resolve, memory);
			write_particle_float_range(native.emitterLife, data.emitter_life, resolve, memory);
			write_particle_float_range(native.emitterDelay, data.emitter_delay, resolve, memory);
			native.randomSeed = data.random_seed;
			write_particle_float_range(native.spawnRangeSq, data.spawn_range_sq, resolve, memory);
			native.fadeOutMaxDistance = data.fade_out_max_distance;
			write_particle_curve_def(native.fadeCurveDef, data.fade_curve_def, resolve, memory);
			native.spawnFrustumCullRadius = data.spawn_frustum_cull_radius;
			native.flags = data.flags;
			native.gravityOptions = static_cast<ParticleGravityOptions>(plain::enum_value(asset_data::vfx::particle_gravity_options_names, data.gravity_options, "ParticleEmitterDef.gravityOptions"));
			plain::write_block(&native.groupIDs, data.group_i_ds);
			write_particle_float_range(native.emitByDistanceDensity, data.emit_by_distance_density, resolve, memory);
			native.instancePool = data.instance_pool;
			native.soloInstanceMax = data.solo_instance_max;
			native.instanceAction = static_cast<ParticleInstancePoolAction>(plain::enum_value(asset_data::vfx::particle_instance_pool_action_names, data.instance_action, "ParticleEmitterDef.instanceAction"));
			native.m_dataFlags = static_cast<ParticleDataFlags>(plain::flag_value(asset_data::vfx::particle_data_flags_names, data.m_data_flags, "ParticleEmitterDef.m_dataFlags"));
			write_particle_float_range(native.particleSpawnShapeRange, data.particle_spawn_shape_range, resolve, memory);
			native.numStates = plain::count_of<std::int32_t>(data.state_defs);
		}

		std::size_t size_particle_emitter_def([[maybe_unused]] const asset_data::vfx::particle_emitter_def& data)
		{
			std::size_t size = 0;
			if (data.state_defs)
			{
				size += plain::sized(std::max<std::size_t>(data.state_defs->size(), 1) * sizeof(ParticleStateDef));
				for (const auto& item : *data.state_defs)
				{
					size += size_particle_state_def(item);
				}
			}
			size += size_particle_float_range(data.particle_spawn_rate);
			size += size_particle_float_range(data.particle_life);
			size += size_particle_float_range(data.particle_delay);
			size += size_particle_int_range(data.particle_burst_count);
			size += size_particle_float_range(data.emitter_life);
			size += size_particle_float_range(data.emitter_delay);
			size += size_particle_float_range(data.spawn_range_sq);
			size += size_particle_curve_def(data.fade_curve_def);
			size += size_particle_float_range(data.emit_by_distance_density);
			size += size_particle_float_range(data.particle_spawn_shape_range);
			return size;
		}

		asset_data::vfx::particle_system read_particle_system(const ParticleSystemDef& native)
		{
			asset_data::vfx::particle_system data;
			data.name = plain::read_string(native.name);
			data.emitter_defs = plain::read_array(native.emitterDefs, native.numEmitters, "ParticleSystemDef.emitterDefs", [&](const auto& item) { return read_particle_emitter_def(item); });
			data.scripted_input_node_defs = plain::read_array(native.scriptedInputNodeDefs, native.numScriptedInputNodes, "ParticleSystemDef.scriptedInputNodeDefs", [&](const auto& item) { return read_particle_scripted_input_node(item); });
			data.version = native.version;
			data.flags = native.flags;
			data.occlusion_override_emitter_index = native.occlusionOverrideEmitterIndex;
			data.phase_options = plain::enum_name(asset_data::vfx::particle_phase_options_names, static_cast<std::int64_t>(native.phaseOptions), "ParticleSystemDef.phaseOptions");
			data.draw_frustum_cull_radius = native.drawFrustumCullRadius;
			data.update_frustum_cull_radius = native.updateFrustumCullRadius;
			data.sun_distance = native.sunDistance;
			data.pre_roll_m_sec = native.preRollMSec;
			data.editor_position = plain::read_scalars<float, 4>(&native.editorPosition);
			data.editor_rotation = plain::read_scalars<float, 4>(&native.editorRotation);
			data.game_tweak_position = plain::read_scalars<float, 4>(&native.gameTweakPosition);
			data.game_tweak_rotation = plain::read_scalars<float, 4>(&native.gameTweakRotation);
			return data;
		}

		void write_particle_system(ParticleSystemDef& native, const asset_data::vfx::particle_system& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.emitterDefs = plain::write_array<ParticleEmitterDef>(data.emitter_defs, memory, [&](auto& out, const auto& in) { write_particle_emitter_def(out, in, resolve, memory); });
			native.scriptedInputNodeDefs = plain::write_array<ParticleScriptedInputNodeDef>(data.scripted_input_node_defs, memory, [&](auto& out, const auto& in) { write_particle_scripted_input_node(out, in, resolve, memory); });
			native.version = data.version;
			native.flags = data.flags;
			native.occlusionOverrideEmitterIndex = data.occlusion_override_emitter_index;
			native.phaseOptions = static_cast<ParticlePhaseOptions>(plain::enum_value(asset_data::vfx::particle_phase_options_names, data.phase_options, "ParticleSystemDef.phaseOptions"));
			native.drawFrustumCullRadius = data.draw_frustum_cull_radius;
			native.updateFrustumCullRadius = data.update_frustum_cull_radius;
			native.sunDistance = data.sun_distance;
			native.preRollMSec = data.pre_roll_m_sec;
			plain::write_scalars(&native.editorPosition, data.editor_position);
			plain::write_scalars(&native.editorRotation, data.editor_rotation);
			plain::write_scalars(&native.gameTweakPosition, data.game_tweak_position);
			plain::write_scalars(&native.gameTweakRotation, data.game_tweak_rotation);
			native.numEmitters = plain::count_of<std::int32_t>(data.emitter_defs);
			native.numScriptedInputNodes = plain::count_of<std::int32_t>(data.scripted_input_node_defs);
		}

		std::size_t size_particle_system([[maybe_unused]] const asset_data::vfx::particle_system& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			if (data.emitter_defs)
			{
				size += plain::sized(std::max<std::size_t>(data.emitter_defs->size(), 1) * sizeof(ParticleEmitterDef));
				for (const auto& item : *data.emitter_defs)
				{
					size += size_particle_emitter_def(item);
				}
			}
			if (data.scripted_input_node_defs)
			{
				size += plain::sized(std::max<std::size_t>(data.scripted_input_node_defs->size(), 1) * sizeof(ParticleScriptedInputNodeDef));
				for (const auto& item : *data.scripted_input_node_defs)
				{
					size += size_particle_scripted_input_node(item);
				}
			}
			return size;
		}

	}

	std::expected<asset_data::vfx::particle_system, std::string> to_neutral(const ParticleSystemDef& native)
	{
		try
		{
			auto data = read_particle_system(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	ParticleSystemDef* from_neutral(const asset_data::vfx::particle_system& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<ParticleSystemDef>();
		write_particle_system(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::vfx::particle_system& data)
	{
		return plain::rounded(sizeof(ParticleSystemDef)) + size_particle_system(data) + 64;
	}

}
