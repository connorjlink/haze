import std;

#include <ast/expression/Expression.h>
#include <command/Command.h>
#include <toolchain/RISCVAssemblerParser.h>
#include <utility/ExtendedInteger.h>

// Haze AssemblerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

#define ASSERT_IS_INTEGER_LITERAL(x) \
	if (x->etype() != ExpressionType::INTEGER_LITERAL) \
	{ \
		USE_SAFE(ErrorReporter)->post_error("term must result in a constant expression", NULL_TOKEN); \
		return nullptr; \
	} 

#define ASSERT_IN_RANGE(x, a, b, c, d) \
	if (x < a || x > b) \
	{ \
		USE_SAFE(ErrorReporter)->post_error(std::format("value {} is outside the its range [{}, {}]", x.magnitude, c, d), NULL_TOKEN);  \
		return nullptr; \
	}

namespace hz
{
	CommandHandle AssemblerParser::parse_dotdefine_command()
	{
		consume(TokenKind::PERIOD);
		consume(TokenKind::DEFINE);

		const auto identifier_expression = parse_identifier_expression();
		
		consume(TokenKind::EQUALS);

		// if the optimized value is not a constant expression, it isn't a valid definition
		const auto value_expression = parse_expression_optimized();

		if (!value_expression.is_valid())
		{
			USE_SAFE(ErrorReporter)->post_error("definitions must result in a constant expression", value_expression->_token);
			return nullptr;
		}

		const auto& identifier = identifier_expression->name;
		const auto value = AS_INTEGER_LITERAL_EXPRESSION(value_expression)->value;

		USE_SAFE(SymbolDatabase)->add_define(identifier, peek());

		auto define_symbol = USE_SAFE(SymbolDatabase)->reference_define(identifier, peek());
		define_symbol->kind = kind;
		define_symbol->value = integer_literal_raw(value);

		// NOTE: exports the constant expression name symbol only
		USE_SAFE(SymbolExporter)->enqueue(define_symbol, identifier_expression->_token);

		return new DotDefineCommand{ identifier, value, identifier_expression->_token };
	}

	Node* AssemblerParser::parse_dotorg_command()
	{
		consume(TokenKind::DOTORG);
		const auto& token = lookahead();

		const auto address_expression = parse_literal();
		if (address_expression->etype() != ExpressionType::INTEGER_LITERAL)
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"address of `.org` must result in a constant expression"), token);
			return nullptr;
		}

		const auto address = AS_INTEGER_LITERAL_EXPRESSION(address_expression)->value;
		return new DotOrgCommand{ integer_literal_raw(address), address_expression->_token };
	}

	Node* AssemblerParser::parse_label_command()
	{
		const auto identifier_expression = parse_identifier_expression();
		consume(TokenKind::COLON);

		const auto& identifier = identifier_expression->name;

		USE_SAFE(SymbolDatabase)->add_label(identifier, lookbehind());
		// NOTE: we don't yet know the address since we haven't linked to resolve it yet
		//AS_LABEL_SYMBOL(reference_symbol(Symbol::Type::LABEL, identifier))->address = 

		return new LabelCommand{ identifier, identifier_expression->_token };
	}

	Node* AssemblerParser::parse_command()
	{
		const auto& first = peek();

		switch (first.kind)
		{
			case TokenKind::DOTDEFINE: return parse_dotdefine_command();
			case TokenKind::DOTORG: return parse_dotorg_command();
			case TokenKind::IDENTIFIER: return parse_label_command();
		}

		const auto instruction = parse_instruction_command();
		return new InstructionCommand{ first, instruction->emit(), "" };
	}

	std::vector<Node*> AssemblerParser::parse()
	{
		std::vector<Node*> instructions{};

		while (peek().kind != TokenKind::END)
		{
			instructions.emplace_back(parse_command());
		}

		return instructions;
	}
}