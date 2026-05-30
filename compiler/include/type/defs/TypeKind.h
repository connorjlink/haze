#ifndef HAZE_TYPEKIND_H
#define HAZE_TYPEKIND_H

// Haze TypeKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#include <type/defs/TypeKind.x>

	enum class TypeKind
	{
#define X(enumerator, name, type) enumerator,
		TYPE_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(TypeKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case TypeKind::enumerator: return #name;
			TYPE_KINDS(X)
#undef X
		}

		return "<unknown type kind>";
	}
}

#endif
