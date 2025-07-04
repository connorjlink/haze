import std;

#include <runtime/Evaluator.h>
#include <ast/IntegerLiteralExpression.h>
#include <ast/StringExpression.h>
#include <error/CommonErrors.h>
#include <runtime/models/Variable.h>

// Haze Evaluator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Variable* node_to_variable(Node* node)
	{
		if (node->ntype() != NodeType::EXPRESSION)
		{
			CommonErrors::invalid_node_type(node->ntype(), NULL_TOKEN);
			return nullptr;
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
				switch (integer_literal->itype())
				{
					case UBYTE: return new UnsignedByteVariable{ AS_UNSIGNED_BYTE_INTEGER_LITERAL(integer_literal)->value };
					case SBYTE: return new SignedByteVariable{ AS_SIGNED_BYTE_INTEGER_LITERAL(integer_literal)->value };

					case UWORD: return new UnsignedWordVariable{ AS_UNSIGNED_WORD_INTEGER_LITERAL(integer_literal)->value };
					case SWORD: return new SignedWordVariable{ AS_SIGNED_BYTE_INTEGER_LITERAL(integer_literal)->value };

					case UDWORD: return new UnsignedDoubleWordVariable{ AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(integer_literal)->value };
					case SDWORD: return new SignedDoubleWordVariable{ AS_SIGNED_BYTE_INTEGER_LITERAL(integer_literal)->value };

					case UQWORD: return new UnsignedQuadWordVariable{ AS_UNSIGNED_QUAD_WORD_INTEGER_LITERAL(integer_literal)->value };
					case SQWORD: return new SignedQuadWordVariable{ AS_SIGNED_QUAD_WORD_INTEGER_LITERAL(integer_literal)->value };
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

		return nullptr;
	}

	Node* variable_to_node(Variable* variable)
	{
		using enum VariableType;
		switch (variable->vtype())
		{
			case UBYTE: 
			{
				auto unsigned_byte_variable = AS_UBYTE_VARIABLE(variable);
				return new IntegerLiteralExpression{ new UnsignedByteIntegerLiteral{ unsigned_byte_variable->value }, NULL_TOKEN };
			} break;
				
			case SBYTE:
			{
				auto signed_byte_variable = AS_SBYTE_VARIABLE(variable);
				return new IntegerLiteralExpression{ new SignedByteIntegerLiteral{ signed_byte_variable->value }, NULL_TOKEN };
			} break;
				
			case UWORD:
			{
				auto unsigned_word_variable = AS_UWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ new UnsignedWordIntegerLiteral{ unsigned_word_variable->value }, NULL_TOKEN };
			} break;
				
			case SWORD:
			{
				auto signed_word_variable = AS_SWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ new SignedWordIntegerLiteral{ signed_word_variable->value }, NULL_TOKEN };
			} break;
				
			case UDWORD:
			{
				auto unsigned_double_word_variable = AS_UDWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ new UnsignedDoubleWordIntegerLiteral{ unsigned_double_word_variable->value }, NULL_TOKEN };
			} break;
				
			case SDWORD:
			{
				auto signed_double_word_variable = AS_SDWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ new SignedDoubleWordIntegerLiteral{ signed_double_word_variable->value }, NULL_TOKEN };
			} break;
				
			case UQWORD:
			{
				auto unsigned_quad_word_variable = AS_UQWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ new UnsignedQuadWordIntegerLiteral{ unsigned_quad_word_variable->value }, NULL_TOKEN };
			} break;
				
			case SQWORD: 
			{
				auto signed_quad_word_variable = AS_SQWORD_VARIABLE(variable);
				return new IntegerLiteralExpression{ new SignedQuadWordIntegerLiteral{ signed_quad_word_variable->value }, NULL_TOKEN };
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