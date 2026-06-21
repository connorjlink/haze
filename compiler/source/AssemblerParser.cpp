import std;

#include <ast/expression/Expression.h>
#include <command/Command.h>
#include <toolchain/RISCVAssemblerParser.h>
#include <utility/ExtendedInteger.h>

// Haze AssemblerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	AssemblerParser::NodeReference<DotOrgCommand> AssemblerParser::parse_dotorg_command()
	{
		const auto token = consume(TokenKind::DOTORG);
		
		const auto expression = parse_integer_literal_expression();
		if (!expression.is_constant())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"address of `{}` must evaluate to a constant expression", token.text), token);
			return MAKE_INVALID_HANDLE(command_storage, Command);
		}

		return AS_HANDLE(MAKE_DOTORG_COMMAND(token, expression.get().value));
	}

	AssemblerParser::NodeReference<DotByteCommand> AssemblerParser::parse_dotbyte_command()
	{
		const auto token = consume(TokenKind::DOTBYTE);

		auto bytes = ByteRange{};

		auto integer = peek();
		do
		{
			integer = consume(TokenKind::INTEGER_LITERAL);

			const auto value = std::get<TokenInteger>(integer.value);
			if (value > std::numeric_limits<Byte>::max())
			{
				USE_SAFE(ErrorReporter)->post_error(std::format(
					"value `{}` is outside of the permissible range [{}, {}]", 
					value, std::numeric_limits<Byte>::min(), std::numeric_limits<Byte>::max()), NULL_TOKEN);
				return MAKE_INVALID_HANDLE(command_storage, Command);
			}
			
			bytes.push_back(static_cast<Byte>(value));

			if (peek().kind == TokenKind::COMMA)
			{
				consume(TokenKind::COMMA);
			}

		} while (token.kind == TokenKind::INTEGER_LITERAL);

		return AS_HANDLE(MAKE_DOTBYTE_COMMAND(token, std::move(bytes)));
	}

	AssemblerParser::NodeReference<LabelCommand> AssemblerParser::parse_label_command()
	{
		const auto identifier_expression = parse_identifier_expression();
		consume(TokenKind::COLON);

		const auto& identifier = identifier_expression->name;

		USE_SAFE(SymbolDatabase)->add_label(identifier, lookbehind());
		// NOTE: we don't yet know the address since we haven't linked to resolve it yet
		//AS_LABEL_SYMBOL(reference_symbol(Symbol::Type::LABEL, identifier))->address = 

		return new LabelCommand{ identifier, identifier_expression->_token };
	}

	AssemblerParser::NodeHandle AssemblerParser::parse_command()
	{
		switch (peek().kind)
		{
			case TokenKind::DOTORG:     return AS_HANDLE(parse_dotorg_command());
			case TokenKind::DOTBYTE:    return AS_HANDLE(parse_dotbyte_command());
			case TokenKind::DOTGLOBAL:  return AS_HANDLE(parse_dotglobal_command());
			case TokenKind::IDENTIFIER: return AS_HANDLE(parse_label_command());
		}

		return AS_HANDLE(parse_instruction_command());
	}

	std::vector<AssemblerParser::NodeHandle> AssemblerParser::parse_implementation()
	{
		auto result = std::vector<NodeHandle>{};

		while (peek().kind != TokenKind::END)
		{
			result.emplace_back(parse_command());
		}

		return result;
	}
}