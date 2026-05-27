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


	enum class ExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/PrimaryExpressionKind.x>
#undef X

#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/PostfixExpressionKind.x>
#undef X

#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/UnaryExpressionKind.x>
#undef X

#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/BinaryExpressionKind.x>
#undef X
	};


	enum class PrimaryExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/PrimaryExpressionKind.x>
#undef X
	};

	constexpr std::string_view to_string(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PrimaryExpressionKind::enumerator: return #name;
#include <ast/expression/defs/PrimaryExpressionKind.x>
#undef X
		}

		return "<unknown primary expression kind>";
	}

	Associativity associativity_of(PrimaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PrimaryExpressionKind::enumerator: return Associativity::associativity;
#include <ast/expression/defs/PrimaryExpressionKind.x>
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
#define X(enumerator, associativity, precedence, type, name) case PrimaryExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/PrimaryExpressionKind.x>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid primary expression kind `{}`", kind));

		return 0;
	}


	enum class PostfixExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/PostfixExpressionKind.x>
#undef X
	};

	constexpr std::string_view to_string(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PostfixExpressionKind::enumerator: return #name;
#include <ast/expression/defs/PostfixExpressionKind.x>
#undef X
		}

		return "<unknown postfix expression kind>";
	}

	Associativity associativity_of(PostfixExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case PostfixExpressionKind::enumerator: return Associativity::associativity;
#include <ast/expression/defs/PostfixExpressionKind.x>
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
#define X(enumerator, associativity, precedence, type, name) case PostfixExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/PostfixExpressionKind.x>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid postfix expression kind `{}`", kind));

		return 0;
	}


	enum class UnaryExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/UnaryExpressionKind.x>
#undef X
	};

	constexpr std::string_view to_string(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case UnaryExpressionKind::enumerator: return #name;
#include <ast/expression/defs/UnaryExpressionKind.x>
#undef X
		}

		return "<unknown unary expression kind>";
	}

	Associativity associativity_of(UnaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case UnaryExpressionKind::enumerator: return Associativity::associativity;
#include <ast/expression/defs/UnaryExpressionKind.x>
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
#define X(enumerator, associativity, precedence, type, name) case UnaryExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/UnaryExpressionKind.x>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid unary expression kind `{}`", kind));

		return 0;
	}


	enum class BinaryExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
#include <ast/expression/defs/BinaryExpressionKind.x>
#undef X
	};

	constexpr std::string_view to_string(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case BinaryExpressionKind::enumerator: return #name;
#include <ast/expression/defs/BinaryExpressionKind.x>
#undef X
		}

		return "<unknown binary expression kind>";
	}

	Associativity associativity_of(BinaryExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case BinaryExpressionKind::enumerator: return Associativity::associativity;
#include <ast/expression/defs/BinaryExpressionKind.x>
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
#define X(enumerator, associativity, precedence, type, name) case BinaryExpressionKind::enumerator: return precedence;
#include <ast/expression/defs/BinaryExpressionKind.x>
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid binary expression kind `{}`", kind));

		return 0;
	}
}

#endif
