#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include "SymbolType.h"
#include "StructMember.h"
#include "Function.h"
#include "ExtendedInteger.h"

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
		Type* return_type;
		// all stored as ArgumentExpresion under the hood
		std::vector<Expression*> arguments;
		std::uint32_t locals_count;

	public:
		auto arity() const
		{
			return arguments.size();
		}

	public:
		FunctionSymbol(const std::string& name, Type* return_type)
			: Symbol{ name }, return_type{ return_type }
		{
			entrypoint = { 0 };
			arguments = {};
			locals_count = { 0 };
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class ArgumentSymbol : public Symbol
	{
	public:
		Allocation* allocation;
		Type* type;

	public:
		ArgumentSymbol(const std::string& name, Type* type)
			: Symbol{ name }, allocation{ nullptr }, type{ type }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class VariableSymbol : public Symbol
	{
	public:
		Allocation* allocation;
		Type* type;

	public:
		VariableSymbol(const std::string& name, Type* type, Allocation* allocation)
			: Symbol{ name }, type{ type }, allocation{ allocation }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class DefineSymbol : public Symbol
	{
	public:
		Type* type;
		ExtendedInteger value;

	public:
		DefineSymbol(const std::string& name, Type* type, ExtendedInteger value)
			: Symbol{ name }, type{ type }, value { value }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class LabelSymbol : public Symbol
	{
	public:
		platform_address_size address;

	public:
		LabelSymbol(const std::string& name, platform_address_size address)
			: Symbol{ name }, address{ address }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class StructSymbol : public Symbol
	{
	public:
		std::unordered_map<std::string, StructMember> members;

	public:
		StructSymbol(const std::string& name)
			: Symbol{ name }, members{}
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};
}

#endif
