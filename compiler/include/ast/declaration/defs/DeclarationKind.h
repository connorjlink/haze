#ifndef HAZE_DECLARATIONKIND_H
#define	HAZE_DECLARATIONKIND_H

// Haze DeclarationKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#include <ast/declaration/defs/DeclarationKind.x>

	enum class DeclarationKind
	{
#define X(enumerator, type, name) enumerator,
		DECLARATION_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(DeclarationKind kind)
	{
		switch (kind)
		{
#define X(enumerator, type, name) case DeclarationKind::enumerator: return #name;
			DECLARATION_KINDS(X)
#undef X
		}

		return "<unknown declaration kind>";
	}
}

#endif
