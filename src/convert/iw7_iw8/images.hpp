#pragma once

#include "source.hpp"

#include <DirectXTex.h>

namespace zonetool::convert::iw7_iw8
{
	// The top level of each face of an image (one, the six of a cube map, or six for each cube map of an array) in a
	// format that DirectXTex processes, at most `limit` pixels (0: the size that the extraction keeps for material
	// images).
	std::vector<DirectX::ScratchImage> load_image_faces(const iw7::GfxImage& image, DXGI_FORMAT format, std::size_t limit = 0);

	// The colour map of a material, or nullptr.
	const iw7::GfxImage* color_image(const source_map& source, const iw7::Material* material);
}
