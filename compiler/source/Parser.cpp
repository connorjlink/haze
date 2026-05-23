import std;

#include <ast/AdjustExpression.h>
#include <ast/BinaryExpression.h>
#include <ast/IntegerLiteralExpression.h>
#include <ast/FunctionCallExpression.h>
#include <ast/StringExpression.h>
#include <command/DotDefineCommand.h>
#include <error/CommonErrors.h>
#include <error/ErrorReporter.h>
#include <symbol/Symbol.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/Parser.h>
#include <toolchain/CompilerParser.h>
#include <type/Type.h>

// Haze Parser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	using namespace hz;

	bool is_binary_operator(TokenKind type)
	{
		return _binary_expression_map.contains(type);
	}
}

namespace hz
{
	Parser::Parser(const std::string& filepath)
		: cursor{ 0 }, _tokens{ _tokens }, filepath{ filepath }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "parsing");
	}

	Parser::~Parser()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}


	Token& Parser::lookbehind()
	{
		if (cursor > 0)
		{
			return _tokens[cursor - 1];
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(
			"invalid token backtrack", peek());
	}

	Token& Parser::peek()
	{
		return _tokens[cursor];
	}

	Token& Parser::lookahead()
	{
		if (cursor < _tokens.size() - 1)
		{
			return _tokens[cursor + 1];
		}

		USE_SAFE(ErrorReporter)->post_uncorrectable(
			"unexpectedly reached the end of file", peek());
	}

	Token Parser::consume(TokenKind type)
	{
		const auto& current = peek();
		if (current.type == type)
		{
			cursor++;
			return current;
		}

		auto convert = [&](auto v) -> const std::string&
		{
			const auto item = token_map.at(v);

			if (item)
			{
				return *item;
			}

			USE_SAFE(ErrorReporter)->post_error("invalid token", current);
			return { "[error]" };
		};

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"expected token `{}` but got `{}`",
				convert(type), ((current.type == TokenKind::IDENTIFIER || current.type == TokenKind::INT) ? current.text : convert(current.type))), current);
		return current;
	}

	std::vector<Token> Parser::fetch_until(TokenKind type)
	{
		std::vector<Token> tokens;

		while (peek().type != type)
		{
			tokens.emplace_back(peek());
			consume(peek().type);
		}

		return tokens;
	}

#pragma message ("TODO: refactor the dotdefine to be overriden by subclasses")
	Node* Parser::parse_dotdefine_command()
	{
		consume(TokenKind::DOTDEFINE);

		Type* type = nullptr;

		if (ptype() == ParserType::COMPILER)
		{
			auto compiler_parser = AS_COMPILER_PARSER(this);
			type = compiler_parser->parse_type();
		}

		else
		{
			// default to unsigned 32 bits non-compiler workloads since we don't have the machinery for type resolution otherwise
			type = new IntType{ TypeQualifier::IMMUTABLE, TypeSignedness::UNSIGNED, IntTypeKind::INT32, TypeStorage::VALUE };
		}
		
		const auto identifier_expression = parse_identifier_expression();
		consume(TokenKind::EQUALS);

		// if the optimized value is not a constant expression, it isn't a valid definition
		const auto value_expression = parse_expression_optimized();

		if (value_expression->etype() != ExpressionType::INTEGER_LITERAL)
		{
			USE_SAFE(ErrorReporter)->post_error("definitions must result in a constant expression", value_expression->_token);
			return nullptr;
		}

		const auto& identifier = identifier_expression->name;
		const auto value = AS_INTEGER_LITERAL_EXPRESSION(value_expression)->value;

		USE_SAFE(SymbolDatabase)->add_define(identifier, peek());
		
		auto define_symbol = USE_SAFE(SymbolDatabase)->reference_define(identifier, peek());
		define_symbol->type = type;
		define_symbol->value = integer_literal_raw(value);

		// NOTE: exports the constant expression name symbol only
		USE_SAFE(SymbolExporter)->enqueue(define_symbol, identifier_expression->_token);

		return new DotDefineCommand{ identifier, value, identifier_expression->_token };
	}

	IdentifierExpression* Parser::parse_identifier_expression(IdentifierType itype)
	{
		const auto name_token = consume(TokenKind::IDENTIFIER);
		return new IdentifierExpression{ name_token.text, name_token, itype };
	}

	IntegerLiteralExpression* Parser::parse_integerliteral_expression()
	{
		const auto integer_literal_token = consume(TokenKind::INT);
		const auto& integer_string = integer_literal_token.text;

		BigInteger integer_value{};

		const auto [pointer, error] = std::from_chars(integer_string.data(), integer_string.data() + integer_string.size(), integer_value);
		if (error != std::errc())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"unparseable integer literal `{}`", integer_string), integer_literal_token);
			return nullptr;
		}

		const auto& specifier = peek();
		consume(specifier.type);

		return new IntegerLiteralExpression{ integer_value, integer_literal_token };
	}

	StringExpression* Parser::parse_string_expression()
	{
		const auto message_token = consume(TokenKind::STRING);
		return new StringExpression{ message_token.text, message_token };
	}

	FunctionCallExpression* Parser::parse_functioncall_expression()
	{
		const auto name_token = consume(TokenKind::IDENTIFIER);

		consume(TokenKind::LPAREN);
		const auto arguments = AS_COMPILER_PARSER(this)->parse_arguments(false);
		consume(TokenKind::RPAREN);

		if (!USE_SAFE(SymbolDatabase)->has_symbol(name_token.text))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"function `{}` is undefined", name_token.text), name_token);
			return nullptr;
		}

		const auto function_symbol = USE_SAFE(SymbolDatabase)->reference_function(name_token.text, name_token);

		if (function_symbol->arity() != arguments.size())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"function `{}` was defined with {} arguments but called with {}",
					name_token.text, function_symbol->arity(), arguments.size()), name_token);
			return nullptr;
		}

		// NOTE: exports the function name symbol only
		USE_SAFE(SymbolExporter)->enqueue(function_symbol, name_token);

		return new FunctionCallExpression{ name_token.text, arguments, name_token };
	}

	Expression* Parser::parse_parenthesis_expression()
	{
		consume(TokenKind::LPAREN);
		const auto expression = parse_expression();
		consume(TokenKind::RPAREN);

		return expression;
	}

	AdjustExpression* Parser::parse_increment_expression()
	{
		consume(TokenKind::TILDE);
		
		if (peek().type == TokenKind::IDENTIFIER)
		{
			const auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ true, identifier_expression, identifier_expression->_token };
		}

		else if (peek().type == TokenKind::INT)
		{
			const auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ true, integer_literal_expression, integer_literal_expression->_token };
		}

		USE_SAFE(ErrorReporter)->post_error("increment target must evaluate to a modifiable l-value", peek());
		return nullptr;
	}

	AdjustExpression* Parser::parse_decrement_expression()
	{
		consume(TokenKind::EXCLAMATION);

		if (peek().type == TokenKind::IDENTIFIER)
		{
			const auto identifier_expression = parse_identifier_expression();
			return new AdjustExpression{ false, identifier_expression, identifier_expression->_token };
		}

		else if (peek().type == TokenKind::INT)
		{
			const auto integer_literal_expression = parse_integerliteral_expression();
			return new AdjustExpression{ false, integer_literal_expression, integer_literal_expression->_token };
		}

		USE_SAFE(ErrorReporter)->post_error("decrement target must evaluate to a modifiable l-value", peek());
		return nullptr;
	}


	Expression* Parser::parse_generic_expression()
	{
		using enum TokenKind;
		switch (peek().type)
		{
			case LPAREN:
			{
				return parse_parenthesis_expression();
			} break;

			case INT:
			{
				return parse_integerliteral_expression();
			} break;
			
			case STRING:
			{
				return parse_string_expression();
			} break;

			case IDENTIFIER:
			{
				if (lookahead().type == LPAREN)
				{
					return parse_functioncall_expression();
				}

				return parse_identifier_expression();
			} break;

			case TILDE:
			{
				return parse_increment_expression();
			} break;

			case EXCLAMATION:
			{
				return parse_decrement_expression();
			} break;

			default:
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"expected an expression but got `{}`", peek().text), peek());
				return nullptr;
			} break;
		}
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
		// parse until the minimum precedence level is reached (so parse everything possible basically)
		const auto infix_expression = parse_infix_expression(parse_generic_expression(), Precedence::MINIMUM);

		if (infix_expression != nullptr)
		{
			return AS_EXPRESSION(infix_expression->copy());
		}

		return nullptr;
	}

	Expression* Parser::parse_infix_expression(Expression* left, Precedence minimum_precedence)
	{
		static const std::unordered_map<TokenKind, Precedence> precedences
		{
			{ TokenKind::EQUALS,            Precedence::ASSIGN },
			{ TokenKind::EQUALSEQUALS,      Precedence::EQUALITY },
			{ TokenKind::EXCLAMATIONEQUALS, Precedence::EQUALITY },
			{ TokenKind::GREATER,           Precedence::COMPARE },
			{ TokenKind::LESS,              Precedence::COMPARE },
			{ TokenKind::PLUS,              Precedence::TERM },
			{ TokenKind::MINUS,             Precedence::TERM },
			{ TokenKind::STAR,              Precedence::FACTOR },
		};

		do
		{
			const auto& next = peek();

			if (!::is_binary_operator(next.type) || precedences.at(next.type) < minimum_precedence)
			{
				break;
			}

			consume(next.type);

			auto right = parse_expression();

			do
			{
				const auto& lookahead = peek();

				if (!::is_binary_operator(lookahead.type) || precedences.at(lookahead.type) <= precedences.at(next.type))
				{
					break;
				}

#pragma message("TODO: maybe add precedence+1 for the next call of the recursive function")
				right = parse_infix_expression(right, precedences.at(lookahead.type));

			} while(true);

			switch (next.type)
			{
				case TokenKind::PLUS: left = new PlusBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::MINUS: left = new MinusBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::STAR: left = new TimesBinaryExpression{ left, right, left->_token }; break;

				case TokenKind::EQUALS: left = new AssignBinaryExpression{ left, right, left->_token }; break;

				case TokenKind::EQUALSEQUALS: left = new EqualityBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::EXCLAMATIONEQUALS: left = new InequalityBinaryExpression{ left, right, left->_token }; break;

				case TokenKind::GREATER: left = new GreaterBinaryExpression{ left, right, left->_token }; break;
				case TokenKind::LESS: left = new LessBinaryExpression{ left, right, left->_token }; break;
			}
		} while (true);

		return left;
	}

	void Parser::reload(const std::vector<Token>& tokens, const std::string& filepath)
	{
		_tokens = tokens;
		filepath = filepath;
	}
}
