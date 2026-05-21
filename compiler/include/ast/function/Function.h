#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <ast/expression/Expression.h>
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

	struct FunctionSumStorage;

	using FunctionHandleFacade = SumHandle<FunctionSumStorage>;

	template<typename T>
	using FunctionReferenceFacade = SumReference<T, FunctionSumStorage>;

	using FunctionFacade = SumMemberBase<FunctionSumStorage>;

	struct FunctionBase : public FunctionFacade
	{
	public:
		Token token;

	public:
		FunctionBase(const Token& token)
			: token{ token }
		{
		}
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
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};


	using FunctionTypes = SumTypeList
	<
		Function
	>;

	using FunctionSum = MakeSum<ASTMethods, FunctionTypes>::Type;

	template<typename T>
	using FunctionReference = FunctionSum::template Reference<T>;

	using FunctionHandle = FunctionReference<Function>;



}

#endif
