#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <string_view>

namespace hz
{
    class Allocation;

    enum class SymbolType
    {
        FUNCTION,
        ARGUMENT,
        VARIABLE,
    };

    struct FunctionSymbol;
    struct ArgumentSymbol;
    struct VariableSymbol;

    struct Symbol
    {
        SymbolType type;
        std::string_view name;

        union
        {
            FunctionSymbol* function;
            ArgumentSymbol* argument;
            VariableSymbol* variable;
        } as;
    };

    struct FunctionSymbol
    {
        Allocation* allocation;
    };

    struct ArgumentSymbol
    {
        Allocation* allocation = nullptr;
    };

    enum class VariableSymbolType
    {
        NORMAL,
        CONSTANT,
    };

    struct NormalVariableSymbol;
    struct ConstantVariableSymbol;

    struct VariableSymbol
    {
        VariableSymbolType type;

        union
        {
            NormalVariableSymbol* normal;
            ConstantVariableSymbol* constant;
        } as;

        Allocation* allocation;
    };

    struct NormalVariableSymbol
    {
        Allocation* allocation;
    };

    struct ConstantVariableSymbol
    {
        int value;
    };
}

#endif //HAZE_SYMBOL_H
