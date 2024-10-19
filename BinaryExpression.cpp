import std;

#include "BinaryExpression.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "IdentifierExpression.h"
#include "Allocation.h"
#include "Generator.h"
#include "Evaluator.h"
#include "CommonErrors.h"
#include "ErrorReporter.h"

// Haze BinaryExpression.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	void generate_error(std::string op, Token token)
	{
		_error_reporter->post_error(std::format("unsupported compiler binary operator `{}`", op), token);
	}
}

namespace hz
{
	BinaryExpressionType PlusBinaryExpression::btype() const
	{
		return BinaryExpressionType::PLUS;
	}

	BinaryExpressionType MinusBinaryExpression::btype() const
	{
		return BinaryExpressionType::MINUS;
	}

	BinaryExpressionType TimesBinaryExpression::btype() const
	{
		return BinaryExpressionType::TIMES;
	}

	BinaryExpressionType AssignBinaryExpression::btype() const
	{
		return BinaryExpressionType::ASSIGN;
	}

	BinaryExpressionType EqualityBinaryExpression::btype() const
	{
		return BinaryExpressionType::EQUALITY;
	}

	BinaryExpressionType InequalityBinaryExpression::btype() const
	{
		return BinaryExpressionType::INEQUALITY;
	}

	BinaryExpressionType GreaterBinaryExpression::btype() const
	{
		return BinaryExpressionType::GREATER;
	}

	BinaryExpressionType LessBinaryExpression::btype() const
	{
		return BinaryExpressionType::LESS;
	}


	PlusBinaryExpression* PlusBinaryExpression::copy() const
	{
		return new PlusBinaryExpression{ *this };
	}

	MinusBinaryExpression* MinusBinaryExpression::copy() const
	{
		return new MinusBinaryExpression{ *this };
	}

	TimesBinaryExpression* TimesBinaryExpression::copy() const
	{
		return new TimesBinaryExpression{ *this };
	}

	AssignBinaryExpression* AssignBinaryExpression::copy() const
	{
		return new AssignBinaryExpression{ *this };
	}

	EqualityBinaryExpression* EqualityBinaryExpression::copy() const
	{
		return new EqualityBinaryExpression{ *this };
	}

	InequalityBinaryExpression* InequalityBinaryExpression::copy() const
	{
		return new InequalityBinaryExpression{ *this };
	}

	GreaterBinaryExpression* GreaterBinaryExpression::copy() const
	{
		return new GreaterBinaryExpression{ *this };
	}

	LessBinaryExpression* LessBinaryExpression::copy() const
	{
		return new LessBinaryExpression{ *this };
	}



	void PlusBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		_generator->compute_add(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void MinusBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		_generator->compute_subtract(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void TimesBinaryExpression::generate(Allocation*)
	{
		::generate_error("multiplication", _token);
	}

	void AssignBinaryExpression::generate(Allocation* received_allocation)
	{
		if (left->etype() == ExpressionType::IDENTIFIER)
		{
			auto identifier_expression = AS_IDENTIFIER_EXPRESSION(left);

			right->generate(received_allocation);
			// figure out how to assign the lhs
			_generator->update_local(identifier_expression->name, received_allocation->read());
		}

		else
		{
			CommonErrors::must_be_lvalue("the left-hand side of an assignment expression", left->_token);
		}
	}

	void EqualityBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		_generator->compute_equality(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void InequalityBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};
		
		left->generate(received_allocation);
		right->generate(temp.source());

		_generator->compute_inequality(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void GreaterBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		_generator->compute_greater(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}

	void LessBinaryExpression::generate(Allocation* received_allocation)
	{
		AutoStackAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.source());

		_generator->compute_less(received_allocation->read(), temp.source()->read(), received_allocation->read());
	}



	Expression* PlusBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{
			left_optimized = AS_EXPRESSION(left->copy());
			right_optimized = AS_EXPRESSION(right->copy());

			if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
				AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value == 1)
			{
				return right_optimized;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value.is_zero())
			{
				return left_optimized;
			}

			else if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{


				return new IntegerLiteralExpression
				{
					AS_INTEGER_LITERAL_EXPRESSION(left)->value + AS_INTEGER_LITERAL_EXPRESSION(right)->value,
					_token,
				};
			}

			else
			{
				//return new PlusBinaryExpression{ left_optimized, right_optimized };
				return nullptr;
			}
		}

		else
		{
			if (!left_optimized)
			{
				return new PlusBinaryExpression
				{ 
					AS_EXPRESSION(left->copy()), 
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new PlusBinaryExpression
				{ 
					left_optimized, 
					AS_EXPRESSION(right->copy()),
					
					_token,
				};
			}

			else
			{
				return new PlusBinaryExpression
				{ 
					left_optimized, 
					right_optimized,
					_token,
				};
			}
		}
	}

	Expression* MinusBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{
			left_optimized = AS_EXPRESSION(left->copy());
			right_optimized = AS_EXPRESSION(right->copy());

			if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
				AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value.is_zero())
			{
				//(0 - x) simplifies to -x
				AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value *= -1;
				return right_optimized;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value.is_zero())
			{
				return left_optimized;
			}

			else if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				return new IntegerLiteralExpression
				{
					AS_INTEGER_LITERAL_EXPRESSION(left)->value - AS_INTEGER_LITERAL_EXPRESSION(right)->value,
					_token,
				};
			}

			else
			{
				//return new MinusBinaryExpression{ left_optimized, right_optimized };
				return nullptr;
			}
		}

		else
		{
			if (!left_optimized)
			{
				return new MinusBinaryExpression
				{
					AS_EXPRESSION(left->copy()), 
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new MinusBinaryExpression
				{ 
					left_optimized, 
					AS_EXPRESSION(right->copy()),
					_token,
				};
			}

			else
			{
				return new MinusBinaryExpression
				{ 
					left_optimized, 
					right_optimized,
					_token,
				};
			}
		}
	}

	Expression* TimesBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{
			left_optimized = AS_EXPRESSION(left->copy());
			right_optimized = AS_EXPRESSION(right->copy());

			if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				auto left_value = AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value;
				auto result = left_optimized;

				if (left_value.is_zero())
				{
					AS_INTEGER_LITERAL_EXPRESSION(result)->value = 0;
				}

				else if (left_value == 1)
				{
					delete result;
					result = right_optimized;
				}

				else
				{
					if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
					{
						AS_INTEGER_LITERAL_EXPRESSION(result)->value *= 
							AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value;
					}

					else
					{
						_error_reporter->post_error("unsupported compiler code generation for multiplication", right_optimized->_token);
						return nullptr;
					}
				}

				return result;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL)
			{
				auto right_value = AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value;
				auto result = right_optimized;

				if (right_value == 0)
				{
					AS_INTEGER_LITERAL_EXPRESSION(result)->value = 0;
				}

				else if (right_value == 1)
				{
					delete result;
					result = left_optimized;
				}

				else
				{
					if (left_optimized->etype() == ExpressionType::INTEGER_LITERAL)
					{
						AS_INTEGER_LITERAL_EXPRESSION(result)->value *= 
							AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value;
					}

					else
					{
						_error_reporter->post_error("unsupported compiler code generation for multiplication", left_optimized->_token);
						return nullptr;
					}
				}

				return result;
			}

			else
			{
				return nullptr;
			}
		}

		else
		{
			if (!left_optimized)
			{
				return new TimesBinaryExpression
				{ 
					AS_EXPRESSION(left->copy()), 
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new TimesBinaryExpression
				{ 
					left_optimized, 
					AS_EXPRESSION(right->copy()),
					_token,
				};
			}

			else
			{
				return new TimesBinaryExpression
				{
					left_optimized, 
					right_optimized,
					_token,
				};
			}
		}
	}

	Expression* AssignBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* EqualityBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* InequalityBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* GreaterBinaryExpression::optimize()
	{
		return nullptr;
	}

	Expression* LessBinaryExpression::optimize()
	{
		const auto left_optimized = AS_EXPRESSION(left->optimize());
		const auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized)
		{

		}

		else
		{
			if (!left_optimized)
			{
				return new LessBinaryExpression
				{
					AS_EXPRESSION(left->copy()),
					right_optimized,
					_token,
				};
			}

			else if (!right_optimized)
			{
				return new LessBinaryExpression
				{
					left_optimized,
					AS_EXPRESSION(right->copy()),
					_token,
				};
			}

			else
			{
				return new LessBinaryExpression
				{
					left_optimized,
					right_optimized,
					_token,
				};
			}
		}
	}



	Node* PlusBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() == ExpressionType::INTEGER_LITERAL &&
			AS_EXPRESSION(right_evaluated)->etype() == ExpressionType::INTEGER_LITERAL)
		{
			const auto left_evaluated_variable = node_to_variable(left_evaluated);
			const auto right_evaluated_variable = node_to_variable(right_evaluated);

			return new IntegerLiteralExpression
			{ 
				std::get<std::uint32_t>(node_to_variable(left_evaluated)) + std::get<std::uint32_t>(node_to_variable(right_evaluated)),
				_token,
			};
		}

		else if (AS_EXPRESSION(left_evaluated)->etype() == ExpressionType::STRING &&
				 AS_EXPRESSION(right_evaluated)->etype() == ExpressionType::STRING)
		{
			return new StringExpression
			{
				std::format("{}{}",
					std::get<std::string>(node_to_variable(left_evaluated)),
					std::get<std::string>(node_to_variable(right_evaluated))),
				_token,
			};
		}

		_error_reporter->post_uncorrectable("both operands of a binary expression must evaluate to integers", NULL_TOKEN);
	}

	Node* MinusBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("binary expression operands must evaluate to an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(node_to_variable(left_evaluated)) - std::get<std::uint32_t>(node_to_variable(right_evaluated)),
			_token,
		};
	}

	Node* TimesBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("binary expression operands must evaluate to an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(node_to_variable(left_evaluated)) * std::get<std::uint32_t>(node_to_variable(right_evaluated)),
			_token,
		};
	}

	Node* AssignBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left;
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::IDENTIFIER)
		{
			_error_reporter->post_error("the left-hand operand of an assignment must result in a modifiable l-value", left->_token);
		}

		if (AS_EXPRESSION(right_evaluated)->etype() == ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() == ExpressionType::IDENTIFIER)
		{
			context->define_variable(
				AS_IDENTIFIER_EXPRESSION(left_evaluated)->name, node_to_variable(right_evaluated));
		}

		else
		{
			_error_reporter->post_error("the right-hand operand of an assignment must result in an r-value", right->_token);
		}

		return nullptr;
	}

	Node* EqualityBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("binary expression operands must result in an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(node_to_variable(left_evaluated)) == std::get<std::uint32_t>(node_to_variable(right_evaluated)),
			_token,
		};
	}

	Node* InequalityBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("binary expression operands must result in an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(node_to_variable(left_evaluated)) != std::get<std::uint32_t>(node_to_variable(right_evaluated)),
			_token,
		};
	}

	Node* GreaterBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("binary expression operands must result in an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(node_to_variable(left_evaluated)) > std::get<std::uint32_t>(node_to_variable(right_evaluated)),
			_token,
		};
	}

	Node* LessBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			_error_reporter->post_error("binary expression operands must result in an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(node_to_variable(left_evaluated)) < std::get<std::uint32_t>(node_to_variable(right_evaluated)),
			_token,
		};
	}
}