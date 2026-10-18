#pragma once

namespace zonetool::iw8
{
	// A dump file of an asset. Raw files, script files, string tables, localized strings, Lua files and fonts use the
	// files of the other games (src/core/assets) when those files hold the asset. Other assets use <pool>/<name>.iw8.
	struct asset_file
	{
		std::string path;
		std::string data;
		bool shared{};
	};

	// The files that a build reads for an asset, in order. The data of the files is empty.
	std::vector<asset_file> asset_file_locations(std::int32_t type, const std::string& name);

	// The name of a localized string in a zone. Stock names are lowercase, and the scripts use them in uppercase.
	std::string localize_name(const std::string& name);

	std::string dump_asset_data(std::int32_t type, const void* header);
	// The assets that the fields of an asset point at, by type and name.
	std::vector<std::pair<std::int32_t, std::string>> asset_dependencies(std::int32_t type, const void* header);
	asset_file dump_asset_file(std::int32_t type, const void* header);
	void dump_asset(std::int32_t type, const void* header);

	// Parses asset dumps. An asset that a dump names gets a name stub, and references() lists it.
	class asset_parser
	{
	public:
		void* parse(std::string_view data);
		// The asset from the first of its files in the search paths, or nullptr when there is no file.
		void* parse_file(std::int32_t type, const std::string& name);
		void* parse_asset_file(std::int32_t type, const std::string& name, const asset_file& file);
		void* stub(std::int32_t type, const std::string& name);

		const std::vector<std::pair<std::int32_t, std::string>>& references() const;

		void* allocate(std::size_t size);
		const char* duplicate(std::string_view value);

		template <typename T>
		T* allocate(const std::size_t count = 1)
		{
			return static_cast<T*>(this->allocate(sizeof(T) * count));
		}

	private:
		std::deque<std::unique_ptr<std::uint8_t[]>> memory_;
		std::vector<std::pair<std::int32_t, std::string>> references_;
	};
}
