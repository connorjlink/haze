import std;

#include <ast/expression/Expression.h>

// Haze Expression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::string BinaryExpression::format(void) const
	{
		return std::format("{} {} {}",
			left.format(),
			binary_expression_kind_map.at(kind),
			right.format());
	}

	void BinaryExpression::generate(const Storage& ast) const
	{
#pragma message("TODO: implement binary expression code generation")
	}

	ExpressionHandle BinaryExpression::evaluate(const Storage& ast, const Context& context) const
	{
		const auto left_value = left.evaluate(ast, context);
		const auto right_value = right.evaluate(ast, context);

		using enum BinaryExpressionKind;
		switch (kind)
		{
			case ADD:
			{
				return left_value + right_value;
			} break;

			case SUBTRACT:
			{
				return left_value - right_value;
			} break;

			case MULTIPLY:
			{
				return left_value * right_value;
			} break;

			case DIVIDE:
			{
				return left_value / right_value;
			} break;

			case MODULO:
			{
				return left_value % right_value;
			} break;

			case LEFT_SHIFT:
			{
				return left_value << right_value;
			} break;

			case RIGHT_SHIFT:
			{
				return left_value >> right_value;
			} break;

			case BITWISE_AND:
			{
				return left_value & right_value;
			} break;
		}

#pragma message("TODO: implement binary expression evaluation")
		return *this;
	}

	ExpressionHandle BinaryExpression::optimize(const Storage& ast) const
	{
		const auto left_value = left.optimize(ast);
		const auto right_value = right.optimize(ast);

		// fold valid constant expressions
		if (left_value.is_constant() && right_value.is_constant())
		{
			auto context = Context{};
#pragma message("TODO: implement binary expression optimization")
		}

		return make_invalid_handle(ast);
	}

	ExpressionHandle BinaryExpression::get_type(const Storage& ast) const
	{
		if (!left.check_types(ast) || !right.check_types(ast))
		{
			return make_invalid_handle(ast);
		}

		const left_type = left.get_type(ast);
		const right_type = right.get_type(ast);

#pragma message("TODO: implement binary expression type checking")

		using enum BinaryExpressionKind;
		switch (kind)
		{
			case ADD:
			case SUBTRACT:
			case MULTIPLY:
			case DIVIDE:
			case MODULO:
			{
				// semantically, arithmetic operators apply to any combination of integral and floating point types

				if (left_type.is_floating_point())
				{
					// if either operand is floating point, the result is floating point
					if (right_type.is_floating_point())
					{
						return left_type;
					}
					else if (right_type.is_integral())
					{
						return left_type;
					}
				}
				else if (left_type.is_integral())
				{
					if (right_type.is_integral())
					{
						// if both operands are integral, the result is the type of the larger operand
						return Type::get_larger_integral_type(left_type, right_type);
					}
					else if (right_type.is_floating_point())
					{
						return right_type;
					}
				}

				USE_SAFE(ErrorReporter)->post_error(std::format(
					"invalid operand types for binary operator `{}`: `{}` and `{}`",
					binary_expression_kind_map.at(kind), to_string(left_type), to_string(right_type)), NULL_TOKEN);
				return make_invalid_handle(ast);
			} break;

			case LEFT_SHIFT:
			case RIGHT_SHIFT:
			case BITWISE_AND:
			case BITWISE_OR:
			case BITWISE_XOR:
			{
				// for now, only allow these operations on integers
				if (!(left_type.is_integral() && right_type.is_integral()))
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"invalid operand types for bitwise operator `{}`: `{}` and `{}`",
						binary_expression_kind_map.at(kind), to_string(left_type), to_string(right_type)), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				return Type::get_larger_integral_type(left_type, right_type);
			} break;

			case EQUAL:
			case NOT_EQUAL:
			{
				// TODO:

			} break;

			case LESS:
			case LESS_EQUAL:
			case GREATER:
			case GREATER_EQUAL:
			{
				// semantically, relational operators apply to any scalar (integral or compatible pointer-to types)
				if (left_type.is_pointer() && right_type.is_pointer())
				{
					if (!Type::is_compatible(left_type.pointee_type(), right_type.pointee_type()))
					{
						USE_SAFE(ErrorReporter)->post_error(std::format(
							"cannot compare pointers of incompatible types `{}` and `{}`", to_string(left_type), to_string(right_type)), NULL_TOKEN);
						return make_invalid_handle(ast);
					}

					return Type::get_pointer_integral_type(left_type, right_type);
				}

				if (!(left_type.is_integral() && right_type.is_integral()))
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"invalid operand types for relational operator `{}`: `{}` and `{}`",
						binary_expression_kind_map.at(kind), to_string(left_type), to_string(right_type)), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				return Type::get_promoted_integral_type(left_type, right_type);
			} break;

			case LOGICAL_AND:
			case LOGICAL_OR:
			{
				// semantically, boolean operators apply to any scalar (integral or pointer-to types)
				if (!(left_type.is_scalar() && right_type.is_scalar()))
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"invalid operand types for logical operator `{}`: `{}` and `{}`",
						binary_expression_kind_map.at(kind), to_string(left_type), to_string(right_type)), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				return Type::get_promoted_integral_type(left_type, right_type);
			} break;

			case DOT:
			{
				if (!left_type.is_struct_or_union())
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"left operand of member access operator must be a struct or union type, got `{}`", to_string(left_type)), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				if (!right_type.is_identifier())
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"right operand of member access operator must be an identifier, got `{}`", to_string(right_type)), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				const auto members = left_type.members().value();
				const auto member_name = right.identifier(ast).name;
				if (!members.contains(member_name))
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"type `{}` has no member named `{}`", to_string(left_type), member_name), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				return members.at(member_name).type;
			} break;

			case ARROW:
			{
				if (!(left_type.is_pointer() && left_type.pointee_type().is_struct_or_union()))
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"left operand of member access operator must be a pointer to struct or union type, got `{}`", to_string(left_type)), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				if (!right_type.is_identifier())
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"right operand of member access operator must be an identifier, got `{}`", to_string(right_type)), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				const auto members = left_type.pointee_type().members().value();
				const auto member_name = right.identifier(ast).name;
				if (!members.contains(member_name))
				{
					USE_SAFE(ErrorReporter)->post_error(std::format(
						"type `{}` has no member named `{}`", to_string(left_type), member_name), NULL_TOKEN);
					return make_invalid_handle(ast);
				}

				return members.at(member_name).type;
			} break;

			default:
			{
				// unknown, semi-panic
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"cannot check type of type `{}`", binary_expression_kind_map.at(kind)), NULL_TOKEN);
				return make_invalid_handle(ast);
			} break;
		}

	}
}
