#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <allocator/Value.h>
#include <ast/expression/Expression.h>
#include <symbol/defs/SymbolKind.h>
#include <type/Type.h>
#include <utility/Constants.h>
#include <utility/ExtendedInteger.h>

// Haze Symbol.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	struct SymbolSumStorage;

	using SymbolHandle = SumHandle<SymbolSumStorage>;

	template<typename T>
	using SymbolReference = SumReference<T, SymbolSumStorage>;

	using SymbolFacade = SumMemberBase<SymbolSumStorage>;

	class SymbolBase : public SymbolFacade
	{
	public:
		std::string name;
		SourceLocation location;
#pragma message("TODO: implement TOKEN confidence system. GOOD STATEMENT/FUNCTION++, error tokens--")
		int confidence = 0;
		bool was_referenced = false;

	public:
		SymbolBase(const std::string& name, const Token& token)
			: name{ name }, location{ token.location }
		{
		}

	public:
		template<typename Self>
		SymbolKind symbol_kind(this Self&& self)
		{
			switch (self.tag_type())
			{
#define X(enumerator, type, name) case TypeIndexV<type, typename Storage::Type>: return SymbolKind::enumerator;
				SYMBOL_KINDS(X)
#undef X
			}

			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid symbol tag `{}`", self.tag_type()), self.token);

			return SymbolKind::UNKNOWN;
		}
	};
}

namespace hz
{
	class FunctionSymbol : public SymbolBase
	{
	private:
		Address entrypoint = 0;

	public:
		std::uint32_t locals_count = 0;
		TypeHandle return_type;
		// all stored as ArgumentExpresion under the hood
		std::vector<ExpressionReference<ArgumentExpression>> arguments{};

	public:
		auto arity() const
		{
			return arguments.size();
		}

	public:
		FunctionSymbol(const std::string& name, const Token& token, TypeHandle return_type)
			: SymbolBase{ name, token }, return_type{ return_type }
		{
		}
	};

	class ArgumentSymbol : public SymbolBase
	{
	public:
		TypeHandle type;

	public:
		ArgumentSymbol(const std::string& name, const Token& token, TypeHandle type)
			: SymbolBase{ name, token }, type{ type }
		{
		}
	};

	class VariableSymbol : public SymbolBase
	{
	public:
		ValueHandle value;
		TypeHandle type;

	public:
		VariableSymbol(const std::string& name, const Token& token, TypeHandle type, ValueHandle value)
			: SymbolBase{ name, token }, type{ type }, value{ value }
		{
		}
	};

	class DefineSymbol : public SymbolBase
	{
	public:
		TypeHandle type;
		ExtendedInteger value;

	public:
		DefineSymbol(const std::string& name, const Token& token, TypeHandle type, ExtendedInteger value)
			: SymbolBase{ name, token }, type{ type }, value { value }
		{
		}
	};

	class LabelSymbol : public SymbolBase
	{
	public:
		Address address;

	public:
		LabelSymbol(const std::string& name, const Token& token, Address address)
			: SymbolBase{ name, token }, address{ address }
		{
		}
	};

	class StructOrUnionSymbol : public SymbolBase
	{
	public:
		std::unordered_map<std::string, StructOrUnionType::Member> members;

	public:
		StructOrUnionSymbol(const std::string& name, const Token& token)
			: SymbolBase{ name, token }
		{
		}
	};
}

#endif
