#ifndef HAZE_INTERPRETERPARSER_H
#define HAZE_INTERPRETERPARSER_H

#include "CompilerParser.h"

#include "IntrinsicDeclarator.h"
#include "FunctionDeclarator.h"
#include "StatementDeclarator.h"
#include "ExpressionDeclarator.h"

namespace hz
{
	class InterpreterParser : public CompilerParser
	{
	private:
		IntrinsicDeclarator* parse_intrinsic_declarator();
		FunctionDeclarator* parse_function_declarator();
		StatementDeclarator* parse_statement_declarator();
		ExpressionDeclarator* parse_expression_declarator();

	private:
		Node* parse_declarator();
		std::vector<Node*> parse_declarators();

	public:
		virtual std::vector<Node*> parse() final override;

	public:
		using CompilerParser::CompilerParser;
	};
}

#endif
