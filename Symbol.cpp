#include "Symbol.h"

namespace hz
{
	Symbol::Type FunctionSymbol::ytype() const
	{
		return Symbol::Type::FUNCTION;
	}

	Symbol::Type ArgumentSymbol::ytype() const
	{
		return Symbol::Type::ARGUMENT;
	}

	Symbol::Type VariableSymbol::ytype() const
	{
		return Symbol::Type::VARIABLE;
	}
}
