#pragma once

namespace zonetool::iw8
{
	// The dump file of an asset: <pool>/<name>.iw8 in the dump folder, or in the search paths of a build.
	std::string asset_file_path(std::int32_t type, const std::string& name);

	std::string dump_asset_data(std::int32_t type, const void* header);
	void dump_asset(std::int32_t type, const void* header);

	// Parses asset dumps. An asset that a dump names gets a name stub, and references() lists it.
	class asset_parser
	{
	public:
		void* parse(std::string_view data);
		// The asset from its dump file in the search paths, or nullptr when there is none.
		void* parse_file(std::int32_t type, const std::string& name);
		void* stub(std::int32_t type, const std::string& name);

		const std::vector<std::pair<std::int32_t, std::string>>& references() const;

	private:
		std::deque<std::unique_ptr<std::uint8_t[]>> memory_;
		std::deque<std::string> names_;
		std::vector<std::pair<std::int32_t, std::string>> references_;

		void* allocate(std::size_t size);
	};
}
