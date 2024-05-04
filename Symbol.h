#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <string_view>

#define AS_FUNCTION(x) static_cast<FunctionSymbol*>(x)
#define AS_ARGUMENT(x) static_cast<ArgumentSymbol*>(x)
#define AS_VARIABLE(x) static_cast<VariableSymbol*>(x)

#define AS_RUNTIME_VARIABLE(x) static_cast<RuntimeVariableSymbol*>(x)
#define AS_CONSTANT_VARIABLE(x) static_cast<ConstantVariableSymbol*>(x)

namespace hz
{
    class Allocation;

    class Symbol
    {
    public:
        enum class Type
        {
            FUNCTION,
            ARGUMENT,
            VARIABLE,
        };

    private:
        std::string_view name;

    public:
        virtual Symbol::Type ytype() const = 0;
    };

    class FunctionSymbol : public Symbol
    {
    private:
        Allocation* allocation;

    public:
        virtual Symbol::Type ytype() const final override;
    };

    class ArgumentSymbol : public Symbol
    {
    private:
        Allocation* allocation;

    public:
        virtual Symbol::Type ytype() const final override;
    };

    class VariableSymbol : public Symbol
    {
    public:
        enum class Type
        {
            RUNTIME,
            CONSTANT,
        };

    public:
        virtual Symbol::Type ytype() const final override;
        virtual VariableSymbol::Type vtype() const = 0;
    };

    class RuntimeVariableSymbol : public VariableSymbol
    {
    private:
        Allocation* allocation;

    public:
        virtual VariableSymbol::Type vtype() const final override;
    };

    class ConstantVariableSymbol : public VariableSymbol
    {
    public:
        std::uint16_t value;

    public:
        virtual VariableSymbol::Type vtype() const final override;
    };
}

#endif //HAZE_SYMBOL_H
