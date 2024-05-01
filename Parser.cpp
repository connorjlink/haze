#include "Parser.h"
#include "Log.h"
#include "IdentifierExpression.h"

#include <iostream>
#include <fmt/format.h>

namespace
{
    enum
    {
        TL_PROGRAM,
        TL_FUNCTION,
        TL_STATEMENT,
    };

    void ast_indent(int times)
    {
        for (auto i = 0; i < 2 * times; i++)
        {
            std::cout << ' ';
        }
    }

    void ast_print(const std::string& text, int indentation)
    {
        std::cout << "[AST] ";
        ::ast_indent(indentation);
        std::cout << text;
    }

    void ast_nl()
    {
        std::cout << std::endl;
    }

    bool is_binary_operator(hz::TokenType type)
    {
        return type == hz::TokenType::PLUS || type == hz::TokenType::STAR;
    }

    auto find(std::string_view name, std::vector<hz::Symbol>& symbols)
    {
        return std::find_if(symbols.begin(), symbols.end(), [&](auto symbol)
        {
            return symbol.name == name;
        });
    }
}

namespace hz
{
    void Parser::add_symbol(SymbolType type, std::string_view name)
    {
        if (query_symbol(name))
        {
            Log::error(fmt::format("symbol {} is multiply defined", name));
        }

        symbol_table.emplace_back(Symbol{ type, name });
    }

    bool Parser::query_symbol(std::string_view name)
    {
        return ::find(name, symbol_table) != std::end(symbol_table);
    }

    Symbol* Parser::reference_symbol(SymbolType type, std::string_view name)
    {
        auto symbol = ::find(name, symbol_table);
        if (symbol == std::end(symbol_table))
        {
            //TODO: ensure to output undefined symbol type in the ternary if more enum variants are added
            Log::error(fmt::format("{} {} is undefined", type == SymbolType::VARIABLE ? "variable" : "function", name));
        }

        if (symbol->type == type)
        {
            return &(*symbol);
        }

        Log::error(fmt::format("the specified symbol {} does not match the expected type", name));
    }

    const Token& Parser::peek() const
    {
        return tokens[cursor];
    }

    const Token& Parser::next() const
    {
        if (cursor < tokens.size())
        {
            return tokens[cursor + 1];
        }

        Log::error(fmt::format("({}) unexpectedly reached the end of file", peek().line));
    }

    std::string_view Parser::consume(const Token& token)
    {
        const auto& current = peek();
        if (current.type == token.type)
        {
            cursor++;
            return std::string_view{ current.value.value_or("") };
        }

        Log::error(fmt::format("({}) expected token '{}' but got '{}'", token.line, debug_tokens.at(token.type),
            ((current.type == TokenType::IDENTIFIER || current.type == TokenType::INT) ? current.value.value() : debug_tokens.at(current.type))));
    }

    Expression* Parser::parse_identifier_expression()
    {
        const auto name = consume(Token{ TokenType::IDENTIFIER });
        return new IdentifierExpression{ name };

        return new Expression{ ExpressionType::IDENTIFIER, .as.identifier = new Identifier{ name } };
    }

    Expression* Parser::parse_intlit_expression()
    {
        //TODO: optimize the C++ conversion to integer? (maybe dont require a copy)
        const auto value = std::stoi(std::string{ consume(Token{ TokenType::INT }) });
        return new Expression{ ExpressionType::INTEGER_LITERAL, .as.integer_literal = new IntegerLiteral{ value } };
    }

    Expression* Parser::parse_paren_expression()
    {
        (void)consume(Token{ TokenType::LPAREN });
        const auto expression = new_parse_expression();
        (void)consume(Token{ TokenType::RPAREN });
        return expression;
    }

    std::vector<Expression*> Parser::parse_function_call_arguments()
    {
        std::vector<Expression*> arguments;

        while (peek().type != TokenType::RPAREN)
        {
            arguments.emplace_back(new_parse_expression());

            if (peek().type != TokenType::RPAREN)
            {
                (void)consume(Token{ TokenType::COMMA });

                if (peek().type == TokenType::RPAREN)
                {
                    Log::error(fmt::format("({}) expected another argument", peek().line));
                }
            }
        }

        return arguments;
    }

    Expression* Parser::parse_function_call_expression()
    {
        const auto name = consume(Token{ TokenType::IDENTIFIER });

        (void)consume(Token{ TokenType::LPAREN });
        const auto arguments = parse_function_call_arguments();
        (void)consume(Token{ TokenType::RPAREN });

        return new Expression{ ExpressionType::FUNCTION_CALL, .as.function_call = new FunctionCall{ name, arguments } };
    }

    Expression* Parser::parse_expression()
    {
        Expression* expression;

        using enum TokenType;
        switch (peek().type)
        {
            case LPAREN:
            {
                expression = parse_paren_expression();
            } break;

            case INT:
            {
                expression = parse_intlit_expression();
            } break;

            case IDENTIFIER:
            {
                if (next().type == LPAREN)
                {
                    expression = parse_function_call_expression();
                    break;
                }

                expression = parse_identifier_expression();
            } break;

            default:
            {
                Log::error(fmt::format("({}) unexpected expression type", peek().line));
            } break;
        }

        return expression;
    }

    Expression* Parser::new_parse_primary()
    {
        return parse_expression();
    }

    Expression* Parser::new_parse_expression()
    {
        return new_parse_expression1(new_parse_primary(), PRECEDENCE_MIN);
    }

    Expression* Parser::new_parse_expression1(Expression* left, Precedence min_precedence)
    {
        static const std::unordered_map<TokenType, Precedence> precedences =
        {
            { TokenType::PLUS, PRECEDENCE_TERM },
            { TokenType::STAR, PRECEDENCE_FACTOR },
        };

        static const std::unordered_map<TokenType, BinaryExpressionType> binary_expression_types =
        {
            { TokenType::PLUS, BinaryExpressionType::PLUS },
            { TokenType::STAR, BinaryExpressionType::MULTIPLY },
        };

        do
        {
            const auto& next = peek();

            if (!is_binary_operator(next.type) || precedences.at(next.type) < min_precedence)
            {
                break;
            }

            consume(Token{ next.type });

            auto right = new_parse_primary();

            do
            {
                const auto& lookahead = peek();

                if (!is_binary_operator(lookahead.type) || precedences.at(lookahead.type) <= precedences.at(next.type))
                {
                    break;
                }

                //TODO: maybe add precedence+1 for the next call of the recursive function
                right = new_parse_expression1(right, precedences.at(lookahead.type));

            } while(true);

            left = new Expression{ ExpressionType::BINARY_EXPRESSION, .as.binary_expression = new BinaryExpression{ binary_expression_types.at(next.type), left, right } };
        } while (true);

        return left;
    }



    Statement* Parser::parse_null_statement()
    {
        (void)consume(Token{ TokenType::SEMICOLON });
        return nullptr;
    }

    Statement* Parser::parse_vardecl_statement()
    {
        //TODO: add other type specifiers?
        (void)consume(Token{ TokenType::BYTE });

        const auto name = consume(Token{ TokenType::IDENTIFIER });
        symbol_table.emplace_back(Symbol{ SymbolType::VARIABLE, .as.variable = new VariableSymbol{ VariableSymbolType::NORMAL } });

        if (peek().type == TokenType::EQUALS)
        {
            (void)consume(Token{ TokenType::EQUALS });
            const auto value = new_parse_expression();
            (void)consume(Token{ TokenType::SEMICOLON });

            return new Statement{ StatementType::VARIABLE_DECLARATION, .as.variable_declaration = new VariableDeclaration{ name, value } };
        }

        (void)consume(Token{ TokenType::SEMICOLON });

        return new Statement{ StatementType::VARIABLE_DECLARATION, .as.variable_declaration = nullptr };
    }

    Statement* Parser::parse_compound_statement()
    {
        (void)consume(Token{ TokenType::LBRACE });
        const auto statements = parse_statements();
        (void)consume(Token{ TokenType::RBRACE });

        return new Statement{ StatementType::COMPOUND, .as.compound_statement = new Compound{ statements } };
    }

    Statement* Parser::parse_return_statement()
    {
        (void)consume(Token{ TokenType::RETURN });
        const auto expression = new_parse_expression();
        (void)consume(Token{ TokenType::SEMICOLON });

        return new Statement{ StatementType::RETURN, .as.return_statement = new Return{ expression } };
    }

    Statement* Parser::parse_statement()
    {
        using enum TokenType;
        switch (peek().type)
        {
            case    LBRACE: return parse_compound_statement();
            case      BYTE: return parse_vardecl_statement();
            case SEMICOLON: return parse_null_statement();
            case    RETURN: return parse_return_statement();

            default:
            {
                Log::error(fmt::format("({}) unexpected statement type", peek().line));
            } break;
        }
    }

    std::vector<Statement*> Parser::parse_statements()
    {
        std::vector<Statement*> statements;

        while (peek().type != TokenType::RBRACE)
        {
            statements.emplace_back(parse_statement());
        }

        return statements;
    }

    Argument Parser::parse_argument()
    {
        //TODO: add other type specifiers for arguments?
        (void)consume(Token{ TokenType::BYTE });
        const auto name = consume(Token{ TokenType::IDENTIFIER });
        add_symbol(SymbolType::ARGUMENT, name);

        return Argument{ name };
    }

    std::vector<Argument> Parser::parse_arguments()
    {
        std::vector<Argument> arguments;

        while (peek().type != TokenType::RPAREN)
        {
            arguments.emplace_back(parse_argument());

            if (peek().type != TokenType::RPAREN)
            {
                (void)consume(Token{ TokenType::COMMA });

                if (peek().type == TokenType::RPAREN)
                {
                    Log::error(fmt::format("({}) expected another argument but got ')'", peek().line));
                }
            }
        }

        return arguments;
    }

    Function* Parser::parse_function()
    {
        (void)consume(Token{ TokenType::FUNCTION });
        //TODO: support return types other than byte (particularly void functions)
        (void)consume(Token{ TokenType::BYTE });

        const auto name = consume(Token{ TokenType::IDENTIFIER });
        add_symbol(SymbolType::FUNCTION, name);

        (void)consume(Token{ TokenType::EQUALS });

        (void)consume(Token{ TokenType::LPAREN });
        const auto arguments = parse_arguments();
        (void)consume(Token{ TokenType::RPAREN });

        const auto body = parse_compound_statement();

        return new Function{ name, arguments, body };
    }

    std::vector<Function*> Parser::parse_functions()
    {
        std::vector<Function*> functions;

        while (peek().type != TokenType::END)
        {
            functions.emplace_back(parse_function());
        }

        return functions;
    }

    void Parser::parse_program()
    {
        program = parse_functions();

        if (std::find_if(program.begin(), program.end(), [&](auto function)
            {
                return (function->name == "main");
            }) == std::end(program))
        {
            Log::error("no main() function was defined");
        }
    }

    void Parser::parse()
    {
        parse_program();
    }

    std::string Parser::format_expression(Expression* expression)
    {
        std::string result(1 << 5, '\0');

        if (expression != nullptr)
        {
            using enum ExpressionType;
            switch (expression->type)
            {
                case INTEGER_LITERAL:
                {
                    result = fmt::format("integer literal ({})", expression->as.integer_literal->value);
                } break;

                case IDENTIFIER:
                {
                    result = fmt::format("identifier ({})", expression->as.identifier->name);
                } break;

                case FUNCTION_CALL:
                {
                    result = fmt::format("function call ({})", expression->as.function_call->name);
                } break;

                case BINARY_EXPRESSION:
                {
                    char op;

                    using enum BinaryExpressionType;
                    switch (expression->as.binary_expression->type)
                    {
                        case     PLUS: op = '+'; break;
                        case    MINUS: op = '-'; break;
                        case MULTIPLY: op = '*'; break;
                    }

                    result = fmt::format("binary expression ({} {} {})",
                        format_expression(expression->as.binary_expression->left), op, format_expression(expression->as.binary_expression->right));
                } break;
            }
        }

        return result;
    }

    void Parser::print_expression(Expression* expression)
    {
        std::cout << format_expression(expression);
    }

    void Parser::print_statement(Statement const* statement, int indent)
    {
        ::ast_print("Statement: ", indent);

        using enum Statement::Type;
        switch (statement->stype())
        {
            case COMPOUND:
            {
                const auto compound_statement = statement->as.compound_statement;

                std::cout << "compound:";
                ::ast_nl();

                for (const auto statement : compound_statement->statements)
                {
                    print_statement(statement, indent + 2);
                }
            } break;

            case VARIABLE_DECLARATION:
            {
                const auto variable_declaration = statement->as.variable_declaration;

                std::cout << fmt::format("variable declaration ({} = {})",
                    variable_declaration->name, format_expression(variable_declaration->expression));

                ::ast_nl();
            } break;

            case RETURN:
            {
                const auto return_statement = statement->as.return_statement;

                std::cout << fmt::format("return ({})", format_expression(return_statement->expression));

                ::ast_nl();
            }
        }
    }

    void Parser::print_function(Function* function)
    {
        ::ast_print("Function: ", TL_FUNCTION);

        std::cout << fmt::format(" ({})\n", fmt::join(function->arguments, ", "));

        print_statement(function->get_body(), TL_STATEMENT);
    }

    void Parser::print_program()
    {
        if (!program.empty())
        {
            ::ast_print("Program: ", TL_PROGRAM);
            ::ast_nl();

            for (const auto function : program)
            {
                print_function(function);
            }
        }
    }

    void Parser::print_ast()
    {
        print_program();
    }
}