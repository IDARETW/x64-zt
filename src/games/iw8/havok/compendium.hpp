#pragma once

#include "tagfile.hpp"

namespace zonetool::iw8::havok
{
	// The Replay global Havok type compendium: the code_post_gfx physics library physicsglobaltypecompendium. It is read
	// from the zone folder once.
	const type_table& replay_compendium(const std::filesystem::path& zone_folder);
}
