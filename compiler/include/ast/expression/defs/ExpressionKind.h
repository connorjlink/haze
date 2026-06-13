#ifndef HAZE_EXPRESSIONKIND_H
#define HAZE_EXPRESSIONKIND_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/AutoEnum.h>

// Haze ExpressionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct Associativity
	{
		LEFT,
		RIGHT,
	};

	using Precedence = std::uint8_t;


#define PRIMARY_EXPRESSION_KINDS(X) \
	X(IDENTIFIER,        LEFT, 0, IdentifierExpression,       identifier) \
	X(INTEGER_LTIERAL,   LEFT, 0, IntegerLiteralExpression,   integer literal) \
	X(FLOAT_LITERAL,     LEFT, 0, FloatLiteralExpression,     float literal) \
	X(CHARACTER_LITERAL, LEFT, 0, CharacterLiteralExpression, character literal) \
	X(STRING_LITERAL,    LEFT, 0, StringLiteralExpression,    string literal) \
	X(PARENTHESIZED,     LEFT, 0, ParenthesizedExpression,    parenthesized)

#define POSTFIX_EXPRESSION_KINDS(X) \
	X(POSTFIX_INCREMENT,             LEFT, 1, PostfixIncrementExpression,           postfix increment) \
	X(POSTFIX_DECREMENT,             LEFT, 1, PostfixDecrementExpression,           postfix decrement) \
	X(FUNCTION_CALL,                 LEFT, 1, FunctionCallExpression,               function call) \
	X(SUBSCRIPT,                     LEFT, 1, SubscriptExpression,                  subscript) \
	X(MEMBER_ACCESS,                 LEFT, 1, MemberAccessExpression,               member access) \
	X(MEMBER_ACCESS_THROUGH_POINTER, LEFT, 1, MemberAccessThroughPointerExpression, member access through pointer) \
	X(COMPOUND_LITERAL,              LEFT, 1, CompoundLiteralExpression,            compound literal)

#define UNARY_EXPRESSION_KINDS(X) \
	X(PREFIX_INCREMENT,  RIGHT, 2, PrefixIncrementExpression,  prefix increment) \
	X(PREFIX_DECREMENT,  RIGHT, 2, PrefixDecrementExpression,  prefix decrement) \
	X(PLUS,              RIGHT, 2, PlusExpression,             unary plus) \
	X(MINUS,             RIGHT, 2, MinusExpression,            unary minus) \
	X(BITWISE_NOT,       RIGHT, 2, BitwiseNotExpression,       bitwise not) \
	X(LOGICAL_NOT,       RIGHT, 2, LogicalNotExpression,       logical not) \
	X(CAST,              RIGHT, 2, CastExpression,             type cast) \
	X(ADDRESS_OF,        RIGHT, 2, AddressOfExpression,        address-of) \
	X(DEREFERENCE,       RIGHT, 2, DereferenceExpression,      dereference) \
	X(SIZEOF_EXPRESSION, RIGHT, 2, SizeOfExpressionExpression, size-of expression) \
	X(SIZEOF_TYPE,       RIGHT, 2, SizeOfTypeExpression,       size-of type)

#define BINARY_EXPRESSION_KINDS(X) \
	X(MULTIPLY,               LEFT,   3, MultiplyExpression,             multiply) \
	X(DIVIDE,                 LEFT,   3, DivideExpression,               divide) \
	X(MODULO,                 LEFT,   3, ModuloExpression,               modulo) \
	X(ADD,                    LEFT,   4, AddExpression,                  add) \
	X(SUBTRACT,               LEFT,   4, SubtractExpression,             subtract) \
	X(LEFT_SHIFT,             LEFT,   5, LeftShiftExpression,            left shift) \
	X(RIGHT_SHIFT,            LEFT,   5, RightShiftExpression,           right shift) \
	X(LESS,                   LEFT,   6, LessThanExpression,             less than) \
	X(LESS_EQUAL,             LEFT,   6, LessThanOrEqualToExpression,    less than or equal to) \
	X(GREATER,                LEFT,   6, GreaterThanExpression,          greater than) \
	X(GREATER_EQUAL,          LEFT,   6, GreaterThanOrEqualToExpression, greater than or equal to) \
	X(EQUAL,                  LEFT,   7, EqualToExpression,              equal to) \
	X(NOT_EQUAL,              LEFT,   7, NotEqualToExpression,           not equal to) \
	X(BITWISE_AND,            LEFT,   8, BitwiseAndExpression,           bitwise AND) \
	X(BITWISE_XOR,            LEFT,   9, BitwiseXorExpression,           bitwise exclusive OR) \
	X(BITWISE_OR,             LEFT,  10, BitwiseOrExpression,            bitwise OR) \
	X(LOGICAL_AND,            LEFT,  11, LogicalAndExpression,           logical AND) \
	X(LOGICAL_OR,             LEFT,  12, LogicalOrExpression,            logical OR) \
	X(TERNARY,                RIGHT, 13, TernaryExpression,              ternary) \
	X(ASSIGNMENT,             RIGHT, 14, AssignmentExpression,           assignment) \
	X(ADD_ASSIGNMENT,         RIGHT, 14, AddAssignmentExpression,        add assignment) \
	X(SUBTRACT_ASSIGNMENT,    RIGHT, 14, SubtractAssignmentExpression,   subtract assignment) \
	X(MULTIPLY_ASSIGNMENT,    RIGHT, 14, MultiplyAssignmentExpression,   multiply assignment) \
	X(DIVIDE_ASSIGNMENT,      RIGHT, 14, DivideAssignmentExpression,     divide assignment) \
	X(MODULO_ASSIGNMENT,      RIGHT, 14, ModuloAssignmentExpression,     modulo assignment) \
	X(LEFT_SHIFT_ASSIGNMENT,  RIGHT, 14, LeftShiftAssignmentExpression,  left shift assignment) \
	X(RIGHT_SHIFT_ASSIGNMENT, RIGHT, 14, RightShiftAssignmentExpression, right shift assignment) \
	X(BITWISE_AND_ASSIGNMENT, RIGHT, 14, BitwiseAndAssignmentExpression, bitwise AND assignment) \
	X(BITWISE_XOR_ASSIGNMENT, RIGHT, 14, BitwiseXorAssignmentExpression, bitwise exclusive OR assignment) \
	X(BITWISE_OR_ASSIGNMENT,  RIGHT, 14, BitwiseOrAssignmentExpression,  bitwise OR assignment) \
	X(COMMA,                  LEFT,  15, CommaExpression,                comma)


	enum struct ExpressionKind
	{
#define X(enumerator, associativity, precedence, type, name) enumerator,
		PRIMARY_EXPRESSION_KINDS(X)
		POSTFIX_EXPRESSION_KINDS(X)
		UNARY_EXPRESSION_KINDS(X)
		BINARY_EXPRESSION_KINDS(X)
#undef X
	};

	constexpr std::string_view to_string(ExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case ExpressionKind::enumerator: return #name;
			PRIMARY_EXPRESSION_KINDS(X)
			POSTFIX_EXPRESSION_KINDS(X)
			UNARY_EXPRESSION_KINDS(X)
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		return "<unknown expression kind>";
	}

	Associativity associativity_of(ExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case ExpressionKind::enumerator: return Associativity::associativity;
			PRIMARY_EXPRESSION_KINDS(X)
			POSTFIX_EXPRESSION_KINDS(X)
			UNARY_EXPRESSION_KINDS(X)
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid expression kind `{}`", kind), NULL_TOKEN);

		return Associativity::LEFT;
	}

	std::uint8_t precedence_of(ExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, associativity, precedence, type, name) case ExpressionKind::enumerator: return precedence;
			PRIMARY_EXPRESSION_KINDS(X)
			POSTFIX_EXPRESSION_KINDS(X)
			UNARY_EXPRESSION_KINDS(X)
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid expression kind `{}`", kind), NULL_TOKEN);

		return 0;
	}
}

#endif
