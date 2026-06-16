#ifndef HAZE_FUNCTION_H
#define HAZE_FUNCTION_H

#include <allocator/Allocator.h>
#include <ast/AST.h>
#include <ast/expression/Expression.h>
#include <ast/function/defs/FunctionKind.h>
#include <ast/statement/Statement.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <toolchain/Parser.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Function.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Function)

#define FUNCTION_AST_METHODS(X, handlet) \
	BASE_AST_METHODS(X, handlet) \
	X(function_kind, FunctionKind) \
	X(get_type, TypeHandle)

	DEFINE_SUM(Function, FUNCTION_AST_METHODS)


	struct FunctionBase 
		: public FunctionFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = FunctionStorage;

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
	//////////////////////////////////////////////////////
	// Standard Functions
	//////////////////////////////////////////////////////

	struct SymbolDatabase;

	struct Function 
		: public FunctionBase
		, public InjectService<Generator, Parser, Allocator>
		, public InjectSingleton<SymbolDatabase>
	{
	public:
		std::string_view name;
		TypeHandle return_type;
		std::vector<ExpressionHandle> arguments;
		StatementHandle body;

	public:
		FunctionKind function_kind() const;
		std::string format(std::uint32_t) const;
		std::string format_signature() const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		Function(std::string_view name, TypeHandle return_type, std::vector<ExpressionHandle> arguments, StatementHandle body, const Token& token)
			: FunctionBase{ token }, name{ name }, return_type{ return_type }, arguments{ std::move(arguments) }, body{ body }
		{
		}
	};
#define MAKE_FUNCTION(name, returntype, arguments, statement, token) Function{ name, returntype, arguments, make_handle(ast, statement), token }


	//////////////////////////////////////////////////////
	// All Functions
	//////////////////////////////////////////////////////

	template<typename SumMemberT, typename StorageT>
	concept IsFunction = SumTuple<SumMemberT, StorageT, FunctionMethods<typename StorageT::Anchor>>;

	using FunctionKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		FUNCTION_KINDS(X)
#undef X
		void
	>;
	

	using FunctionSumImplementation = MakeSum<FunctionMethods, FunctionKinds>::Type;

	struct FunctionStorage : public FunctionSumImplementation::Storage
	{
		using FunctionSumImplementation::Storage::Storage;

		using Type = FunctionSumImplementation::Type;
		using Anchor = FunctionSumImplementation::Anchor;
	};


	constexpr Function& function_handle_to_function(FunctionHandle handle)
	{
		return handle.get<Function>();
	}
}

#endif
