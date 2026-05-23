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
	using namespace riscv;

	ArchitectureKind RISCVAssemblerParser::ctype() const
	{
		return ArchitectureKind::RISCV;
	}

	Instruction* RISCVAssemblerParser::parse_instruction_command()
	{
		const auto& opcode_token = peek();
		consume(opcode_token.type);

		using enum TokenKind;
		switch (opcode_token.type)
		{
			// I instruction set
			case LB:
			{
				const auto rd = PARSE_REGISTER();
				consume(TokenKind::COMMA);
				const auto integer_literal = parse_integerliteral_expression();
				const auto immediate = static_cast<Offset>(integer_literal_int(integer_literal->value));
				consume(TokenKind::LPAREN);
				const auto rs1 = PARSE_REGISTER();
				consume(TokenKind::RPAREN);
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
			case TokenKind::X0:  case TokenKind::ZERO:                   return X0;
			case TokenKind::X1:  case TokenKind::RA:                     return X1;
			case TokenKind::X2:  case TokenKind::SP:                     return X2;
			case TokenKind::X3:  case TokenKind::GP:                     return X3;
			case TokenKind::X4:  case TokenKind::TP:                     return X4;
			case TokenKind::X5:  case TokenKind::T0:                     return X5;
			case TokenKind::X6:  case TokenKind::T1:                     return X6;
			case TokenKind::X7:  case TokenKind::T2:                     return X7;
			case TokenKind::X8:  case TokenKind::S0: case TokenKind::FP: return X8;
			case TokenKind::X9:  case TokenKind::S1:                     return X9;
			case TokenKind::X10: case TokenKind::A0:                     return X10;
			case TokenKind::X11: case TokenKind::A1:                     return X11;
			case TokenKind::X12: case TokenKind::A2:                     return X12;
			case TokenKind::X13: case TokenKind::A3:                     return X13;
			case TokenKind::X14: case TokenKind::A4:                     return X14;
			case TokenKind::X15: case TokenKind::A5:                     return X15;
			case TokenKind::X16: case TokenKind::A6:                     return X16;
			case TokenKind::X17: case TokenKind::A7:                     return X17;
			case TokenKind::X18: case TokenKind::S2:                     return X18;
			case TokenKind::X19: case TokenKind::S3:                     return X19;
			case TokenKind::X20: case TokenKind::S4:                     return X20;
			case TokenKind::X21: case TokenKind::S5:                     return X21;
			case TokenKind::X22: case TokenKind::S6:                     return X22;
			case TokenKind::X23: case TokenKind::S7:                     return X23;
			case TokenKind::X24: case TokenKind::S8:                     return X24;
			case TokenKind::X25: case TokenKind::S9:                     return X25;
			case TokenKind::X26: case TokenKind::S10:                    return X26;
			case TokenKind::X27: case TokenKind::S11:                    return X27;
			case TokenKind::X28: case TokenKind::T3:                     return X28;
			case TokenKind::X29: case TokenKind::T4:                     return X29;
			case TokenKind::X30: case TokenKind::T5:                     return X30;
			case TokenKind::X31: case TokenKind::T6:                     return X31;
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"unrecognized register `{}`", register_token.text), register_token);

		return std::int8_t{ -1 };
	}
}
