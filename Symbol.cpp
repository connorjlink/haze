#include "Symbol.h"

// Haze Symbol.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	SymbolType FunctionSymbol::ytype() const
	{
		return SymbolType::FUNCTION;
	}

	SymbolType ArgumentSymbol::ytype() const
	{
		return SymbolType::ARGUMENT;
	}

	SymbolType VariableSymbol::ytype() const
	{
		return SymbolType::VARIABLE;
	}

	SymbolType DefineSymbol::ytype() const
	{
		return SymbolType::DEFINE;
	}

	SymbolType LabelSymbol::ytype() const
	{
		return SymbolType::LABEL;
	}
}
