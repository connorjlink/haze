#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

#include <utility/Bimap.h>

// Haze Token.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TokenType
	{
		INT,

		IDENTIFIER,

		SEMICOLON,
		COLON,
		COMMA,
		EQUALS,

		AMPERSAND,
		POUND,

		PERCENT,

		PIPE,

		CARET,

		LPAREN, RPAREN,
		LBRACE, RBRACE,
		LBRACKET, RBRACKET,

		PLUS, MINUS, STAR, SLASH,

		EQUALSEQUALS, EXCLAMATIONEQUALS,

		GREATER, LESS,

		LESSLESS, GREATERGREATER,

		TILDE, EXCLAMATION,

		FUNCTION,

		IMMUTABLE,
		MUTABLE,

		UNSIGNED,
		SIGNED,

		VALUE,
		PTR,

		STRING,
		STRUCT,
		NVR,

		RETURN,
		WHILE,
		FOR,
		IF,
		ELSE,

		INTRINSIC,
		GEOMETRY,

		DOTDEFINE, DOTORG,
		DOTHOOK, DOTUNHOOK,

		ASM,

		// riscv register set
		X0,   X1, X2, X3, X4, X5, X6, X7, X8,     X9, X10, X11, X12, X13, X14, X15, X16, X17, X18, X19, X20, X21, X22, X23, X24, X25, X26, X27, X28, X29, X30, X31,
		ZERO, RA, SP, GP, TP, T0, T1, T2, S0, FP, S1, A0,  A1,  A2,  A3,  A4,  A5,  A6,  A7,  S2,  S3,  S4,  S5,  S6,  S7,  S8,  S9,  S10, S11, T3,  T4,  T5,  T6,
		
		// I instruction set
		LB, LH, LW, LBU, LHU,
		SB, SH, SW,
		LUI, AUIPC,
		ADDI, ADD, SUB,
		SLTI, SLT, SLTIU, SLTU,
		ANDI, AND, ORI, OR, XORI, XOR,
		SLLI, SLL, SRLI, SRL, SRAI, SRA,
		JAL, JALR,
		BEQ, BNE, BLT, BGE, BLTU, BGEU,
		NOP, ECALL, EBREAK, WFI, FENCE,

		// M extension
		MUL, MULH, MULHU, MULHSU, DIV, DIVU, REM, REMU,

		// A extension
		LRW, SCW,
		AMOSWAPW, AMOADDW, AMOXORW, AMOANDW, AMOORW, AMOMINW, AMOMAXW, AMOMINUW, AMOMAXUW,

		// C extension
		CLWSP, CSWSP, CLW, CSW, 
		CJ, CJAL, CJR, CJALR, 
		CBEQZ, CBNEZ, 
		CLI, CLUI, 
		CADDI, CADDI16SP, CADDI4SPN,
		CSLLI, CSRLI, CSRAI, 
		CANDI, CMV, CADD, CAND, COR, CXOR, CSUB, 
		CNOP, CEBREAK,

		// Zicsr extension
		CSRRW, CSRRS, CSRRC, CSRRWI, CSRRSI, CSRRCI,

		// Zifencei extension
		FENCEI,

		// U/S mode
		URET, SRET, MRET,

		PRINT,

		END, ERROR,

		DECLARE,

		U8, U16, U32, U64,
		/*S8,*/ S16, S32, S64,
	};

#define S(x) std::string_view{ x }

	static const Bimap<std::string_view, TokenType> _token_map
	{
		bimap_t{ S(";"), TokenType::SEMICOLON },
		bimap_t{ S(":"), TokenType::COLON },
		bimap_t{ S(","), TokenType::COMMA },
		bimap_t{ S("="), TokenType::EQUALS },
		bimap_t{ S("&"), TokenType::AMPERSAND },
		bimap_t{ S("#"), TokenType::POUND },
		bimap_t{ S("("), TokenType::LPAREN  },
		bimap_t{ S(")"), TokenType::RPAREN },
		bimap_t{ S("{"), TokenType::LBRACE },
		bimap_t{ S("}"), TokenType::RBRACE },
		bimap_t{ S("["), TokenType::LBRACKET },
		bimap_t{ S("]"), TokenType::RBRACKET },
		bimap_t{ S("+"), TokenType::PLUS },
		bimap_t{ S("-"), TokenType::MINUS },
		bimap_t{ S("*"), TokenType::STAR },
		bimap_t{ S("=="), TokenType::EQUALSEQUALS },
		bimap_t{ S("!="), TokenType::EXCLAMATIONEQUALS },
		bimap_t{ S(">"), TokenType::GREATER },
		bimap_t{ S("<"), TokenType::LESS },
		bimap_t{ S("~"), TokenType::TILDE },
		bimap_t{ S("!"), TokenType::EXCLAMATION },
		
		bimap_t{ S("immutable"), TokenType::IMMUTABLE },
		bimap_t{ S("mutable"), TokenType::MUTABLE },

		bimap_t{ S("unsigned"), TokenType::UNSIGNED },
		bimap_t{ S("signed"), TokenType::SIGNED },

		bimap_t{ S("value"), TokenType::VALUE },
		bimap_t{ S("ptr"), TokenType::PTR },

		bimap_t{ S("struct"), TokenType::STRUCT },

		bimap_t{ S("function"), TokenType::FUNCTION },
		bimap_t{ S("nvr"), TokenType::NVR},
		bimap_t{ S("return"), TokenType::RETURN },
		bimap_t{ S("while"), TokenType::WHILE },
		bimap_t{ S("for"), TokenType::FOR },
		bimap_t{ S("if"), TokenType::IF },
		bimap_t{ S("else"), TokenType::ELSE },
		bimap_t{ S("asm"), TokenType::ASM },
		
		bimap_t{ S("intrinsic"), TokenType::INTRINSIC },
		bimap_t{ S("print"), TokenType::PRINT },
		bimap_t{ S("declare"), TokenType::DECLARE },
		
		bimap_t{ S(".define"), TokenType::DOTDEFINE },
		bimap_t{ S(".org"), TokenType::DOTORG },
		
		bimap_t{ S(".hook"), TokenType::DOTHOOK },
		bimap_t{ S(".unhook"), TokenType::DOTUNHOOK },
		
		bimap_t{ S("x0"), TokenType::X0 },
		bimap_t{ S("x1"), TokenType::X1 },
		bimap_t{ S("x2"), TokenType::X2 },
		bimap_t{ S("x3"), TokenType::X3 },
		bimap_t{ S("x4"), TokenType::X4 },
		bimap_t{ S("x5"), TokenType::X5 },
		bimap_t{ S("x6"), TokenType::X6 },
		bimap_t{ S("x7"), TokenType::X7 },
		bimap_t{ S("x8"), TokenType::X8 },
		bimap_t{ S("x9"), TokenType::X9 },
		bimap_t{ S("x10"), TokenType::X10 },
		bimap_t{ S("x11"), TokenType::X11 },
		bimap_t{ S("x12"), TokenType::X12 },
		bimap_t{ S("x13"), TokenType::X13 },
		bimap_t{ S("x14"), TokenType::X14 },
		bimap_t{ S("x15"), TokenType::X15 },
		bimap_t{ S("x16"), TokenType::X16 },
		bimap_t{ S("x17"), TokenType::X17 },
		bimap_t{ S("x18"), TokenType::X18 },
		bimap_t{ S("x19"), TokenType::X19 },
		bimap_t{ S("x20"), TokenType::X20 },
		bimap_t{ S("x21"), TokenType::X21 },
		bimap_t{ S("x22"), TokenType::X22 },
		bimap_t{ S("x23"), TokenType::X23 },
		bimap_t{ S("x24"), TokenType::X24 },
		bimap_t{ S("x25"), TokenType::X25 },
		bimap_t{ S("x26"), TokenType::X26 },
		bimap_t{ S("x27"), TokenType::X27 },
		bimap_t{ S("x28"), TokenType::X28 },
		bimap_t{ S("x29"), TokenType::X29 },
		bimap_t{ S("x30"), TokenType::X30 },
		bimap_t{ S("x31"), TokenType::X31 },

		bimap_t{ S("lb"), TokenType::LB },
		bimap_t{ S("lh"), TokenType::LH },
		bimap_t{ S("lw"), TokenType::LW },
		bimap_t{ S("lbu"), TokenType::LBU },
		bimap_t{ S("lhu"), TokenType::LHU },
		bimap_t{ S("sb"), TokenType::SB },
		bimap_t{ S("sh"), TokenType::SH },
		bimap_t{ S("sw"), TokenType::SW },
		bimap_t{ S("lui"), TokenType::LUI },
		bimap_t{ S("auipc"), TokenType::AUIPC },
		bimap_t{ S("addi"), TokenType::ADDI },
		bimap_t{ S("add"), TokenType::ADD },
		bimap_t{ S("sub"), TokenType::SUB },
		bimap_t{ S("slti"), TokenType::SLTI },
		bimap_t{ S("slt"), TokenType::SLT },
		bimap_t{ S("sltiu"), TokenType::SLTIU },
		bimap_t{ S("sltu"), TokenType::SLTU },
		bimap_t{ S("andi"), TokenType::ANDI },
		bimap_t{ S("and"), TokenType::AND },
		bimap_t{ S("ori"), TokenType::ORI },
		bimap_t{ S("or"), TokenType::OR },
		bimap_t{ S("xori"), TokenType::XORI },
		bimap_t{ S("xor"), TokenType::XOR },
		bimap_t{ S("slli"), TokenType::SLLI },
		bimap_t{ S("sll"), TokenType::SLL },
		bimap_t{ S("srli"), TokenType::SRLI },
		bimap_t{ S("srl"), TokenType::SRL },
		bimap_t{ S("srai"), TokenType::SRAI },
		bimap_t{ S("sra"), TokenType::SRA },
		bimap_t{ S("jal"), TokenType::JAL },
		bimap_t{ S("jalr"), TokenType::JALR },
		bimap_t{ S("beq"), TokenType::BEQ },
		bimap_t{ S("bne"), TokenType::BNE },
		bimap_t{ S("blt"), TokenType::BLT },
		bimap_t{ S("bge"), TokenType::BGE },
		bimap_t{ S("bltu"), TokenType::BLTU },
		bimap_t{ S("bgeu"), TokenType::BGEU },
		bimap_t{ S("nop"), TokenType::NOP },
		bimap_t{ S("ecall"), TokenType::ECALL },
		bimap_t{ S("ebreak"), TokenType::EBREAK },
		bimap_t{ S("wfi"), TokenType::WFI },
		bimap_t{ S("fence"), TokenType::FENCE },

		bimap_t{ S("mul"), TokenType::MUL },
		bimap_t{ S("mulh"), TokenType::MULH },
		bimap_t{ S("mulhu"), TokenType::MULHU },
		bimap_t{ S("mulhsu"), TokenType::MULHSU },
		bimap_t{ S("div"), TokenType::DIV },
		bimap_t{ S("divu"), TokenType::DIVU },
		bimap_t{ S("rem"), TokenType::REM },
		bimap_t{ S("remu"), TokenType::REMU },

		bimap_t{ S("lr.w"), TokenType::LRW },
		bimap_t{ S("sc.w"), TokenType::SCW },
		bimap_t{ S("amoswap.w"), TokenType::AMOSWAPW },
		bimap_t{ S("amoadd.w"), TokenType::AMOADDW },
		bimap_t{ S("amoxor.w"), TokenType::AMOXORW },
		bimap_t{ S("amoandw.w"), TokenType::AMOANDW },
		bimap_t{ S("amoorw.w"), TokenType::AMOORW },
		bimap_t{ S("amominw.w"), TokenType::AMOMINW },
		bimap_t{ S("amomaxw.w"), TokenType::AMOMAXW },
		bimap_t{ S("amominu.w"), TokenType::AMOMINUW },
		bimap_t{ S("amomaxu.w"), TokenType::AMOMAXUW },

		bimap_t{ S("c.lwsp"), TokenType::CLWSP },
		bimap_t{ S("c.swsp"), TokenType::CSWSP },
		bimap_t{ S("c.lw"), TokenType::CLW },
		bimap_t{ S("c.sw"), TokenType::CSW },
		bimap_t{ S("c.j"), TokenType::CJ },
		bimap_t{ S("c.jal"), TokenType::CJAL },
		bimap_t{ S("c.jr"), TokenType::CJR },
		bimap_t{ S("c.jalr"), TokenType::CJALR },
		bimap_t{ S("c.beqz"), TokenType::CBEQZ },
		bimap_t{ S("c.bnez"), TokenType::CBNEZ },
		bimap_t{ S("c.li"), TokenType::CLI },
		bimap_t{ S("c.lui"), TokenType::CLUI },
		bimap_t{ S("c.addi"), TokenType::CADDI },
		bimap_t{ S("c.addi16sp"), TokenType::CADDI16SP },
		bimap_t{ S("c.addi4spn"), TokenType::CADDI4SPN },
		bimap_t{ S("c.slli"), TokenType::CSLLI },
		bimap_t{ S("c.srli"), TokenType::CSRLI },
		bimap_t{ S("c.srai"), TokenType::CSRAI },
		bimap_t{ S("c.andi"), TokenType::CANDI },
		bimap_t{ S("c.mv"), TokenType::CMV },
		bimap_t{ S("c.add"), TokenType::CADD },
		bimap_t{ S("c.and"), TokenType::CAND },
		bimap_t{ S("c.or"), TokenType::COR },
		bimap_t{ S("c.xor"), TokenType::CXOR },
		bimap_t{ S("c.sub"), TokenType::CSUB },
		bimap_t{ S("c.nop"), TokenType::CNOP },
		bimap_t{ S("c.ebreak"), TokenType::CEBREAK },

		bimap_t{ S("c.srrw"), TokenType::CSRRW },
		bimap_t{ S("c.srrs"), TokenType::CSRRS },
		bimap_t{ S("c.srrc"), TokenType::CSRRC },
		bimap_t{ S("c.srrwi"), TokenType::CSRRWI },
		bimap_t{ S("c.srrsi"), TokenType::CSRRSI },
		bimap_t{ S("c.srrci"), TokenType::CSRRCI },

		bimap_t{ S("fencei"), TokenType::FENCEI },

		bimap_t{ S("uret"), TokenType::URET },
		bimap_t{ S("sret"), TokenType::SRET },
		bimap_t{ S("mret"), TokenType::MRET },

		bimap_t{ S("string"), TokenType::STRING },
		bimap_t{ S("[identifier]"), TokenType::IDENTIFIER },

		bimap_t{ S("eof"), TokenType::END },

		bimap_t{ S("u8"), TokenType::U8 },
		bimap_t{ S("u16"), TokenType::U16 },
		bimap_t{ S("u32"), TokenType::U32 },
		bimap_t{ S("u64"), TokenType::U64 },

		bimap_t{ S("s8"), TokenType::S8 },
		bimap_t{ S("s16"), TokenType::S16 },
		bimap_t{ S("s32"), TokenType::S32 },
		bimap_t{ S("s64"), TokenType::S64 },
	};

	struct SourceLocation
	{
		std::string filepath;
		// quick character index into the file
		std::size_t position;
		std::int32_t line, column;
	};

	struct Token
	{
		TokenType type;
		std::string text;
		SourceLocation location;
	};

	// NOTE: intentionally not marked constexpr to avoid errors about dropping qualifiers
	// could make certain inward-facing Token fields mutable, but this feels cleaner
	inline static const auto NULL_TOKEN = Token{ TokenType::END, "", { "unknown", 0, -1, -1 } };
	inline SourceLocation null_location(const std::string& filepath)
	{
		// specified file (optional) at (1, 1)
		return SourceLocation{ filepath, 0, 1, 1 };
	}
}

#endif
