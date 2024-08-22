#include "BinaryExpression.h"
#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Allocator.h"
#include "Log.h"
#include "Generator.h"
#include "Evaluator.h"

#include <format>

#define MULTIPLICATION_ERROR do { Log::error("machine code generation is unsupported for runtime multiplication"); } while (0)

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

	[[noreturn]]
	void TimesBinaryExpression::generate(Allocation* receieved_allocation)
	{
		DISCARD receieved_allocation;
		MULTIPLICATION_ERROR;
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
				return new IntegerLiteralExpression{
					AS_INTEGER_LITERAL_EXPRESSION(left)->value + AS_INTEGER_LITERAL_EXPRESSION(right)->value };
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
				return new PlusBinaryExpression{ AS_EXPRESSION(left->copy()), right_optimized };
			}

			else if (!right_optimized)
			{
				return new PlusBinaryExpression{ left_optimized, AS_EXPRESSION(right->copy()) };
			}

			else
			{
				return new PlusBinaryExpression{ left_optimized, right_optimized };
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
				return new IntegerLiteralExpression{
					AS_INTEGER_LITERAL_EXPRESSION(left)->value - AS_INTEGER_LITERAL_EXPRESSION(right)->value };
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
				return new MinusBinaryExpression{ AS_EXPRESSION(left->copy()), right_optimized };
			}

			else if (!right_optimized)
			{
				return new MinusBinaryExpression{ left_optimized, AS_EXPRESSION(right->copy()) };
			}

			else
			{
				return new MinusBinaryExpression{ left_optimized, right_optimized };
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
				return new TimesBinaryExpression{ AS_EXPRESSION(left->copy()), right_optimized };
			}

			else if (!right_optimized)
			{
				return new TimesBinaryExpression{ left_optimized, AS_EXPRESSION(right->copy()) };
			}

			else
			{
				return new TimesBinaryExpression{ left_optimized, right_optimized };
			}
		}
	}



	Node* PlusBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			Log::error("both operands of a binary expression must evaluate to integers");
		}

		return new IntegerLiteralExpression{ std::get<int>(harvest(left_evaluated)) + std::get<int>(harvest(right_evaluated)) };
	}

	Node* MinusBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			Log::error("both operands of a binary expression must evaluate to integers");
		}

		return new IntegerLiteralExpression{ std::get<int>(harvest(left_evaluated)) - std::get<int>(harvest(right_evaluated)) };
	}

	Node* TimesBinaryExpression::evaluate(Context* context) const
	{
		const auto left_evaluated = left->evaluate(context);
		const auto right_evaluated = right->evaluate(context);

		if (AS_EXPRESSION(left_evaluated)->etype() != ExpressionType::INTEGER_LITERAL ||
			AS_EXPRESSION(right_evaluated)->etype() != ExpressionType::INTEGER_LITERAL)
		{
			Log::error("both operands of a binary expression must evaluate to integers");
		}

		return new IntegerLiteralExpression{ std::get<int>(harvest(left_evaluated)) * std::get<int>(harvest(right_evaluated)) };
	}
}