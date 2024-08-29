#include "BinaryExpression.h"
#include "IntegerLiteralExpression.h"
#include "StringExpression.h"
#include "IdentifierExpression.h"
#include "Allocation.h"
#include "Allocator.h"
#include "Log.h"
#include "Generator.h"
#include "Evaluator.h"
#include "ErrorReporter.h"

#include <format>

#define MULTIPLICATION_ERROR do { Log::error("machine code generation is unsupported for runtime multiplication"); } while (0)

namespace
{
	using namespace hz;

	void generate_error(std::string context, Token token)
	{
		_error_reporter->post_error(std::format("invalid {} in a compiled context", context), token);
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



	std::string PlusBinaryExpression::string() const
	{
		return std::format("binary expression ({} + {})", left->string(), right->string());
	}

	std::string MinusBinaryExpression::string() const
	{
		return std::format("binary expression ({} - {})", left->string(), right->string());
	}

	std::string TimesBinaryExpression::string() const
	{
		return std::format("binary expression ({} * {})", left->string(), right->string());
	}

	std::string AssignBinaryExpression::string() const
	{
		return std::format("binary expression ({} = {})", left->string(), right->string());
	}

	std::string EqualityBinaryExpression::string() const
	{
		return std::format("binary expression ({} == {})", left->string(), right->string());
	}

	std::string InequalityBinaryExpression::string() const
	{
		return std::format("binary expression ({} != {})", left->string(), right->string());
	}

	std::string GreaterBinaryExpression::string() const
	{
		return std::format("binary expression ({} > {})", left->string(), right->string());
	}

	std::string LessBinaryExpression::string() const
	{
		return std::format("binary expression ({} < {})", left->string(), right->string());
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
		ManagedStaticAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.allocation);

		_generator->make_iadd(received_allocation->read(), temp.allocation->read());
	}

	void MinusBinaryExpression::generate(Allocation* received_allocation)
	{
		ManagedStaticAllocation temp{};

		left->generate(received_allocation);
		right->generate(temp.allocation);

		_generator->make_isub(received_allocation->read(), temp.allocation->read());
	}

	void TimesBinaryExpression::generate(Allocation*)
	{
		::generate_error("multiplication", _token);
	}

	void AssignBinaryExpression::generate(Allocation*)
	{
		::generate_error("assignment", _token);
	}

	void EqualityBinaryExpression::generate(Allocation*)
	{
		::generate_error("comparison", _token);
	}

	void InequalityBinaryExpression::generate(Allocation*)
	{
		::generate_error("comparison", _token);
	}

	void GreaterBinaryExpression::generate(Allocation*)
	{
		::generate_error("comparison", _token);
	}

	void LessBinaryExpression::generate(Allocation*)
	{
		::generate_error("comparison", _token);
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
				AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value == 0)
			{
				return right_optimized;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value == 0)
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
				AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value == 0)
			{
				//(0 - x) simplifies to -x
				AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value *= -1;
				return right_optimized;
			}

			else if (right_optimized->etype() == ExpressionType::INTEGER_LITERAL &&
					 AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value == 0)
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

				if (left_value == 0)
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
						MULTIPLICATION_ERROR;
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
						MULTIPLICATION_ERROR;
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
		return nullptr;
	}



	Node* PlusBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() == ExpressionType::INTEGER_LITERAL &&
			AS_EXPRESSION(right_evaluated)->etype() == ExpressionType::INTEGER_LITERAL)
		{
			return new IntegerLiteralExpression
			{ 
				std::get<std::uint32_t>(harvest(left_evaluated)) + std::get<std::uint32_t>(harvest(right_evaluated)),
				_token,
			};
		}

		else if (AS_EXPRESSION(left_evaluated)->etype() == ExpressionType::STRING &&
				 AS_EXPRESSION(right_evaluated)->etype() == ExpressionType::STRING)
		{
			return new StringExpression
			{
				std::format("{}{}",
					std::get<std::string>(harvest(left_evaluated)),
					std::get<std::string>(harvest(right_evaluated))),
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
			std::get<std::uint32_t>(harvest(left_evaluated)) - std::get<std::uint32_t>(harvest(right_evaluated)),
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
			std::get<std::uint32_t>(harvest(left_evaluated)) * std::get<std::uint32_t>(harvest(right_evaluated)),
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
				AS_IDENTIFIER_EXPRESSION(left_evaluated)->name, harvest(right_evaluated));
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
			_error_reporter->post_error("binary expression operands must evaluate to an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(harvest(left_evaluated)) == std::get<std::uint32_t>(harvest(right_evaluated)),
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
			_error_reporter->post_error("binary expression operands must evaluate to an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(harvest(left_evaluated)) != std::get<std::uint32_t>(harvest(right_evaluated)),
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
			_error_reporter->post_error("binary expression operands must evaluate to an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(harvest(left_evaluated)) > std::get<std::uint32_t>(harvest(right_evaluated)),
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
			_error_reporter->post_error("binary expression operands must evaluate to an integer", _token);
		}

		return new IntegerLiteralExpression
		{ 
			std::get<std::uint32_t>(harvest(left_evaluated)) < std::get<std::uint32_t>(harvest(right_evaluated)),
			_token,
		};
	}
}