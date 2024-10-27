import std;

#include "CommonErrors.h"
#include "Parser.h"
#include "Node.h"
#include "Statement.h"
#include "Expression.h"
#include "IntegerLiteral.h"
#include "ErrorReporter.h"

// Haze CommonErrors.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void CommonErrors::internal_compiler_error(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("[internal compiler error] {}", message), token);
	}

	void CommonErrors::internal_linker_error(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("[internal linker error] {}", message), token);
	}


	void CommonErrors::unsupported_instruction_format(const std::string& instruction, const std::string& format)
	{
		_error_reporter->post_error(std::format("unsupported instruction format `{}` for `{}`", format, instruction), NULL_TOKEN);
	}

	std::string CommonErrors::invalid_generic_type(const std::string& context, const std::string& type)
	{
		return std::format("invalid {} type `{}`", context, type);
	}

	void CommonErrors::invalid_parser_type(ParserType ptype, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("parser", _parser_type_map.at(ptype)), token);
	}

	void CommonErrors::invalid_node_type(NodeType ntype, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("node", _node_type_map.at(ntype)), token);
	}

	void CommonErrors::invalid_statement_type(StatementType stype, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("statement", _statement_type_map.at(stype)), token);
	}

	void CommonErrors::invalid_expression_type(ExpressionType etype, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("expression", _expression_type_map.at(etype)), token);
	}

	void CommonErrors::invalid_integer_literal_type(IntegerLiteralType itype, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("integer literal", _integer_literal_type_map.at(itype)), token);
	}

	void CommonErrors::must_be_lvalue(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("{} must be a modifiable l-value", message), token);
	}

	void CommonErrors::must_be_rvalue(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("{} must evaluate to an r-value", message), token);
	}

	void CommonErrors::must_be_constexpr(const std::string& message, const Token& token)
	{
		_error_reporter->post_error(std::format("{} must evaluate to a constant expression", message), token);
	}

	void CommonErrors::invalid_type(const std::string& component, const Token& token)
	{
		_error_reporter->post_error(std::format("invalid type {} `{}`", component, token.value), token);
	}

	void CommonErrors::unsupported_statement(const std::string& source, const std::string& type, const Token& token)
	{
		_error_reporter->post_error(std::format("unsupported {} statement type `{}`", source, type), token);
	}

	void CommonErrors::unsupported_expression(const std::string& source, const std::string& type, const Token& token)
	{
		_error_reporter->post_error(std::format("unsupported {} expression type `{}`", source, type), token);
	}
}
