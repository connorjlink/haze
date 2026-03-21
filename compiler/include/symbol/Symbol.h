#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <symbol/SymbolType.h>
#include <ast/models/StructMember.h>
#include <utility/ExtendedInteger.h>
#include <utility/PlatformVariables.h>

// Haze Symbol.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Allocation;
	class Expression;

	class Symbol
	{
	public:
		std::string name;
		int confidence = 0;
#pragma message("TODO: implement TOKEN confidence system. GOOD STATEMENT/FUNCTION++, error tokens--")

	public:
		SourceLocation location;
		bool was_referenced;

	public:
		Symbol(const std::string& name, const Token& token)
			: name{ name }, was_referenced{ false }
		{
			location = token.location;
		}
		virtual ~Symbol() = default;

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
		FunctionSymbol(const std::string& name, const Token& token, Type* return_type)
			: Symbol{ name, token }, return_type{ return_type }
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
		ArgumentSymbol(const std::string& name, const Token& token, Type* type)
			: Symbol{ name, token }, allocation{ nullptr }, type{ type }
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
		VariableSymbol(const std::string& name, const Token& token, Type* type, Allocation* allocation)
			: Symbol{ name, token }, type{ type }, allocation{ allocation }
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
		DefineSymbol(const std::string& name, const Token& token, Type* type, ExtendedInteger value)
			: Symbol{ name, token }, type{ type }, value { value }
		{
		}

	public:
		virtual SymbolType ytype() const final override;
	};

	class LabelSymbol : public Symbol
	{
	public:
		native_uint address;

	public:
		LabelSymbol(const std::string& name, const Token& token, native_uint address)
			: Symbol{ name, token }, address{ address }
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
		StructSymbol(const std::string& name, const Token& token)
			: Symbol{ name, token }
		{
			members = {};
		}

	public:
		virtual SymbolType ytype() const final override;
	};
}

#endif
