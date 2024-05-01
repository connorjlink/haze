#include "Optimizer.h"
#include "Parser.h"
#include "Log.h"
#include "Allocator.h"
#include "IntegerLiteralExpression.h"

namespace hz
{
    Optimizer::Optimizer(std::vector<Function*>& program, Parser* parser, std::size_t parameters)
        : program(program), parser(parser), parameters(parameters)
    {
    }

    bool Optimizer::optimize_expression(Expression* expression)
    {
        using enum Expression::Type;
        switch (expression->etype())
        {
            case INTEGER_LITERAL:
            {
                //No further optimizations needed
                return false;
            } break;

            case IDENTIFIER:
            {
                auto identifier_expression = expression->as.identifier;
                if (identifier_expression->is_constexpr)
                {
                    auto symbol = parser->reference_symbol(SymbolType::VARIABLE, identifier_expression->name)->as.variable;
                    if (symbol->type == VariableSymbolType::CONSTANT)
                    {
                        delete expression;
                        auto new_expression = new IntegerLiteralExpression{ static_cast<ConstantSymbol*>(symbol)->value };
                        expression = new_expression;
                        return true;
                    }
                }

                return false;
            } break;

            case FUNCTION_CALL:
            {
                //TODO: implement determination if a function is constexpr
                //in that case, we can fold it down to a single integer literal node
                //for now, we don't have that functionality so just terminate further optimization of this node
                return false;
            } break;
            
            case BINARY_EXPRESSION:
            {
                auto binary_expression = expression->as.binary_expression;

                auto left = binary_expression->left;
                auto right = binary_expression->right;

                while (optimize_expression(left));
                while (optimize_expression(right));

                using enum Expression::Type;
                using enum BinaryExpression::Operator;

                bool made_changes = false;

                //Optimize useless math expressions
                {
                    if (binary_expression->op() == MULTIPLY)
                    {
                        if (left->type == INTEGER_LITERAL)
                        {
                            //0 * X
                            if (left->as.integer_literal->value == 0)
                            {
                                delete expression;
                                expression = nullptr;
                                made_changes = true;
                            }

                            //1 * X
                            else if (left->as.integer_literal->value == 1)
                            {
                                delete expression;
                                expression = right;
                                made_changes = true;
                            }
                        }

                        else if (right->type == INTEGER_LITERAL)
                        {
                            //X * 0
                            if (right->as.integer_literal->value == 0)
                            {
                                delete expression;
                                expression = nullptr;
                                made_changes = true;
                            }

                            //X * 1
                            else if (right->as.integer_literal->value == 1)
                            {
                                delete expression;
                                expression = left;
                                made_changes = true;
                            }
                        }
                    }

                    else if (binary_expression->type == PLUS ||
                             binary_expression->type == MINUS)
                    {
                        //0 ± X
                        if (left->type == INTEGER_LITERAL &&
                            left->as.integer_literal->value == 0)
                        {
                            delete expression;
                            expression = right;
                            made_changes = true;
                        }
                    }
                }

                //Fold constant expression
                {
                    auto left_evaluated = -1, right_evaluated = -1;

                    auto try_evaluate = [this](Expression* expression, int& evaluated)
                    {
                        if (expression->etype() == INTEGER_LITERAL)
                        {
                            evaluated = expression->as.integer_literal->value;
                        }

                        else if (expression->type == IDENTIFIER && expression->as.identifier->is_constexpr)
                        {
                            auto symbol = parser->reference_symbol(SymbolType::VARIABLE, expression->as.identifier->name);
                            if (symbol->as.variable->type == VariableSymbolType::CONSTANT)
                            {
                                evaluated = symbol->as.variable->as.constant->value;
                            }
                        }
                    };

                    try_evaluate(left, left_evaluated);

                    //Skip checking the RHS if the LHS could not be evaluated
                    if (left_evaluated != -1)
                    {
                        try_evaluate(right, right_evaluated);

                        //We can proceed with optimizations
                        if (right_evaluated != -1)
                        {
#ifndef NEW_EXPRESSION
#define NEW_EXPRESSION(x) do { delete expression; auto new_expression = new Expression{ ExpressionType::INTEGER_LITERAL, .as.integer_literal = new IntegerLiteral{ left_evaluated x right_evaluated } }; expression = new_expression; made_changes = true; } while (0)
                            using enum BinaryExpressionType;
                            switch (binary_expression->type)
                            {
                                case     PLUS: NEW_EXPRESSION(+); break;
                                case    MINUS: NEW_EXPRESSION(-); break;
                                case MULTIPLY: NEW_EXPRESSION(*); break;
                            }
#undef NEW_EXPRESSION
#endif
                        }
                    }
                }

                return made_changes;
            } break;
        }
    }

    bool Optimizer::optimize_statement(Statement* statement)
    {
        using enum StatementType;
        switch (statement->type)
        {
            case COMPOUND:
            {
                bool made_changes = false;

                auto compound_statement = statement->as.compound_statement;
                for (auto substatement : compound_statement->statements)
                {
                    made_changes += optimize_statement(substatement);
                }

                return made_changes;
            } break;

            case VARIABLE_DECLARATION:
            {
                auto variable_declaration_statement = statement->as.variable_declaration;
                auto variable_definition = variable_declaration_statement->expression;

                //Has the variable been defined or not?
                if (variable_definition != nullptr)
                {
                    if (optimize_expression(variable_definition))
                    {
                        return optimize_statement(statement);
                    }
                }

                return false;
            } break;

            case RETURN:
            {
                auto return_statement = statement->as.return_statement;
                return optimize_expression(return_statement->expression);
            } break;
        }
    }

    bool Optimizer::optimize_ast(std::vector<Function*>& functions)
    {
        for (auto function : functions)
        {
            if (optimize_statement(function->body))
            {
                return optimize_ast(functions);
            }
        }

        return false;
    }

    void Optimizer::optimize()
    {
        if (parameters & OPTIMIZE_AST)
        {
            for (auto pass = 0; optimize_ast(program); pass++)
            {
                Log::info(fmt::format("AST optimization pass {} successful", pass));
            }
        }
    }
}