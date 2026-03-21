import std;

#include <toolchain/RISCVAssemblerParser.h>
#include <command/models/IntegerLiteral.h>
#include <ast/IntegerLiteralExpression.h>
#include <riscv/RISCVInstruction.h>
#include <riscv/RISCVRegister.h>

// Haze RISCVAssemblerParser.cpp
// (c) Connor J. Link. All Rights Reserved.

#define PARSE_REGISTER() static_cast<RISCVRegister>(parse_register())

namespace hz
{
	ArchitectureType RISCVAssemblerParser::ctype() const
	{
		return ArchitectureType::RISCV;
	}

	Instruction* RISCVAssemblerParser::parse_instruction_command()
	{
		const auto& opcode_token = peek();
		consume(opcode_token.type);

		using enum TokenType;
		switch (opcode_token.type)
		{
			// I instruction set
			case LB:
			{
				const auto rd = PARSE_REGISTER();
				consume(TokenType::COMMA);
				const auto integer_literal = parse_integerliteral_expression();
				const auto immediate = static_cast<native_int>(integer_literal_int(integer_literal->value));
				consume(TokenType::LPAREN);
				const auto rs1 = PARSE_REGISTER();
				consume(TokenType::RPAREN);
				return new lb(rd, immediate, rs1);
			} break;

			case LH:
			case LW:
			case LBU:
			case LHU:
			case SB:
			case SH:
			case SW:
			case LUI:
			case AUIPC:
			case ADDI:
			case ADD:
			case SUB:
			case SLTI:
			case SLT:
			case SLTIU:
			case SLTU:
			case ANDI:
			case AND:
			case ORI:
			case OR:
			case XORI:
			case XOR:
			case SLLI:
			case SLL:
			case SRLI:
			case SRL:
			case SRAI:
			case SRA:
			case JAL:
			case JALR:
			case BEQ:
			case BNE:
			case BLT:
			case BGE:
			case BLTU:
			case BGEU:
			case NOP:
			case ECALL:
			case EBREAK:
			case WFI:
			case FENCE: 

			// M extension
			case MUL:
			case MULH:
			case MULHU:
			case MULHSU:
			case DIV:
			case DIVU:
			case REM:
			case REMU:

			// A extension
			case LRW:
			case SCW:
			case AMOSWAPW:
			case AMOADDW:
			case AMOXORW:
			case AMOANDW:
			case AMOORW:
			case AMOMINW:
			case AMOMAXW:
			case AMOMINUW:
			case AMOMAXUW:

			// C extension
			case CLWSP:
			case CSWSP:
			case CLW:
			case CSW:
			case CJ:
			case CJAL:
			case CJR:
			case CJALR:
			case CBEQZ:
			case CBNEZ:
			case CLI:
			case CLUI:
			case CADDI:
			case CADDI16SP:
			case CADDI4SPN:
			case CSLLI:
			case CSRLI:
			case CSRAI:
			case CANDI:
			case CMV:
			case CADD:
			case CAND:
			case COR:
			case CXOR:
			case CSUB:
			case CNOP:
			case CEBREAK:
			
			// Zicsr extension
			case CSRRW:
			case CSRRS:
			case CSRRC:
			case CSRRWI:
			case CSRRSI:
			case CSRRCI:

			// Ziencei extension
			case FENCEI:

			// U/S mode
			case URET:
			case SRET:
			case MRET:
		}
		
		USE_SAFE(ErrorReporter)->post_error(std::format(
			"unrecognized instruction opcode `{}`", opcode_token.text), opcode_token);

		return nullptr;
	}

	std::int8_t RISCVAssemblerParser::parse_register()
	{
		const auto& register_token = peek();
		consume(register_token.type);

		switch (register_token.type)
		{
			case TokenType::X0:  case TokenType::ZERO:                   return X0;
			case TokenType::X1:  case TokenType::RA:                     return X1;
			case TokenType::X2:  case TokenType::SP:                     return X2;
			case TokenType::X3:  case TokenType::GP:                     return X3;
			case TokenType::X4:  case TokenType::TP:                     return X4;
			case TokenType::X5:  case TokenType::T0:                     return X5;
			case TokenType::X6:  case TokenType::T1:                     return X6;
			case TokenType::X7:  case TokenType::T2:                     return X7;
			case TokenType::X8:  case TokenType::S0: case TokenType::FP: return X8;
			case TokenType::X9:  case TokenType::S1:                     return X9;
			case TokenType::X10: case TokenType::A0:                     return X10;
			case TokenType::X11: case TokenType::A1:                     return X11;
			case TokenType::X12: case TokenType::A2:                     return X12;
			case TokenType::X13: case TokenType::A3:                     return X13;
			case TokenType::X14: case TokenType::A4:                     return X14;
			case TokenType::X15: case TokenType::A5:                     return X15;
			case TokenType::X16: case TokenType::A6:                     return X16;
			case TokenType::X17: case TokenType::A7:                     return X17;
			case TokenType::X18: case TokenType::S2:                     return X18;
			case TokenType::X19: case TokenType::S3:                     return X19;
			case TokenType::X20: case TokenType::S4:                     return X20;
			case TokenType::X21: case TokenType::S5:                     return X21;
			case TokenType::X22: case TokenType::S6:                     return X22;
			case TokenType::X23: case TokenType::S7:                     return X23;
			case TokenType::X24: case TokenType::S8:                     return X24;
			case TokenType::X25: case TokenType::S9:                     return X25;
			case TokenType::X26: case TokenType::S10:                    return X26;
			case TokenType::X27: case TokenType::S11:                    return X27;
			case TokenType::X28: case TokenType::T3:                     return X28;
			case TokenType::X29: case TokenType::T4:                     return X29;
			case TokenType::X30: case TokenType::T5:                     return X30;
			case TokenType::X31: case TokenType::T6:                     return X31;
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"unrecognized register `{}`", register_token.text), register_token);

		return std::int8_t{ -1 };
	}
}
