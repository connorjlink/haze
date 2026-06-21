#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

#include <utility/Bimap.h>

// Haze Token.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct TokenKind
	{
		IDENTIFIER,

		SEMICOLON,
		COLON,
		COMMA,
		EQUALS,
		PERIOD,

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
		GREATEREQUALS, LESSEQUALS,

		LESSLESS, GREATERGREATER,
		LESSLESSEQUALS, GREATERGREATEREQUALS,

		TILDE, EXCLAMATION,
		QUOTE, DOUBLEQUOTE,

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
		DOTORG,
		DOTBYTE,
		DOTGLOBAL,

		INTEGER_LITERAL,
		FLOAT_LITERAL,
		STRING_LITERAL,
		WIDE_STRING_LITERAL,
		CHARACTER_LITERAL,

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

	using namespace std::literals;

	static const Bimap<std::string_view, TokenKind> token_map
	{
		bimap_t{ "int"sv, TokenKind::INT },
		bimap_t{ "identifier"sv, TokenKind::IDENTIFIER },

		bimap_t{ ";"sv, TokenKind::SEMICOLON },
		bimap_t{ ":"sv, TokenKind::COLON },
		bimap_t{ ","sv, TokenKind::COMMA },
		bimap_t{ "."sv, TokenKind::PERIOD },
		bimap_t{ "~"sv, TokenKind::TILDE },
		bimap_t{ "!"sv, TokenKind::EXCLAMATION },
		bimap_t{ "\'"sv, TokenKind::QUOTE },
		bimap_t{ "\""sv, TokenKind::DOUBLEQUOTE },
		bimap_t{ "="sv, TokenKind::EQUALS },
		bimap_t{ "=="sv, TokenKind::EQUALSEQUALS },
		bimap_t{ "!="sv, TokenKind::EXCLAMATIONEQUALS },
		bimap_t{ ">"sv, TokenKind::GREATER },
		bimap_t{ ">="sv, TokenKind::GREATEREQUALS },
		bimap_t{ "<"sv, TokenKind::LESS },
		bimap_t{ "<="sv, TokenKind::LESSEQUALS },

		bimap_t{ "#"sv, TokenKind::POUND },
		bimap_t{ "##"sv, TokenKind::POUNDPOUND },

		bimap_t{ "("sv, TokenKind::LPAREN },
		bimap_t{ ")"sv, TokenKind::RPAREN },
		bimap_t{ "{"sv, TokenKind::LBRACE },
		bimap_t{ "}"sv, TokenKind::RBRACE },
		bimap_t{ "["sv, TokenKind::LBRACKET },
		bimap_t{ "]"sv, TokenKind::RBRACKET },

		bimap_t{ "+"sv, TokenKind::PLUS },
		bimap_t{ "+="sv, TokenKind::PLUSEQUALS },
		bimap_t{ "-"sv, TokenKind::MINUS },
		bimap_t{ "-="sv, TokenKind::MINUSEQUALS },
		bimap_t{ "*"sv, TokenKind::STAR },
		bimap_t{ "*="sv, TokenKind::STAREQUALS },
		bimap_t{ "/"sv, TokenKind::SLASH },
		bimap_t{ "/="sv, TokenKind::SLASHEQUALS },
		bimap_t{ "%"sv, TokenKind::PERCENT },
		bimap_t{ "%="sv, TokenKind::PERCENTEQUALS },
		bimap_t{ "^"sv, TokenKind::CARET },
		bimap_t{ "^="sv, TokenKind::CARETEQUALS },
		bimap_t{ "&"sv, TokenKind::AMPERSAND },
		bimap_t{ "&="sv, TokenKind::AMPERSANDEQUALS },
		bimap_t{ "&&"sv, TokenKind::AMPERSANDAMPERSAND },
		bimap_t{ "|"sv, TokenKind::PIPE },
		bimap_t{ "|="sv, TokenKind::PIPEEQUALS },
		bimap_t{ "||"sv, TokenKind::PIPEPIPE },
		bimap_t{ "<<"sv, TokenKind::LESSLESS },
		bimap_t{ "<<="sv, TokenKind::LESSLESSEQUALS },
		bimap_t{ ">>"sv, TokenKind::GREATERGREATER },
		bimap_t{ ">>="sv, TokenKind::GREATERGREATEREQUALS },

		bimap_t{ "unsigned"sv, TokenKind::UNSIGNED },
		bimap_t{ "signed"sv, TokenKind::SIGNED },
		
		bimap_t{ "inline"sv, TokenKind::INLINE },
		bimap_t{ "extern"sv, TokenKind::EXTERN },
		bimap_t{ "static"sv, TokenKind::STATIC },
		bimap_t{ "auto"sv, TokenKind::AUTO },
		bimap_t{ "register"sv, TokenKind::REGISTER },
		bimap_t{ "volatile"sv, TokenKind::VOLATILE },
		bimap_t{ "restrict"sv, TokenKind::RESTRICT },
		bimap_t{ "const"sv, TokenKind::CONST },
		bimap_t{ "asm"sv, TokenKind::ASM },

		bimap_t{ "typedef"sv, TokenKind::TYPEDEF },
		bimap_t{ "struct"sv, TokenKind::STRUCT },
		bimap_t{ "union"sv, TokenKind::UNION },
		bimap_t{ "enum"sv, TokenKind::ENUM },
		bimap_t{ "void"sv, TokenKind::VOID },
		bimap_t{ "int"sv, TokenKind::INT },
		bimap_t{ "char"sv, TokenKind::CHAR },
		bimap_t{ "short"sv, TokenKind::SHORT },
		bimap_t{ "long"sv, TokenKind::LONG },
		bimap_t{ "float"sv, TokenKind::FLOAT },
		bimap_t{ "double"sv, TokenKind::DOUBLE },

		bimap_t{ "return"sv, TokenKind::RETURN },
		bimap_t{ "do"sv, TokenKind::DO },
		bimap_t{ "while"sv, TokenKind::WHILE },
		bimap_t{ "for"sv, TokenKind::FOR },
		bimap_t{ "if"sv, TokenKind::IF },
		bimap_t{ "else"sv, TokenKind::ELSE },

		bimap_t{ "ifdef"sv, TokenKind::IFDEF },
		bimap_t{ "ifndef"sv, TokenKind::IFNDEF },
		bimap_t{ "elif"sv, TokenKind::ELIF },
		bimap_t{ "endif"sv, TokenKind::ENDIF },
		bimap_t{ "error"sv, TokenKind::ERROR },
		bimap_t{ "warning"sv, TokenKind::WARNING },
		bimap_t{ "include"sv, TokenKind::INCLUDE },
		bimap_t{ "pragma"sv, TokenKind::PRAGMA },
		bimap_t{ "define"sv, TokenKind::DEFINE },
		bimap_t{ "undef"sv, TokenKind::UNDEF },
		bimap_t{ "line"sv, TokenKind::LINE },
		bimap_t{ "defined"sv, TokenKind::DEFINED },
		bimap_t{ ".org"sv, TokenKind::DOTORG },
		bimap_t{ ".byte"sv, TokenKind::DOTBYTE },
		bimap_t{ ".global"sv, TokenKind::DOTGLOBAL },
		
		bimap_t{ "x0"sv, TokenKind::X0 },
		bimap_t{ "x1"sv, TokenKind::X1 },
		bimap_t{ "x2"sv, TokenKind::X2 },
		bimap_t{ "x3"sv, TokenKind::X3 },
		bimap_t{ "x4"sv, TokenKind::X4 },
		bimap_t{ "x5"sv, TokenKind::X5 },
		bimap_t{ "x6"sv, TokenKind::X6 },
		bimap_t{ "x7"sv, TokenKind::X7 },
		bimap_t{ "x8"sv, TokenKind::X8 },
		bimap_t{ "x9"sv, TokenKind::X9 },
		bimap_t{ "x10"sv, TokenKind::X10 },
		bimap_t{ "x11"sv, TokenKind::X11 },
		bimap_t{ "x12"sv, TokenKind::X12 },
		bimap_t{ "x13"sv, TokenKind::X13 },
		bimap_t{ "x14"sv, TokenKind::X14 },
		bimap_t{ "x15"sv, TokenKind::X15 },
		bimap_t{ "x16"sv, TokenKind::X16 },
		bimap_t{ "x17"sv, TokenKind::X17 },
		bimap_t{ "x18"sv, TokenKind::X18 },
		bimap_t{ "x19"sv, TokenKind::X19 },
		bimap_t{ "x20"sv, TokenKind::X20 },
		bimap_t{ "x21"sv, TokenKind::X21 },
		bimap_t{ "x22"sv, TokenKind::X22 },
		bimap_t{ "x23"sv, TokenKind::X23 },
		bimap_t{ "x24"sv, TokenKind::X24 },
		bimap_t{ "x25"sv, TokenKind::X25 },
		bimap_t{ "x26"sv, TokenKind::X26 },
		bimap_t{ "x27"sv, TokenKind::X27 },
		bimap_t{ "x28"sv, TokenKind::X28 },
		bimap_t{ "x29"sv, TokenKind::X29 },
		bimap_t{ "x30"sv, TokenKind::X30 },
		bimap_t{ "x31"sv, TokenKind::X31 },

		bimap_t{ "lb"sv, TokenKind::LB },
		bimap_t{ "lh"sv, TokenKind::LH },
		bimap_t{ "lw"sv, TokenKind::LW },
		bimap_t{ "lbu"sv, TokenKind::LBU },
		bimap_t{ "lhu"sv, TokenKind::LHU },
		bimap_t{ "sb"sv, TokenKind::SB },
		bimap_t{ "sh"sv, TokenKind::SH },
		bimap_t{ "sw"sv, TokenKind::SW },
		bimap_t{ "lui"sv, TokenKind::LUI },
		bimap_t{ "auipc"sv, TokenKind::AUIPC },
		bimap_t{ "addi"sv, TokenKind::ADDI },
		bimap_t{ "add"sv, TokenKind::ADD },
		bimap_t{ "sub"sv, TokenKind::SUB },
		bimap_t{ "slti"sv, TokenKind::SLTI },
		bimap_t{ "slt"sv, TokenKind::SLT },
		bimap_t{ "sltiu"sv, TokenKind::SLTIU },
		bimap_t{ "sltu"sv, TokenKind::SLTU },
		bimap_t{ "andi"sv, TokenKind::ANDI },
		bimap_t{ "and"sv, TokenKind::AND },
		bimap_t{ "ori"sv, TokenKind::ORI },
		bimap_t{ "or"sv, TokenKind::OR },
		bimap_t{ "xori"sv, TokenKind::XORI },
		bimap_t{ "xor"sv, TokenKind::XOR },
		bimap_t{ "slli"sv, TokenKind::SLLI },
		bimap_t{ "sll"sv, TokenKind::SLL },
		bimap_t{ "srli"sv, TokenKind::SRLI },
		bimap_t{ "srl"sv, TokenKind::SRL },
		bimap_t{ "srai"sv, TokenKind::SRAI },
		bimap_t{ "sra"sv, TokenKind::SRA },
		bimap_t{ "jal"sv, TokenKind::JAL },
		bimap_t{ "jalr"sv, TokenKind::JALR },
		bimap_t{ "beq"sv, TokenKind::BEQ },
		bimap_t{ "bne"sv, TokenKind::BNE },
		bimap_t{ "blt"sv, TokenKind::BLT },
		bimap_t{ "bge"sv, TokenKind::BGE },
		bimap_t{ "bltu"sv, TokenKind::BLTU },
		bimap_t{ "bgeu"sv, TokenKind::BGEU },
		bimap_t{ "nop"sv, TokenKind::NOP },
		bimap_t{ "ecall"sv, TokenKind::ECALL },
		bimap_t{ "ebreak"sv, TokenKind::EBREAK },
		bimap_t{ "wfi"sv, TokenKind::WFI },
		bimap_t{ "fence"sv, TokenKind::FENCE },

		bimap_t{ "mul"sv, TokenKind::MUL },
		bimap_t{ "mulh"sv, TokenKind::MULH },
		bimap_t{ "mulhu"sv, TokenKind::MULHU },
		bimap_t{ "mulhsu"sv, TokenKind::MULHSU },
		bimap_t{ "div"sv, TokenKind::DIV },
		bimap_t{ "divu"sv, TokenKind::DIVU },
		bimap_t{ "rem"sv, TokenKind::REM },
		bimap_t{ "remu"sv, TokenKind::REMU },

		bimap_t{ "lr.w"sv, TokenKind::LRW },
		bimap_t{ "sc.w"sv, TokenKind::SCW },
		bimap_t{ "amoswap.w"sv, TokenKind::AMOSWAPW },
		bimap_t{ "amoadd.w"sv, TokenKind::AMOADDW },
		bimap_t{ "amoxor.w"sv, TokenKind::AMOXORW },
		bimap_t{ "amoandw.w"sv, TokenKind::AMOANDW },
		bimap_t{ "amoorw.w"sv, TokenKind::AMOORW },
		bimap_t{ "amominw.w"sv, TokenKind::AMOMINW },
		bimap_t{ "amomaxw.w"sv, TokenKind::AMOMAXW },
		bimap_t{ "amominu.w"sv, TokenKind::AMOMINUW },
		bimap_t{ "amomaxu.w"sv, TokenKind::AMOMAXUW },

		bimap_t{ "c.lwsp"sv, TokenKind::CLWSP },
		bimap_t{ "c.swsp"sv, TokenKind::CSWSP },
		bimap_t{ "c.lw"sv, TokenKind::CLW },
		bimap_t{ "c.sw"sv, TokenKind::CSW },
		bimap_t{ "c.j"sv, TokenKind::CJ },
		bimap_t{ "c.jal"sv, TokenKind::CJAL },
		bimap_t{ "c.jr"sv, TokenKind::CJR },
		bimap_t{ "c.jalr"sv, TokenKind::CJALR },
		bimap_t{ "c.beqz"sv, TokenKind::CBEQZ },
		bimap_t{ "c.bnez"sv, TokenKind::CBNEZ },
		bimap_t{ "c.li"sv, TokenKind::CLI },
		bimap_t{ "c.lui"sv, TokenKind::CLUI },
		bimap_t{ "c.addi"sv, TokenKind::CADDI },
		bimap_t{ "c.addi16sp"sv, TokenKind::CADDI16SP },
		bimap_t{ "c.addi4spn"sv, TokenKind::CADDI4SPN },
		bimap_t{ "c.slli"sv, TokenKind::CSLLI },
		bimap_t{ "c.srli"sv, TokenKind::CSRLI },
		bimap_t{ "c.srai"sv, TokenKind::CSRAI },
		bimap_t{ "c.andi"sv, TokenKind::CANDI },
		bimap_t{ "c.mv"sv, TokenKind::CMV },
		bimap_t{ "c.add"sv, TokenKind::CADD },
		bimap_t{ "c.and"sv, TokenKind::CAND },
		bimap_t{ "c.or"sv, TokenKind::COR },
		bimap_t{ "c.xor"sv, TokenKind::CXOR },
		bimap_t{ "c.sub"sv, TokenKind::CSUB },
		bimap_t{ "c.nop"sv, TokenKind::CNOP },
		bimap_t{ "c.ebreak"sv, TokenKind::CEBREAK },

		bimap_t{ "c.srrw"sv, TokenKind::CSRRW },
		bimap_t{ "c.srrs"sv, TokenKind::CSRRS },
		bimap_t{ "c.srrc"sv, TokenKind::CSRRC },
		bimap_t{ "c.srrwi"sv, TokenKind::CSRRWI },
		bimap_t{ "c.srrsi"sv, TokenKind::CSRRSI },
		bimap_t{ "c.srrci"sv, TokenKind::CSRRCI },

		bimap_t{ "fencei"sv, TokenKind::FENCEI },

		bimap_t{ "uret"sv, TokenKind::URET },
		bimap_t{ "sret"sv, TokenKind::SRET },
		bimap_t{ "mret"sv, TokenKind::MRET },

		bimap_t{ "[identifier]"sv, TokenKind::IDENTIFIER },

		bimap_t{ "eof"sv, TokenKind::END },
	};

	struct SourceLocation
	{
		std::filesystem::path filepath;
		// quick character index into the file
		std::size_t position;
		std::int32_t line, column;
	};

	using TokenFloat = long double;
	using TokenInteger = long long int;

	using TokenValue = std::variant
	<
		std::monostate,
		TokenFloat,
		TokenInteger
	>;

	struct Token
	{
		std::string_view text;
		SourceLocation location;
		TokenKind kind;
		TokenValue value;
	};

	// NOTE: intentionally not marked constexpr to avoid errors about dropping qualifiers
	// could make certain inward-facing Token fields mutable, but this feels cleaner
	inline const auto NULL_TOKEN = Token
	{ 
		.text = "",
		.location = SourceLocation{ std::filesystem::path{ "unknown" }, 0, -1, -1 },
		.kind = TokenKind::END,
		.value = std::monostate{},
	};

	inline SourceLocation null_location(const std::filesystem::path& filepath)
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
