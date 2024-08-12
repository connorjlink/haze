#include "ForStatement.h"
#include "IntegerLiteralExpression.h"
#include "NullStatement.h"
#include "Allocation.h"
#include "Allocator.h"
#include ""

#include <format>

namespace hz
{
	StatementType ForStatement::stype() const
	{
		return StatementType::FOR;
	}

	std::string ForStatement::string() const
	{
		return std::format("for statement ({}; {}; {}) \n[\n{}\n]\n", 
			initialization->string(), condition->string(), expression->string(), body->string());
	}

	ForStatement* ForStatement::copy() const
	{
		return new ForStatement{ *this };
	}

	void ForStatement::generate(Allocation*)
	{
		// we need to force allocate this one :(
		auto condition_allocation = _allocator->allocate_static(DC, true);

		// TODO: finish for statement generation
	}

	Statement* ForStatement::optimize()
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

		auto condition_optimized_evaluated = AS_EXPRESSION(condition_optimized->evaluate(_context));

		if (condition_optimized_evaluated != nullptr)
		{
			auto value = AS_INTEGER_LITERAL_EXPRESSION(condition_optimized_evaluated)->value;

			// The value is constexpr false so we don't even need to run the loop
			if (value == 0)
			{
				return new NullStatement{};
			}
		}

		return new ForStatement{ initialization_optimized, condition_optimized, expression_optimized, body_optimized };
	}

	Node* ForStatement::evaluate(Context* context) const
	{
		DISCARD initialization->evaluate(context);

		auto condition_evaluated = condition->evaluate(context);

		while (harvest(condition_evaluated) != 0)
		{
			DISCARD body->evaluate(context);
			DISCARD expression->evaluate(context);

			condition_evaluated = condition->evaluate(context);
		}

		return nullptr;
	}
}