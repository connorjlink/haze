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
    public:
        std::unordered_map<std::string, std::string> _function_label_map;

    private:
        Statement* parse_statement(const std::string&);
        std::vector<Statement*> parse_statements(const std::string&);

    protected:
        Statement* parse_null_statement(const std::string&);
        Statement* parse_variabledeclaration_statement(const std::string&);
        Statement* parse_compound_statement(const std::string&);
        Statement* parse_return_statement(const std::string&);
        Statement* parse_inline_asm_statement(const std::string&);
        Statement* parse_while_statement(const std::string&);
        Statement* parse_for_statement(const std::string&);
        Statement* parse_if_statement(const std::string&);
        Statement* parse_expression_statement(const std::string&);

    private:
        Expression* parse_member_declaration_statement(const std::string&);
        std::vector<Expression*> parse_member_declaration_statements();

    public:
        Statement* parse_struct_declaration_statement(const std::string&);

    // interpreter-specific functionality
    // but it needs to interface with statements, so we include it in the compiler as well
    private:
        Statement* parse_print_statement(const std::string&);
        Statement* parse_hook_statement(const std::string&);
        Statement* parse_unhook_statement(const std::string&);

    public:
        std::vector<Expression*> parse_arguments(bool);

    public:
        Type* parse_type();

    public:
        TypeQualifier parse_type_qualifier(bool = false);
        TypeSignedness parse_type_signedness(bool = false);
        TypeSpecifier parse_type_specifier(bool = false);
        TypeStorage parse_type_storage(bool = false);

    public:
        Node* parse_function();
        std::vector<Node*> parse_functions();

    public:
        virtual ParserType ptype() const override;
        virtual std::vector<Node*> parse() override;

    public:
        using Parser::Parser;
    };
}

#endif
