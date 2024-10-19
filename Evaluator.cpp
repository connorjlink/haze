import std;

#include "Evaluator.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "CommonErrors.h"
#include "ErrorReporter.h"

// Haze Evaluator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Variable* node_to_variable(Node* node)
	{
		if (node->ntype() != NodeType::EXPRESSION)
		{
			CommonErrors::invalid_node_type(node->ntype(), NULL_TOKEN);
		}

		auto expression = AS_EXPRESSION_NODE(node);

		using enum ExpressionType;
		switch (expression->etype())
		{
			case INTEGER_LITERAL:
			{
				auto integer_literal_expression = AS_INTEGER_LITERAL_EXPRESSION(expression);
				auto& integer_literal = integer_literal_expression->value;

				using enum IntegerLiteralType;
				switch (integer_literal.type)
				{
					case UBYTE: return new UnsignedByteVariable{ integer_literal.storage.ubyte };
					case SBYTE: return new SignedByteVariable{ integer_literal.storage.sbyte };

					case UWORD: return new UnsignedWordVariable{ integer_literal.storage.uword };
					case SWORD: return new SignedWordVariable{ integer_literal.storage.sword };

					case UDWORD: return new UnsignedDoubleWordVariable{ integer_literal.storage.udword };
					case SDWORD: return new SignedDoubleWordVariable{ integer_literal.storage.sdword };

					case UQWORD: return new UnsignedQuadWordVariable{ integer_literal.storage.uqword };
					case SQWORD: return new SignedQuadWordVariable{ integer_literal.storage.sqword };
				}
			} break;

			case STRING:
			{
				auto string_expression = AS_STRING_EXPRESSION(expression);
				return new StringVariable{ string_expression->message };
			} break;

			default:
			{
				CommonErrors::invalid_expression_type(expression->etype(), NULL_TOKEN);
			} break;
		}


		/*if (node->ntype() == NodeType::EXPRESSION)
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
		return { (std::uint32_t)-1 };*/
	}

	Node* variable_to_node(Variable* variable)
	{
		using enum VariableType;
		switch (variable->vtype())
		{
			case UBYTE: 
			{
				auto unsigned_byte_variable = AS_UBYTE_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .ubyte = unsigned_byte_variable->value }, IntegerLiteralType::UBYTE }, NULL_TOKEN };
			} break;
				
			case SBYTE:
			{
				auto signed_byte_variable = AS_SBYTE_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .sbyte = signed_byte_variable->value }, IntegerLiteralType::SBYTE }, NULL_TOKEN };
			} break;
				
			case UWORD:
			{
				auto unsigned_word_variable = AS_UWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .uword = unsigned_word_variable->value }, IntegerLiteralType::UWORD }, NULL_TOKEN };
			} break;
				
			case SWORD:
			{
				auto signed_word_variable = AS_SWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .sword = signed_word_variable->value }, IntegerLiteralType::SWORD }, NULL_TOKEN };
			} break;
				
			case UDWORD:
			{
				auto unsigned_double_word_variable = AS_UDWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .udword = unsigned_double_word_variable->value }, IntegerLiteralType::UDWORD }, NULL_TOKEN };
			} break;
				
			case SDWORD:
			{
				auto signed_double_word_variable = AS_SDWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .sdword = signed_double_word_variable->value }, IntegerLiteralType::SDWORD }, NULL_TOKEN };
			} break;
				
			case UQWORD:
			{
				auto unsigned_quad_word_variable = AS_UQWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .uqword = unsigned_quad_word_variable->value }, IntegerLiteralType::UQWORD }, NULL_TOKEN };
			} break;
				
			case SQWORD: 
			{
				auto signed_quad_word_variable = AS_SQWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ { { .sqword = signed_quad_word_variable->value }, IntegerLiteralType::SQWORD }, NULL_TOKEN };
			} break;
				
			default:
			{
				CommonErrors::invalid_generic_type("variable type", _variable_type_map.at(variable->vtype()));
				return nullptr;
			} break;
		}
	}

	std::string format(Variable* value)
	{
		return value->format();
	}
}