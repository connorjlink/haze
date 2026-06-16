#ifndef HAZE_AUTOENUM_H
#define HAZE_AUTOENUM_H

#include <utility/AutoMacroTraits.h>
#include <utility/Formatter.h>

// Haze AutoEnum.h
// (c) Connor J. Link. All Rights Reserved.

#define _PRIVATE_DEFINE_ENUM(enummember, switchcase, datamember, structtype, kinds, type, name, extras) \
	enum struct type extras \
	{ \
		kinds(enummember) \
	}; \
	kinds(structtype) \
	constexpr std::string_view to_string(type kind) \
	{ \
		switch (kind) \
		{ \
			kinds(switchcase) \
		} \
		return "<unknown " #name ">"; \
	} \
	template<> \
	struct hz::IsAutoEnum<type> : std::true_type \
	{ \
	}; \
	template<> \
	constexpr std::optional<type> from_string<type>(std::string_view string) \
	{ \
		struct Mapping \
		{ \
			std::string_view enumerator; \
			type value; \
		}; \
		auto lookup = []() \
		{ \
			std::array mappings \
			{ \
				kinds(datamember) \
			}; \
			std::sort(mappings.begin(), mappings.end(), [](const Mapping& a, const Mapping& b) \
			{ \
				return a.enumerator < b.enumerator; \
			}); \
			return mappings; \
		}(); \
		const auto it = std::lower_bound(lookup.begin(), lookup.end(), string, [](const Mapping& mapping, std::string_view val) \
		{ \
			return mapping.enumerator < val; \
		}); \
		if (it != lookup.end() && it->enumerator == string) \
		{ \
			return it->value; \
		} \
		return std::nullopt; \
	}

#define _PRIVATE_AUTOENUM_ENUM_MEMBER(enumerator, type, name, enumeration) enumerator,
#define _PRIVATE_AUTOENUM_SWITCH_CASE(enumerator, type, name, enumeration) case enumeration::enumerator: return #name;
#define _PRIVATE_AUTOENUM_DATA_MEMBER(enumerator, type, name, enumeration) Mapping{ #name, enumeration::enumerator },
#define _PRIVATE_AUTOENUM_STRUCT_TYPE(enumerator, type, name, enumeration) struct type;

#define AUTOENUM_EXPAND(x) x

#define AUTOENUM_ENUM_MEMBER(...) AUTOENUM_EXPAND(AUTOENUM_ROUTER(_PRIVATE_AUTOENUM_ENUM_MEMBER, __VA_ARGS__))
#define AUTOENUM_SWITCH_CASE(...) AUTOENUM_EXPAND(AUTOENUM_ROUTER(_PRIVATE_AUTOENUM_SWITCH_CASE, __VA_ARGS__))
#define AUTOENUM_DATA_MEMBER(...) AUTOENUM_EXPAND(AUTOENUM_ROUTER(_PRIVATE_AUTOENUM_DATA_MEMBER, __VA_ARGS__))
#define AUTOENUM_STRUCT_TYPE(...) AUTOENUM_EXPAND(AUTOENUM_ROUTER(_PRIVATE_AUTOENUM_STRUCT_TYPE, __VA_ARGS__))

// NOTE: define AUTOENUM_ROUTER prior to invoking this macro to route properly the arguments through each X-chain per distinct enumeration type
#define DEFINE_ENUM(kinds, type, name) \
	_PRIVATE_DEFINE_ENUM(AUTOENUM_ENUM_MEMBER, AUTOENUM_SWITCH_CASE, AUTOENUM_DATA_MEMBER, AUTOENUM_STRUCT_TYPE, kinds, type, name, )

#define DEFINE_ENUM_BASE(kinds, type, name, base) \
	_PRIVATE_DEFINE_ENUM(AUTOENUM_ENUM_MEMBER, AUTOENUM_SWITCH_CASE, AUTOENUM_DATA_MEMBER, AUTOENUM_STRUCT_TYPE, kinds, type, name, : base)

#endif
