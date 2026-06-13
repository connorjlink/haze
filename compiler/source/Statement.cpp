import std;

#include <ast/statement/Statement.h>
#include <type/Type.h>
#include <utility/ExtendedInteger.h>
#include <utility/Random.h>

// Haze Statement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#pragma message("TODO: get rid of get_type() for statements only")

	//////////////////////////////////////////////////////
	// Null Statement
	//////////////////////////////////////////////////////

	std::string NullStatement::format(std::uint32_t) const
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

	std::string ExpressionStatement::format(std::uint32_t) const
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
		const auto expression_optimized = expression.optimize(storage);
		if (!expression_optimized)
		{
			return MAKE_INVALID_HANDLE(storage, Statement);
		}

		return expression_optimized;
	}

	TypeHandle ExpressionStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for expression statement
		return MAKE_INVALID_HANDLE(storage, Type);
	}


	//////////////////////////////////////////////////////
	// Return Statement
	//////////////////////////////////////////////////////

	std::string ReturnStatement::format(std::uint32_t) const
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
		const auto value = co_await ASTAwaiter{ expression.evaluate(storage, context) };

		co_return value;
	}

	StatementHandle ReturnStatement::optimize(const Storage& storage) const
	{
		const auto expression_optimized = expression.optimize(storage);
		if (!expression_optimized)
		{
			return MAKE_INVALID_HANDLE(storage, Statement);
		}

		return expression_optimized;
	}

	TypeHandle ReturnStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for return statement
		return MAKE_INVALID_HANDLE(storage, Type);
	}




	//////////////////////////////////////////////////////
	// For Statement
	//////////////////////////////////////////////////////

	std::string ForStatement::format(std::uint32_t) const
	{
		return std::format("for ({}; {}; {}) {}", initialization, condition, increment, body);
	}

	void ForStatement::generate(const Storage& storage) const
	{
		// 3 digits of randomness for now
		const auto uuid = hz::generate(3);

		const auto begin_for_label = std::format("begin_for_{:03d}", uuid);
		const auto end_for_label = std::format("end_for_{:03d}", uuid);

		initialization.generate(storage);

		REQUIRE_SAFE(Generator)->branch_label(begin_for_label);

		// scoping so that the for body can re-use the condition register
		{
			AutoStackAllocation condition_allocation{};
			condition.generate(storage);

			REQUIRE_SAFE(Generator)->check_ifnz(end_for_label, condition_allocation.source()->read());
		}

		body.generate(storage);
		increment.generate(storage);

		REQUIRE_SAFE(Generator)->goto_command(begin_for_label);

		REQUIRE_SAFE(Generator)->branch_label(end_for_label);
	}

	ASTTask ForStatement::evaluate(const Storage& storage, Context& context) const
	{
		co_await ASTAwaiter{ initialization.evaluate(storage, context) };

		auto condition_evaluated = co_await ASTAwaiter{ condition.evaluate(storage, context) };

		if (condition.expression_kind() != ExpressionType::INTEGER_LITERAL)
		{
			USE_SAFE(ErrorReporter)->post_error(
				"`for` loop conditions must result in an r-value", condition_evaluated.token);
			return nullptr;
		}

		auto integer_literal = AS_INTEGER_LITERAL_EXPRESSION(condition_evaluated)->value;

		while (integer_literal_raw(integer_literal) != EI(std::intmax_t{ 0 }))
		{
			body->evaluate(context);
			expression->evaluate(context);

			condition_evaluated = AS_EXPRESSION(condition->evaluate(context));
			integer_literal = AS_INTEGER_LITERAL_EXPRESSION(condition_evaluated)->value;
		}

		return nullptr;

	}

	StatementHandle ForStatement::optimize(const Storage& storage) const
	{
		auto initialization_optimized = AS_STATEMENT(initialization->optimize());
		auto condition_optimized = AS_EXPRESSION(condition->optimize());
		auto expression_optimized = AS_EXPRESSION(expression->optimize());
		auto body_optimized = AS_STATEMENT(body->optimize());

		if (initialization_optimized == nullptr &&
			condition_optimized == nullptr &&
			expression_optimized == nullptr &&
			body_optimized == nullptr)
		{
			return nullptr;
		}

		if (initialization_optimized == nullptr)
		{
			initialization_optimized = initialization;
		}

		if (condition_optimized == nullptr)
		{
			condition_optimized = condition;
		}

		if (expression_optimized == nullptr)
		{
			expression_optimized = expression;
		}

		if (body_optimized == nullptr)
		{
			body_optimized = body;
		}

		auto condition_optimized_evaluated = AS_EXPRESSION(condition_optimized->evaluate(USE_SAFE(Context).get()));

		if (condition_optimized_evaluated != nullptr)
		{
			auto value = AS_INTEGER_LITERAL_EXPRESSION(condition_optimized_evaluated)->value;

			// The value is constexpr false so we don't even need to run the loop
			if (value == 0)
			{
				return new NullStatement{ _token };
			}
		}

		return new ForStatement{ initialization_optimized, condition_optimized, expression_optimized, body_optimized, _token };
	}

	TypeHandle ForStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for for statement
		return MAKE_INVALID_HANDLE(storage, Type);
	}


	//////////////////////////////////////////////////////
	// Compound Statement
	//////////////////////////////////////////////////////

	std::string CompoundStatement::format(std::uint32_t indentation_level) const
	{
		return std::format("{{{}}}",
			substatements 
				| std::ranges::views::transform([](const auto& substatement) { return substatement.format(indentation_level + TAB); })
				| std::ranges::views::join_with("\n\n")
				| std::ranges::views::transform([&](const auto& line) { return std::format("{}{}", indentation_table[indentation_level], line); })
				| std::ranges::to<std::string>());
	}

	void CompoundStatement::generate(const Storage& storage) const
	{
		for (const auto& substatement : substatements)
		{
			if (!substatement)
			{
				continue;
			}

			substatement.generate(storage);
		}
	}

	ASTTask CompoundStatement::evaluate(const Storage& storage, Context& context) const
	{
		for (const auto& substatement : substatements)
		{
			if (substatement)
			{
				continue;
			}

			co_await ASTAwaiter{ substatement.evaluate(storage, context) };
		}

		co_return MAKE_INVALID_HANDLE(storage, Variable);
	}

	StatementHandle CompoundStatement::optimize(const Storage& storage) const
	{
		auto result = std::vector<StatementHandle>{};

		bool did_optimize = false;

		for (const auto& substatement : substatements)
		{
			if (!substatement)
			{
				continue;
			}

			const auto substatement_optimized = substatement.optimize(storage);
			if (!substatement_optimized)
			{
				result.emplace_back(substatement);
				continue;
			}

			result.emplace_back(substatement_optimized);
			did_optimize = true;
		}

		if (!did_optimize)
		{
			return MAKE_INVALID_HANDLE(storage, Statement);
		}

		return MAKE_HANDLE(CompoundStatement, Statement, storage, MAKE_COMPOUND_STATEMENT(result, token));
	}

	TypeHandle CompoundStatement::get_type(const TypeSumStorage& storage) const
	{
		// no type result possible for compound statement
		return MAKE_INVALID_HANDLE(storage, Type);
	}



	//////////////////////////////////////////////////////
	// Inline Assembly Statement
	//////////////////////////////////////////////////////

	std::string InlineAssemblyStatement::format(std::uint32_t) const
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

}
