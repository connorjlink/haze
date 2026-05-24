#ifndef HAZE_EXPRESSIONKIND_H
#define HAZE_EXPRESSIONKIND_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze ExpressionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class Associativity
	{
		LEFT,
		RIGHT,
	};


	enum class PrimaryExpressionKind
	{
#define X(enumerator, associativity, precedence, name) enumerator,
#include <ast/expression/defs/PrimaryExpressionKind.def>
#undef X
	};

	constexpr std::string_view to_string(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case PrimaryExpressionKind::enumerator: return name;
#include <ast/expression/defs/PrimaryExpressionKind.def>
#undef X
		}

		return "<unknown primary expression kind>";
	}

	Associativity associativity_of(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case PrimaryExpressionKind::enumerator: return associativity;
#include <ast/expression/defs/PrimaryExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid primary expression kind `{}`", kind));

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case PrimaryExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/PrimaryExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid primary expression kind `{}`", kind));

		return 0;
	}


	enum class PostfixExpressionKind
	{
#define X(enumerator, associativity, precedence, name) enumerator,
#include <ast/expression/defs/PostfixExpressionKind.def>
#undef X
	};

	constexpr std::string_view to_string(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case PostfixExpressionKind::enumerator: return name;
#include <ast/expression/defs/PostfixExpressionKind.def>
#undef X
		}

		return "<unknown postfix expression kind>";
	}

	Associativity associativity_of(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case PostfixExpressionKind::enumerator: return associativity;
#include <ast/expression/defs/PostfixExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid postfix expression kind `{}`", kind));

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case PostfixExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/PostfixExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid postfix expression kind `{}`", kind));

		return 0;
	}


	enum class UnaryExpressionKind
	{
#define X(enumerator, associativity, precedence, name) enumerator,
#include <ast/expression/defs/UnaryExpressionKind.def>
#undef X
	};

	constexpr std::string_view to_string(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case UnaryExpressionKind::enumerator: return name;
#include <ast/expression/defs/UnaryExpressionKind.def>
#undef X
		}

		return "<unknown unary expression kind>";
	}

	Associativity associativity_of(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case UnaryExpressionKind::enumerator: return associativity;
#include <ast/expression/defs/UnaryExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid unary expression kind `{}`", kind));

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case UnaryExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/UnaryExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid unary expression kind `{}`", kind));

		return 0;
	}


	enum class BinaryExpressionKind
	{
#define X(enumerator, associativity, precedence, name) enumerator,
#include <ast/expression/defs/BinaryExpressionKind.def>
#undef X
	};

	constexpr std::string_view to_string(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case BinaryExpressionKind::enumerator: return name;
#include <ast/expression/defs/BinaryExpressionKind.def>
#undef X
		}

		return "<unknown binary expression kind>";
	}

	Associativity associativity_of(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case BinaryExpressionKind::enumerator: return associativity;
#include <ast/expression/defs/BinaryExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid binary expression kind `{}`", kind));

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, name) case BinaryExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/BinaryExpressionKind.def>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid binary expression kind `{}`", kind));

		return 0;
	}
}

#endif
