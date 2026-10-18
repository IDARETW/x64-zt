#pragma once

// Havok 2018.2 tagfiles (TAG0) as Replay 1.20 stores them: compact files that name the types of the Replay global type
// compendium (TCRF), and self-describing files with a TYPE section.
namespace zonetool::iw8::havok
{
	struct type_member
	{
		std::string name;
		std::uint32_t flags{};
		std::uint32_t offset{};
		std::uint32_t type{};
	};

	struct type_info
	{
		std::string name;
		// Template parameters: a name that starts with 't' takes a type index, a name that starts with 'v' a value.
		std::vector<std::pair<std::string, std::uint64_t>> parameters;
		std::uint32_t parent{};
		std::uint32_t optionals{};
		std::uint64_t format{};
		std::uint32_t subtype{};
		std::uint64_t version{};
		std::uint32_t size{};
		std::uint32_t alignment{};
		std::uint64_t flags{};
		std::vector<type_member> members;
		// hkPropertyId stores 3 above its member count.
		std::uint32_t member_count_high{};
		std::optional<std::uint32_t> hash;
		bool has_body{};
	};

	enum class pointer_kind
	{
		object,
		relative_array,
		array,
	};

	// The types of a TYPE section. Index 0 is the null type.
	class type_table
	{
	public:
		static type_table read(std::span<const std::uint8_t> tagfile);

		std::size_t size() const;
		const type_info& at(std::uint32_t index) const;
		std::string identity(std::uint32_t index) const;
		std::optional<std::uint32_t> find(std::string_view identity) const;

		std::uint32_t size_of(std::uint32_t index) const;
		std::uint32_t alignment_of(std::uint32_t index) const;
		pointer_kind kind_of(std::uint32_t pointer_type) const;

	private:
		std::vector<type_info> types_;
		std::unordered_map<std::string, std::uint32_t> identities_;
	};

	struct pointer_field
	{
		// Byte offset in the data of the item.
		std::uint32_t offset{};
		std::uint32_t pointer_type{};
		std::uint32_t target{};
	};

	struct item
	{
		std::uint32_t type{};
		std::uint8_t flags{};
		std::uint32_t count{};
		std::vector<std::uint8_t> data;
		std::vector<pointer_field> pointers;
	};

	// The objects of a tagfile. Item 0 is the null item and item 1 the root object.
	struct tagfile
	{
		std::string sdk_version;
		std::optional<std::uint64_t> compendium;
		std::vector<item> items;
		// The local types of a self-describing tagfile.
		std::optional<type_table> types;
	};

	// A self-describing tagfile names a type that the compendium does not have.
	struct unknown_type : std::runtime_error
	{
		using std::runtime_error::runtime_error;
	};

	// The signature that compact Replay physics tagfiles give for the global type compendium.
	constexpr std::uint64_t replay_compendium_signature = 0xC16B2E49324C3540;

	// The type compendium signature of a compact tagfile, or nullopt for a self-describing one.
	std::optional<std::uint64_t> compendium_reference(std::span<const std::uint8_t> data);

	tagfile read_tagfile(std::span<const std::uint8_t> data, const type_table* compendium);

	// A compact tagfile that names the types of the compendium.
	std::vector<std::uint8_t> write_compact_tagfile(const tagfile& file, const type_table& compendium);
	// A self-describing tagfile: its TYPE section holds the compendium types that the items use.
	std::vector<std::uint8_t> write_self_describing_tagfile(const tagfile& file, const type_table& compendium);
}
