import std;

#include <allocator/Allocation.h>
#include <ast/IntegerLiteralExpression.h>
#include <ast/WhileStatement.h>
#include <command/models/IntegerLiteral.h>
#include <error/CommonErrors.h>
#include <toolchain/Generator.h>
#include <utility/Random.h>

// Haze WhileStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType WhileStatement::stype() const
	{
		return StatementType::WHILE;
	}

	WhileStatement* WhileStatement::copy() const
	{
		return new WhileStatement{ *this };
	}

	void WhileStatement::generate(Allocation*)
	{
		// 3 digits of randomness for now
		const auto uuid = hz::generate(3);

		const auto begin_while_label = std::format("begin_while_{:03d}", uuid);
		const auto end_while_label = std::format("end_while_{:03d}", uuid);

		REQUIRE_SAFE(Generator)->branch_label(begin_while_label);

		// scoping so that the while body can re-use the condition register
		{
			AutoStackAllocation condition_allocation{};
			condition->generate(condition_allocation.source());

			REQUIRE_SAFE(Generator)->check_ifz(end_while_label, condition_allocation.source()->read());
		}

		body->generate();

		REQUIRE_SAFE(Generator)->goto_command(begin_while_label);

		REQUIRE_SAFE(Generator)->branch_label(end_while_label);
	}

	Statement* WhileStatement::optimize()
	{
		auto condition_optimized = AS_EXPRESSION(condition->optimize());
		auto body_optimized = AS_STATEMENT(body->optimize());

		if (condition_optimized == nullptr &&
			body_optimized == nullptr)
		{
			return nullptr;
		}

		if (condition_optimized == nullptr)
		{
			condition_optimized = condition;
		}

		if (body_optimized == nullptr)
		{
			body_optimized = body;
		}

		return new WhileStatement{ condition_optimized, body_optimized, _token };
	}

	Node* WhileStatement::evaluate(Context* context) const
	{
		auto condition_evaluated = condition->evaluate(context);
		
		if (AS_EXPRESSION(condition_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			CommonErrors::must_be_rvalue("`while` loop conditions", condition->_token);
			return nullptr;
		}

		const auto integer_literal = AS_INTEGER_LITERAL_EXPRESSION(condition_evaluated);
		while (TEST_VALUE_EQUALS(integer_literal, 0))
		{
			body->evaluate(context);
			condition_evaluated = condition->evaluate(context);
		}

		return nullptr;
	}
}