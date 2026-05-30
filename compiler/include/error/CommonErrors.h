#ifndef HAZE_COMMONERRORS_H
#define HAZE_COMMONERRORS_H

// Haze CommonErrors.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class StatementKind;
	enum class ExpressionKind;
	enum class BinaryExpressionKind;
	enum class CommandKind;
	enum class IntKind;
	enum class FloatKind;
	enum class TokenKind;
	enum class SymbolKind;
	enum class StructOrUnionKind;

	class Type;
	class Expression;

	struct Token;

	class CommonErrors
	{
	public:
		static void internal_compiler_error(const std::string&, const Token&);
		static void internal_linker_error(const std::string&, const Token&);

	public:
		static void unsupported_instruction_format(const std::string&, const std::string&);
		static void unsupported_instruction_range(const std::string&, const std::string&);

	public:
		static std::string invalid_generic_type(const std::string&, const std::string&);

	public:
		static void invalid_statement_type(StatementKind, const Token&);
		static void invalid_expression_type(ExpressionKind, const Token&);
		static void invalid_command_type(CommandKind, const Token&);

	public:
		static void invalid_int_type(IntKind, const Token&);
		static void invalid_float_type(FloatKind, const Token&);
		static void invalid_token_type(TokenKind, const Token&);
		static void invalid_symbol_type(SymbolKind, const Token&);
		static void invalid_struct_or_union_type(StructOrUnionKind, const Token&);

	public:
		static void must_be_lvalue(const std::string&, const Token&);
		static void must_be_rvalue(const std::string&, const Token&);

	public:
		static void must_be_constexpr(const std::string&, const Token&);

	public:
		static void invalid_type(const std::string&, const Token&);
		static void invalid_register(const std::string&, const Token&);

	public:
		static void integer_size_mismatch(IntKind, IntKind, const Token&);
		static void invalid_binary_expression(ExpressionKind, ExpressionKind, BinaryExpressionKind, const Token&);

	public:
		static void type_qualifier_mismatch(TypeHandle, ExpressionHandle, const Token&);
		static void type_signedness_mismatch(TypeHandle, ExpressionHandle, const Token&);
		static void type_storage_mismatch(TypeHandle, ExpressionHandle, const Token&);
		static void int_type_specifier_mismatch(TypeHandle, ExpressionHandle, const Token&);

	public:
		static void unsupported_statement(const std::string&, const std::string&, const Token&);
		static void unsupported_expression(const std::string&, const std::string&, const Token&);
	};
}

#endif
