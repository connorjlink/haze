#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <string>

#define AS_FUNCTION_SYMBOL(x) static_cast<FunctionSymbol*>(x)
#define AS_ARGUMENT_SYMBOL(x) static_cast<ArgumentSymbol*>(x)
#define AS_VARIABLE_SYMBOL(x) static_cast<VariableSymbol*>(x)
#define AS_DEFINE_SYMBOL(x) static_cast<DefineSymbol*>(x)
#define AS_LABEL_SYMBOL(x) static_cast<LabelSymbol*>(x)

namespace hz
{
	class Allocation;
	enum class ReturnType;

	class Symbol
	{
	public:
		enum class Type
		{
			FUNCTION,
			ARGUMENT,
			VARIABLE,
			DEFINE,
			LABEL,
		};

	public:
		std::string name;
		bool was_referenced;

	public:
		explicit Symbol(std::string name)
			: name{ std::move(name) }, was_referenced{ false }
		{
		}

	public:
		virtual Symbol::Type ytype() const = 0;
	};

	class FunctionSymbol : public Symbol
	{
	private:
		std::uint16_t entrypoint;

	public:
		ReturnType return_type;
		std::uint8_t arity;

	public:
		explicit FunctionSymbol(std::string name, ReturnType return_type = static_cast<ReturnType>(0))
			: Symbol{ std::move(name) }, entrypoint{ 0 }, return_type{}, arity{ 0 }
		{
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
			: Symbol{ std::move(name) }, allocation{ nullptr }
		{
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
			: Symbol{ std::move(name) }, allocation{ allocation }
		{
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};

	class DefineSymbol : public Symbol
	{
	public:
		std::uint16_t value;

	public:
		explicit DefineSymbol(std::string name, std::uint16_t value)
			: Symbol{ std::move(name) }, value{ value }
		{
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};

	class LabelSymbol : public Symbol
	{
	public:
		std::uint16_t address;

	public:
		explicit LabelSymbol(std::string name, std::uint16_t address)
			: Symbol{ std::move(name) }, address{ address }
		{
		}

	public:
		virtual Symbol::Type ytype() const final override;
	};
}

#endif //HAZE_SYMBOL_H
