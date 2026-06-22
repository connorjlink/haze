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
	X(IDENTIFIER,        IDENTIFIER,        LEFT, 0, IdentifierExpression,       identifier) \
	X(INTEGER_LITERAL,   INTEGER_LITERAL,   LEFT, 0, IntegerLiteralExpression,   integer literal) \
	X(FLOAT_LITERAL,     FLOAT_LITERAL,     LEFT, 0, FloatLiteralExpression,     float literal) \
	X(CHARACTER_LITERAL, CHARACTER_LITERAL, LEFT, 0, CharacterLiteralExpression, character literal) \
	X(STRING_LITERAL,    STRING_LITERAL,    LEFT, 0, StringLiteralExpression,    string literal) \
	X(PARENTHESIZED,     PARENTHESIZED,     LEFT, 0, ParenthesizedExpression,    parenthesized)

#define POSTFIX_EXPRESSION_KINDS(X) \
	X(POSTFIX_INCREMENT,             PLUSPLUS,   LEFT, 1, PostfixIncrementExpression,           postfix increment) \
	X(POSTFIX_DECREMENT,             MINUSMINUS, LEFT, 1, PostfixDecrementExpression,           postfix decrement) \
	X(FUNCTION_CALL,                 LPAREN,     LEFT, 1, FunctionCallExpression,               function call) \
	X(SUBSCRIPT,                     LBRACKET,   LEFT, 1, SubscriptExpression,                  subscript) \
	X(MEMBER_ACCESS,                 DOT,        LEFT, 1, MemberAccessExpression,               member access) \
	X(MEMBER_ACCESS_THROUGH_POINTER, ARROW,      LEFT, 1, MemberAccessThroughPointerExpression, member access through pointer) \
	X(COMPOUND_LITERAL,              LBRACE,     LEFT, 1, CompoundLiteralExpression,            compound literal)

#define UNARY_EXPRESSION_KINDS(X) \
	X(PREFIX_INCREMENT,  PLUSPLUS,    RIGHT, 2, PrefixIncrementExpression,  prefix increment) \
	X(PREFIX_DECREMENT,  MINUSMINUS,  RIGHT, 2, PrefixDecrementExpression,  prefix decrement) \
	X(PLUS,              PLUS,        RIGHT, 2, PlusExpression,             unary plus) \
	X(MINUS,             MINUS,       RIGHT, 2, MinusExpression,            unary minus) \
	X(BITWISE_NOT,       TILDE,       RIGHT, 2, BitwiseNotExpression,       bitwise not) \
	X(LOGICAL_NOT,       EXCLAMATION, RIGHT, 2, LogicalNotExpression,       logical not) \
	X(CAST,              LPAREN,      RIGHT, 2, CastExpression,             type cast) \
	X(ADDRESS_OF,        AMPERSAND,   RIGHT, 2, AddressOfExpression,        address-of) \
	X(DEREFERENCE,       STAR,        RIGHT, 2, DereferenceExpression,      dereference) \
	X(SIZEOF_EXPRESSION, SIZEOF,      RIGHT, 2, SizeOfExpressionExpression, size-of expression) \
	X(SIZEOF_TYPE,       SIZEOF,      RIGHT, 2, SizeOfTypeExpression,       size-of type)

#define BINARY_EXPRESSION_KINDS(X) \
	X(MULTIPLY,               STAR,                 LEFT,   3, MultiplyExpression,             multiply) \
	X(DIVIDE,                 SLASH,                LEFT,   3, DivideExpression,               divide) \
	X(MODULO,                 PERCENT,              LEFT,   3, ModuloExpression,               modulo) \
	X(ADD,                    PLUS,                 LEFT,   4, AddExpression,                  add) \
	X(SUBTRACT,               MINUS,                LEFT,   4, SubtractExpression,             subtract) \
	X(LEFT_SHIFT,             LESSLESS,             LEFT,   5, LeftShiftExpression,            left shift) \
	X(RIGHT_SHIFT,            GREATERGREATER,       LEFT,   5, RightShiftExpression,           right shift) \
	X(LESS,                   LESS,                 LEFT,   6, LessThanExpression,             less than) \
	X(LESS_EQUAL,             LESS,                 LEFT,   6, LessThanOrEqualToExpression,    less than or equal to) \
	X(GREATER,                GREATER,              LEFT,   6, GreaterThanExpression,          greater than) \
	X(GREATER_EQUAL,          GREATER,              LEFT,   6, GreaterThanOrEqualToExpression, greater than or equal to) \
	X(EQUAL,                  EQUALS,               LEFT,   7, EqualToExpression,              equal to) \
	X(NOT_EQUAL,              EXCLAMATIONEQUALS,    LEFT,   7, NotEqualToExpression,           not equal to) \
	X(BITWISE_AND,            AMPERSAND,            LEFT,   8, BitwiseAndExpression,           bitwise AND) \
	X(BITWISE_XOR,            CARET,                LEFT,   9, BitwiseXorExpression,           bitwise exclusive OR) \
	X(BITWISE_OR,             PIPE,                 LEFT,  10, BitwiseOrExpression,            bitwise OR) \
	X(LOGICAL_AND,            AMPERSANDAMPERSAND,   LEFT,  11, LogicalAndExpression,           logical AND) \
	X(LOGICAL_OR,             PIPEPIPE,             LEFT,  12, LogicalOrExpression,            logical OR) \
	X(TERNARY,                QUESTION,             RIGHT, 13, TernaryExpression,              ternary) \
	X(ASSIGNMENT,             EQUALS,               RIGHT, 14, AssignmentExpression,           assignment) \
	X(ADD_ASSIGNMENT,         PLUSEQUALS,           RIGHT, 14, AddAssignmentExpression,        add assignment) \
	X(SUBTRACT_ASSIGNMENT,    MINUSEQUALS,          RIGHT, 14, SubtractAssignmentExpression,   subtract assignment) \
	X(MULTIPLY_ASSIGNMENT,    STAREQUALS,           RIGHT, 14, MultiplyAssignmentExpression,   multiply assignment) \
	X(DIVIDE_ASSIGNMENT,      SLASHEQUALS,          RIGHT, 14, DivideAssignmentExpression,     divide assignment) \
	X(MODULO_ASSIGNMENT,      PERCENTEQUALS,        RIGHT, 14, ModuloAssignmentExpression,     modulo assignment) \
	X(LEFT_SHIFT_ASSIGNMENT,  LESSLESSEQUALS,       RIGHT, 14, LeftShiftAssignmentExpression,  left shift assignment) \
	X(RIGHT_SHIFT_ASSIGNMENT, GREATERGREATEREQUALS, RIGHT, 14, RightShiftAssignmentExpression, right shift assignment) \
	X(BITWISE_AND_ASSIGNMENT, AMPERSANDEQUALS,      RIGHT, 14, BitwiseAndAssignmentExpression, bitwise AND assignment) \
	X(BITWISE_XOR_ASSIGNMENT, CARETEQUALS,          RIGHT, 14, BitwiseXorAssignmentExpression, bitwise exclusive OR assignment) \
	X(BITWISE_OR_ASSIGNMENT,  PIPEEQUALS,           RIGHT, 14, BitwiseOrAssignmentExpression,  bitwise OR assignment) \
	X(COMMA,                  COMMA,                LEFT,  15, CommaExpression,                comma)

#define EXPRESSION_KINDS(X) \
	PRIMARY_EXPRESSION_KINDS(X) \
	POSTFIX_EXPRESSION_KINDS(X) \
	UNARY_EXPRESSION_KINDS(X) \
	BINARY_EXPRESSION_KINDS(X)


#define AUTOENUM_ROUTER(X, enumerator, token, associativity, precedence, type, name) X(enumerator, type, name, ExpressionKind)

	DEFINE_ENUM(EXPRESSION_KINDS, ExpressionKind, expression kind)

#undef AUTOENUM_ROUTER


	Associativity associativity_of(ExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, token, associativity, precedence, type, name) case ExpressionKind::enumerator: return Associativity::associativity;
			EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid expression kind `{}`", kind), NULL_TOKEN);

		return Associativity::LEFT;
	}

	Precedence precedence_of(ExpressionKind kind)
	{
		switch (kind)
		{
#define X(enumerator, token, associativity, precedence, type, name) case ExpressionKind::enumerator: return precedence;
			EXPRESSION_KINDS(X)
#undef X
		}

		USE_UNSAFE(ErrorReporter)->post_error(std::format(
			"invalid expression kind `{}`", kind), NULL_TOKEN);

		return 0;
	}
}

#endif
