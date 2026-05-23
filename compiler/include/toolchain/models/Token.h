#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

#include <utility/Bimap.h>

// Haze Token.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class TokenKind
	{
		IDENTIFIER,

		SEMICOLON,
		COLON,
		COMMA,
		EQUALS,

		POUND, POUNDPOUND,

		AMPERSAND, AMPERSANDEQUALS,
		AMPERSANDAMPERSAND,
		
		PIPE, PIPEEQUALS,
		PIPEPIPE,

		LPAREN, RPAREN,
		LBRACE, RBRACE,
		LBRACKET, RBRACKET,

		PLUS, MINUS, STAR, SLASH, PERCENT,
		PLUSEQUALS, MINUSEQUALS, STAREQUALS, SLASHEQUALS, PERCENTEQUALS,

		CARET, CARETEQUALS,

		EQUALSEQUALS, EXCLAMATIONEQUALS,

		GREATER, LESS,

		LESSLESS, GREATERGREATER,
		LESSLESSEQUALS, GREATERGREATEREQUALS,

		TILDE, EXCLAMATION,

		UNSIGNED,
		SIGNED,

		INLINE,
		EXTERN,
		STATIC,
		AUTO,
		REGISTER,
		VOLATILE,
		RESTRICT,
		CONST,
		ASM,

		TYPEDEF,
		STRUCT,
		UNION,
		ENUM,
		VOID,
		INT,
		CHAR,
		SHORT,
		LONG,
		FLOAT,
		DOUBLE,

		RETURN,
		DO,
		WHILE,
		FOR,
		IF,
		ELSE,

		ENDIF,
		IFDEF,
		IFNDEF,
		ELIF,
		ERROR,
		WARNING,
		INCLUDE,
		PRAGMA,
		DEFINE,
		UNDEF,
		LINE,
		DEFINED,

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

		END,
	};

#define S(x) std::string_view{ x }

	static const Bimap<std::string_view, TokenKind> token_map
	{
		bimap_t{ S("int"), TokenKind::INT },
		bimap_t{ S("identifier"), TokenKind::IDENTIFIER },

		bimap_t{ S(";"), TokenKind::SEMICOLON },
		bimap_t{ S(":"), TokenKind::COLON },
		bimap_t{ S(","), TokenKind::COMMA },
		bimap_t{ S("~"), TokenKind::TILDE },
		bimap_t{ S("!"), TokenKind::EXCLAMATION },
		bimap_t{ S("="), TokenKind::EQUALS },
		bimap_t{ S("=="), TokenKind::EQUALSEQUALS },
		bimap_t{ S("!="), TokenKind::EXCLAMATIONEQUALS },
		bimap_t{ S(">"), TokenKind::GREATER },
		bimap_t{ S(">="), TokenKind::GREATERGREATEREQUALS },
		bimap_t{ S("<"), TokenKind::LESS },
		bimap_t{ S("<="), TokenKind::LESSLESSEQUALS },

		bimap_t{ S("#"), TokenKind::POUND },
		bimap_t{ S("##"), TokenKind::POUNDPOUND },

		bimap_t{ S("("), TokenKind::LPAREN },
		bimap_t{ S(")"), TokenKind::RPAREN },
		bimap_t{ S("{"), TokenKind::LBRACE },
		bimap_t{ S("}"), TokenKind::RBRACE },
		bimap_t{ S("["), TokenKind::LBRACKET },
		bimap_t{ S("]"), TokenKind::RBRACKET },

		bimap_t{ S("+"), TokenKind::PLUS },
		bimap_t{ S("+="), TokenKind::PLUSEQUALS },
		bimap_t{ S("-"), TokenKind::MINUS },
		bimap_t{ S("-="), TokenKind::MINUSEQUALS },
		bimap_t{ S("*"), TokenKind::STAR },
		bimap_t{ S("*="), TokenKind::STAREQUALS },
		bimap_t{ S("/"), TokenKind::SLASH },
		bimap_t{ S("/="), TokenKind::SLASHEQUALS },
		bimap_t{ S("%"), TokenKind::PERCENT },
		bimap_t{ S("%="), TokenKind::PERCENTEQUALS },
		bimap_t{ S("^"), TokenKind::CARET },
		bimap_t{ S("^="), TokenKind::CARETEQUALS },
		bimap_t{ S("&"), TokenKind::AMPERSAND },
		bimap_t{ S("&="), TokenKind::AMPERSANDEQUALS },
		bimap_t{ S("&&"), TokenKind::AMPERSANDAMPERSAND },
		bimap_t{ S("|"), TokenKind::PIPE },
		bimap_t{ S("|="), TokenKind::PIPEEQUALS },
		bimap_t{ S("||"), TokenKind::PIPEPIPE },
		bimap_t{ S("<<"), TokenKind::LESSLESS },
		bimap_t{ S("<<="), TokenKind::LESSLESSEQUALS },
		bimap_t{ S(">>"), TokenKind::GREATERGREATER },
		bimap_t{ S(">>="), TokenKind::GREATERGREATEREQUALS },

		bimap_t{ S("unsigned"), TokenKind::UNSIGNED },
		bimap_t{ S("signed"), TokenKind::SIGNED },
		
		bimap_t{ S("inline"), TokenKind::INLINE },
		bimap_t{ S("extern"), TokenKind::EXTERN },
		bimap_t{ S("static"), TokenKind::STATIC },
		bimap_t{ S("auto"), TokenKind::AUTO },
		bimap_t{ S("register"), TokenKind::REGISTER },
		bimap_t{ S("volatile"), TokenKind::VOLATILE },
		bimap_t{ S("restrict"), TokenKind::RESTRICT },
		bimap_t{ S("const"), TokenKind::CONST },
		bimap_t{ S("asm"), TokenKind::ASM },

		bimap_t{ S("typedef"), TokenKind::TYPEDEF },
		bimap_t{ S("struct"), TokenKind::STRUCT },
		bimap_t{ S("union"), TokenKind::UNION },
		bimap_t{ S("enum"), TokenKind::ENUM },
		bimap_t{ S("void"), TokenKind::VOID },
		bimap_t{ S("int"), TokenKind::INT },
		bimap_t{ S("char"), TokenKind::CHAR },
		bimap_t{ S("short"), TokenKind::SHORT },
		bimap_t{ S("long"), TokenKind::LONG },
		bimap_t{ S("float"), TokenKind::FLOAT },
		bimap_t{ S("double"), TokenKind::DOUBLE },

		bimap_t{ S("return"), TokenKind::RETURN },
		bimap_t{ S("do"), TokenKind::DO },
		bimap_t{ S("while"), TokenKind::WHILE },
		bimap_t{ S("for"), TokenKind::FOR },
		bimap_t{ S("if"), TokenKind::IF },
		bimap_t{ S("else"), TokenKind::ELSE },

		bimap_t{ S("ifdef"), TokenKind::IFDEF },
		bimap_t{ S("ifndef"), TokenKind::IFNDEF },
		bimap_t{ S("elif"), TokenKind::ELIF },
		bimap_t{ S("endif"), TokenKind::ENDIF },
		bimap_t{ S("error"), TokenKind::ERROR },
		bimap_t{ S("warning"), TokenKind::WARNING },
		bimap_t{ S("include"), TokenKind::INCLUDE },
		bimap_t{ S("pragma"), TokenKind::PRAGMA },
		bimap_t{ S("define"), TokenKind::DEFINE },
		bimap_t{ S("undef"), TokenKind::UNDEF },
		bimap_t{ S("line"), TokenKind::LINE },
		bimap_t{ S("defined"), TokenKind::DEFINED },
		
		bimap_t{ S("x0"), TokenKind::X0 },
		bimap_t{ S("x1"), TokenKind::X1 },
		bimap_t{ S("x2"), TokenKind::X2 },
		bimap_t{ S("x3"), TokenKind::X3 },
		bimap_t{ S("x4"), TokenKind::X4 },
		bimap_t{ S("x5"), TokenKind::X5 },
		bimap_t{ S("x6"), TokenKind::X6 },
		bimap_t{ S("x7"), TokenKind::X7 },
		bimap_t{ S("x8"), TokenKind::X8 },
		bimap_t{ S("x9"), TokenKind::X9 },
		bimap_t{ S("x10"), TokenKind::X10 },
		bimap_t{ S("x11"), TokenKind::X11 },
		bimap_t{ S("x12"), TokenKind::X12 },
		bimap_t{ S("x13"), TokenKind::X13 },
		bimap_t{ S("x14"), TokenKind::X14 },
		bimap_t{ S("x15"), TokenKind::X15 },
		bimap_t{ S("x16"), TokenKind::X16 },
		bimap_t{ S("x17"), TokenKind::X17 },
		bimap_t{ S("x18"), TokenKind::X18 },
		bimap_t{ S("x19"), TokenKind::X19 },
		bimap_t{ S("x20"), TokenKind::X20 },
		bimap_t{ S("x21"), TokenKind::X21 },
		bimap_t{ S("x22"), TokenKind::X22 },
		bimap_t{ S("x23"), TokenKind::X23 },
		bimap_t{ S("x24"), TokenKind::X24 },
		bimap_t{ S("x25"), TokenKind::X25 },
		bimap_t{ S("x26"), TokenKind::X26 },
		bimap_t{ S("x27"), TokenKind::X27 },
		bimap_t{ S("x28"), TokenKind::X28 },
		bimap_t{ S("x29"), TokenKind::X29 },
		bimap_t{ S("x30"), TokenKind::X30 },
		bimap_t{ S("x31"), TokenKind::X31 },

		bimap_t{ S("lb"), TokenKind::LB },
		bimap_t{ S("lh"), TokenKind::LH },
		bimap_t{ S("lw"), TokenKind::LW },
		bimap_t{ S("lbu"), TokenKind::LBU },
		bimap_t{ S("lhu"), TokenKind::LHU },
		bimap_t{ S("sb"), TokenKind::SB },
		bimap_t{ S("sh"), TokenKind::SH },
		bimap_t{ S("sw"), TokenKind::SW },
		bimap_t{ S("lui"), TokenKind::LUI },
		bimap_t{ S("auipc"), TokenKind::AUIPC },
		bimap_t{ S("addi"), TokenKind::ADDI },
		bimap_t{ S("add"), TokenKind::ADD },
		bimap_t{ S("sub"), TokenKind::SUB },
		bimap_t{ S("slti"), TokenKind::SLTI },
		bimap_t{ S("slt"), TokenKind::SLT },
		bimap_t{ S("sltiu"), TokenKind::SLTIU },
		bimap_t{ S("sltu"), TokenKind::SLTU },
		bimap_t{ S("andi"), TokenKind::ANDI },
		bimap_t{ S("and"), TokenKind::AND },
		bimap_t{ S("ori"), TokenKind::ORI },
		bimap_t{ S("or"), TokenKind::OR },
		bimap_t{ S("xori"), TokenKind::XORI },
		bimap_t{ S("xor"), TokenKind::XOR },
		bimap_t{ S("slli"), TokenKind::SLLI },
		bimap_t{ S("sll"), TokenKind::SLL },
		bimap_t{ S("srli"), TokenKind::SRLI },
		bimap_t{ S("srl"), TokenKind::SRL },
		bimap_t{ S("srai"), TokenKind::SRAI },
		bimap_t{ S("sra"), TokenKind::SRA },
		bimap_t{ S("jal"), TokenKind::JAL },
		bimap_t{ S("jalr"), TokenKind::JALR },
		bimap_t{ S("beq"), TokenKind::BEQ },
		bimap_t{ S("bne"), TokenKind::BNE },
		bimap_t{ S("blt"), TokenKind::BLT },
		bimap_t{ S("bge"), TokenKind::BGE },
		bimap_t{ S("bltu"), TokenKind::BLTU },
		bimap_t{ S("bgeu"), TokenKind::BGEU },
		bimap_t{ S("nop"), TokenKind::NOP },
		bimap_t{ S("ecall"), TokenKind::ECALL },
		bimap_t{ S("ebreak"), TokenKind::EBREAK },
		bimap_t{ S("wfi"), TokenKind::WFI },
		bimap_t{ S("fence"), TokenKind::FENCE },

		bimap_t{ S("mul"), TokenKind::MUL },
		bimap_t{ S("mulh"), TokenKind::MULH },
		bimap_t{ S("mulhu"), TokenKind::MULHU },
		bimap_t{ S("mulhsu"), TokenKind::MULHSU },
		bimap_t{ S("div"), TokenKind::DIV },
		bimap_t{ S("divu"), TokenKind::DIVU },
		bimap_t{ S("rem"), TokenKind::REM },
		bimap_t{ S("remu"), TokenKind::REMU },

		bimap_t{ S("lr.w"), TokenKind::LRW },
		bimap_t{ S("sc.w"), TokenKind::SCW },
		bimap_t{ S("amoswap.w"), TokenKind::AMOSWAPW },
		bimap_t{ S("amoadd.w"), TokenKind::AMOADDW },
		bimap_t{ S("amoxor.w"), TokenKind::AMOXORW },
		bimap_t{ S("amoandw.w"), TokenKind::AMOANDW },
		bimap_t{ S("amoorw.w"), TokenKind::AMOORW },
		bimap_t{ S("amominw.w"), TokenKind::AMOMINW },
		bimap_t{ S("amomaxw.w"), TokenKind::AMOMAXW },
		bimap_t{ S("amominu.w"), TokenKind::AMOMINUW },
		bimap_t{ S("amomaxu.w"), TokenKind::AMOMAXUW },

		bimap_t{ S("c.lwsp"), TokenKind::CLWSP },
		bimap_t{ S("c.swsp"), TokenKind::CSWSP },
		bimap_t{ S("c.lw"), TokenKind::CLW },
		bimap_t{ S("c.sw"), TokenKind::CSW },
		bimap_t{ S("c.j"), TokenKind::CJ },
		bimap_t{ S("c.jal"), TokenKind::CJAL },
		bimap_t{ S("c.jr"), TokenKind::CJR },
		bimap_t{ S("c.jalr"), TokenKind::CJALR },
		bimap_t{ S("c.beqz"), TokenKind::CBEQZ },
		bimap_t{ S("c.bnez"), TokenKind::CBNEZ },
		bimap_t{ S("c.li"), TokenKind::CLI },
		bimap_t{ S("c.lui"), TokenKind::CLUI },
		bimap_t{ S("c.addi"), TokenKind::CADDI },
		bimap_t{ S("c.addi16sp"), TokenKind::CADDI16SP },
		bimap_t{ S("c.addi4spn"), TokenKind::CADDI4SPN },
		bimap_t{ S("c.slli"), TokenKind::CSLLI },
		bimap_t{ S("c.srli"), TokenKind::CSRLI },
		bimap_t{ S("c.srai"), TokenKind::CSRAI },
		bimap_t{ S("c.andi"), TokenKind::CANDI },
		bimap_t{ S("c.mv"), TokenKind::CMV },
		bimap_t{ S("c.add"), TokenKind::CADD },
		bimap_t{ S("c.and"), TokenKind::CAND },
		bimap_t{ S("c.or"), TokenKind::COR },
		bimap_t{ S("c.xor"), TokenKind::CXOR },
		bimap_t{ S("c.sub"), TokenKind::CSUB },
		bimap_t{ S("c.nop"), TokenKind::CNOP },
		bimap_t{ S("c.ebreak"), TokenKind::CEBREAK },

		bimap_t{ S("c.srrw"), TokenKind::CSRRW },
		bimap_t{ S("c.srrs"), TokenKind::CSRRS },
		bimap_t{ S("c.srrc"), TokenKind::CSRRC },
		bimap_t{ S("c.srrwi"), TokenKind::CSRRWI },
		bimap_t{ S("c.srrsi"), TokenKind::CSRRSI },
		bimap_t{ S("c.srrci"), TokenKind::CSRRCI },

		bimap_t{ S("fencei"), TokenKind::FENCEI },

		bimap_t{ S("uret"), TokenKind::URET },
		bimap_t{ S("sret"), TokenKind::SRET },
		bimap_t{ S("mret"), TokenKind::MRET },

		bimap_t{ S("[identifier]"), TokenKind::IDENTIFIER },

		bimap_t{ S("eof"), TokenKind::END },
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
		TokenKind type;
		std::string text;
		SourceLocation location;
	};

	// NOTE: intentionally not marked constexpr to avoid errors about dropping qualifiers
	// could make certain inward-facing Token fields mutable, but this feels cleaner
	inline static const auto NULL_TOKEN = Token{ TokenKind::END, "", { "unknown", 0, -1, -1 } };
	inline SourceLocation null_location(const std::string& filepath)
	{
		// specified file (optional) at (1, 1)
		return SourceLocation{ filepath, 0, 1, 1 };
	}
}

template<>
struct std::formatter<hz::TokenKind>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}
	auto format(const hz::TokenKind& token, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", hz::token_map.at(token).value_or("error"));
	}
};

#endif
