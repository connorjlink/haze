#include "Evaluator.h"
#include "IntegerLiteralExpression.h"
#include "Log.h"

namespace hz
{
	VariableType harvest(Node* node)
	{
		if (node->ntype() == NodeType::EXPRESSION)
		{
			auto expression = AS_EXPRESSION(node);

			if (expression->etype() == ExpressionType::INTEGER_LITERAL)
			{
				auto integer_literal_expression = AS_INTEGER_LITERAL_EXPRESSION(expression);

				return integer_literal_expression->value;
			}

			Log::error("Interpreter evaluation data harvesting only accepts integer literal expressions");
		}

		Log::error("Interpreter evaluation data harvesting only accepts expression nodes");
	}
}