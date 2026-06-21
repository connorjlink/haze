#ifndef HAZE_COMMONERRORS_H
#define HAZE_COMMONERRORS_H

#include <ast/expression/defs/ExpressionKind.h>
#include <ast/statement/defs/StatementKind.h>
#include <cli/defs/ArchitectureKind.h>
#include <cli/defs/OptimizationFlag.h>
#include <cli/defs/OptionKind.h>
#include <cli/defs/VerbosityKind.h>
#include <command/defs/CommandKind.h>
#include <type/defs/FloatKind.h>
#include <type/defs/IntKind.h>
#include <type/defs/StructOrUnionKind.h>
#include <utility/Sum.h>

// Haze CommonErrors.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Token;

	FORWARD_DECLARE_SUM(Type)
	FORWARD_DECLARE_SUM(Expression)


	namespace internal
	{
		template<typename T>
		concept IsPrintable = (std::is_same_v<T, std::string_view> or std::is_same_v<T, const std::string&>);
	}

	struct CommonErrors
	{
		template<internal::IsPrintable T = std::string_view>
		static void internal_compiler_error(T message, const Token& token)
		{
			USE_UNSAFE(ErrorReporter)->post_error(std::format("[internal compiler error] {}", message), token);
		}

		template<internal::IsPrintable T = std::string_view>
		void internal_linker_error(T message, const Token& token)
		{
			USE_UNSAFE(ErrorReporter)->post_error(std::format("[internal linker error] {}", message), token);
		}

		template<internal::IsPrintable T = std::string_view>
		static void unsupported_instruction_format(T instruction, T format)
		{
			USE_UNSAFE(ErrorReporter)->post_error(std::format("unsupported instruction format `{}` for `{}`", format, instruction), NULL_TOKEN);
		}

		template<internal::IsPrintable T = std::string_view>
		static void unsupported_instruction_range(T instruction, T range)
		{
			USE_UNSAFE(ErrorReporter)->post_error(std::format("unsupported instruction range `{}` for `{}`", range, instruction), NULL_TOKEN);
		}

		template<internal::IsPrintable T = std::string_view>
		static T invalid_generic_type(T context, T type)
		{
			return std::format("invalid {} type `{}`", context, type);
		}

		template<internal::IsPrintable T = std::string_view>
		static void invalid_statement_kind(StatementKind kind, const Token& token)
		{
			internal_compiler_error<T>(invalid_generic_type<T>("statement", to_string(kind)), token);
		}

		template<internal::IsPrintable T = std::string_view>
		static void invalid_expression_kind(ExpressionKind kind, const Token& token)
		{
			internal_compiler_error<T>(invalid_generic_type<T>("expression", to_string(kind)), token);
		}

		template<internal::IsPrintable T = std::string_view>
		static void invalid_command_kind(CommandKind kind, const Token& token)
		{
			internal_compiler_error<T>(invalid_generic_type<T>("command", to_string(kind)), token);
		}

		template<internal::IsPrintable T = std::string_view>
		void invalid_int_kind(IntKind kind, const Token& token)
		{
			internal_compiler_error<T>(invalid_generic_type<T>("integer", to_string(kind)), token);
		}

		template<internal::IsPrintable T = std::string_view>
		static void invalid_float_kind(FloatKind kind, const Token& token)
		{
			internal_compiler_error<T>(invalid_generic_type<T>("float", to_string(kind)), token);
		}

		template<internal::IsPrintable T = std::string_view>
		static void invalid_token_kind(TokenKind kind, const Token& token)
		{
			// NOTE: prefer the value() call to get a proper exception in case of invalid token kind
			internal_compiler_error<T>(invalid_generic_type<T>("token", token_map.at(kind).value()), token);
		}

#pragma message("TODO: determine when to include the symbol kind enumeration")
		//template<internal::IsPrintable T = std::string_view>
		//void invalid_symbol_type(SymbolKind kind, const Token& token)
		//{
		//	internal_compiler_error(invalid_generic_type("symbol", to_string(kind)), token);
		//}

		template<internal::IsPrintable T = std::string_view>
		static void invalid_struct_or_union_kind(StructOrUnionKind kind, const Token& token)
		{
			internal_compiler_error<T>(invalid_generic_type<T>("struct or union", to_string(kind)), token);
		}

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
