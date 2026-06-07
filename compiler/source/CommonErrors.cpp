import std;

#include <ast/expression/Expression.h>
#include <ast/statement/Statement.h>
#include <command/defs/CommandKind.h>
#include <error/CommonErrors.h>
#include <symbol/defs/SymbolKind.h>
#include <type/Type.h>
#include <type/TypeCheck.h>

// Haze CommonErrors.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void CommonErrors::internal_compiler_error(const std::string& message, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("[internal compiler error] {}", message), token);
	}

	void CommonErrors::internal_linker_error(const std::string& message, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("[internal linker error] {}", message), token);
	}


	void CommonErrors::unsupported_instruction_format(const std::string& instruction, const std::string& format)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("unsupported instruction format `{}` for `{}`", format, instruction), NULL_TOKEN);
	}

	void CommonErrors::unsupported_instruction_range(const std::string& instruction, const std::string& range)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("unsupported instruction range `{}` for `{}`", range, instruction), NULL_TOKEN);
	}

	std::string CommonErrors::invalid_generic_type(const std::string& context, const std::string& type)
	{
		return std::format("invalid {} type `{}`", context, type);
	}

	void CommonErrors::invalid_statement_type(StatementKind kind, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("statement", std::string{ to_string(kind) }), token);
	}

	void CommonErrors::invalid_expression_type(ExpressionKind kind, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("expression", std::string{ to_string(kind) }), token);
	}

	void CommonErrors::invalid_command_type(CommandKind kind, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("command", std::string{ to_string(kind) }), token);
	}

	void CommonErrors::invalid_int_type(IntKind kind, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("integer", std::string{ to_string(kind) }), token);
	}

	void CommonErrors::invalid_float_type(FloatKind kind, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("float", std::string{ to_string(kind) }), token);
	}

	void CommonErrors::invalid_token_type(TokenKind kind, const Token& token)
	{
		// NOTE: prefer the value() call to get a proper exception in case of invalid token kind
		internal_compiler_error(invalid_generic_type("token", std::string{ token_map.at(kind).value() }), token);
	}

	void CommonErrors::invalid_symbol_type(SymbolKind kind, const Token& token)
	{
#pragma message("TODO: migrate the generic type function to accept also std::string_view")
		internal_compiler_error(invalid_generic_type("symbol", std::string{ to_string(kind) }), token);
	}

	void CommonErrors::invalid_struct_or_union_type(StructOrUnionKind kind, const Token& token)
	{
		internal_compiler_error(invalid_generic_type("struct or union", std::string{ to_string(kind) }), token);
	}

	void CommonErrors::must_be_lvalue(const std::string& message, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("{} must be a modifiable l-value", message), token);
	}

	void CommonErrors::must_be_rvalue(const std::string& message, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("{} must evaluate to an r-value", message), token);
	}

	void CommonErrors::must_be_constexpr(const std::string& message, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("{} must evaluate to a constant expression", message), token);
	}

	void CommonErrors::invalid_type(const std::string& component, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("invalid type {} `{}`", component, token.text), token);
	}

	void CommonErrors::invalid_register(const std::string& message, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("invalid register index `{}`", message), token);
	}

	void CommonErrors::integer_size_mismatch(IntegerLiteralType lhs, IntegerLiteralType rhs, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("integer type mismatch between `{}` and `{}`",
			_integer_literal_type_map.at(lhs), _integer_literal_type_map.at(rhs)), token);
	}

	void CommonErrors::invalid_binary_expression(ExpressionType lhs, ExpressionType rhs, BinaryExpressionType op, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("invalid binary expression `{}` between `{}` and `{}`",
			binary_expression_kind_map.at(op), _expression_type_map.at(lhs), _expression_type_map.at(rhs)), token);
	}

	void CommonErrors::type_qualifier_mismatch(TypeHandle type, ExpressionHandle expression, const Token& token)
	{
		const auto expected_qualifier = type->qualifier;

		const auto evaluated_type = resolve_type(expression);
		const auto erroring_qualifier = evaluated_type->qualifier;

		USE_UNSAFE(ErrorReporter)->post_error(std::format("type qualifier mismatch; expected `{}` but got `{}`",
			type_qualifier_map.at(expected_qualifier), type_qualifier_map.at(erroring_qualifier)), token);
	}

	void CommonErrors::type_signedness_mismatch(TypeHandle type, ExpressionHandle expression, const Token& token)
	{
		const auto expected_signedness = AS_INT_TYPE(type)->signedness;

		const auto evaluated_type = resolve_type(expression);
		const auto erroring_signedness = AS_INT_TYPE(evaluated_type)->signedness;

		USE_UNSAFE(ErrorReporter)->post_error(std::format("type signedness mismatch; expected `{}` but got `{}`",
			_type_signedness_map.at(erroring_signedness), _type_signedness_map.at(erroring_signedness)), token);
	}

	void CommonErrors::type_storage_mismatch(TypeHandle type, ExpressionHandle expression, const Token& token)
	{
		const auto expected_storage = type->storage;

		const auto evaluated_type = resolve_type(expression);
		const auto erroring_storage = evaluated_type->storage;

		USE_UNSAFE(ErrorReporter)->post_error(std::format("type storage mismatch; expected `{}` but got `{}`",
			_type_storage_map.at(expected_storage), _type_storage_map.at(erroring_storage)), token);
	}

	void CommonErrors::int_type_specifier_mismatch(TypeHandle type, ExpressionHandle expression, const Token& token)
	{
		const auto expected_specifier = AS_INT_TYPE(type)->int_kind;

		const auto evaluated_type = AS_INT_TYPE(resolve_type(expression));
		if (evaluated_type == nullptr)
		{
			USE_UNSAFE(ErrorReporter)->post_error(std::format("unresolvable type specifier and mismatch; expected `{}`",
				_int_type_type_map.at(expected_specifier)), token);
			return;
		}

		const auto erroring_specifier = evaluated_type->int_kind;

		USE_UNSAFE(ErrorReporter)->post_error(std::format("type specifier mismatch; expected `{}` but got `{}`",
			_int_type_type_map.at(expected_specifier), _int_type_type_map.at(erroring_specifier)), token);
	}

	void CommonErrors::unsupported_statement(const std::string& source, const std::string& type, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("unsupported {} statement type `{}`", source, type), token);
	}

	void CommonErrors::unsupported_expression(const std::string& source, const std::string& type, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("unsupported {} expression type `{}`", source, type), token);
	}
}
