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

#define DEFINE_BITFIELD_INTERNAL(enummember, appendname, valuematch, kinds, type, name, extras) \
	enum struct type extras \
	{ \
		kinds(enummember) \
	}; \
	constexpr std::string to_string(type kind) \
	{ \
		auto result = std::string{}; \
		kinds(appendname) \
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
				kinds(valuematch) \
			} \
			if (end == std::string_view::npos) \
			{ \
				break; \
			} \
			start = end + 1; \
		} \
		return result; \
	}


#define _PRIVATE_BITFIELD_ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define _PRIVATE_BITFIELD_APPEND_NAME(enumerator, name, value) \
	if (kind & value) \
	{ \
		if (!result.empty()) \
		{ \
			result += ", "; \
		} \
		result += #name; \
	}
#define _PRIVATE_BITFIELD_VALUE_MATCH(enumerator, name, value) \
	if (token == #name) \
	{ \
		result |= EnumType::enumerator; \
	}

#define BITFIELD_EXPAND(x) x

#define BITFIELD_ENUM_MEMBER(...) BITFIELD_EXPAND(BITFIELD_ROUTER(_PRIVATE_BITFIELD_ENUM_MEMBER, __VA_ARGS__))
#define BITFIELD_APPEND_NAME(...) BITFIELD_EXPAND(BITFIELD_ROUTER(_PRIVATE_BITFIELD_APPEND_NAME, __VA_ARGS__))
#define BITFIELD_VALUE_MATCH(...) BITFIELD_EXPAND(BITFIELD_ROUTER(_PRIVATE_BITFIELD_VALUE_MATCH, __VA_ARGS__))

// NOTE: define BITFIELD_ROUTER prior to invoking this macro to route properly the arguments through each X-chain per distinct enumeration type
#define DEFINE_BITFIELD(kinds, type, name) \
	DEFINE_BITFIELD_INTERNAL(BITFIELD_ENUM_MEMBER, BITFIELD_APPEND_NAME, BITFIELD_VALUE_MATCH, kinds, type, name, : std::uint32_t)

#define DEFINE_BITFIELD_BASE(kinds, type, name, base) \
	DEFINE_BITFIELD_INTERNAL(BITFIELD_ENUM_MEMBER, BITFIELD_APPEND_NAME, BITFIELD_VALUE_MATCH, kinds, type, name, : base)

#endif
