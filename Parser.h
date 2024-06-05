#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include "Token.h"
#include "Symbol.h"

#include "Expression.h"
#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "FunctionCallExpression.h"

#include "Statement.h"

#include "Node.h"
#include "Function.h"
#include "Instruction.h"

#define AS_COMPILER_PARSER(x) static_cast<CompilerParser*>(x)
#define AS_ASSEMBLER_PARSER(x) static_cast<AssemblerParser*>(x)

#define AS_FUNCTION(x) static_cast<Function*>(x)
#define AS_INSTRUCTION(x) static_cast<Instruction*>(x)

namespace hz
{
    class Parser
    {
    private:
        std::size_t cursor;
        std::vector<Token> tokens;

    private:
        std::vector<Symbol*> symbol_table;

    protected:
        void add_symbol(Symbol::Type, std::string);

    public:
        bool query_symbol(std::string);
        Symbol* reference_symbol(Symbol::Type, std::string, bool = false);

    protected:
        const Token& peek() const;
        const Token& lookahead() const;
        std::string consume(TokenType);
        std::vector<Token> fetchUntil(TokenType) const;

    protected:
        enum class Precedence
        {
            MINIMUM,
            TERM, // + (or - later)
            FACTOR, // * (or / later maybe)
        };

    protected:
        IdentifierExpression* parse_identifier_expression();
        IntegerLiteralExpression* parse_integerliteral_expression();
        FunctionCallExpression* parse_functioncall_expression();
        Expression* parse_parenthesis_expression();

    protected:
        Expression* parse_expression();

    private:
        Expression* parse_generic_expression();
        Expression* parse_infix_expression(Expression*, Precedence);

    public:
        virtual std::vector<Node*> parse() = 0;

    public:
        explicit Parser(std::vector<Token>&& tokens)
            : cursor{ 0 }, tokens{ std::move(tokens) }
        {
        }
    };

    class CompilerParser : public Parser
    {
    private:
        Statement* parse_statement(std::string);
        std::vector<Statement*> parse_statements(std::string);

    private:
        Statement* parse_null_statement(std::string);
        Statement* parse_variabledeclaration_statement(std::string);
        Statement* parse_compound_statement(std::string);
        Statement* parse_return_statement(std::string);
        Statement* parse_asm_statement(std::string);

    public:
        Expression* parse_argument();
        std::vector<Expression*> parse_arguments(bool);

    public:
        Node* parse_function();
        std::vector<Node*> parse_functions();

    public:
        virtual std::vector<Node*> parse() final override;

    public:
        using Parser::Parser;
    };

    class AssemblerParser : public Parser
    {
    private:
        Node* parse_instruction();
        std::vector<Node*> parse_instructions();

    public:
        virtual std::vector<Node*> parse() final override;

    public:
        using Parser::Parser;
    };

    extern Parser* parser;
}

#endif //HAZE_PARSER_H
