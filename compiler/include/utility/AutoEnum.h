#ifndef HAZE_AUTOENUM_H
#define HAZE_AUTOENUM_H

// Haze AutoEnum.h
// (c) Connor J. Link. All Rights Reserved.

// NOTE: the below declaration is only a guide to avoid from_string being an overload only on return type
namespace hz
{
	template<typename T>
		requires std::is_scoped_enum_v<T>
	constexpr std::optional<T> from_string(std::string_view) = delete;
}

#define DEFINE_ENUM_INTERNAL(enummember, switchcase, mapmember, forwarddeclaration, kinds, type, name, extras) \
	enum class type extras \
	{ \
		kinds(enummember) \
	}; \
	kinds(forwarddeclaration) \
	constexpr std::string_view to_string(type kind) \
	{ \
		switch (kind) \
		{ \
			kinds(switchcase) \
		} \
		return "<unknown " #name ">"; \
	} \
	template<> \
	constexpr std::optional<type> from_string<type>(std::string_view string) \
	{ \
		struct Mapping \
		{ \
			std::string_view enumerator; \
			type value; \
		}; \
		constexpr auto lookup = []() \
		{ \
			std::array mappings \
			{ \
				kinds(mapmember) \
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


#pragma message("TODO: include specific serialize/deserialize functions for bitfield type enums")

#define DEFINE_ENUM(enummember, switchcase, mapmember, forwarddeclaration, kinds, type, name) \
	DEFINE_ENUM_INTERNAL(enummember, switchcase, mapmember, forwarddeclaration, kinds, type, name, )

#define DEFINE_ENUM_BACKED(enummember, switchcase, mapmember, forwarddeclaration, kinds, type, name, base) \
	DEFINE_ENUM_INTERNAL(enummember, switchcase, mapmember, forwarddeclaration, kinds, type, name, base)

#endif
