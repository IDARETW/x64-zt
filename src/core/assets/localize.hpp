#pragma once

#include <formats/localized_strings.hpp>

namespace zonetool
{
	template <ASSET_TEMPLATE>
	class localize : public asset_interface
	{
	private:
		std::string name_;
		S* asset_ = nullptr;

	public:
		static void add_localized_string(zone_base* zone, const std::string& name, const std::string& value, const std::string& path)
		{
			S loc{};
			loc.name = name.data();
			loc.value = value.data();

			auto type = zone->get_type_by_name("localize");
			if (type == -1)
			{
				ZONETOOL_ERROR("Could not translate typename localize to an integer!");
			}
			try
			{
				zone->add_asset_of_type_by_pointer(type, &loc);
			}
			catch (const std::exception& e)
			{
				ZONETOOL_FATAL("A fatal exception occured while adding localizedstring: \"%s\" from file: \"%s\", exception was: \n%s",
					name.data(), path.data(), e.what());
			}
		}

		static bool parse_localizedstrings_json(zone_base* zone, const std::string& file_name)
		{
			return localized_strings::parse_json(file_name, [&](const std::string& name, const std::string& value)
			{
				add_localized_string(zone, name, value, "localizedstrings\\"s + file_name + ".json");
			});
		}

		static bool parse_localizedstrings_file(zone_base* zone, const std::string& file_name)
		{
			return localized_strings::parse_str(file_name, [&](const std::string& name, const std::string& value)
			{
				add_localized_string(zone, name, value, "localizedstrings\\"s + file_name + ".str");
			});
		}

		S* parse(const std::string& name, zone_memory* mem)
		{
			const auto path = "localizedstrings\\"s + name;
			auto file = filesystem::file(path);
			file.open("rb");

			if (file.get_fp())
			{
				ZONETOOL_INFO("Parsing localizedstring \"%s\"...", name.data());

				auto* localized = mem->allocate<S>();
				localized->name = mem->duplicate_string(name);

				const auto size = file.size();
				auto data = file.read_bytes(size);
				localized->value = mem->duplicate_string(std::string(data.begin(), data.end()));

				file.close();

				return localized;
			}

			return nullptr;
		}

		void init(void* asset, zone_memory* mem)
		{
			if (asset)
			{
				auto* m_asset = reinterpret_cast<S*>(asset);

				// allocate asset in zone memory
				this->asset_ = mem->allocate<S>();
				this->asset_->name = mem->duplicate_string(m_asset->name);
				this->asset_->value = mem->duplicate_string(m_asset->value);
			}
			else
			{
				this->asset_ = db_find_x_asset_header_safe<H, E>(this->type(), this->name().data()).localize;
				if (!this->asset_)
				{
					return;
				}
			}
			this->name_ = this->asset_->name;
		}

		void init(const std::string& name, zone_memory* mem)
		{
			this->name_ = name;

			if (this->referenced())
			{
				this->asset_ = mem->allocate<typename std::remove_reference<decltype(*this->asset_)>::type>();
				this->asset_->name = mem->duplicate_string(name);
				return;
			}

			this->asset_ = parse(name, mem);
			if (!this->asset_)
			{
				this->asset_ = db_find_x_asset_header_safe<H, E>(this->type(), this->name_.data()).localize;
			}
		}

		void prepare(zone_buffer* buf, zone_memory* mem)
		{
		}

		void load_depending(zone_base* zone)
		{
		}

		void* pointer() override { return asset_; }

		bool referenced() override { return name_.starts_with(","); }

		std::string name()
		{
			return this->name_;
		}

		std::int32_t type()
		{
			return Type;
		}

		static void read(zone_reader& reader, S* asset)
		{
			reader.push_stream(Streams::XFILE_BLOCK_VIRTUAL);
			reader.read_string(asset->value);
			reader.read_string(asset->name);
			reader.pop_stream();
		}

		void write(zone_base* zone, zone_buffer* buf)
		{
			auto* data = this->asset_;
			auto* dest = buf->write<S>(data);

			buf->push_stream(Streams::XFILE_BLOCK_VIRTUAL);

			std::string name = this->name();
			if (buf->mode == game::iw7)
			{
				name = utils::string::to_lower(name);
			}

			if (data->value)
			{
				dest->value = buf->write_str(data->value);
			}
			dest->name = buf->write_str(name);

			buf->pop_stream();
		}

		static void dump(S* asset)
		{
			if (asset)
			{
				const auto path = "localizedstrings\\"s + asset->name;
				auto file = filesystem::file(path);
				file.open("wb");
				file.write(asset->value, strlen(asset->value), 1);
				file.close();
			}
		}
	};
}
