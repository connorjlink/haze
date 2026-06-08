#ifndef HAZE_AUTOBITFIELD_H
#define HAZE_AUTOBITFIELD_H

#include <utility/Formatter.h>

// Haze AutoBitfield.h
// (c) Connor J. Link. All Rights Reserved.

// NOTE: the below declaration is only a guide to avoid from_string being an overload only on return type
namespace hz
{
	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr T from_string(std::string_view) = delete;
}

#define DEFINE_BITFIELD_INTERNAL(enummember, ifbranch, bitcheck, kinds, type, name, extras) \
	enum class type extras \
	{ \
		kinds(enummember) \
	}; \
	constexpr std::string to_string(type kind) \
	{ \
		auto result = std::string{}; \
		kinds(ifbranch) \
		return result; \
	} \
	template<> \
	type from_string<type>(std::string_view string) \
	{ \
		using EnumType = type; \
		type result = static_cast<type>(0); \
		if (string.empty()) \
		{ \
			return result; \
		} \
		for (auto start = 0uz; start < string.size();) \
		{ \
			const auto end = string.find(',', start); \
			auto token = string.substr(start, end == std::string_view::npos ? std::string_view::npos : end - start); \
			const auto first = token.find_first_not_of(WHITESPACE); \
			if (first != std::string_view::npos) \
			{ \
				const auto last = token.find_last_not_of(WHITESPACE); \
				token = token.substr(first, (last - first + 1)); \
				auto found = false; \
				kinds(bitcheck) \
			} \
			if (end == std::string_view::npos) \
			{ \
				break; \
			} \
			start = end + 1; \
		} \
		return result; \
	}


#pragma message("TODO: duplicate this adapter pattern across to AutoEnum also")

#define BITFIELD_ENUM_MEMBER(enumerator, name, value) enumerator = value,

#define BITFIED_IF_BRANCH(enumerator, name, value) \
	if (kind & value) \
	{ \
		if (!result.empty()) \
		{ \
			result += ", "; \
		} \
		result += #name; \
	}

#define BITFIELD_BIT_CHECK(enumerator, name, value) \
	if (token == #name) \
	{ \
		result |= EnumType::enumerator; \
	}

#define DEFINE_BITFIELD(enummember, ifbranch, bitcheck, kinds, type, name) \
	DEFINE_BITFIELD_INTERNAL(ENUM_MEMBER, ifbranch, bitcheck, kinds, type, name, : std::uint32_t)

#define DEFINE_BITFIELD_BACKED(enummember, ifbranch, bitcheck, kinds, type, name, base) \
	DEFINE_BITFIELD_INTERNAL(ENUM_MEMBER, ifbranch, bitcheck, kinds, type, name, : base)
		


}

#endif
