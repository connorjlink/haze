#include "AssemblerParser.h"

#include "DotOrgCommand.h"

#include "LabelCommand.h"

#include "InstructionCommand.h"
#include "Constants.h"
#include "Log.h"

#define ASSERT_IS_INTEGER_LITERAL(x) if (x->etype() != Expression::Type::INTEGER_LITERAL) Log::error("term must evaluate to a constant expression")
#define ASSERT_IN_RANGE(x, a, b) if (x < a || x > b - 1) Log::error(std::format("value {} is outside the its range [0, {}]", x, b - 1))

namespace hz
{
	Register AssemblerParser::parse_register()
	{
		static const std::unordered_map<TokenType, Register> registers
		{
			{ TokenType::R0, R0 },
			{ TokenType::R1, R1 },
			{ TokenType::R2, R2 },
			{ TokenType::R3, R3 },
		};

		if (const auto it_operand = registers.find(peek().type); it_operand != std::end(registers))
		{
			DISCARD consume(peek().type);
			return it_operand->second;
		}

		Log::error(std::format("expected a register but got {}", peek().value));
	}

	//TODO: roll this function and parse_immediate() below it into a "nice" macro to reduce code duplication
	Expression* AssemblerParser::parse_address()
	{
		DISCARD consume(TokenType::AMPERSAND);

		if (peek().type == TokenType::IDENTIFIER)
		{
			auto LabelCommand = peek().value;
			DISCARD consume(TokenType::IDENTIFIER);

			return new IdentifierExpression{ LabelCommand };
		}

		const auto address_expression = parse_expression_optimized();
		ASSERT_IS_INTEGER_LITERAL(address_expression);

		const auto address = AS_INTEGER_LITERAL_EXPRESSION(address_expression)->value;
		ASSERT_IN_RANGE(address, 0, DWORD_MAX);

		return new IntegerLiteralExpression{ address };
	}

	Expression* AssemblerParser::parse_immediate()
	{
		DISCARD consume(TokenType::POUND);

		if (peek().type == TokenType::IDENTIFIER)
		{
			const auto LabelCommand = peek().value;
			DISCARD consume(TokenType::IDENTIFIER);

			return new IdentifierExpression{ LabelCommand };
		}

		const auto immediate_expression = parse_expression_optimized();
		ASSERT_IS_INTEGER_LITERAL(immediate_expression);

		const auto immediate = AS_INTEGER_LITERAL_EXPRESSION(immediate_expression)->value;
		ASSERT_IN_RANGE(immediate, 0, WORD_MAX);

		return new IntegerLiteralExpression{ immediate };
	}

	Node* AssemblerParser::parse_dotdefine_command()
	{
		DISCARD consume(TokenType::DOTDEFINE);
		const auto identifier_expresion = parse_identifier_expression();
		DISCARD consume(TokenType::EQUALS);

		const auto value_expression = parse_expression();
		ASSERT_IS_INTEGER_LITERAL(value_expression);

		const auto identifier = identifier_expresion->name;
		const auto value = AS_INTEGER_LITERAL_EXPRESSION(value_expression)->value;

		add_symbol(Symbol::Type::DEFINE, identifier);
		AS_DEFINE_SYMBOL(reference_symbol(Symbol::Type::DEFINE, identifier))->value = value;

		return nullptr;
	}

	Node* AssemblerParser::parse_dotorg_command()
	{
		DISCARD consume(TokenType::DOTORG);

		const auto address_expression = parse_address();
		const auto address = AS_INTEGER_LITERAL_EXPRESSION(address_expression)->value;

		return new DotOrgCommand{ address };
	}

	Node* AssemblerParser::parse_label_command()
	{
		const auto identifier_expression = parse_identifier_expression();
		DISCARD consume(TokenType::COLON);

		const auto identifier = identifier_expression->name;

		add_symbol(Symbol::Type::LABEL, identifier);
		//AS_LABEL_SYMBOL(reference_symbol(Symbol::Type::LABEL, identifier))->address = 

		return new LabelCommand{ identifier };
	}

	Node* AssemblerParser::parse_instruction_command()
	{
		//TODO: this is probably useless and should later be removed but I am leaving for now in case I come up with a need for it :)
		/*static const std::unordered_map<TokenType, Opcode> opcodes
		{
			{ TokenType::MOVE, MOVE },
			{ TokenType::LOAD, LOAD },
			{ TokenType::COPY, COPY },
			{ TokenType::SAVE, SAVE },
			{ TokenType::IADD, IADD },
			{ TokenType::ISUB, ISUB },
			{ TokenType::BAND, BAND },
			{ TokenType::BIOR, BIOR },
			{ TokenType::BXOR, BXOR },
			{ TokenType::CALL, CALL },
			{ TokenType::EXIT, EXIT },
			{ TokenType::PUSH, PUSH },
			{ TokenType::PULL, PULL },
			{ TokenType::BREZ, BREZ },
		};*/


		//TODO: patch branch_target resolution to more generally just resolve labels!
		switch (peek().type)
		{
			case TokenType::MOVE:
			{
				DISCARD consume(TokenType::MOVE);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ MOVE, operand1, operand2 };
			} break;

			case TokenType::LOAD:
			{
				DISCARD consume(TokenType::LOAD);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_address();

				if (operand2->etype() == Expression::Type::IDENTIFIER)
				{
					return new InstructionCommand{ LOAD, operand1, DC, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand2)->name };
				}

				return new InstructionCommand{ LOAD, operand1, DC, 0, AS_INTEGER_LITERAL_EXPRESSION(operand2)->value };
			} break;

			case TokenType::COPY:
			{
				DISCARD consume(TokenType::COPY);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_immediate();

				if (operand2->etype() == Expression::Type::IDENTIFIER)
				{
					return new InstructionCommand{ COPY, operand1, DC, 0xCC, 0, AS_IDENTIFIER_EXPRESSION(operand2)->name };
				}

				const auto value = AS_INTEGER_LITERAL_EXPRESSION(operand2)->value;
				ASSERT_IN_RANGE(value, 0, DWORD_MAX - 1);

				return new InstructionCommand{ COPY, operand1, DC, static_cast<std::uint8_t>(value) };
			} break;

			case TokenType::SAVE:
			{
				DISCARD consume(TokenType::SAVE);
				DISCARD consume(TokenType::AMPERSAND);
				const auto operand1 = parse_address();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				if (operand1->etype() == Expression::Type::IDENTIFIER)
				{
					return new InstructionCommand{ SAVE, DC, operand2, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new InstructionCommand{ SAVE, DC, operand2, 0, AS_INTEGER_LITERAL_EXPRESSION(operand1)->value };
			} break;

			case TokenType::IADD:
			{
				DISCARD consume(TokenType::IADD);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ IADD, operand1, operand2 };
			} break;

			case TokenType::ISUB:
			{
				DISCARD consume(TokenType::ISUB);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ ISUB, operand1, operand2 };
			} break;

			case TokenType::BAND:
			{
				DISCARD consume(TokenType::BAND);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ BAND, operand1, operand2 };
			} break;

			case TokenType::BIOR:
			{
				DISCARD consume(TokenType::BIOR);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ BIOR, operand1, operand2 };
			} break;

			case TokenType::BXOR:
			{
				DISCARD consume(TokenType::BXOR);
				const auto operand1 = parse_register();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ BXOR, operand1, operand2 };
			} break;

			case TokenType::CALL:
			{
				DISCARD consume(TokenType::CALL);
				const auto operand1 = parse_address();

				if (operand1->etype() == Expression::Type::IDENTIFIER)
				{
					return new InstructionCommand{ CALL, DC, DC, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new InstructionCommand{ CALL, DC, DC, 0, AS_INTEGER_LITERAL_EXPRESSION(operand1)->value };
			} break;

			case TokenType::EXIT:
			{
				DISCARD consume(TokenType::EXIT);

				return new InstructionCommand{ EXIT, DC, DC };
			} break;

			case TokenType::PUSH:
			{
				DISCARD consume(TokenType::PUSH);
				const auto operand1 = parse_register();

				return new InstructionCommand{ PUSH, DC, operand1 };
			} break;

			case TokenType::PULL:
			{
				DISCARD consume(TokenType::PULL);
				const auto operand1 = parse_register();

				return new InstructionCommand{ PULL, operand1, DC };
			} break;

			case TokenType::BREZ:
			{
				DISCARD consume(TokenType::BREZ);
				const auto operand1 = parse_address();
				DISCARD consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				if (operand1->etype() == Expression::Type::IDENTIFIER)
				{
					return new InstructionCommand{ BRNZ, DC, operand2, 0, 0xCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new InstructionCommand{ BRNZ, DC, operand2, 0, AS_INTEGER_LITERAL_EXPRESSION(operand1)->value };
			} break;

			default:
			{
				Log::error(std::format("unrecognized instruction mnemonic {}", peek().value));
			} break;
		}
	}


	Node* AssemblerParser::parse_command()
	{
		switch (peek().type)
		{
			case TokenType::DOTDEFINE: return parse_dotdefine_command();
			case TokenType::DOTORG: return parse_dotorg_command();
			case TokenType::IDENTIFIER: return parse_label_command();
			default: return parse_instruction_command();
		}
	}

	std::vector<Node*> AssemblerParser::parse_commands()
	{
		std::vector<Node*> instructions;

		while (peek().type != TokenType::END)
		{
			instructions.emplace_back(parse_command());
		}

		return instructions;
	}

	std::vector<Node*> AssemblerParser::parse()
	{
		return parse_commands();
	}
}