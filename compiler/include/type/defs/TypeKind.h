#ifndef HAZE_TYPEKIND_H
#define HAZE_TYPEKIND_H

// Haze TypeKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TypeKind
	{
#define X(enumerator, name) enumerator,
#include <type/defs/TypeKind.def>
#undef X
	};

	constexpr std::string_view to_string(TypeKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case TypeKind::enumerator: return name;
#include <type/defs/TypeKind.def>
#undef X
		}

		return "<unknown type kind>";
	}
}

#endif
