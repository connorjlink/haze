#include "BinaryExpression.h"
#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Allocator.h"
#include "Log.h"
#include "Generator.h"

#include <format>

#define MULTIPLICATION_ERROR do { Log::error("machine code generation is unsupported for runtime multiplication"); } while (0)

namespace hz
{
	BinaryExpression::Type PlusBinaryExpression::btype() const
	{
		return BinaryExpression::Type::PLUS;
	}

	BinaryExpression::Type MinusBinaryExpression::btype() const
	{
		return BinaryExpression::Type::MINUS;
	}

	BinaryExpression::Type TimesBinaryExpression::btype() const
	{
		return BinaryExpression::Type::TIMES;
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
		auto temp_allocation = allocator->allocate_static();

		left->generate(received_allocation);
		right->generate(temp_allocation);

		generator->iadd(received_allocation->read(), temp_allocation->read());
		temp_allocation->deallocate();
	}

	void MinusBinaryExpression::generate(Allocation* received_allocation)
	{
		auto temp_allocation = allocator->allocate_static();

		left->generate(received_allocation);
		right->generate(temp_allocation);

		generator->isub(received_allocation->read(), temp_allocation->read());
		temp_allocation->deallocate();
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

		if (!left_optimized && !right_optimized) return nullptr;
		if (!left_optimized) left_optimized = left;
		if (!right_optimized) right_optimized = right;

		if (left_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
			AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value == 0)
		{
			return right_optimized;
		}

		else if (right_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
				 AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value == 0)
		{
			return left_optimized;
		}

		else
		{
			auto optimized = copy();
			optimized->left = left_optimized;
			optimized->right = right_optimized;
			return optimized;
		}
	}

	Expression* MinusBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized) return nullptr;
		if (!left_optimized) left_optimized = left;
		if (!right_optimized) right_optimized = right;

		if (left_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
			AS_INTEGER_LITERAL_EXPRESSION(left_optimized)->value == 0)
		{
			//(0 - x) simplifies to -x
			AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value *= -1;
			return right_optimized;
		}

		else if (right_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
				 AS_INTEGER_LITERAL_EXPRESSION(right_optimized)->value == 0)
		{
			return left_optimized;
		}

		else
		{
			auto optimized = copy();
			optimized->left = left_optimized;
			optimized->right = right_optimized;
			return optimized;
		}
	}

	Expression* TimesBinaryExpression::optimize()
	{
		auto left_optimized = AS_EXPRESSION(left->optimize());
		auto right_optimized = AS_EXPRESSION(right->optimize());

		if (!left_optimized && !right_optimized) return nullptr;
		if (!left_optimized) left_optimized = left;
		if (!right_optimized) right_optimized = right;

		if (left_optimized->etype() == Expression::Type::INTEGER_LITERAL)
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
				if (right_optimized->etype() == Expression::Type::INTEGER_LITERAL)
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

		else if (right_optimized->etype() == Expression::Type::INTEGER_LITERAL)
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
				if (left_optimized->etype() == Expression::Type::INTEGER_LITERAL)
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
			auto optimized = copy();
			optimized->left = left_optimized;
			optimized->right = right_optimized;
			return optimized;
		}
	}
}