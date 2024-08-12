#include "IfStatement.h"
#include "IntegerLiteralExpression.h"

#include "Allocation.h"
#include "Allocator.h"

#include "Utility.h"
#include "Evaluator.h"

#include <format>
#include "Log.h"

namespace hz
{
	StatementType IfStatement::stype() const
	{
		return StatementType::IF;
	}

	std::string IfStatement::string() const
	{
		return std::format("if statement ({}) \n[\n{}\n]\n else \n[\n{}\n]\n",
			condition->string(), if_body->string(), else_body ? else_body->string() : "");
	}

	IfStatement* IfStatement::copy() const
	{
		return new IfStatement{ *this };
	}

	void IfStatement::generate(Allocation*)
	{
		// we need to force allocate this one :(
		auto condition_allocation = _allocator->allocate_static(DC, true);

		// TODO: finish if statement generation here
	}

	Statement* IfStatement::optimize()
	{
		auto condition_optimized = AS_EXPRESSION(condition->optimize());
		auto if_body_optimized = AS_STATEMENT(if_body->optimize());
		auto else_body_optimized = AS_STATEMENT(else_body ? else_body->optimize() : nullptr);

		if (condition_optimized == nullptr &&
			if_body_optimized == nullptr &&
			else_body_optimized == nullptr)
		{
			return nullptr;
		}

		if (condition_optimized == nullptr)
		{
			condition_optimized = condition;
		}

		if (if_body_optimized == nullptr)
		{
			if_body_optimized = if_body;
		}

		if (else_body_optimized == nullptr)
		{
			else_body_optimized = else_body;
		}

		auto condition_optimized_evaluated = AS_EXPRESSION(condition_optimized->evaluate(_context));

		if (condition_optimized_evaluated != nullptr)
		{
			auto value = AS_INTEGER_LITERAL_EXPRESSION(condition_optimized_evaluated)->value;

			// The condition is constexpr true, so run if branch only
			if (value != 0)
			{
				return if_body_optimized;
			}

			// otherwise run the else branch
			else
			{
				return else_body_optimized;
			}
		}

		return new IfStatement{ condition_optimized, if_body_optimized, else_body_optimized };
	}

	Node* IfStatement::evaluate(Context* context) const
	{
		auto condition_evaluated = condition->evaluate(context);
		if (harvest(condition_evaluated) != 0)
		{
			DISCARD if_body->evaluate(context);
		}

		else
		{
			DISCARD else_body->evaluate(context);
		}

		return nullptr;
	}
}