#include "Evaluator.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "Log.h"

#include <format>
#include <variant>

namespace hz
{
	variable_t harvest(Node* node)
	{
		if (node->ntype() == NodeType::EXPRESSION)
		{
			auto expression = AS_EXPRESSION(node);

			if (expression->etype() == ExpressionType::INTEGER_LITERAL)
			{
				auto integer_literal_expression = AS_INTEGER_LITERAL_EXPRESSION(expression);

				return { integer_literal_expression->value };
			}

			else if (expression->etype() == ExpressionType::STRING)
			{
				auto string_expression = AS_STRING_EXPRESSION(expression);

				return { string_expression->message };
			}

			Log::error("interpreter evaluation data harvesting only accepts literal-type expressions");
		}

		Log::error("interpreter evaluation data harvesting only accepts expressions");
	}

	Node* unharvest(variable_t value)
	{
		switch (value.index())
		{
			case 0: return new IntegerLiteralExpression{ std::get<0>(value) };
			case 1: return new StringExpression{ std::get<1>(value) };
			default: Log::error(std::format("variable_t type index {} is invalid and cannot be evaluated", value.index()));
		}
	}

	std::string format(variable_t value)
	{
		return std::visit([](const auto& arg) { return std::format("{}", arg); }, value);
	}
}