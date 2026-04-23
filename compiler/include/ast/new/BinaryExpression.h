#ifndef HAZE_BINARYEXPRESSION_H
#define HAZE_BINARYEXPRESSION_H

#include <ast/new/AST.h>
#include <ast/new/Expression.h>
#include <type/Type.h>

// Haze BinaryExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class BinaryExpression : public ExpressionBase
    {
    public:
        enum class Type
        {
            ADD,
            ADD_ASSIGNMENT,
            SUBTRACT,
            SUBTRACT_ASSIGNMENT,
            MULTIPLY,
            MULTIPLY_ASSIGNMENT,
            DIVIDE,
            DIVIDE_ASSIGNMENT,
            MODULO,
            MODULO_ASSIGNMENT,
            LEFT_SHIFT,
            LEFT_SHIFT_ASSIGNMENT,
            RIGHT_SHIFT,
            RIGHT_SHIFT_ASSIGNMENT,
            BITWISE_AND,
            BITWISE_AND_ASSIGNMENT,
            BITWISE_OR,
            BITWISE_OR_ASSIGNMENT,
            BITWISE_XOR,
            BITWISE_XOR_ASSIGNMENT,
            LOGICAL_AND,
            LOGICAL_OR,
            EQUAL,
            NOT_EQUAL,
            LESS,
            LESS_EQUAL,
            GREATER,
            GREATER_EQUAL,
            DOT,
            ARROW,
            COMMA,
        };

        static const std::unordered_map<ExpressionType, std::string_view> _binary_expression_type_map
        {
            { Type::ADD, " + " },
            { Type::ADD_ASSIGNMENT, " += " },
            { Type::SUBTRACT, " - " },
            { Type::SUBTRACT_ASSIGNMENT, " -= " },
            { Type::MULTIPLY, " * " },
            { Type::MULTIPLY_ASSIGNMENT, " *= " },
            { Type::DIVIDE, " / " },
            { Type::DIVIDE_ASSIGNMENT, " /= " },
            { Type::MODULO, " % " },
            { Type::MODULO_ASSIGNMENT, " %= " },
            { Type::LEFT_SHIFT, " << " },
            { Type::LEFT_SHIFT_ASSIGNMENT, " <<= " },
            { Type::RIGHT_SHIFT, " >> " },
            { Type::RIGHT_SHIFT_ASSIGNMENT, " >>= " },
            { Type::BITWISE_AND, " & " },
            { Type::BITWISE_AND_ASSIGNMENT, " &= " },
            { Type::BITWISE_OR, " | " },
            { Type::BITWISE_OR_ASSIGNMENT, " |= " },
            { Type::BITWISE_XOR, " ^ " },
            { Type::BITWISE_XOR_ASSIGNMENT, " ^= " },
            { Type::LOGICAL_AND, " && " },
            { Type::LOGICAL_OR, " || " },
            { Type::EQUAL, " == " },
            { Type::NOT_EQUAL, " != " },
            { Type::LESS, " < " },
            { Type::LESS_EQUAL, " <= " },
            { Type::GREATER, " > " },
            { Type::GREATER_EQUAL, " >= " },
            { Type::DOT, "." },
            { Type::ARROW, "->" },
            { Type::COMMA, ", " }
        };

    public:
        SumHandle left;
        SumHandle right;
        Type binary_type;

    public:
        std::string format() const
        {
            return std::format("{} {} {}", 
                left.format(), 
                _binary_expression_type_map.at(binary_type), 
                right.format());
        }

        void generate(const Storage& ast) const
        {

        }

        SumHandle evaluate(const Storage& ast, const Context& context) const
        {
            const auto left_value = left.evaluate(ast, context);
            const auto right_value = right.evaluate(ast, context);

            using enum Type;
            switch (binary_type)
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

        SumHandle optimize(const Storage& ast) const
        {
            const auto left_value = left.optimize(ast);
            const auto right_value = right.optimize(ast);

            // fold valid constant expressions
            if (left_value.is_constant() && right_value.is_constant())
            {   
                auto context = Context{};
            }

            return make_invalid_handle(ast);
        }

        SumHandle get_type(const Storage& ast) const
        {
            if (!left.check_types(ast) || !right.check_types(ast))
            {
                return false;
            }

#pragma message("TODO: implement binary expression type checking")

            using enum Type;
            switch (binary_type)
            {
                case ADD:
                case SUBTRACT:
                case MULTIPLY:
                case DIVIDE:
                case MODULO:
                {
                    // semantically, arithmetic operators apply to any combination of integral and floating point types

                    if (left.type(ast).is_floating_point())
                    {
                        // if either operand is floating point, the result is floating point
                        if (right.type(ast).is_floating_point())
                        {
                            return left.type(ast);
                        }
                        else if (right.type(ast).is_integral())
                        {
                            return left.type(ast);
                        }
                    }
                    else if (left.type(ast).is_integral())
                    {
                        if (right.type(ast).is_integral())
                        {
                            // if both operands are integral, the result is the type of the larger operand
                            return Type::get_larger_integral_type(left.type(ast), right.type(ast));
                        }
                        else if (right.type(ast).is_floating_point())
                        {
                            return right.type(ast);
                        }
                    }

                    USE_SAFE(ErrorReporter)->post_error(std::format(
                        "invalid operand types for binary operator `{}`: `{}` and `{}`", 
                            _binary_expression_type_map.at(binary_type), left.type(ast).string(), right.type(ast).string()), NULL_TOKEN);
                    return make_invalid_handle(ast);
                } break;

                case LEFT_SHIFT:
                case RIGHT_SHIFT:
                case BITWISE_AND:
                case BITWISE_OR:
                case BITWISE_XOR:
                {
                    // for now, only allow these operations on integers
                    return left.type(ast).is_integral() && right.type(ast).is_integral();
                } break;

                case EQUAL:
                case NOT_EQUAL:
                {


                } break;

                case LESS:
                case LESS_EQUAL:
                case GREATER:
                case GREATER_EQUAL:
                {
                    // semantically, relational operators apply to any scalar (integral or compatible pointer-to types)
                    if (left.type(ast).is_pointer() && right.type(ast).is_pointer())
                    {
                        return Type::is_compatible(left.type(ast).pointee_type(), right.type(ast).pointee_type());
                    }

                    return left.type(ast).is_integral() && right.type(ast).is_integral();
                } break;

                case LOGICAL_AND:
                case LOGICAL_OR:
                {
                    // semantically, boolean operators apply to any scalar (integral or pointer-to types)
                    return left.type(ast).is_scalar() && right.type(ast).is_scalar();
                } break;

                case DOT:
                {
                    if (const auto type = left.type(ast); type.is_struct_or_union())
                    {
                        if (!right.type(ast).is_identifier())
                        {
                            USE_SAFE(ErrorReporter)->post_error(std::format(
                                "right operand of member access operator must be an identifier, got `{}`", right.type(ast).string()), NULL_TOKEN);
                            return false;
                        }

                        const auto members = type.members().value();
                        const auto member_name = right.identifier(ast).name;
                        if (!members.contains(member_name))
                        {
                            USE_SAFE(ErrorReporter)->post_error(std::format(
                                "type `{}` has no member named `{}`", type.string(), member_name), NULL_TOKEN);
                            return false;
                        }

                        return true;
                    }

                    USE_SAFE(ErrorReporter)->post_error(std::format(
                        "left operand of member access operator must be a struct or union type, got `{}`", left.type(ast).string()), NULL_TOKEN);
                    return false;
                } break;

                case ARROW:
                {
                    if (const auto type = left.type(ast); type.is_pointer() && type.pointee_type().is_struct_or_union())
                    {
                        if (!right.type(ast).is_identifier())
                        {
                            USE_SAFE(ErrorReporter)->post_error(std::format(
                                "right operand of member access operator must be an identifier, got `{}`", right.type(ast).string()), NULL_TOKEN);
                            return false;
                        }

                        const auto members = type.pointee_type().members().value();
                        const auto member_name = right.identifier(ast).name;
                        if (!members.contains(member_name))
                        {
                            USE_SAFE(ErrorReporter)->post_error(std::format(
                                "type `{}` has no member named `{}`", type.string(), member_name), NULL_TOKEN);
                            return false;
                        }

                        return true;
                    }

                    USE_SAFE(ErrorReporter)->post_error(std::format(
                        "left operand of member access operator must be a pointer to struct or union type, got `{}`", left.type(ast).string()), NULL_TOKEN);
                    return false;
                } break;

                default:
                {
                    // unknown, semi-panic
                    USE_SAFE(ErrorReporter)->post_error(std::format(
                        "cannot check type of type `{}`", _binary_expression_type_map.at(binary_type)), NULL_TOKEN);
                    return false;
                } break;
            }
            
        }

    public:
        BinaryExpression(SumHandle left, SumHandle right, BinaryExpressionType type)
            : left{ left }, right{ right }, type{ type }
        {
        }
    };
#define MAKE_BINARY_EXPRESSION(left, right, type) BinaryExpression{ MAKE_HANDLE(ast, left), MAKE_HANDLE(ast, right), type }

}

#endif
