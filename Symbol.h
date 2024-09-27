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

	public:
		bool was_referenced;

	public:
		Symbol(const std::string& name)
			: name{ name }, was_referenced{ false }
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
		FunctionSymbol(const std::string& name, TypeSpecifier return_type = static_cast<TypeSpecifier>(0))
			: Symbol{ name }, entrypoint{ 0 }, return_type{}, arguments{}, locals_count{ 0 }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class ArgumentSymbol : public Symbol
	{
	public:
		Allocation* allocation;
		TypeSpecifier type_specifier;

	public:
		ArgumentSymbol(const std::string& name, TypeSpecifier type_specifier)
			: Symbol{ name }, allocation{ nullptr }, type_specifier{ type_specifier }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class VariableSymbol : public Symbol
	{
	public:
		Allocation* allocation;
		TypeSpecifier type_specifier;

	public:
		VariableSymbol(const std::string& name, Allocation* allocation, TypeSpecifier type_specifier)
			: Symbol{ name }, allocation{ allocation }, type_specifier{ type_specifier }
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
		DefineSymbol(const std::string& name, std::uint32_t value)
			: Symbol{ name }, value{ value }
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
		LabelSymbol(const std::string& name, std::uint32_t address)
			: Symbol{ name }, address{ address }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};
}

#endif //HAZE_SYMBOL_H
