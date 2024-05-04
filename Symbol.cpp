#include "Symbol.h"

#include "Function.h"

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


    VariableSymbol::Type RuntimeVariableSymbol::vtype() const
    {
        return VariableSymbol::Type::RUNTIME;
    }

    VariableSymbol::Type ConstantVariableSymbol::vtype() const
    {
        return VariableSymbol::Type::CONSTANT;
    }
}
