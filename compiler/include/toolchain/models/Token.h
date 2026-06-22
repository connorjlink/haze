#ifndef HAZE_TOKEN_H
#define HAZE_TOKEN_H

#include <utility/Bimap.h>

// Haze Token.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TOKEN_KINDS(X) \
	X(IDENTIFIER,           "<identifier>") \
	X(QUESTION,             "?") \
	X(SEMICOLON,            ";") \
	X(COLON,                ":") \
	X(COMMA,                ",") \
	X(PERIOD,               ".") \
	X(DOT,                  ".") \
	X(ARROW,                "->") \
	X(LPAREN,               "(") \
	X(RPAREN,               ")") \
	X(LBRACE,               "{") \
	X(RBRACE,               "}") \
	X(LBRACKET,             "[") \
	X(RBRACKET,             "]") \
	X(PLUS,                 "+") \
	X(MINUS,                "-") \
	X(STAR,                 "*") \
	X(SLASH,                "/") \
	X(PERCENT,              "%") \
	X(PLUSEQUALS,           "+=") \
	X(MINUSEQUALS,          "-=") \
	X(STAREQUALS,           "*=") \
	X(SLASHEQUALS,          "/=") \
	X(PERCENTEQUALS,        "%=") \
	X(CARET,                "^") \
	X(CARETEQUALS,          "^=") \
	X(AMPERSAND,            "&") \
	X(AMPERSANDEQUALS,      "&=") \
	X(AMPERSANDAMPERSAND,   "&&") \
	X(PIPE,                 "|") \
	X(PIPEEQUALS,           "|=") \
	X(PIPEPIPE,             "||") \
	X(EQUALS,               "=") \
	X(EQUALSEQUALS,         "==") \
	X(EXCLAMATION,          "!") \
	X(EXCLAMATIONEQUALS,    "!=") \
	X(GREATER,              ">") \
	X(GREATEREQUALS,        ">=") \
	X(GREATERGREATER,       ">>") \
	X(GREATERGREATEREQUALS, ">>=") \
	X(LESS,                 "<") \
	X(LESSEQUALS,           "<=") \
	X(LESSLESS,             "<<") \
	X(LESSLESSEQUALS,       "<<=") \
	X(TILDE,                "~") \
	X(QUOTE,                "\'") \
	X(DOUBLEQUOTE,          "\"") \
	X(POUND,                "#") \
	X(POUNDPOUND,           "##") \
	X(UNSIGNED,             "unsigned") \
	X(SIGNED,               "signed") \
	X(INLINE,               "inline") \
	X(EXTERN,               "extern") \
	X(STATIC,               "static") \
	X(AUTO,                 "auto") \
	X(REGISTER,             "register") \
	X(VOLATILE,             "volatile") \
	X(RESTRICT,             "restrict") \
	X(CONST,                "const") \
	X(ASM,                  "asm") \
	X(TYPEDEF,              "typedef") \
	X(STRUCT,               "struct") \
	X(UNION,                "union") \
	X(ENUM,                 "enum") \
	X(VOID,                 "void") \
	X(CHAR,                 "char") \
	X(SHORT,                "short") \
	X(INT,                  "int") \
	X(LONG,                 "long") \
	X(FLOAT,                "float") \
	X(DOUBLE,               "double") \
	X(RETURN,               "return") \
	X(DO,                   "do") \
	X(WHILE,                "while") \
	X(FOR,                  "for") \
	X(IF,                   "if") \
	X(ELSE,                 "else") \
	X(CONTINUE,             "continue") \
	X(BREAK,                "break") \
	X(IFDEF,                "ifdef") \
	X(IFNDEF,               "ifndef") \
	X(ELIF,                 "elif") \
	X(ERROR,                "error") \
	X(WARNING,              "warning") \
	X(INCLUDE,              "include") \
	X(pragma,               "pragma") \
	X(DEFINE,               "define") \
	X(UNDEF,                "undef") \
	X(ENDIF,                "endif") \
	X(LINE,                 "line") \
	X(DEFINED,              "defined") \
	X(DOTORG,               ".org") \
	X(DOTBYTE,              ".byte") \
	X(DOTGLOBAL,            ".global") \
	X(INTEGER_LITERAL,      "<integer literal>") \
	X(FLOAT_LITERAL,        "<float literal>") \
	X(STRING_LITERAL,       "<string literal>") \
	X(WIDE_STRING_LITERAL,  "<wide string literal>") \
	X(CHARACTER_LITERAL,    "<character literal>") \
	X(X0,                   "x0") \
	X(ZERO,                 "zero") \
	X(X1,                   "x1") \
	X(RA,                   "ra") \
	X(X2,                   "x2") \
	X(SP,                   "sp") \
	X(X3,                   "x3") \
	X(GP,                   "gp") \
	X(X4,                   "x4") \
	X(TP,                   "tp") \
	X(X5,                   "x5") \
	X(T0,                   "t0") \
	X(X6,                   "x6") \
	X(T1,                   "t1") \
	X(X7,                   "x7") \
	X(T2,                   "t2") \
	X(X8,                   "x8") \
	X(S0,                   "s0") \
	X(FP,                   "fp") \
	X(X9,                   "x9") \
	X(S1,                   "s1") \
	X(X10,                  "x10") \
	X(A0,                   "a0") \
	X(X11,                  "x11") \
	X(A1,                   "a1") \
	X(X12,                  "x12") \
	X(A2,                   "a2") \
	X(X13,                  "x13") \
	X(A3,                   "a3") \
	X(X14,                  "x14") \
	X(A4,                   "a4") \
	X(X15,                  "x15") \
	X(A5,                   "a5") \
	X(X16,                  "x16") \
	X(A6,                   "a6") \
	X(X17,                  "x17") \
	X(A7,                   "a7") \
	X(X18,                  "x18") \
	X(S2,                   "s2") \
	X(X19,                  "x19") \
	X(S3,                   "s3") \
	X(X20,                  "x20") \
	X(S4,                   "s4") \
	X(X21,                  "x21") \
	X(S5,                   "s5") \
	X(X22,                  "x22") \
	X(S6,                   "s6") \
	X(X23,                  "x23") \
	X(S7,                   "s7") \
	X(X24,                  "x24") \
	X(S8,                   "s8") \
	X(X25,                  "x25") \
	X(S9,                   "s9") \
	X(X26,                  "x26") \
	X(S10,                  "s10") \
	X(X27,                  "x27") \
	X(S11,                  "s11") \
	X(X28,                  "x28") \
	X(T3,                   "t3") \
	X(X29,                  "x29") \
	X(T4,                   "t4") \
	X(X30,                  "x30") \
	X(T5,                   "t5") \
	X(X31,                  "x31") \
	X(T6,                   "t6") \
	X(LB,                   "lb") \
	X(LH,                   "lh") \
	X(LW,                   "lw") \
	X(LBU,                  "lbu") \
	X(LHU,                  "lhu") \
	X(SB,                   "sb") \
	X(SH,                   "sh") \
	X(SW,                   "sw") \
	X(LUI,                  "lui") \
	X(AUIPC,                "auipc") \
	X(ADDI,                 "addi") \
	X(ADD,                  "add") \
	X(SUB,                  "sub") \
	X(SLTI,                 "slti") \
	X(SLT,                  "slt") \
	X(SLTIU,                "sltiu") \
	X(SLTU,                 "sltu") \
	X(ANDI,                 "andi") \
	X(AND,                  "and") \
	X(ORI,                  "ori") \
	X(OR,                   "or") \
	X(XORI,                 "xori") \
	X(XOR,                  "xor") \
	X(SLLI,                 "slli") \
	X(SLL,                  "sll") \
	X(SRLI,                 "srli") \
	X(SRL,                  "srl") \
	X(SRAI,                 "srai") \
	X(SRA,                  "sra") \
	X(JAL,                  "jal") \
	X(JALR,                 "jalr") \
	X(BEQ,                  "beq") \
	X(BNE,                  "bne") \
	X(BLT,                  "blt") \
	X(BGE,                  "bge") \
	X(BLTU,                 "bltu") \
	X(BGEU,                 "bgeu") \
	X(NOP,                  "nop") \
	X(ECALL,                "ecall") \
	X(EBREAK,               "ebreak") \
	X(WFI,                  "wfi") \
	X(FENCE,                "fence") \
	X(MUL,                  "mul") \
	X(MULH,                 "mulh") \
	X(MULHU,                "mulhu") \
	X(MULHSU,               "mulhsu") \
	X(DIV,                  "div") \
	X(DIVU,                 "divu") \
	X(REM,                  "rem") \
	X(REMU,                 "remu") \
	X(LRW,                  "lr.w") \
	X(SCW,                  "sc.w") \
	X(AMOSWAPW,             "amoswap.w") \
	X(AMOADDW,              "amoadd.w") \
	X(AMOXORW,              "amoxor.w") \
	X(AMOANDW,              "amoand.w") \
	X(AMOORW,               "amoor.w") \
	X(AMOMINW,              "amomin.w") \
	X(AMOMAXW,              "amomax.w") \
	X(AMOMINUW,             "amominu.w") \
	X(AMOMAXUW,             "amomaxu.w") \
	X(CLWSP,                "clwsp") \
	X(CSWSP,                "cswsp") \
	X(CLW,                  "clw") \
	X(CSW,                  "csw") \
	X(CJ,                   "cj") \
	X(CJAL,                 "cjal") \
	X(CJR,                  "cjr") \
	X(CJALR,                "cjalr") \
	X(CBEQZ,                "cbeqz") \
	X(CBNEZ,                "cbnez") \
	X(CLI,                  "cli") \
	X(CLUI,                 "clui") \
	X(CADDI,                "caddi") \
	X(CADDI16SP,            "caddi16sp") \
	X(CADDI4SPN,            "caddi4spn") \
	X(CSLLI,                "cslli") \
	X(CSRLI,                "csrli") \
	X(CSRAI,                "csrai") \
	X(CANDI,                "candi") \
	X(CMV,                  "cmv") \
	X(CADD,                 "cadd") \
	X(CAND,                 "cand") \
	X(COR,                  "cor") \
	X(CXOR,                 "cxor") \
	X(CSUB,                 "csub") \
	X(CNOP,                 "cnop") \
	X(CEBREAK,              "cebreak") \
	X(CSRRW,                "csrrw") \
	X(CSRRS,                "csrrs") \
	X(CSRRC,                "csrrc") \
	X(CSRRWI,               "csrrwi") \
	X(CSRRSI,               "csrrsi") \
	X(CSRRCI,               "csrrci") \
	X(FENCEI,               "fence.i") \
	X(URET,                 "uret") \
	X(SRET,                 "sret") \
	X(MRET,                 "mret") \
	X(END,                  "<end of file>") \
	
	
	enum struct TokenKind
	{
#define X(enumerator, token) enumerator,
		TOKEN_KINDS(X)
#undef X
	};

	using namespace std::literals;

	static const Bimap<std::string_view, TokenKind> token_map
	{
#define X(enumerator, token) bimap_t{ token, TokenKind::enumerator },
		TOKEN_KINDS(X)
#undef X
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
