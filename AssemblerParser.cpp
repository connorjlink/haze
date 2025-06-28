import std;

#include "AssemblerParser.h"
#include "DotOrgCommand.h"
#include "LabelCommand.h"
#include "InstructionCommand.h"
#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "Constants.h"
#include "SymbolType.h"
#include "ExtendedInteger.h"
#include "ErrorReporter.h"
#include "SymbolDatabase.h"

// Haze AssemblerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

#define ASSERT_IS_INTEGER_LITERAL(x) if (x->etype() != ExpressionType::INTEGER_LITERAL) { _error_reporter->post_error("term must result in a constant expression", NULL_TOKEN); return nullptr; }
#define ASSERT_IN_RANGE(x, a, b) if (x < EI(a) || x > EI(b - 1)) { _error_reporter->post_error(std::format("value {} is outside the its range [0, {}]", x.magnitude, b - 1), NULL_TOKEN); return nullptr; }

namespace hz
{
	ParserType AssemblerParser::ptype() const
	{
		return ParserType::ASSEMBLER;
	}

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
			consume(peek().type);
			return it_operand->second;
		}

		_error_reporter->post_error(std::format("expected a register but got {}", peek().text), peek());
		return static_cast<Register>(0);
	}

	//TODO: roll this function and parse_immediate() below it into a "nice" macro to reduce code duplication
	Expression* AssemblerParser::parse_address()
	{
		consume(TokenType::AMPERSAND);

		if (peek().type == TokenType::IDENTIFIER)
		{
			auto label_command_token = peek();
			consume(TokenType::IDENTIFIER);

			return new IdentifierExpression{ label_command_token.text, label_command_token };
		}

		const auto address_expression = parse_expression_optimized();
		ASSERT_IS_INTEGER_LITERAL(address_expression);

		const auto address = AS_INTEGER_LITERAL_EXPRESSION(address_expression)->value;
		ASSERT_IN_RANGE(integer_literal_raw(address), 0, DWORD_MAX);

		return new IntegerLiteralExpression{ address, address_expression->_token };
	}

	Expression* AssemblerParser::parse_immediate()
	{
		consume(TokenType::POUND);

		if (peek().type == TokenType::IDENTIFIER)
		{
			const auto label_command_token = peek();
			consume(TokenType::IDENTIFIER);

			return new IdentifierExpression{ label_command_token.text, label_command_token };
		}

		const auto immediate_expression = parse_expression_optimized();
		ASSERT_IS_INTEGER_LITERAL(immediate_expression);

		const auto immediate = AS_INTEGER_LITERAL_EXPRESSION(immediate_expression)->value;
		ASSERT_IN_RANGE(integer_literal_raw(immediate), 0, WORD_MAX);

		return new IntegerLiteralExpression{ immediate, immediate_expression->_token };
	}

	Node* AssemblerParser::parse_dotorg_command()
	{
		consume(TokenType::DOTORG);

		const auto address_expression = parse_address();
		const auto address = AS_INTEGER_LITERAL_EXPRESSION(address_expression)->value;

		return new DotOrgCommand{ integer_literal_raw(address), address_expression->_token };
	}

	Node* AssemblerParser::parse_label_command()
	{
		const auto identifier_expression = parse_identifier_expression();
		consume(TokenType::COLON);

		const auto identifier = identifier_expression->name;

		_database->add_label(identifier, lookbehind());
		// NOTE: we don't yet know the address since we haven't linked to resolve it yet
		//AS_LABEL_SYMBOL(reference_symbol(Symbol::Type::LABEL, identifier))->address = 

		return new LabelCommand{ identifier, identifier_expression->_token };
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
				const auto move_token = consume(TokenType::MOVE);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ move_token, Opcode::MOVE, operand1, operand2 };
			} break;

			case TokenType::LOAD:
			{
				const auto load_token = consume(TokenType::LOAD);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_address();

				if (operand2->etype() == ExpressionType::IDENTIFIER)
				{
					return new InstructionCommand{ load_token, Opcode::LOAD, operand1, DC, 0, 0xCCCCCCCC, AS_IDENTIFIER_EXPRESSION(operand2)->name };
				}

				return new InstructionCommand{ load_token, Opcode::LOAD, operand1, DC, 0, integer_literal_raw(AS_INTEGER_LITERAL_EXPRESSION(operand2)->value) };
			} break;

			case TokenType::COPY:
			{
				const auto copy_token = consume(TokenType::COPY);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_immediate();

				if (operand2->etype() == ExpressionType::IDENTIFIER)
				{
					return new InstructionCommand{ copy_token, Opcode::COPY, operand1, DC, 0xCC, 0, AS_IDENTIFIER_EXPRESSION(operand2)->name };
				}

				const auto value = integer_literal_raw(AS_INTEGER_LITERAL_EXPRESSION(operand2)->value);
				ASSERT_IN_RANGE(value, 0, WORD_MAX);

				return new InstructionCommand{ copy_token, Opcode::COPY, operand1, DC, static_cast<platform_address_size>(value.magnitude) };
			} break;

			case TokenType::SAVE:
			{
				const auto save_token = consume(TokenType::SAVE);

				consume(TokenType::AMPERSAND);
				const auto operand1 = parse_address();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				if (operand1->etype() == ExpressionType::IDENTIFIER)
				{
					return new InstructionCommand{ save_token, Opcode::SAVE, DC, operand2, 0, 0xCCCCCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new InstructionCommand{ save_token, Opcode::SAVE, DC, operand2, 0, integer_literal_raw(AS_INTEGER_LITERAL_EXPRESSION(operand1)->value) };
			} break;

			case TokenType::IADD:
			{
				const auto iadd_token = consume(TokenType::IADD);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ iadd_token, Opcode::IADD, operand1, operand2 };
			} break;

			case TokenType::ISUB:
			{
				const auto isub_token = consume(TokenType::ISUB);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ isub_token, Opcode::ISUB, operand1, operand2 };
			} break;

			case TokenType::BAND:
			{
				const auto band_token = consume(TokenType::BAND);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ band_token, Opcode::BAND, operand1, operand2 };
			} break;

			case TokenType::BIOR:
			{
				const auto bior_token = consume(TokenType::BIOR);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ bior_token, Opcode::BIOR, operand1, operand2 };
			} break;

			case TokenType::BXOR:
			{
				const auto bxor_token = consume(TokenType::BXOR);

				const auto operand1 = parse_register();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				return new InstructionCommand{ bxor_token, Opcode::BXOR, operand1, operand2 };
			} break;

			case TokenType::CALL:
			{
				const auto call_token = consume(TokenType::CALL);
				const auto operand1 = parse_address();

				if (operand1->etype() == ExpressionType::IDENTIFIER)
				{
					return new InstructionCommand{ call_token, Opcode::CALL, DC, DC, 0, 0xCCCCCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new InstructionCommand{ call_token, Opcode::CALL, DC, DC, 0, integer_literal_raw(AS_INTEGER_LITERAL_EXPRESSION(operand1)->value) };
			} break;

			case TokenType::EXIT:
			{
				const auto exit_token = consume(TokenType::EXIT);

				return new InstructionCommand{ exit_token, Opcode::EXIT, DC, DC };
			} break;

			case TokenType::PUSH:
			{
				const auto push_token = consume(TokenType::PUSH);
				const auto operand1 = parse_register();

				return new InstructionCommand{ push_token, Opcode::PUSH, DC, operand1 };
			} break;

			case TokenType::PULL:
			{
				const auto pull_token = consume(TokenType::PULL);
				const auto operand1 = parse_register();

				return new InstructionCommand{ pull_token, Opcode::PULL, operand1, DC };
			} break;

			case TokenType::BRNZ:
			{
				const auto brnz_token = consume(TokenType::BRNZ);

				const auto operand1 = parse_address();
				consume(TokenType::COMMA);
				const auto operand2 = parse_register();

				if (operand1->etype() == ExpressionType::IDENTIFIER)
				{
					return new InstructionCommand{ brnz_token, Opcode::BRNZ, DC, operand2, 0, 0xCCCCCCCC, AS_IDENTIFIER_EXPRESSION(operand1)->name };
				}

				return new InstructionCommand{ brnz_token, Opcode::BRNZ, DC, operand2, 0, integer_literal_raw(AS_INTEGER_LITERAL_EXPRESSION(operand1)->value) };
			} break;

			case TokenType::BOOL:
			{
				const auto bool_token = consume(TokenType::BOOL);
				const auto operand1 = parse_register();

				return new InstructionCommand{ bool_token, Opcode::BOOL, DC, operand1  };
			} break;

			case TokenType::STOP:
			{
				const auto stop_token = consume(TokenType::STOP);

				return new InstructionCommand{ stop_token, Opcode::STOP, DC, DC };
			} break;

			default:
			{
				_error_reporter->post_error(std::format("invalid instruction mnemonic `{}`", peek().text), peek());
				return nullptr;
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