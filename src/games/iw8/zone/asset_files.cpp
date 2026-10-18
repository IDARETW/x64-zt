#include <std_include.hpp>
#include "../iw8.hpp"
#include "asset_files.hpp"
#include "loader.hpp"

namespace zonetool::iw8
{
	namespace
	{
		std::string name_of(const std::int32_t type, const void* header)
		{
			const auto name = get_asset_name(type, header);
			return name ? name : "";
		}

		const asset_format& format_of(const std::int32_t type)
		{
			const auto format = find_asset_format(type);
			if (!format)
			{
				throw std::runtime_error(std::format("asset type {} has no format", type));
			}

			return *format;
		}
	}

	std::string asset_file_path(const std::int32_t type, const std::string& name)
	{
		// Characters that Windows does not take in a file name; the path keeps the folders of the name.
		auto file = name;
		for (auto& c : file)
		{
			if (static_cast<unsigned char>(c) < 32 || std::string_view("<>:\"|?*").contains(c))
			{
				c = '_';
			}
		}

		return std::format("{}/{}.iw8", format_of(type).name, file);
	}

	std::string dump_asset_data(const std::int32_t type, const void* header)
	{
		const auto& format = format_of(type);
		dump_stream stream(name_of);
		stream.begin(type, header, format.size);
		format.dump(stream, const_cast<void*>(header));
		return stream.finish();
	}

	void dump_asset(const std::int32_t type, const void* header)
	{
		const auto data = dump_asset_data(type, header);
		filesystem::file file(asset_file_path(type, name_of(type, header)));
		file.open("wb");
		file.write(data.data(), data.size(), 1);
		file.close();
	}

	void* asset_parser::parse(const std::string_view data)
	{
		parse_stream stream(data,
			[this](const std::size_t size)
			{
				return this->allocate(size);
			},
			[this](const std::int32_t type, const std::string& name)
			{
				this->references_.emplace_back(type, name);
				return this->stub(type, name);
			});

		const auto& format = format_of(stream.type());
		const auto header = stream.begin(format.size);
		format.parse(stream, header);
		stream.finish();
		return header;
	}

	void* asset_parser::parse_file(const std::int32_t type, const std::string& name)
	{
		filesystem::file file(asset_file_path(type, name));
		file.open("rb");
		if (!file.get_fp())
		{
			return nullptr;
		}

		const auto bytes = file.read_bytes(file.size());
		file.close();

		try
		{
			return this->parse(std::string_view(reinterpret_cast<const char*>(bytes.data()), bytes.size()));
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::format("{} \"{}\": {}", format_of(type).name, name, e.what()));
		}
	}

	void* asset_parser::stub(const std::int32_t type, const std::string& name)
	{
		const auto header = static_cast<const char**>(this->allocate(format_of(type).size));
		*header = this->names_.emplace_back(name).data();
		return header;
	}

	const std::vector<std::pair<std::int32_t, std::string>>& asset_parser::references() const
	{
		return this->references_;
	}

	void* asset_parser::allocate(const std::size_t size)
	{
		auto& memory = this->memory_.emplace_back(std::make_unique<std::uint8_t[]>(std::max<std::size_t>(size, 1)));
		std::memset(memory.get(), 0, std::max<std::size_t>(size, 1));
		return memory.get();
	}
}
