#ifndef HAZE_SYMBOLKIND_H
#define HAZE_SYMBOLKIND_H

// Haze SymbolKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class SymbolKind
	{
#define X(enumerator, name) enumerator,
#include <symbol/defs/SymbolKind.x>
#undef X
	};

	constexpr std::string_view to_string(SymbolKind kind)
	{
		switch (kind)
		{
#define X(enumerator, name) case SymbolKind::enumerator: return #name;
#include <symbol/defs/SymbolKind.x>
#undef X
		}

		return "<unknown symbol kind>";
	}
}

#endif 
