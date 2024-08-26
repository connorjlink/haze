#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include "SymbolType.h"
#include "Function.h"

#include <string>

namespace hz
{
	class Allocation;

	class Symbol
	{
	public:
		std::string name;
		bool was_referenced;
		ReturnType type;

	public:
		Symbol(std::string name)
			: name{ std::move(name) }, was_referenced{ false }, type{ ReturnType::BYTE }
		{
		}

	public:
		virtual SymbolType ytype() const = 0;
	};

	class FunctionSymbol : public Symbol
	{
	private:
		std::uint16_t entrypoint;

	public:
		ReturnType return_type;
		std::uint8_t arity;

	public:
		FunctionSymbol(std::string name, ReturnType return_type = static_cast<ReturnType>(0))
			: Symbol{ std::move(name) }, entrypoint{ 0 }, return_type{}, arity{ 0 }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class ArgumentSymbol : public Symbol
	{
	public:
		Allocation* allocation;

	public:
		ArgumentSymbol(std::string name)
			: Symbol{ std::move(name) }, allocation{ nullptr }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class VariableSymbol : public Symbol
	{
	public:
		Allocation* allocation;

	public:
		VariableSymbol(std::string name, Allocation* allocation)
			: Symbol{ std::move(name) }, allocation{ allocation }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class DefineSymbol : public Symbol
	{
	public:
		std::uint16_t value;

	public:
		DefineSymbol(std::string name, std::uint16_t value)
			: Symbol{ std::move(name) }, value{ value }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class LabelSymbol : public Symbol
	{
	public:
		std::uint16_t address;

	public:
		LabelSymbol(std::string name, std::uint16_t address)
			: Symbol{ std::move(name) }, address{ address }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};
}

#endif //HAZE_SYMBOL_H
