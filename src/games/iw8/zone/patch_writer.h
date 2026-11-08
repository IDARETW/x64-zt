#pragma once

#include <filesystem>

namespace zonetool::iw8
{
	// Writes the Replay patch (.fp) that turns the base fastfile into the patched one. The game applies a patch beside
	// the deployed zone of the same name (xfile.cpp apply_iw8_patch), so a rebuilt zone ships as a small delta.
	//
	// The base and the patched zones are read as their own payloads (xfile::read with apply_patch = false), so any
	// .fp beside them is ignored. Both headers are copied byte for byte: the reader accepts a patch only when its base
	// header equals the deployed file's header.
	void write_patch(const std::filesystem::path& base, const std::filesystem::path& patched,
		const std::filesystem::path& output);
}
