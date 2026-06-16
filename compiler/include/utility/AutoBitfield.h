#ifndef HAZE_AUTOBITFIELD_H
#define HAZE_AUTOBITFIELD_H

#include <utility/AutoMacroTraits.h>
#include <utility/Constants.h>
#include <utility/Formatter.h>

// Haze AutoBitfield.h
// (c) Connor J. Link. All Rights Reserved.

#define _PRIVATE_DEFINE_BITFIELD(enummember, appendname, valuematch, kinds, type, name, extras) \
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
	struct hz::IsAutoBitfield<type> : std::true_type \
	{ \
	}; \
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


#define _PRIVATE_AUTOBITFIELD_ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define _PRIVATE_AUTOBITFIELD_APPEND_NAME(enumerator, name, value) \
	if (kind & value) \
	{ \
		if (!result.empty()) \
		{ \
			result += ", "; \
		} \
		result += #name; \
	}
#define _PRIVATE_AUTOBITFIELD_VALUE_MATCH(enumerator, name, value) \
	if (token == #name) \
	{ \
		result |= EnumType::enumerator; \
	}

#define AUTOBITFIELD_EXPAND(x) x

#define AUTOBITFIELD_ENUM_MEMBER(...) AUTOBITFIELD_EXPAND(AUTOBITFIELD_ROUTER(_PRIVATE_AUTOBITFIELD_ENUM_MEMBER, __VA_ARGS__))
#define AUTOBITFIELD_APPEND_NAME(...) AUTOBITFIELD_EXPAND(AUTOBITFIELD_ROUTER(_PRIVATE_AUTOBITFIELD_APPEND_NAME, __VA_ARGS__))
#define AUTOBITFIELD_VALUE_MATCH(...) AUTOBITFIELD_EXPAND(AUTOBITFIELD_ROUTER(_PRIVATE_AUTOBITFIELD_VALUE_MATCH, __VA_ARGS__))

// NOTE: define AUTOBITFIELD_ROUTER prior to invoking this macro to route properly the arguments through each X-chain per distinct enumeration type
#define DEFINE_BITFIELD(kinds, type, name) \
	_PRIVATE_DEFINE_BITFIELD(AUTOBITFIELD_ENUM_MEMBER, AUTOBITFIELD_APPEND_NAME, AUTOBITFIELD_VALUE_MATCH, kinds, type, name, : std::uint32_t)

#define DEFINE_BITFIELD_BASE(kinds, type, name, base) \
	_PRIVATE_DEFINE_BITFIELD(AUTOBITFIELD_ENUM_MEMBER, AUTOBITFIELD_APPEND_NAME, AUTOBITFIELD_VALUE_MATCH, kinds, type, name, : base)

#endif
