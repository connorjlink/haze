#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <string_view>

#define AS_FUNCTION_SYMBOL(x) static_cast<FunctionSymbol*>(x)
#define AS_ARGUMENT_SYMBOL(x) static_cast<ArgumentSymbol*>(x)
#define AS_VARIABLE_SYMBOL(x) static_cast<VariableSymbol*>(x)

#define AS_RUNTIME_VARIABLE_SYMBOL(x) static_cast<RuntimeVariableSymbol*>(x)
#define AS_CONSTANT_VARIABLE_SYMBOL(x) static_cast<ConstantVariableSymbol*>(x)

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

	public:
		std::string_view name;

	public:
		virtual Symbol::Type ytype() const = 0;
	};

	class FunctionSymbol : public Symbol
	{
	private:
		Allocation* allocation;

	public:
		explicit FunctionSymbol(std::string_view name)
			: allocation(nullptr)
		{
			this->name = name;
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};

	class ArgumentSymbol : public Symbol
	{
	private:
		Allocation* allocation;

	public:
		explicit ArgumentSymbol(std::string_view name)
			: allocation(nullptr)
		{
			this->name = name;
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};

	class VariableSymbol : public Symbol
	{
	public:
		Allocation* allocation;

	public:
		explicit VariableSymbol(std::string_view name)
			: allocation(nullptr)
		{
			this->name = name;
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};
}

#endif //HAZE_SYMBOL_H
