import std;

#include <symbol/Symbol.h>

// Haze Symbol.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	SymbolKind FunctionSymbol::ytype() const
	{
		return SymbolKind::FUNCTION;
	}

	SymbolKind ArgumentSymbol::ytype() const
	{
		return SymbolKind::ARGUMENT;
	}

	SymbolKind VariableSymbol::ytype() const
	{
		return SymbolKind::VARIABLE;
	}

	SymbolKind DefineSymbol::ytype() const
	{
		return SymbolKind::DEFINE;
	}

	SymbolKind LabelSymbol::ytype() const
	{
		return SymbolKind::LABEL;
	}

	SymbolKind StructSymbol::ytype() const
	{
		return SymbolKind::STRUCT;
	}
}
