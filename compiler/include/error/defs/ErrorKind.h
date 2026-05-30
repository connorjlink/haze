#ifndef HAZE_ERRORKIND_H
#define HAZE_ERRORKIND_H

#include <utility/AutoEnum.h>

// Haze ErrorKind.h
// (c) Connor J. Link. All Rights Reserved.

// weird Windows.h stuff... again...
#undef ERROR

namespace hz
{
#include <error/defs/ErrorKind.x>

#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case SymbolKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, SYMBOL_KINDS, ErrorKind, error kind)
}

#endif
