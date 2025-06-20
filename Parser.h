#ifndef HAZE_PARSER_H
#define HAZE_PARSER_H

#include "Token.h"
#include "ParserType.h"

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
	{
	protected:
		std::size_t cursor;
		std::vector<Token> tokens;

	protected:
		const std::string& _filepath;

	private:
		std::unordered_map<std::string, Symbol*> symbol_table;

	protected:
		void add_symbol(SymbolType, const std::string&, const Token&);
		void add_function(const std::string&, const Token&, Type*);
		void add_argument(const std::string&, const Token&);
		void add_variable(const std::string&, const Token&);
		void add_define(const std::string&, const Token&);
		void add_label(const std::string&, const Token&);
		void add_struct(const std::string&, const Token&);

	public:
		SymbolType query_symbol_type(const std::string&, const Token&);

	public:
		Symbol* reference_symbol(SymbolType, const std::string&, const Token&, bool = false);
		FunctionSymbol* reference_function(const std::string&, const Token&, bool = false);
		ArgumentSymbol* reference_argument(const std::string&, const Token&, bool = false);
		VariableSymbol* reference_variable(const std::string&, const Token&, bool = false);
		DefineSymbol* reference_define(const std::string&, const Token&, bool = false);
		LabelSymbol* reference_label(const std::string&, const Token&, bool = false);
		StructSymbol* reference_struct(const std::string&, const Token&, bool = false);

	public:
		bool has_symbol(const std::string&);

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
		IdentifierExpression* parse_identifier_expression();
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
		IdentifierExpression* parse_identifier();

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
