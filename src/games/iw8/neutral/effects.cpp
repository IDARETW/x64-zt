// Emitted by tools/gen_iw8_neutral.py from tools/neutral_families/effects.json: do not edit.
#include <std_include.hpp>
#include "effects.hpp"
#include "plain.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		asset_data::effects::fx_particle_sim_animation_header read_fx_particle_sim_animation_header(const FxParticleSimAnimationHeader& native)
		{
			asset_data::effects::fx_particle_sim_animation_header data;
			data.playback_rate = native.playbackRate;
			data.duration = native.duration;
			data.frame_count = native.frameCount;
			data.min_x = native.minX;
			data.min_y = native.minY;
			data.min_z = native.minZ;
			data.bounds_x_delta = native.boundsXDelta;
			data.bounds_y_delta = native.boundsYDelta;
			data.bounds_z_delta = native.boundsZDelta;
			data.max_width = native.maxWidth;
			data.max_height = native.maxHeight;
			data.color_table_size = native.colorTableSize;
			data.particle_data_count = native.particleDataCount;
			data.max_active_particles = native.maxActiveParticles;
			data.eval_vis_state_per_particle = native.evalVisStatePerParticle;
			data.sort_particles_at_runtime = native.sortParticlesAtRuntime;
			data.has_orientation3_d = native.hasOrientation3D;
			data.has_index = native.hasIndex;
			return data;
		}

		void write_fx_particle_sim_animation_header(FxParticleSimAnimationHeader& native, const asset_data::effects::fx_particle_sim_animation_header& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.playbackRate = data.playback_rate;
			native.duration = data.duration;
			native.frameCount = data.frame_count;
			native.minX = data.min_x;
			native.minY = data.min_y;
			native.minZ = data.min_z;
			native.boundsXDelta = data.bounds_x_delta;
			native.boundsYDelta = data.bounds_y_delta;
			native.boundsZDelta = data.bounds_z_delta;
			native.maxWidth = data.max_width;
			native.maxHeight = data.max_height;
			native.colorTableSize = data.color_table_size;
			native.particleDataCount = data.particle_data_count;
			native.maxActiveParticles = data.max_active_particles;
			native.evalVisStatePerParticle = data.eval_vis_state_per_particle;
			native.sortParticlesAtRuntime = data.sort_particles_at_runtime;
			native.hasOrientation3D = data.has_orientation3_d;
			native.hasIndex = data.has_index;
		}

		std::size_t size_fx_particle_sim_animation_header([[maybe_unused]] const asset_data::effects::fx_particle_sim_animation_header& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::effects::fx_particle_sim_animation_particle_data read_fx_particle_sim_animation_particle_data(const FxParticleSimAnimationParticleData& native)
		{
			asset_data::effects::fx_particle_sim_animation_particle_data data;
			data.x_normalized_pos = native.xNormalizedPos;
			data.y_normalized_pos = native.yNormalizedPos;
			data.z_normalized_pos = native.zNormalizedPos;
			data.x_normalized_width = native.xNormalizedWidth;
			data.y_normalized_height = native.yNormalizedHeight;
			data.orientation = native.orientation;
			data.lifetime = native.lifetime;
			data.particle_id = native.particleID;
			data.x_normalized_pos_next_frame = native.xNormalizedPosNextFrame;
			data.y_normalized_pos_next_frame = native.yNormalizedPosNextFrame;
			data.z_normalized_pos_next_frame = native.zNormalizedPosNextFrame;
			data.x_normalized_width_next_frame = native.xNormalizedWidthNextFrame;
			data.y_normalized_height_next_frame = native.yNormalizedHeightNextFrame;
			data.orientation_delta = native.orientationDelta;
			data.color_table_index = native.colorTableIndex;
			data.next_color_table_index = native.nextColorTableIndex;
			return data;
		}

		void write_fx_particle_sim_animation_particle_data(FxParticleSimAnimationParticleData& native, const asset_data::effects::fx_particle_sim_animation_particle_data& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.xNormalizedPos = data.x_normalized_pos;
			native.yNormalizedPos = data.y_normalized_pos;
			native.zNormalizedPos = data.z_normalized_pos;
			native.xNormalizedWidth = data.x_normalized_width;
			native.yNormalizedHeight = data.y_normalized_height;
			native.orientation = data.orientation;
			native.lifetime = data.lifetime;
			native.particleID = data.particle_id;
			native.xNormalizedPosNextFrame = data.x_normalized_pos_next_frame;
			native.yNormalizedPosNextFrame = data.y_normalized_pos_next_frame;
			native.zNormalizedPosNextFrame = data.z_normalized_pos_next_frame;
			native.xNormalizedWidthNextFrame = data.x_normalized_width_next_frame;
			native.yNormalizedHeightNextFrame = data.y_normalized_height_next_frame;
			native.orientationDelta = data.orientation_delta;
			native.colorTableIndex = data.color_table_index;
			native.nextColorTableIndex = data.next_color_table_index;
		}

		std::size_t size_fx_particle_sim_animation_particle_data([[maybe_unused]] const asset_data::effects::fx_particle_sim_animation_particle_data& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::effects::fx_particle_sim_animation_particle_data_orientation3_d read_fx_particle_sim_animation_particle_data_orientation3_d(const FxParticleSimAnimationParticleDataOrientation3D& native)
		{
			asset_data::effects::fx_particle_sim_animation_particle_data_orientation3_d data;
			data.y_orientation = native.yOrientation;
			data.z_orientation = native.zOrientation;
			data.y_orientation_delta = native.yOrientationDelta;
			data.z_orientation_delta = native.zOrientationDelta;
			return data;
		}

		void write_fx_particle_sim_animation_particle_data_orientation3_d(FxParticleSimAnimationParticleDataOrientation3D& native, const asset_data::effects::fx_particle_sim_animation_particle_data_orientation3_d& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.yOrientation = data.y_orientation;
			native.zOrientation = data.z_orientation;
			native.yOrientationDelta = data.y_orientation_delta;
			native.zOrientationDelta = data.z_orientation_delta;
		}

		std::size_t size_fx_particle_sim_animation_particle_data_orientation3_d([[maybe_unused]] const asset_data::effects::fx_particle_sim_animation_particle_data_orientation3_d& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::effects::fx_particle_sim_animation_particle_data_index read_fx_particle_sim_animation_particle_data_index(const FxParticleSimAnimationParticleDataIndex& native)
		{
			asset_data::effects::fx_particle_sim_animation_particle_data_index data;
			data.index = native.index;
			data.next_index = native.nextIndex;
			return data;
		}

		void write_fx_particle_sim_animation_particle_data_index(FxParticleSimAnimationParticleDataIndex& native, const asset_data::effects::fx_particle_sim_animation_particle_data_index& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.index = data.index;
			native.nextIndex = data.next_index;
		}

		std::size_t size_fx_particle_sim_animation_particle_data_index([[maybe_unused]] const asset_data::effects::fx_particle_sim_animation_particle_data_index& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::effects::fx_particle_sim_animation_frame read_fx_particle_sim_animation_frame(const FxParticleSimAnimationFrame& native)
		{
			asset_data::effects::fx_particle_sim_animation_frame data;
			data.particle_data_offset = native.particleDataOffset;
			data.num_active_particles = native.numActiveParticles;
			return data;
		}

		void write_fx_particle_sim_animation_frame(FxParticleSimAnimationFrame& native, const asset_data::effects::fx_particle_sim_animation_frame& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.particleDataOffset = data.particle_data_offset;
			native.numActiveParticles = data.num_active_particles;
		}

		std::size_t size_fx_particle_sim_animation_frame([[maybe_unused]] const asset_data::effects::fx_particle_sim_animation_frame& data)
		{
			std::size_t size = 0;
			return size;
		}

		asset_data::effects::particle_sim_animation read_particle_sim_animation(const FxParticleSimAnimation& native)
		{
			asset_data::effects::particle_sim_animation data;
			data.name = plain::read_string(native.name);
			data.material = plain::read_asset(ASSET_TYPE_MATERIAL, native.material);
			data.header = read_fx_particle_sim_animation_header(native.header);
			data.particle_data = plain::read_array(native.particleData, native.header.particleDataCount, "FxParticleSimAnimation.particleData", [&](const auto& item) { return read_fx_particle_sim_animation_particle_data(item); });
			data.particle_data_orientation3_d = plain::read_array(native.particleDataOrientation3D, (static_cast<std::uint8_t>(native.header.hasOrientation3D) == 0 ? 0 : native.header.particleDataCount), "FxParticleSimAnimation.particleDataOrientation3D", [&](const auto& item) { return read_fx_particle_sim_animation_particle_data_orientation3_d(item); });
			data.particle_data_index = plain::read_array(native.particleDataIndex, (static_cast<std::uint8_t>(native.header.hasIndex) == 0 ? 0 : native.header.particleDataCount), "FxParticleSimAnimation.particleDataIndex", [&](const auto& item) { return read_fx_particle_sim_animation_particle_data_index(item); });
			data.frames = plain::read_array(native.frames, native.header.frameCount, "FxParticleSimAnimation.frames", [&](const auto& item) { return read_fx_particle_sim_animation_frame(item); });
			data.color_table = plain::read_array(native.colorTable, native.header.colorTableSize, "FxParticleSimAnimation.colorTable", [&](const auto& item) { return plain::read_scalars<float, 4>(&item); });
			return data;
		}

		void write_particle_sim_animation(FxParticleSimAnimation& native, const asset_data::effects::particle_sim_animation& data, [[maybe_unused]] const asset_resolver& resolve, [[maybe_unused]] plain::context& memory)
		{
			native.name = plain::write_string(memory, data.name);
			native.material = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.material);
			write_fx_particle_sim_animation_header(native.header, data.header, resolve, memory);
			native.particleData = plain::write_array<FxParticleSimAnimationParticleData>(data.particle_data, memory, [&](auto& out, const auto& in) { write_fx_particle_sim_animation_particle_data(out, in, resolve, memory); });
			native.particleDataOrientation3D = plain::write_array<FxParticleSimAnimationParticleDataOrientation3D>(data.particle_data_orientation3_d, memory, [&](auto& out, const auto& in) { write_fx_particle_sim_animation_particle_data_orientation3_d(out, in, resolve, memory); });
			native.particleDataIndex = plain::write_array<FxParticleSimAnimationParticleDataIndex>(data.particle_data_index, memory, [&](auto& out, const auto& in) { write_fx_particle_sim_animation_particle_data_index(out, in, resolve, memory); });
			native.frames = plain::write_array<FxParticleSimAnimationFrame>(data.frames, memory, [&](auto& out, const auto& in) { write_fx_particle_sim_animation_frame(out, in, resolve, memory); });
			native.colorTable = plain::write_array<vec4_t>(data.color_table, memory, [&](auto& out, const auto& in) { plain::write_scalars(&out, in); });
		}

		std::size_t size_particle_sim_animation([[maybe_unused]] const asset_data::effects::particle_sim_animation& data)
		{
			std::size_t size = 0;
			size += plain::string_size(data.name);
			size += size_fx_particle_sim_animation_header(data.header);
			if (data.particle_data)
			{
				size += plain::sized(std::max<std::size_t>(data.particle_data->size(), 1) * sizeof(FxParticleSimAnimationParticleData));
				for (const auto& item : *data.particle_data)
				{
					size += size_fx_particle_sim_animation_particle_data(item);
				}
			}
			if (data.particle_data_orientation3_d)
			{
				size += plain::sized(std::max<std::size_t>(data.particle_data_orientation3_d->size(), 1) * sizeof(FxParticleSimAnimationParticleDataOrientation3D));
				for (const auto& item : *data.particle_data_orientation3_d)
				{
					size += size_fx_particle_sim_animation_particle_data_orientation3_d(item);
				}
			}
			if (data.particle_data_index)
			{
				size += plain::sized(std::max<std::size_t>(data.particle_data_index->size(), 1) * sizeof(FxParticleSimAnimationParticleDataIndex));
				for (const auto& item : *data.particle_data_index)
				{
					size += size_fx_particle_sim_animation_particle_data_index(item);
				}
			}
			if (data.frames)
			{
				size += plain::sized(std::max<std::size_t>(data.frames->size(), 1) * sizeof(FxParticleSimAnimationFrame));
				for (const auto& item : *data.frames)
				{
					size += size_fx_particle_sim_animation_frame(item);
				}
			}
			if (data.color_table)
			{
				size += plain::sized(std::max<std::size_t>(data.color_table->size(), 1) * sizeof(vec4_t));
			}
			return size;
		}

	}

	std::expected<asset_data::effects::particle_sim_animation, std::string> to_neutral(const FxParticleSimAnimation& native)
	{
		try
		{
			auto data = read_particle_sim_animation(native);
			validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	FxParticleSimAnimation* from_neutral(const asset_data::effects::particle_sim_animation& data, const asset_resolver& resolve, zone_memory& memory)
	{
		validate(data);
		plain::context context(memory);
		auto* native = context.allocate<FxParticleSimAnimation>();
		write_particle_sim_animation(*native, data, resolve, context);
		return native;
	}

	std::size_t native_size(const asset_data::effects::particle_sim_animation& data)
	{
		return plain::rounded(sizeof(FxParticleSimAnimation)) + size_particle_sim_animation(data) + 64;
	}

}
