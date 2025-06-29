#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include "Token.h"
#include "ParserType.h"
#include "IdentifierExpression.h"
#include "DependencyInjector.h"
#include "ErrorReporter.h"

// Haze Parser.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Symbol;
	class FunctionSymbol;
	class ArgumentSymbol;
	class VariableSymbol; 
	class StructSymbol;
	class DefineSymbol;
	class LabelSymbol;

	class Node;
	class Expression;
	class IdentifierExpression;
	class IntegerLiteralExpression;
	class StringExpression;
	class FunctionCallExpression;
	class AdjustExpression;
	class Type;

	enum class SymbolType;

	class Parser
		: public InjectSingleton<ErrorReporter>
	{
	protected:
		std::size_t cursor;
		std::vector<Token> tokens;

	protected:
		const std::string& _filepath;

	protected:
		Token& lookbehind();
		Token& peek();
		Token& lookahead();

	protected:
		Token consume(TokenType);
		std::vector<Token> fetch_until(TokenType);

	protected:
		enum class Precedence
		{
			MINIMUM,
			ASSIGN,
			EQUALITY,
			COMPARE,
			TERM, // + (or - later)
			FACTOR, // * (or / later maybe)
		};

	protected:
		Node* parse_dotdefine_command();

	protected:
		IdentifierExpression* parse_identifier_expression(IdentifierType = IdentifierType::UNKNOWN); // explicit contextual type override
		IntegerLiteralExpression* parse_integerliteral_expression();
		StringExpression* parse_string_expression();
		FunctionCallExpression* parse_functioncall_expression();
		Expression* parse_parenthesis_expression();

	protected:
		AdjustExpression* parse_increment_expression();
		AdjustExpression* parse_decrement_expression();

	protected:
		Expression* parse_expression_optimized();
		Expression* parse_expression();

	private:
		Expression* parse_generic_expression();
		Expression* parse_infix_expression(Expression*, Precedence);

	public:
		virtual ParserType ptype() const = 0;
		virtual std::vector<Node*> parse() = 0;

	public:
		Parser(const std::vector<Token>&, const std::string&);
		virtual ~Parser();
	};

	extern Parser* _parser;
}

#endif
