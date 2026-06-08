#ifndef HAZE_SYMBOLKIND_H
#define HAZE_SYMBOLKIND_H

#include <utility/AutoEnum.h>

// Haze SymbolKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define SYMBOL_KINDS(X) \
	X(FUNCTION, function) \
	X(VARIABLE, variable) \
	X(LABEL,    label) \
	X(TYPE,     type) 


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case SymbolKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, SymbolKind::enumerator },
#define FORWARD_DECLARATION(enumerator, name) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, SYMBOL_KINDS, Symbol, symbol)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif 
