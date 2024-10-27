#ifndef HAZE_COMMONERRORS_H
#define HAZE_COMMONERRORS_H

// Haze CommonErrors.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ParserType;
	enum class NodeType;
	enum class StatementType;
	enum class ExpressionType;
	enum class BinaryExpressionType;
	enum class IntegerLiteralType;

	struct Token;

	class CommonErrors
	{
	public:
		static void internal_compiler_error(const std::string&, const Token&);
		static void internal_linker_error(const std::string&, const Token&);

	public:
		static void unsupported_instruction_format(const std::string&, const std::string&);

	public:
		static std::string invalid_generic_type(const std::string&, const std::string&);

	public:
		static void invalid_parser_type(ParserType, const Token&);
		static void invalid_node_type(NodeType, const Token&);
		static void invalid_statement_type(StatementType, const Token&);
		static void invalid_expression_type(ExpressionType, const Token&);

	public:
		static void invalid_integer_literal_type(IntegerLiteralType, const Token&);

	public:
		static void must_be_lvalue(const std::string&, const Token&);
		static void must_be_rvalue(const std::string&, const Token&);

	public:
		static void must_be_constexpr(const std::string&, const Token&);

	public:
		static void invalid_type(const std::string&, const Token&);

	public:
		static void integer_size_mismatch(IntegerLiteralType, IntegerLiteralType, const Token&);
		static void invalid_binary_expression(ExpressionType, ExpressionType, BinaryExpressionType, const Token&);

	public:
		static void unsupported_statement(const std::string&, const std::string&, const Token&);
		static void unsupported_expression(const std::string&, const std::string&, const Token&);
	};
}

#endif
