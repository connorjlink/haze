#include "Parser.h"
#include "Log.h"
#include "Token.h"
#include "Allocator.h"
#include "Utility.h"

#include "CompilerParser.h"

#include "CompoundStatement.h"
#include "VariableStatement.h"
#include "ReturnStatement.h"

#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"
#include "FunctionCallExpression.h"
#include "BinaryExpression.h"

#include "DotDefineCommand.h"

#include <iostream>
#include <format>

#define ASSERT_IS_INTEGER_LITERAL(x) if (x->etype() != ExpressionType::INTEGER_LITERAL) Log::error("term must evaluate to a constant expression")

namespace
{
	enum
	{
		TL_PROGRAM,
		TL_FUNCTION,
		TL_STATEMENT,
	};

	bool is_binary_operator(hz::TokenType type)
	{
		return hz::_binary_expression_map.contains(type);
	}
}

namespace hz
{
	void Parser::add_symbol(SymbolType type, std::string name, Token& location)
	{
		// does the symbol already exist in the registry?
		if (symbol_table.contains(name))
		{
			_error_reporter->post_error(std::format("symbol `{}` was already defined as a {}",
				name, _symbol_map.at(symbol_table.at(name)->ytype())), location);
			return;
		}

		using enum SymbolType;
		switch (type)
		{
			case FUNCTION: symbol_table[name] = new FunctionSymbol{ name }; break;
			case ARGUMENT: symbol_table[name] = new ArgumentSymbol{ name }; break;
			case VARIABLE: symbol_table[name] = new VariableSymbol{ name, nullptr }; break;
			case DEFINE: symbol_table[name] = new DefineSymbol{ name, 0 }; break;
			case LABEL: symbol_table[name] = new LabelSymbol{ name, 0 }; break;

			default:
			{
				_error_reporter->post_error(std::format("invalid symbol type `{}`", location.value), location);
			} break;
		}
	}


	SymbolType Parser::query_symbol_type(std::string name, Token& location)
	{
		if (!symbol_table.contains(name))
		{
			_error_reporter->post_error(std::format("symbol `{}` is undefined", name), location);
			return SymbolType::VARIABLE;
		}

		return symbol_table.at(name)->ytype();
	}

	Symbol* Parser::reference_symbol(SymbolType type, std::string name, Token& location, bool mark_visited)
	{
		if (!_symbol_map.contains(type))
		{
			_error_reporter->post_error(std::format("invalid symbol type `{}`", location.value), location);
			return nullptr;
		}

		if (!symbol_table.contains(name))
		{
			_error_reporter->post_error(std::format("symbol `{}` is undefined", name), location);
			return nullptr;
		}

		auto symbol = symbol_table.at(name);

		if (symbol->ytype() != type)
		{
			_error_reporter->post_error(std::format("symbol `{}` was defined as a {} but referenced as a {}",
				name, _symbol_map.at(symbol->ytype()), _symbol_map.at(type)), location);
			return nullptr;
		}

		if (mark_visited)
		{
			symbol->was_referenced = true;
		}

		return symbol;
	}

	Token& Parser::lookbehind()
	{
		if (cursor > 0)
		{
			return tokens[cursor - 1];
		}

		_error_reporter->post_uncorrectable("invalid token backtrack", peek());
	}

	Token& Parser::peek()
	{
		return tokens[cursor];
	}

	Token& Parser::lookahead()
	{
		if (cursor < tokens.size() - 1)
		{
			return tokens[cursor + 1];
		}

		_error_reporter->post_uncorrectable("unexpectedly reached the end of file", peek());
	}

	std::string Parser::consume(TokenType token)
	{
		const auto& current = peek();
		if (current.type == token)
		{
			cursor++;
			return current.value;
		}

		auto convert = [&](auto v)
		{
			const auto item = _token_map.at(v);

			if (item)
			{
				return *item;
			}

			_error_reporter->post_error("invalid token", current);
			return std::string{ "[error]" };
		};

		_error_reporter->post_error(std::format("expected token `{}` but got `{}`", 
			convert(token), ((current.type == TokenType::IDENTIFIER || current.type == TokenType::INT) ? current.value : convert(current.type))), current);
		return current.value;
	}

	std::vector<Token> Parser::fetch_until(TokenType type)
	{
		std::vector<Token> tokens;

		while (peek().type != type)
		{
			tokens.emplace_back(peek());
			consume(peek().type);
		}

		tokens.emplace_back(TokenType::END, peek().offset, "eof");

		return tokens;
	}

	Node* Parser::parse_dotdefine_command()
	{
		DISCARD consume(TokenType::DOTDEFINE);
		const auto identifier_expresion = parse_identifier_expression();
		DISCARD consume(TokenType::EQUALS);

		const auto value_expression = parse_expression();
		ASSERT_IS_INTEGER_LITERAL(value_expression);

		const auto identifier = identifier_expresion->name;
		const auto value = AS_INTEGER_LITERAL_EXPRESSION(value_expression)->value;

		add_symbol(SymbolType::DEFINE, identifier, peek());
		AS_DEFINE_SYMBOL(reference_symbol(SymbolType::DEFINE, identifier, peek()))->value = value;

		return new DotDefineCommand{ identifier, { value } };
	}

	IdentifierExpression* Parser::parse_identifier_expression()
	{
		const auto name = consume(TokenType::IDENTIFIER);
		return new IdentifierExpression{ name };
	}

	IntegerLiteralExpression* Parser::parse_integerliteral_expression()
	{
#pragma message("TODO: replace stoi with <charconv>")
		return new IntegerLiteralExpression{ std::stoul(consume(TokenType::INT)) };
	}

	StringExpression* Parser::parse_string_expression()
	{
		const auto message = consume(TokenType::STRING);
		return new StringExpression{ std::move(message) };
	}

	FunctionCallExpression* Parser::parse_functioncall_expression()
	{
		const auto name = consume(TokenType::IDENTIFIER);

		DISCARD consume(TokenType::LPAREN);
		// TODO: verify that we are a compiler or interpreter parser before doing this!
		auto arguments = AS_COMPILER_PARSER(this)->parse_arguments(false);
		DISCARD consume(TokenType::RPAREN);

		return new FunctionCallExpression{ name, std::move(arguments) };
	}

	Expression* Parser::parse_parenthesis_expression()
	{
		DISCARD consume(TokenType::LPAREN);
		const auto expression = parse_expression();
		DISCARD consume(TokenType::RPAREN);
		return expression;
	}

	AdjustExpression* Parser::parse_increment_expression()
	{
		DISCARD consume(TokenType::TILDE);
		
		if (peek().type == TokenType::IDENTIFIER)
		{
			auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ true, identifier_expression };
		}

		else if (peek().type == TokenType::INT)
		{
			auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ true, integer_literal_expression };
		}

		Log::error("Increment expresion target must be an integer literal or identifier");
	}

	AdjustExpression* Parser::parse_decrement_expression()
	{
		DISCARD consume(TokenType::EXCLAMATION);

		if (peek().type == TokenType::IDENTIFIER)
		{
			auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ false, identifier_expression };
		}

		else if (peek().type == TokenType::INT)
		{
			auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ false, integer_literal_expression };
		}

		Log::error("Decrement expresion target must be an integer literal or identifier");
	}


	Expression* Parser::parse_generic_expression()
	{
		Expression* expression;

		using enum TokenType;
		switch (peek().type)
		{
			case LPAREN:
			{
				expression = parse_parenthesis_expression();
			} break;

			case INT:
			{
				expression = parse_integerliteral_expression();
			} break;
			
			case STRING:
			{
				expression = parse_string_expression();
			} break;

			case IDENTIFIER:
			{
				if (lookahead().type == LPAREN)
				{
					expression = parse_functioncall_expression();
					break;
				}

				expression = parse_identifier_expression();
			} break;

			case TILDE:
			{
				expression = parse_increment_expression();
			} break;

			case EXCLAMATION:
			{
				expression = parse_decrement_expression();
			} break;

			default:
			{
				Log::error(std::format("({}) unexpected expression type", peek().offset));
			} break;
		}

		return expression;
	}

	Expression* Parser::parse_expression_optimized()
	{
		auto expression = parse_expression();

		while (auto expression_optimized = expression->optimize())
		{
			expression = AS_EXPRESSION(expression_optimized);
		}

		return expression;
	}

	Expression* Parser::parse_expression()
	{
		return AS_EXPRESSION(parse_infix_expression(parse_generic_expression(), Precedence::MINIMUM)->copy());
	}

	Expression* Parser::parse_infix_expression(Expression* left, Precedence min_precedence)
	{
		static const std::unordered_map<TokenType, Precedence> precedences
		{
			{ TokenType::EQUALS,            Precedence::ASSIGN },
			{ TokenType::EQUALSEQUALS,      Precedence::EQUALITY },
			{ TokenType::EXCLAMATIONEQUALS, Precedence::EQUALITY },
			{ TokenType::GREATER,           Precedence::COMPARE },
			{ TokenType::LESS,              Precedence::COMPARE },
			{ TokenType::PLUS,              Precedence::TERM },
			{ TokenType::MINUS,             Precedence::TERM },
			{ TokenType::STAR,              Precedence::FACTOR },
		};

		do
		{
			const auto& next = peek();

			if (!is_binary_operator(next.type) || precedences.at(next.type) < min_precedence)
			{
				break;
			}

			consume(next.type);

			auto right = parse_expression();

			do
			{
				const auto& lookahead = peek();

				if (!is_binary_operator(lookahead.type) || precedences.at(lookahead.type) <= precedences.at(next.type))
				{
					break;
				}

				//TODO: maybe add precedence+1 for the next call of the recursive function
				right = parse_infix_expression(right, precedences.at(lookahead.type));

			} while(true);

			switch (next.type)
			{
				case TokenType::PLUS: left = new PlusBinaryExpression{ left, right }; break;
				case TokenType::MINUS: left = new MinusBinaryExpression{ left, right }; break;
				case TokenType::STAR: left = new TimesBinaryExpression{ left, right }; break;

				case TokenType::EQUALS: left = new AssignBinaryExpression{ left, right }; break;

				case TokenType::EQUALSEQUALS: left = new EqualityBinaryExpression{ left, right }; break;
				case TokenType::EXCLAMATIONEQUALS: left = new InequalityBinaryExpression{ left, right }; break;

				case TokenType::GREATER: left = new GreaterBinaryExpression{ left, right }; break;
				case TokenType::LESS: left = new LessBinaryExpression{ left, right }; break;
			}
		} while (true);

		return left;
	}
}