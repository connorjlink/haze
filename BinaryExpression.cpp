#include "BinaryExpression.h"
#include "IntegerLiteralExpression.h"
#include "Allocation.h"
#include "Log.h"
#include <fmt/format.h>

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
        return fmt::format("binary expression ({} + {})", left->string(), right->string());
    }

    std::string MinusBinaryExpression::string() const
    {
        return fmt::format("binary expression ({} - {})", left->string(), right->string());
    }

    std::string TimesBinaryExpression::string() const
    {
        return fmt::format("binary expression ({} * {})", left->string(), right->string());
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



    Segment PlusBinaryExpression::generate(Allocation* received_allocation)
    {
        auto temp_allocation = Allocator::allocate_static();

        left->generate(received_allocation);
        right->generate(temp_allocation);

        generator->iadd(AS_STATIC(left)->get_register(), AS_STATIC(right)->get_register());
    }

    Segment MinusBinaryExpression::generate(Allocation* received_allocation)
    {
        //TODO:
        auto temp_allocation = Allocator::allocate_static();

        left->generate(received_allocation);
        right->generate(temp_allocation);

        generator->isub(AS_STATIC(left)->get_register(), AS_STATIC(right)->get_register());
    }

    Segment TimesBinaryExpression::generate(Allocation* receieved_allocation)
    {
        MULTIPLICATION_ERROR;
    }



    Expression* PlusBinaryExpression::optimize()
    {
        auto left_optimized = left->optimize();
        auto right_optimized = right->optimize();

        if (left_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
            AS_INTEGER_LITERAL(left_optimized)->value == 0)
        {
            return right_optimized;
        }

        else if (right_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
                 AS_INTEGER_LITERAL(right_optimized)->value == 0)
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
        auto left_optimized = left->optimize();
        auto right_optimized = right->optimize();

        if (left_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
            AS_INTEGER_LITERAL(left_optimized)->value == 0)
        {
            return right_optimized;
        }

        else if (right_optimized->etype() == Expression::Type::INTEGER_LITERAL &&
                 AS_INTEGER_LITERAL(right_optimized)->value == 0)
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
        auto left_optimized = left->optimize();
        auto right_optimized = right->optimize();

        if (left_optimized->etype() == Expression::Type::INTEGER_LITERAL)
        {
            auto left_value = AS_INTEGER_LITERAL(left_optimized)->value;
            auto result = left_optimized;

            if (left_value == 0)
            {
                AS_INTEGER_LITERAL(result)->value = 0;
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
                    AS_INTEGER_LITERAL(result)->value *= AS_INTEGER_LITERAL(right_optimized)->value;
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
            auto right_value = AS_INTEGER_LITERAL(right_optimized)->value;
            auto result = right_optimized;

            if (right_value == 0)
            {
                AS_INTEGER_LITERAL(result)->value = 0;
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
                    AS_INTEGER_LITERAL(result)->value *= AS_INTEGER_LITERAL(left_optimized)->value;
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