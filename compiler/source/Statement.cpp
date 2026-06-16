import std;

#include <ast/statement/Statement.h>
#include <type/Type.h>
#include <utility/ExtendedInteger.h>
#include <utility/Random.h>

// Haze Statement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	//////////////////////////////////////////////////////
	// Null Statement
	//////////////////////////////////////////////////////

	StatementKind NullStatement::statement_kind() const
	{
		return StatementKind::NULL;
	}

	std::string NullStatement::format(std::uint32_t indentation_level) const
	{
		return std::format("{};", indentation_table[indentation_level]);
	}

	void NullStatement::generate(const CommandStorage&) const
	{
		// no code generation necessary for null statement
	}

	ASTTask NullStatement::evaluate(const VariableStorage& storage, Context&) const
	{
		// no evaluation necessary for null statement
		co_return MAKE_INVALID_HANDLE(storage, Variable);
	}

	StatementHandle NullStatement::optimize(const StatementStorage& storage) const
	{
		// no optimizations possible for null statement
		return MAKE_INVALID_HANDLE(storage, Statement);
	}


	//////////////////////////////////////////////////////
	// Expression Statement
	//////////////////////////////////////////////////////

	StatementKind ExpressionStatement::statement_kind() const
	{
		return StatementKind::EXPRESSION;
	}

	std::string ExpressionStatement::format(std::uint32_t indentation_level) const
	{
		return std::format("{}{};", indentation_table[indentation_level], expression);
	}

	void ExpressionStatement::generate(const CommandStorage& storage) const
	{
		// no code generation necessary for expression statement
	}

	ASTTask ExpressionStatement::evaluate(const VariableStorage& storage, Context& context) const
	{
		if (expression)
		{
			co_await ASTAwaiter{ expression.evaluate(storage, context) };
		}

		co_return MAKE_INVALID_HANDLE(storage, Variable);
	}

	StatementHandle ExpressionStatement::optimize(const StatementStorage& storage) const
	{
		const auto expression_optimized = expression.optimize(storage);
		if (!expression_optimized)
		{
			return MAKE_INVALID_HANDLE(storage, Statement);
		}

		return expression_optimized;
	}


	//////////////////////////////////////////////////////
	// Return Statement
	//////////////////////////////////////////////////////

	StatementKind ReturnStatement::statement_kind() const
	{
		return StatementKind::RETURN;
	}

	std::string ReturnStatement::format(std::uint32_t indentation_level) const
	{
		return std::format("{}return {};", 
			indentation_table[indentation_level], expression);
	}

	void ReturnStatement::generate(const CommandStorage& storage) const
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

	ASTTask ReturnStatement::evaluate(const VariableStorage& storage, Context& context) const
	{
		const auto return_value = expression
			? co_await ASTAwaiter{ expression.evaluate(storage, context) }
			: MAKE_INVALID_HANDLE(storage, Variable);

		auto* current_promise = co_await ASTPromiseAwaiter{};
		current_promise->kind = ControlFlowKind::RETURN;

		co_return return_value;
	}

	StatementHandle ReturnStatement::optimize(const StatementStorage& storage) const
	{
		const auto expression_optimized = expression.optimize(storage);
		if (!expression_optimized)
		{
			return MAKE_INVALID_HANDLE(storage, Statement);
		}

		return expression_optimized;
	}


	//////////////////////////////////////////////////////
	// For Statement
	//////////////////////////////////////////////////////

	StatementKind ForStatement::statement_kind() const
	{
		return StatementKind::FOR;
	}

	std::string ForStatement::format(std::uint32_t indentation_level) const
	{
		return std::format("for ({}; {}; {})\n{}", 
			initialization, condition, increment, body.format(indentation_level));
	}

	void ForStatement::generate(const CommandStorage& storage) const
	{
		// 3 digits of randomness for now
		const auto uuid = hz::generate_digit_string(3);

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

	ASTTask ForStatement::evaluate(const VariableStorage& storage, Context& context) const
	{
		const auto _ = context.auto_scope();

		auto* current_promise = co_await ASTPromiseAwaiter{};

		if (initialization)
		{
			co_await ASTAwaiter{ initialization.evaluate(storage, context) };
		}

		while (true)
		{
			if (condition)
			{
				auto condition_value = co_await ASTAwaiter{ condition.evaluate(storage, context) };
				if (!condition_value.is_truthy())
				{
					break;
				}
			}

			if (body)
			{
				co_await ASTAwaiter{ body.evaluate(storage, context) };

				switch (current_promise->kind)
				{
					case ControlFlowKind::BREAK:
					{
						// intercept break operation to quit the loop immediately
						current_promise->kind = ControlFlowKind::NONE;
					} break;

					case ControlFlowKind::CONTINUE:
					{
						// intercept continue operation to proceed direcly to the next increment phase
						current_promise->kind = ControlFlowKind::NONE;
					} continue;

					case ControlFlowKind::RETURN:
					{
						// propagate return operation to unwind
						co_return MAKE_INVALID_HANDLE(storage, Variable);
					} break;
				}
			}

			if (increment)
			{
				co_await ASTAwaiter{ increment.evaluate(storage, context) };
			}
		}

		co_return MAKE_INVALID_HANDLE(storage, Variable);
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


	//////////////////////////////////////////////////////
	// Compound Statement
	//////////////////////////////////////////////////////

	std::string CompoundStatement::format(std::uint32_t indentation_level) const
	{
		return std::format("{{{}}}",
			substatements 
				| TypeHandle get_type(const TypeStorage&) const;::transform([](const auto& substatement) { return substatement.format(indentation_level + TAB); })
				| TypeHandle get_type(const TypeStorage&) const;::join_with("\n\n")
				| TypeHandle get_type(const TypeStorage&) const;::transform([&](const auto& line) { return std::format("{}{}", indentation_table[indentation_level], line); })
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

	ASTTask CompoundStatement::evaluate(const VariableStorage& storage, Context& context) const
	{
		const auto _ = context.auto_scope();

		auto* current_promise = co_await ASTPromiseAwaiter{};

		for (const auto& substatement : substatements)
		{
			if (!substatement)
			{
				continue;
			}

			co_await ASTAwaiter{ substatement.evaluate(storage, context) };

			if (current_promise->kind != ControlFlowKind::NONE)
			{
				co_return MAKE_INVALID_HANDLE(storage, Variable);
			}
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


	//////////////////////////////////////////////////////
	// Inline Assembly Statement
	//////////////////////////////////////////////////////

	std::string InlineAssemblyStatement::format(std::uint32_t indentation_level) const
	{
		return std::format("asm {{\n{}\n}}", 
			commands 
				| TypeHandle get_type(const TypeStorage&) const;::transform([&](const auto& command) { return command.format(indentation_level + TAB); })
				| TypeHandle get_type(const TypeStorage&) const;::join_with("\n")
				| std::ranges::to<std::string>());
	}

	void InlineAssemblyStatement::generate(const StatementStorage& storage) const
	{
		
	}

	ASTTask InlineAssemblyStatement::evaluate(const VariableStorage& storage, Context&) const
	{
		USE_SAFE(ErrorReporter)->post_warning(std::format(
			"inline assembly is not supported in evaluation"), token);

		co_return MAKE_INVALID_HANDLE(storage, Variable);
	}

	StatementHandle InlineAssemblyStatement::optimize(const StatementStorage& storage) const
	{
		// no optimizations possible for null statement
		return MAKE_INVALID_HANDLE(storage, Statement);
	}

}
