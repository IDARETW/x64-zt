#include <std_include.hpp>
#include "particle.hpp"

namespace zonetool::iw8::neutral
{
	namespace
	{
		// The kind of the assets that a module of this type lists (the dispatch of Load_ParticleLinkedAssetListDef).
		const char* linked_kind(const std::uint32_t module_type)
		{
			switch (module_type)
			{
			case 11:
				return "material";
			case 13:
			case 27:
				return "model";
			case 45:
				return "physics_fx";
			case 16:
				return "particle_sim";
			case 5:
				return "decal";
			case 22:
				return "sound";
			case 30:
				return "vector_field";
			case 9:
			case 10:
				return "light_def";
			case 2:
			case 21:
				return "effect";
			default:
				if (module_type >= 55 && module_type <= 61)
				{
					return "effect";
				}

				return nullptr;
			}
		}

		void require_unused_zero(const ParticleLinkedAssetDef& entry, const std::size_t used, const std::string& kind)
		{
			const auto* bytes = reinterpret_cast<const std::uint8_t*>(&entry);
			for (std::size_t index = used; index < sizeof(entry); ++index)
			{
				if (bytes[index])
				{
					std::string hex;
					for (std::size_t each = 0; each < sizeof(entry); ++each)
					{
						hex += std::format("{:02x}", bytes[each]);
					}

					plain::fail("a particle linked asset of kind " + kind, ("holds data at byte " + std::to_string(index) +
						" after the members its kind uses (" + hex + ")").c_str());
				}
			}
		}

		asset_data::particle_linked_asset read_entry(const ParticleLinkedAssetDef& entry, const std::string& kind)
		{
			asset_data::particle_linked_asset out;
			if (kind == "material")
			{
				out.names[0] = plain::read_asset(ASSET_TYPE_MATERIAL, entry.material);
				require_unused_zero(entry, 8, kind);
			}
			else if (kind == "model")
			{
				out.names[0] = plain::read_asset(ASSET_TYPE_XMODEL, entry.model);
				require_unused_zero(entry, 8, kind);
			}
			else if (kind == "light_def")
			{
				out.names[0] = plain::read_asset(ASSET_TYPE_LIGHT_DEF, entry.lightDef);
				require_unused_zero(entry, 8, kind);
			}
			else if (kind == "effect")
			{
				out.names[0] = plain::read_asset(ASSET_TYPE_VFX, entry.particleSystem);
				std::memcpy(&out.selector, reinterpret_cast<const std::uint8_t*>(&entry) + 8, sizeof(out.selector));
				require_unused_zero(entry, 12, kind);
			}
			else if (kind == "vector_field")
			{
				out.names[0] = plain::read_asset(ASSET_TYPE_VECTORFIELD, entry.vectorField);
				require_unused_zero(entry, 8, kind);
			}
			else if (kind == "particle_sim")
			{
				out.names[0] = plain::read_asset(ASSET_TYPE_PARTICLE_SIM_ANIMATION, entry.particleSim);
				require_unused_zero(entry, 8, kind);
			}
			else if (kind == "sound")
			{
				out.names[0] = plain::read_string(entry.sound);
				require_unused_zero(entry, 8, kind);
			}
			else if (kind == "decal")
			{
				for (std::size_t material = 0; material < 3; ++material)
				{
					out.names[material] = plain::read_asset(ASSET_TYPE_MATERIAL, entry.decal.materials[material]);
				}

				require_unused_zero(entry, 24, kind);
			}
			else
			{
				out.names[0] = plain::read_asset(ASSET_TYPE_PHYSICS_FX_PIPELINE, entry.physicsFXData.physicsFXPipeline);
				out.names[1] = plain::read_asset(ASSET_TYPE_PHYSICS_FX_SHAPE, entry.physicsFXData.physicsFXShape);
				require_unused_zero(entry, 16, kind);
			}

			return out;
		}

		void write_entry(ParticleLinkedAssetDef& entry, const asset_data::particle_linked_asset& data, const std::string& kind,
			const asset_resolver& resolve, plain::context& memory)
		{
			if (kind == "material")
			{
				entry.material = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.names[0]);
			}
			else if (kind == "model")
			{
				entry.model = plain::resolve_asset<XModel>(resolve, ASSET_TYPE_XMODEL, data.names[0]);
			}
			else if (kind == "light_def")
			{
				entry.lightDef = plain::resolve_asset<GfxLightDef>(resolve, ASSET_TYPE_LIGHT_DEF, data.names[0]);
			}
			else if (kind == "effect")
			{
				entry.particleSystem = plain::resolve_asset<ParticleSystemDef>(resolve, ASSET_TYPE_VFX, data.names[0]);
				std::memcpy(reinterpret_cast<std::uint8_t*>(&entry) + 8, &data.selector, sizeof(data.selector));
			}
			else if (kind == "vector_field")
			{
				entry.vectorField = plain::resolve_asset<VectorField>(resolve, ASSET_TYPE_VECTORFIELD, data.names[0]);
			}
			else if (kind == "particle_sim")
			{
				entry.particleSim = plain::resolve_asset<FxParticleSimAnimation>(resolve, ASSET_TYPE_PARTICLE_SIM_ANIMATION, data.names[0]);
			}
			else if (kind == "sound")
			{
				entry.sound = plain::write_string(memory, data.names[0]);
			}
			else if (kind == "decal")
			{
				for (std::size_t material = 0; material < 3; ++material)
				{
					entry.decal.materials[material] = plain::resolve_asset<Material>(resolve, ASSET_TYPE_MATERIAL, data.names[material]);
				}
			}
			else
			{
				entry.physicsFXData.physicsFXPipeline = plain::resolve_asset<PhysicsFXPipeline>(resolve, ASSET_TYPE_PHYSICS_FX_PIPELINE, data.names[0]);
				entry.physicsFXData.physicsFXShape = plain::resolve_asset<PhysicsFXShape>(resolve, ASSET_TYPE_PHYSICS_FX_SHAPE, data.names[1]);
			}
		}
	}

	asset_data::particle_linked_asset_list read_particle_linked_asset_list(const ParticleLinkedAssetListDef& native)
	{
		const auto* kind = linked_kind(plain::module_scope::current());
		if (!kind)
		{
			plain::fail("a particle linked asset list", "belongs to a module type that lists no assets");
		}

		asset_data::particle_linked_asset_list data;
		data.kind = kind;
		data.assets = plain::read_array(native.assetList, native.numAssets, "ParticleLinkedAssetListDef.assetList",
			[&](const ParticleLinkedAssetDef& entry) { return read_entry(entry, data.kind); });
		return data;
	}

	void write_particle_linked_asset_list(ParticleLinkedAssetListDef& native, const asset_data::particle_linked_asset_list& data,
		const asset_resolver& resolve, plain::context& memory)
	{
		const auto* expected = linked_kind(plain::module_scope::current());
		if (!expected || data.kind != expected)
		{
			plain::fail("a particle linked asset list", "has a kind that its module type does not list");
		}

		native.numAssets = data.assets ? static_cast<std::int32_t>(data.assets->size()) : 0;
		native.assetList = plain::write_array<ParticleLinkedAssetDef>(data.assets, memory,
			[&](ParticleLinkedAssetDef& entry, const asset_data::particle_linked_asset& value) { write_entry(entry, value, data.kind, resolve, memory); });
	}

	std::size_t size_particle_linked_asset_list(const asset_data::particle_linked_asset_list& data)
	{
		std::size_t size = 0;
		if (data.assets)
		{
			size += plain::rounded(std::max<std::size_t>(data.assets->size(), 1) * sizeof(ParticleLinkedAssetDef));
			for (const auto& asset : *data.assets)
			{
				for (const auto& name : asset.names)
				{
					size += plain::string_size(name);
				}
			}
		}

		return size;
	}

	asset_data::particle_scripted_input_node read_particle_scripted_input_node(const ParticleScriptedInputNodeDef& native)
	{
		asset_data::particle_scripted_input_node data;
		const auto& base = native.nodeBase;
		data.flags = base.m_flags;
		data.emitter_index = base.m_emitterIndex;
		plain::require_zero(base.m_pad, "ParticleScriptedInputNode.m_pad");
		if (base.m_type == PARTICLE_SCRIPTED_INPUT_NODE_KVP)
		{
			data.kind = "kvp";
			data.key = static_cast<std::uint32_t>(native.KVP.m_inputRecord.key);
			data.dirty = native.KVP.m_inputRecord.dirty;
			data.value = native.KVP.m_inputRecord.value;
			plain::require_zero(native.KVP.m_inputRecord.pad, "ParticleScriptedInputRecord.pad");
		}
		else if (base.m_type == PARTICLE_SCRIPTED_INPUT_NODE_EMITTER_DISABLE)
		{
			data.kind = "emitter_disable";
			const auto* bytes = reinterpret_cast<const std::uint8_t*>(&native);
			for (std::size_t index = sizeof(ParticleScriptedInputNode); index < sizeof(native); ++index)
			{
				if (bytes[index])
				{
					plain::fail("a scripted input node", "holds data after an emitter disable node");
				}
			}
		}
		else
		{
			plain::fail("a scripted input node", "has an unknown type");
		}

		return data;
	}

	void write_particle_scripted_input_node(ParticleScriptedInputNodeDef& native, const asset_data::particle_scripted_input_node& data,
		const asset_resolver&, plain::context&)
	{
		auto& base = native.nodeBase;
		base.m_flags = data.flags;
		base.m_emitterIndex = data.emitter_index;
		if (data.kind == "kvp")
		{
			base.m_type = PARTICLE_SCRIPTED_INPUT_NODE_KVP;
			native.KVP.m_inputRecord.key = static_cast<ParticleScriptedInputKey>(data.key);
			native.KVP.m_inputRecord.dirty = data.dirty;
			native.KVP.m_inputRecord.value = data.value;
		}
		else
		{
			base.m_type = PARTICLE_SCRIPTED_INPUT_NODE_EMITTER_DISABLE;
		}
	}

	std::size_t size_particle_scripted_input_node(const asset_data::particle_scripted_input_node&)
	{
		return 0;
	}
}
