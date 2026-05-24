#ifndef HAZE_COMPILERPARSER_H
#define HAZE_COMPILERPARSER_H

#include <ast/expression/Expression.h>
#include <ast/function/Function.h>
#include <ast/statement/Statement.h>
#include <ast/declaration/Declaration.h>
#include <toolchain/Parser.h>
#include <type/Type.h>

// Haze CompilerParser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CompilerParser : public Parser
	{
	public:
		std::unordered_map<std::string, std::string> function_label_map;

	private:
		StatementHandle parse_statement(const std::string&);
		std::vector<StatementHandle> parse_statements(const std::string&);

	protected:
		DeclarationHandle parse_declaration(const std::string&);

	protected:
		StatementReference<NullStatement> parse_null_statement(const std::string&);
		StatementReference<ExpressionStatement> parse_expression_statement(const std::string&);
		StatementReference<ReturnStatement> parse_return_statement(const std::string&);
		StatementReference<IfStatement> parse_if_statement(const std::string&);
		StatementReference<WhileStatement> parse_while_statement(const std::string&);
		StatementReference<DoStatement> parse_do_while_statement(const std::string&);
		StatementReference<ForStatement> parse_for_statement(const std::string&);
		StatementReference<GotoStatement> parse_goto_statement(const std::string&);
		StatementReference<ContinueStatement> parse_continue_statement(const std::string&);
		StatementReference<BreakStatement> parse_break_statement(const std::string&);
		StatementReference<SwitchStatement> parse_switch_statement(const std::string&);
		StatementReference<CompoundStatemnet> parse_compound_statement(const std::string&);
		StatementReference<LabeledStatement> parse_labeled_statement(const std::string&);
		StatementReference<DeclarationStatement> parse_declaration_statement(const std::string&);

		Parser* createassembler_parser(const std::string&);
		StatementReference<InlineAssemblyStatement> parse_inline_asm_statement(const std::string&);

	private:
		MemberDeclarationExpression* parse_member_declaration_statement(const std::string&);
		std::vector<MemberDeclarationExpression*> parse_member_declaration_statements(const std::string&);

	public:
		std::vector<Expression*> parse_arguments(bool);

	public:
		Type* parse_type();

	public:
		TypeQualifier parse_type_qualifier(bool = false);
		TypeSignedness parse_type_signedness(bool = false);
		StorageClass parse_storage_class(bool = false);
		IntKind parse_int_kind(bool = false);

	public:
		FunctionHandle parse_function();
		std::vector<FunctionHandle> parse_functions();

	public:
		virtual ParserType ptype() const override;
		virtual std::vector<Node*> parse() override;

	public:
		using Parser::Parser;
	};
}

#endif
