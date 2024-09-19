import std;

#include "Evaluator.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "ErrorReporter.h"

// Haze Evaluator.cpp
// (c) Connor J. Link. All Rights Reserved.

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

			_error_reporter->post_error(std::format("invalid harvest expression type `{}`", _expression_type_map.at(expression->etype())), NULL_TOKEN);
			return { (std::uint32_t)-1 };
		}

		_error_reporter->post_error(std::format("invalid harvest node type `{}`", _node_map.at(node->ntype())), NULL_TOKEN);
		return { (std::uint32_t)-1 };
	}

	Node* unharvest(variable_t value)
	{
		switch (value.index())
		{
			case 0: return new IntegerLiteralExpression{ std::get<0>(value), NULL_TOKEN };
			case 1: return new StringExpression{ std::get<1>(value), NULL_TOKEN };

			default:
			{
				_error_reporter->post_error(std::format("invalid variable type index {}", value.index()), NULL_TOKEN);
				return nullptr;
			} break;
		}
	}

	std::string format(variable_t value)
	{
		return std::visit([](auto&& arg) { return std::format("{}", arg); }, value);
	}
}