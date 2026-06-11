import std;

#include <ast/statement/Statement.h>
#include <type/Type.h>

// Haze Statement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#pragma message("TODO: get rid of get_type() for statements only")

	//////////////////////////////////////////////////////
	// Null Statement
	//////////////////////////////////////////////////////

	std::string NullStatement::format(void) const
	{
		return ";";
	}

	void NullStatement::generate(const Storage&) const
	{
		// no code generation necessary for null statement
	}

	ASTTask NullStatement::evaluate(const Storage& storage, Context&) const
	{
		// no evaluation necessary for null statement
		co_return MAKE_INVALID_HANDLE(storage, Statement);
	}

	StatementHandle NullStatement::optimize(const Storage& storage) const
	{
		// no optimizations possible for null statement
		return MAKE_INVALID_HANDLE(storage, Statement);
	}

	TypeHandle NullStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for null statement
		return MAKE_INVALID_HANDLE(storage, Type);
	}


	//////////////////////////////////////////////////////
	// Expression Statement
	//////////////////////////////////////////////////////

	std::string ExpressionStatement::format(void) const
	{
		return std::format("{};", expression);
	}

	void ExpressionStatement::generate(const Storage&) const
	{
		// no code generation necessary for expression statement
	}

	ASTTask ExpressionStatement::evaluate(const Storage& storage, Context& context) const
	{
		co_await ASTAwaiter{ expression.evaluate(context) };
		co_return MAKE_INVALID_HANDLE(storage, Statement);
	}

	StatementHandle ExpressionStatement::optimize(const Storage& storage) const
	{
		if (auto expression_optimized = expression.optimize(storage))
		{
			return expression_optimized;
		}

		return MAKE_INVALID_HANDLE(storage, Statement);
	}

	TypeHandle ExpressionStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for expression statement
		return MAKE_INVALID_HANDLE(storage, Type);
	}


	//////////////////////////////////////////////////////
	// Return Statement
	//////////////////////////////////////////////////////

	std::string ReturnStatement::format(void) const
	{
		return std::format("return {};", expression);
	}

	void ReturnStatement::generate(const Storage& storage) const
	{
		const auto& end_function_label = REQUIRE_SAFE(CompilerParser)->function_label_map.at(enclosing_function);

		if (!expression)
		{
			// void function return type
			REQUIRE_SAFE(Generator)->make_return(end_function_label);
			return;
		}

		expression.generate(storage);

		const auto architecture = USE_SAFE(CommandLineOptions)->architecture;
		REQUIRE_SAFE(Generator)->make_return(end_function_label, get_abi_return_register(architecture));
	}

	ASTTask ReturnStatement::evaluate(const Storage& storage, Context& context) const
	{
		const auto expression_evaluated = expression.evaluate(storage, context);
		const auto value = co_await ASTAwaiter{ expression_evaluated };

		co_return value;
	}

	StatementHandle ReturnStatement::optimize(const Storage& storage) const
	{
		if (auto expression_optimized = expression.optimize(storage))
		{
			return expression_optimized;
		}

		return MAKE_INVALID_HANDLE(storage, Statement);
	}

	TypeHandle ReturnStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for return statement statement
		return MAKE_INVALID_HANDLE(storage, Type);
	}

}
