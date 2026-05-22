#ifndef HAZE_TYPEKIND_H
#define HAZE_TYPEKIND_H

// Haze TypeKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeKind
	{
#define X(enumerator, name) enumerator,
#include <type/TypeKind.def>
#undef X
	};

	constexpr std::string_view to_string(TypeKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case TypeKind::enumerator: return name;
#include <type/TypeKind.def>
#undef X
		}

		return "<unknown type kind>";
	}
}

template<>
struct std::formatter<hz::TypeKind>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}
	auto format(const hz::TypeKind& kind, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", to_string(kind));
	}
};

#endif
