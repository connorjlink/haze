#ifndef HAZE_COMPILERPARSER_H
#define HAZE_COMPILERPARSER_H

#include "Parser.h"
#include "TypeSpecifier.h"

// Haze CompilerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class Statement;

    class CompilerParser : public Parser
    {
    private:
        Statement* parse_statement(std::string);
        std::vector<Statement*> parse_statements(std::string);

    protected:
        Statement* parse_null_statement(std::string);
        Statement* parse_variabledeclaration_statement(std::string);
        Statement* parse_compound_statement(std::string);
        Statement* parse_return_statement(std::string);
        Statement* parse_inline_asm_statement(std::string);
        Statement* parse_while_statement(std::string);
        Statement* parse_for_statement(std::string);
        Statement* parse_if_statement(std::string);
        Statement* parse_expression_statement(std::string);

    // interpreter-specific functionality
    // but it needs to interface with statements, so we include it in the compiler as well
    private:
        Statement* parse_print_statement(std::string);
        Statement* parse_hook_statement(std::string);
        Statement* parse_unhook_statement(std::string);

    public:
        TypeSpecifier parse_type_specifier();
        std::vector<Expression*> parse_arguments(bool);

    public:
        Node* parse_function();
        std::vector<Node*> parse_functions();

    public:
        virtual std::vector<Node*> parse() override;

    public:
        using Parser::Parser;
    };
}

#endif
