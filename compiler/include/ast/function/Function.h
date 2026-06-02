#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <ast/AST.h>
#include <ast/expression/Expression.h>
#include <ast/function/defs/FunctionKind.h>
#include <ast/statement/Statement.h>
#include <allocator/Allocator.h>
#include <data/DependencyInjector.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <toolchain/Parser.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Function.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Function)

	template<typename MethodsT>
	using FunctionASTMethods = ASTMethods<MethodsT, FunctionHandle>;

	DEFINE_SUM(Function, AST_METHODS)


	class FunctionBase : public FunctionFacade
	{
	public:
		using Storage = FunctionSumStorage;

	public:
		Token token;

	public:
		FunctionBase(const Token& token)
			: token{ token }
		{
		}

	public:
		template<typename Self>
		FunctionKind function_kind(this Self&&);
	};
}

namespace hz
{
	class Function 
		: public FunctionBase
		, public InjectService<Generator, Parser, Allocator>
		, public InjectSingleton<SymbolDatabase>
	{
	public:
		const std::string& name;
		TypeHandle return_type;
		std::vector<ExpressionHandle> arguments;
		StatementHandle body;

	public:
		Function(const std::string& name, TypeHandle return_type, const std::vector<ExpressionHandle>& arguments, StatementHandle body, const Token& token)
			: FunctionBase{ token }, name{ name }, return_type{ return_type }, arguments{ arguments }, body{ body }
		{
		}

	public:
		FunctionKind function_kind(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};


	//////////////////////////////////////////////////////
	// All Functions
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept FunctionConcept = SumTuple<SumMemberT, SumStorageT, FunctionASTMethods<SumStorageT>>;

	using FunctionKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		FUNCTION_KINDS(X)
#undef X
		void
	>;

	using FunctionSumImplementation = MakeSum<FunctionASTMethods, FunctionKinds>::Type;

	struct FunctionSumStorage : public FunctionSumImplementation::Storage
	{
		using FunctionSumImplementation::Storage::Storage;

		using Type = FunctionSumImplementation::Type;
		using Anchor = FunctionSumImplementation::Anchor;
	};


	template<typename Self>
	FunctionKind FunctionBase::function_kind(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, type, name) case TypeIndexV<type, typename Storage::Type>: return ExpressionKind::enumerator;
			FUNCTION_KINDS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid function tag `{}`", self.tag_type()), self.token);

		// error recovery does not care about function kind
		return FunctionKind::FUNCTION;
	}
}

#endif
