#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include "SymbolType.h"
#include "Function.h"

// Haze Symbol.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Allocation;

	class Symbol
	{
	public:
		std::string name;
		bool was_referenced;
		TypeSpecifier type;

	public:
		Symbol(std::string name)
			: name{ std::move(name) }, was_referenced{ false }, type{ TypeSpecifier::BYTE }
		{
		}

	public:
		virtual SymbolType ytype() const = 0;
	};

	class FunctionSymbol : public Symbol
	{
	private:
		std::uint32_t entrypoint;

	public:
		TypeSpecifier return_type;
		// all stored as ArgumentExpresion under the hood
		std::vector<Expression*> arguments;
		std::uint32_t locals_count;

	public:
		auto arity() const
		{
			return arguments.size();
		}

	public:
		FunctionSymbol(std::string name, TypeSpecifier return_type = static_cast<TypeSpecifier>(0))
			: Symbol{ std::move(name) }, entrypoint{ 0 }, return_type{}, arguments{}, locals_count{ 0 }
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
		std::uint32_t value;

	public:
		DefineSymbol(std::string name, std::uint32_t value)
			: Symbol{ std::move(name) }, value{ value }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class LabelSymbol : public Symbol
	{
	public:
		std::uint32_t address;

	public:
		LabelSymbol(std::string name, std::uint32_t address)
			: Symbol{ std::move(name) }, address{ address }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};
}

#endif //HAZE_SYMBOL_H
