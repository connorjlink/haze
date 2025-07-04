#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include <ast/IdentifierExpression.h>
#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/ParserType.h>
#include <toolchain/models/Token.h>

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

	class Parser
		: public ServiceTag<Parser>
		, public InjectSingleton<ErrorReporter, SymbolDatabase, SymbolExporter, CommandLineOptions>
	{
	protected:
		std::size_t cursor;
		std::vector<Token> _tokens;

	protected:
		std::string _filepath;

	protected:
		// explicitly mutable!
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
		void reload(const std::vector<Token>&, const std::string&);

	public:
		Parser(const std::string&);
		virtual ~Parser();
	};
}

#endif
