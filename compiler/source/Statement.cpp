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
		return std::format(";");
	}

	void NullStatement::generate(const Storage&) const
	{
		// no code generation necessary for null statement
	}

	StatementHandle NullStatement::evaluate(const Storage& storage, Context&) const
	{
		// no evaluation necessary for null statement
		return MAKE_INVALID_HANDLE(storage, Statement);
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

	StatementHandle ExpressionStatement::evaluate(const Storage& storage, Context& context) const
	{
		expression.evaluate(context);
		return MAKE_INVALID_HANDLE(storage, Statement);
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
		auto compiler_parser = REQUIRE_SAFE(CompilerParser).get();
		const auto& end_function_label = compiler_parser->function_label_map.at(enclosing_function);

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

#pragma message("TODO: optimize invalid handles to use a common sentinel instead of creating a duplicate invalid entry every time")
	StatementHandle ReturnStatement::evaluate(const Storage& storage, Context& context) const
	{
		const auto expression_evaluated = expression.evaluate(storage, context);
		context.push_return()


		return MAKE_INVALID_HANDLE(storage, Statement);
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
