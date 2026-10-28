#include <std_include.hpp>
#include "fx.hpp"

#include <zone/zone_memory.hpp>

namespace zonetool::iw8::neutral
{
	namespace
	{
		static_assert(sizeof(VectorSubField::numEntries) == sizeof(asset_data::vector_subfield::entries));

		constexpr std::size_t rounded(const std::size_t size)
		{
			return (size + 7) & ~std::size_t(7);
		}

		const char* copy_text(zone_memory& memory, const std::string& text)
		{
			auto* copy = memory.manual_allocate<char>(rounded(text.size() + 1));
			std::ranges::copy(text, copy);
			return copy;
		}

		bool zero_bytes(const void* data, const std::size_t size)
		{
			const auto* bytes = static_cast<const std::uint8_t*>(data);
			return std::all_of(bytes, bytes + size, [](const std::uint8_t byte) { return !byte; });
		}

		std::array<float, 3> triple(const vec3_t& value)
		{
			return {value.v[0], value.v[1], value.v[2]};
		}

		void require(const bool condition, const char* what)
		{
			if (!condition)
			{
				throw std::runtime_error(what);
			}
		}
	}

	std::expected<asset_data::physics_fx_pipeline, std::string> to_neutral(const PhysicsFXPipeline& native)
	{
		try
		{
			require(native.name, "the pipeline has no name");
			asset_data::physics_fx_pipeline data;
			data.name = native.name;
			data.particles_per_instance = native.numParticlesPerInstance;
			data.friction = native.friction;
			data.restitution = native.restitution;
			data.particle_collisions = native.particleParticleCollisions;
			data.collision_callbacks = native.raiseCollisionCallbacks;
			data.callback_impulse_threshold = native.callbackImpulseThreshold;

			const auto& integrator = native.integrator;
			if (integrator.type == Types_PhysicsFXPipelineIntegrator_LinAngDamping)
			{
				data.damping = {integrator.u.linVelDampingIntegrator.linearDamping, integrator.u.linVelDampingIntegrator.angularDamping};
			}
			else
			{
				require(integrator.type == Types_PhysicsFXPipelineIntegrator_Basic, "the pipeline's integrator type has no neutral name");
				require(zero_bytes(&integrator.u, sizeof(integrator.u)), "the basic integrator's parameters hold data");
			}

			asset_data::validate(data);
			return data;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	PhysicsFXPipeline* from_neutral(const asset_data::physics_fx_pipeline& data, zone_memory& memory)
	{
		asset_data::validate(data);
		auto* native = memory.allocate<PhysicsFXPipeline>();
		native->name = copy_text(memory, data.name);
		native->numParticlesPerInstance = data.particles_per_instance;
		native->friction = data.friction;
		native->restitution = data.restitution;
		native->particleParticleCollisions = data.particle_collisions;
		native->raiseCollisionCallbacks = data.collision_callbacks;
		native->callbackImpulseThreshold = data.callback_impulse_threshold;
		if (data.damping)
		{
			native->integrator.type = Types_PhysicsFXPipelineIntegrator_LinAngDamping;
			native->integrator.u.linVelDampingIntegrator.linearDamping = data.damping->linear;
			native->integrator.u.linVelDampingIntegrator.angularDamping = data.damping->angular;
		}
		else
		{
			native->integrator.type = Types_PhysicsFXPipelineIntegrator_Basic;
		}

		return native;
	}

	std::size_t native_size(const asset_data::physics_fx_pipeline& data)
	{
		return rounded(sizeof(PhysicsFXPipeline)) + rounded(data.name.size() + 1);
	}

	std::expected<vector_field_source, std::string> to_neutral(const VectorField& native)
	{
		try
		{
			require(native.name, "the vector field has no name");
			require(zero_bytes(native.pad, sizeof(native.pad)), "the vector field's padding holds data");
			require(!native.numSubFields || native.subFields, "the vector field has sub fields without data");

			vector_field_source source;
			auto& data = source.field;
			auto& flags = source.provenance.fields;
			data.name = native.name;
			data.mins = triple(native.worldBounds.mins);
			data.maxs = triple(native.worldBounds.maxs);
			data.local_origin = triple(native.localOrigin);
			for (std::size_t index = 0; index < native.numSubFields; ++index)
			{
				const auto& value = native.subFields[index];
				require(zero_bytes(value.pad, sizeof(value.pad)), "a sub field's padding holds data");
				require(value.type < VECTOR_FIELD_NUM_TYPES, "a sub field's type has no neutral name");

				auto& subfield = data.subfields.emplace_back();
				subfield.mins = triple(value.worldBounds.mins);
				subfield.maxs = triple(value.worldBounds.maxs);
				subfield.cell_size = triple(value.ds);
				std::ranges::copy(value.numEntries, subfield.entries.begin());
				subfield.type = asset_data::vector_field_types[value.type];
				flags[std::format("subfield.{}.flags", index)] = asset_data::hex(value.flags);
				const std::size_t cells = static_cast<std::size_t>(value.numEntries[0]) * value.numEntries[1] * value.numEntries[2];
				require(!cells || value.linearData, "a sub field has cells without samples");
				if (cells)
				{
					const auto* samples = reinterpret_cast<const float*>(value.linearData);
					subfield.samples.assign(samples, samples + cells * 4);
				}
			}

			source.provenance.profile = fx_profile;
			source.provenance.kind = "vectorfield";
			source.provenance.name = data.name;
			asset_data::validate(data);
			return source;
		}
		catch (const std::exception& error)
		{
			return std::unexpected(error.what());
		}
	}

	VectorField* from_neutral(const asset_data::vector_field& data, const asset_data::provenance* source, zone_memory& memory)
	{
		asset_data::validate(data);
		if (!source || source->profile != fx_profile)
		{
			throw std::runtime_error("vectorfield " + data.name + " has no Replay sub field flags");
		}

		auto* native = memory.allocate<VectorField>();
		native->name = copy_text(memory, data.name);
		std::ranges::copy(data.mins, native->worldBounds.mins.v);
		std::ranges::copy(data.maxs, native->worldBounds.maxs.v);
		std::ranges::copy(data.local_origin, native->localOrigin.v);
		native->numSubFields = static_cast<std::uint32_t>(data.subfields.size());
		native->subFields = data.subfields.empty() ? nullptr :
			memory.manual_allocate<VectorSubField>(rounded(data.subfields.size() * sizeof(VectorSubField)));
		for (std::size_t index = 0; index < data.subfields.size(); ++index)
		{
			const auto& value = data.subfields[index];
			auto& target = native->subFields[index];
			std::ranges::copy(value.mins, target.worldBounds.mins.v);
			std::ranges::copy(value.maxs, target.worldBounds.maxs.v);
			std::ranges::copy(value.cell_size, target.ds.v);
			std::ranges::copy(value.entries, target.numEntries);
			target.type = static_cast<VectorFieldType>(std::ranges::find(asset_data::vector_field_types, value.type) -
				std::begin(asset_data::vector_field_types));
			target.flags = source->value<std::uint32_t>(std::format("subfield.{}.flags", index));
			if (!value.samples.empty())
			{
				auto* samples = memory.manual_allocate<float>(rounded(value.samples.size() * sizeof(float)));
				std::ranges::copy(value.samples, samples);
				target.linearData = reinterpret_cast<vec4_t*>(samples);
			}
		}

		return native;
	}

	std::size_t native_size(const asset_data::vector_field& data)
	{
		std::size_t size = rounded(sizeof(VectorField)) + rounded(data.name.size() + 1) + rounded(data.subfields.size() * sizeof(VectorSubField));
		for (const auto& subfield : data.subfields)
		{
			size += rounded(subfield.samples.size() * sizeof(float));
		}

		return size;
	}
}
