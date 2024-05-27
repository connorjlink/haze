#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <string>

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
		std::string name;

	public:
		virtual Symbol::Type ytype() const = 0;
	};

	class FunctionSymbol : public Symbol
	{
	private:
		std::uint16_t entrypoint;

	public:
		explicit FunctionSymbol(std::string name)
			: entrypoint(0)
		{
			this->name = std::move(name);
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};

	class ArgumentSymbol : public Symbol
	{
	private:
		Allocation* allocation;

	public:
		explicit ArgumentSymbol(std::string name)
			: allocation(nullptr)
		{
			this->name = std::move(name);
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};

	class VariableSymbol : public Symbol
	{
	public:
		Allocation* allocation;

	public:
		explicit VariableSymbol(std::string name, Allocation* allocation)
			: allocation(allocation)
		{
			this->name = std::move(name);
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};
}

#endif //HAZE_SYMBOL_H
