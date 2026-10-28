#pragma once

#include <core/assets/particle_data.hpp>
#include "resolver.hpp"
#include "plain.hpp"
#include "../structs.hpp"

// The parts of a Replay 1.20 particle system that the generated adapter (tools/gen_iw8_neutral.py, spec vfx.json) hands to
// code written by hand, because what they hold depends on a context that the structs do not state.
namespace zonetool::iw8::neutral
{
	// The list of assets of an event module. The kind of the assets follows from the type of the module that holds the
	// list, which the generated code puts in plain::module_scope while it reads or writes the module.
	asset_data::particle_linked_asset_list read_particle_linked_asset_list(const ParticleLinkedAssetListDef& native);
	void write_particle_linked_asset_list(ParticleLinkedAssetListDef& native, const asset_data::particle_linked_asset_list& data,
		const asset_resolver& resolve, plain::context& memory);
	std::size_t size_particle_linked_asset_list(const asset_data::particle_linked_asset_list& data);

	asset_data::particle_scripted_input_node read_particle_scripted_input_node(const ParticleScriptedInputNodeDef& native);
	void write_particle_scripted_input_node(ParticleScriptedInputNodeDef& native, const asset_data::particle_scripted_input_node& data,
		const asset_resolver& resolve, plain::context& memory);
	std::size_t size_particle_scripted_input_node(const asset_data::particle_scripted_input_node& data);
}
