#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <string_view>

#include "Token.h"
#include "Symbol.h"
#include "Expression.h"
#include "Statement.h"
#include "Function.h"
#include "FunctionCallExpression.h"
#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"

namespace hz
{
    class Result;
    class Function : public Result;
    class Instruction : public Result;

    class Parser
    {
    private:
        std::size_t cursor;
        std::vector<Token> tokens;

    private:
        std::vector<Symbol*> symbol_table;
        void add_symbol(Symbol::Type, std::string_view);

    public:
        bool query_symbol(std::string_view);
        Symbol* reference_symbol(Symbol::Type, std::string_view);

    private:
        const Token& peek() const;
        const Token& lookahead() const;
        //void backtrack();
        std::string_view consume(const TokenType&);

    private:
        enum class Precedence
        {
            MINIMUM,
            TERM, // + (or - later)
            FACTOR, // * (or / later maybe)
        };

    protected:
        IdentifierExpression* parse_identifier_expression();
        IntegerLiteralExpression* parse_integerliteral_expression();
        Expression* parse_parenthesis_expression();

    protected:
        Expression* parse_expression();

    private:
        Expression* parse_generic_expression();
        Expression* parse_infix_expression(Expression*, Precedence);

    public:
        virtual std::vector<Result*> parse() = 0;

    public:
        Parser(std::vector<Token>&& tokens)
            : tokens(std::move(tokens))
        {
        }
    };

    class CompilerParser : public Parser
    {
    private:
        Statement* parse_statement();
        std::vector<Statement*> parse_statements();

    private:
        Statement* parse_null_statement();
        Statement* parse_variabledeclaration_statement();
        Statement* parse_compound_statement();
        Statement* parse_return_statement();

    private:
        Expression* parse_argument();
        std::vector<Expression*> parse_arguments();

    private:
        Function* parse_function();
        std::vector<Function*> parse_functions();

    public:
        std::vector<Function*> parse() final override
        {
            return parse_functions();
        }

    public:
        using Parser::Parser;
    };

    class AssemblyParser : public Parser
    {
    private:
        Instruction* parse_instruction();
        std::vector<Instruction*> parse_instructions();

    public:
        std::vector<Instruction*> parse() final override
        {
            return parse_instructions();
        }

    public:
        using Parser::Parser;
    };



    class Parser
    {
    private:
        const std::vector<Token>& tokens;
        std::size_t cursor = 0;
        std::vector<Function*> program;

    private:
        std::vector<Symbol> symbol_table;
        void add_symbol(Symbol::Type, std::string_view);

    public:
        bool query_symbol(std::string_view);
        Symbol* reference_symbol(Symbol::Type, std::string_view);

    public:
        explicit Parser(const std::vector<Token>& tokens)
            : tokens(tokens)
        {
        }

    public:
        std::vector<Function*>& get_ast()
        {
            return program;
        }

    private:
        const Token& peek() const;
        const Token& next() const;
        std::string_view consume(const TokenType&);

    private:
        enum Precedence
        {
            PRECEDENCE_MIN,
            PRECEDENCE_TERM, // + (- later)
            PRECEDENCE_FACTOR, // * (/ later)
        };

    private:
        Expression* parse_identifier_expression();
        Expression* parse_intlit_expression();
        Expression* parse_paren_expression();
        std::vector<Expression*> parse_function_call_arguments();
        Expression* parse_function_call_expression();

        Expression* new_parse_primary();
        Expression* new_parse_expression();
        Expression* new_parse_expression1(Expression*, Precedence);


        Expression* parse_expression();

        Statement* parse_null_statement();
        Statement* parse_vardecl_statement();
        Statement* parse_compound_statement();
        Statement* parse_return_statement();

        Statement* parse_statement();
        std::vector<Statement*> parse_statements();

        Expression* parse_argument();
        std::vector<Expression*> parse_arguments();

        Function* parse_function();
        std::vector<Function*> parse_functions();

        void parse_program();

        friend class Optimizer;

    private:
        std::string format_expression(Expression*);

    private:
        void print_expression(Expression*);
        void print_statement(Statement*, int);
        void print_function(Function*);
        void print_program();

    public:
        void print_ast();
        void parse();
    };

}

#endif //HAZE_PARSER_H