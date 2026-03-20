import std;

#include <ast/IdentifierExpression.h>
#include <ast/IntegerLiteralExpression.h>
#include <command/DotOrgCommand.h>
#include <command/LabelCommand.h>
#include <command/InstructionCommand.h>
#include <toolchain/AssemblerParser.h>
#include <toolchain/models/Instruction.h>

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
	ParserType AssemblerParser::ptype() const
	{
		return ParserType::ASSEMBLER;
	}

	Expression* AssemblerParser::parse_literal()
	{
		const auto& first = peek();

		if (first.type == TokenType::IDENTIFIER)
		{
			const auto& label_command_token = first;
			consume(TokenType::IDENTIFIER);

			return new IdentifierExpression{ label_command_token.text, label_command_token };
		}

		const auto literal_expression = parse_expression_optimized();
		ASSERT_IS_INTEGER_LITERAL(literal_expression);

		const auto address = AS_INTEGER_LITERAL_EXPRESSION(literal_expression)->value;
		ASSERT_IN_RANGE(integer_literal_raw(address), EI(std::intmax_t{ 0 }), EI(std::uintmax_t{ NATIVE_UMAX }), 0, static_cast<std::uint64_t>(NATIVE_UMAX));

		return new IntegerLiteralExpression{ address, literal_expression->_token };
	}

	Node* AssemblerParser::parse_dotorg_command()
	{
		consume(TokenType::DOTORG);
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
		consume(TokenType::COLON);

		const auto& identifier = identifier_expression->name;

		USE_SAFE(SymbolDatabase)->add_label(identifier, lookbehind());
		// NOTE: we don't yet know the address since we haven't linked to resolve it yet
		//AS_LABEL_SYMBOL(reference_symbol(Symbol::Type::LABEL, identifier))->address = 

		return new LabelCommand{ identifier, identifier_expression->_token };
	}

	Node* AssemblerParser::parse_command()
	{
		const auto& first = peek();

		switch (first.type)
		{
			case TokenType::DOTDEFINE: return parse_dotdefine_command();
			case TokenType::DOTORG: return parse_dotorg_command();
			case TokenType::IDENTIFIER: return parse_label_command();
		}

		const auto instruction = parse_instruction_command();
		return new InstructionCommand{ first, instruction->emit(), "" };
	}

	std::vector<Node*> AssemblerParser::parse()
	{
		std::vector<Node*> instructions{};

		while (peek().type != TokenType::END)
		{
			instructions.emplace_back(parse_command());
		}

		return instructions;
	}
}