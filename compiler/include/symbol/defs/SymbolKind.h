#ifndef HAZE_SYMBOLKIND_H
#define HAZE_SYMBOLKIND_H

#include <utility/AutoEnum.h>

// Haze SymbolKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#include <symbol/defs/SymbolKind.x>

#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case SymbolKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, SYMBOL_KINDS, Symbol, symbol)
}

#endif 
