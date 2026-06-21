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

	void CommonErrors::integer_size_mismatch(IntegerLiteralKind left, IntegerLiteralType right, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("integer type mismatch between `{}` and `{}`",
			_integer_literal_type_map.at(left), _integer_literal_type_map.at(right)), token);
	}

	void CommonErrors::invalid_binary_expression(ExpressionType left, ExpressionType right, BinaryExpressionType op, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("invalid binary expression `{}` between `{}` and `{}`",
			binary_expression_kind_map.at(op), _expression_type_map.at(left), _expression_type_map.at(right)), token);
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
