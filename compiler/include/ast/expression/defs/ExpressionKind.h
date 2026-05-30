#ifndef HAZE_EXPRESSIONKIND_H
#define HAZE_EXPRESSIONKIND_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Formatter.h>

// Haze ExpressionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class Associativity
	{
		LEFT,
		RIGHT,
	};


#include <ast/expression/defs/PrimaryExpressionKind.x>
#include <ast/expression/defs/PostfixExpressionKind.x>
#include <ast/expression/defs/UnaryExpressionKind.x>
#include <ast/expression/defs/BinaryExpressionKind.x>

	enum class ExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
		PRIMARY_EXPRESSION_KINDS(X)
#undef X

#define X(enumerator, associativity, precedence, type, name) enumerator,
		POSTFIX_EXPRESSION_KINDS(X)
#undef X

#define X(enumerator, associativity, precedence, type, name) enumerator,
		UNARY_EXPRESSION_KINDS(X)
#undef X

#define X(enumerator, associativity, precedence, type, name) enumerator,
		BINARY_EXPRESSION_KINDS(X)
#undef X
	};


	enum class PrimaryExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
		PRIMARY_EXPRESSION_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PrimaryExpressionKind::enumerator: return #name;
			PRIMARY_EXPRESSION_KINDS(X)
#undef X
		}

		return "<unknown primary expression kind>";
	}

	Associativity associativity_of(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PrimaryExpressionKind::enumerator: return Associativity::associativity;
			PRIMARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid primary expression kind `{}`", kind), NULL_TOKEN);

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PrimaryExpressionKind::enumerator: return precedence;
			PRIMARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid primary expression kind `{}`", kind), NULL_TOKEN);

		return 0;
	}


	enum class PostfixExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
		POSTFIX_EXPRESSION_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PostfixExpressionKind::enumerator: return #name;
			POSTFIX_EXPRESSION_KINDS(X)
#undef X
		}

		return "<unknown postfix expression kind>";
	}

	Associativity associativity_of(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PostfixExpressionKind::enumerator: return Associativity::associativity;
			POSTFIX_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid postfix expression kind `{}`", kind), NULL_TOKEN);

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PostfixExpressionKind::enumerator: return precedence;
			POSTFIX_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid postfix expression kind `{}`", kind), NULL_TOKEN);

		return 0;
	}


	enum class UnaryExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
		UNARY_EXPRESSION_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case UnaryExpressionKind::enumerator: return #name;
			UNARY_EXPRESSION_KINDS(X)
#undef X
		}

		return "<unknown unary expression kind>";
	}

	Associativity associativity_of(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case UnaryExpressionKind::enumerator: return Associativity::associativity;
			UNARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid unary expression kind `{}`", kind), NULL_TOKEN);

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case UnaryExpressionKind::enumerator: return precedence;
			UNARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid unary expression kind `{}`", kind), NULL_TOKEN);

		return 0;
	}


	enum class BinaryExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
		BINARY_EXPRESSION_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case BinaryExpressionKind::enumerator: return #name;
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		return "<unknown binary expression kind>";
	}

	Associativity associativity_of(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case BinaryExpressionKind::enumerator: return Associativity::associativity;
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid binary expression kind `{}`", kind), NULL_TOKEN);

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case BinaryExpressionKind::enumerator: return precedence;
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid binary expression kind `{}`", kind), NULL_TOKEN);

		return 0;
	}
}

#endif
