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


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, SymbolKind)

	DEFINE_ENUM(SYMBOL_KINDS, SymbolKind, symbol)

#undef AUTOENUM_ROUTEr
}

#endif 
