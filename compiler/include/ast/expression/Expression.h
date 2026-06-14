#ifndef HAZE_EXPRESSION_AST_H
#define HAZE_EXPRESSION_AST_H

#include <ast/AST.h>
#include <ast/expression/defs/ExpressionKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/Generator.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Expression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Context;

	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Expression)

	template<typename MethodsT>
	using ExpressionASTMethods = ASTMethods<MethodsT, ExpressionHandle>;

	DEFINE_SUM(Expression, AST_METHODS)


	struct ExpressionBase
		: public ExpressionFacade
		, public InjectService<Generator>
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = ExpressionSumStorage;

		template<typename Self>
		bool check_types(this Self&& self, const Storage& ast)
		{
			return self.get_type().is_valid();
		}

		template<typename Self>
		ExpressionKind expression_kind(this Self&&);
	};
}

namespace hz
{
	//////////////////////////////////////////////////////
	// Primary Expressions
	//////////////////////////////////////////////////////

	struct IdentifierExpression : public ExpressionBase
	{
	public:
		enum struct Kind
		{
			UNKNOWN, // default
			GLOBAL_VARIABLE,
			LOCAL_VARIABLE,
			FUNCTION,
			ARGUMENT,
			MACRO,
		};

	public:
		std::string name;
		Kind identifier_kind;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		IdentifierExpression(Kind identifier_kind, const std::string& name)
			: identifier_kind{ identifier_kind }, name{ name }
		{
		}
	};
#define MAKE_IDENTIFIER_EXPRESSION(identifier_kind, name) IdentifierExpression{ identifier_kind, name }

	struct IntegerLiteralExpression : public ExpressionBase
	{
	public:
		BigInteger value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		IntegerLiteralExpression(BigInteger value)
			: value{ value }
		{
		}
	};
#define MAKE_INTEGER_LITERAL_EXPRESSION(value) IntegerLiteralExpression{ value }

	struct FloatLiteralExpression : public ExpressionBase
	{
	public:
		BigFloat value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		FloatLiteralExpression(BigFloat value)
			: value{ value }
		{
		}
	};
#define MAKE_FLOAT_LITERAL_EXPRESSION(value) FloatLiteralExpression{ value }

	struct StringLiteralExpression : public ExpressionBase
	{
	public:
		std::string value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		StringLiteralExpression(const std::string& value)
			: value{ value }
		{
		}
	};
#define MAKE_STRING_LITERAL_EXPRESSION(value) StringLiteralExpression{ value }

	struct CharacterLiteralExpression : public ExpressionBase
	{
	public:
		char value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		CharacterLiteralExpression(char value)
			: value{ value }
		{
		}
	};
#define MAKE_CHARACTER_LITERAL_EXPRESSION(value) CharacterLiteralExpression{ value }

	struct ParenthesizedExpression : public ExpressionBase
	{
	public:
		ExpressionHandle inner;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		ParenthesizedExpression(ExpressionHandle inner)
			: inner{ inner }
		{
		}
	};
#define MAKE_PARENTHESIZED_EXPRESSION(inner) ParenthesizedExpression{ make_handle(ast, inner) }

	using PrimaryExpressionTypes = SumTypeList
	<
#define X(enumerator, associativity, precedence, type, name) type,
		PRIMARY_EXPRESSION_KINDS(X)
#undef X
		// NOTE: will be discarded, only to get rid of the extraneous trailing comma from the macro expansion
		void
	>;

	
	//////////////////////////////////////////////////////
	// Postfix Expressions
	//////////////////////////////////////////////////////

	struct PostfixIncrementExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		PostfixIncrementExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_POSTFIX_INCREMENT_EXPRESSION(operand) PostfixIncrementExpression{ make_handle(ast, operand) }

	struct PostfixDecrementExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		PostfixDecrementExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_POSTFIX_DECREMENT_EXPRESSION(operand) PostfixDecrementExpression{ make_handle(ast, operand) }

	struct FunctionCallExpression : public ExpressionBase
	{
	public:
		ExpressionReference<IdentifierExpression> identifier;
		std::vector<ExpressionHandle> arguments;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		FunctionCallExpression(ExpressionReference<IdentifierExpression> identifier, std::vector<ExpressionHandle> arguments)
			: identifier{ identifier }, arguments{ std::move(arguments) }
		{
		}
	};
#define MAKE_FUNCTION_CALL_EXPRESSION(identifier, arguments) FunctionCallExpression{ make_handle(ast, identifier), arguments }

	struct SubscriptExpression : public ExpressionBase
	{
	public:
		ExpressionHandle array;
		ExpressionHandle index;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		SubscriptExpression(ExpressionHandle array, ExpressionHandle index)
			: array{ array }, index{ index }
		{
		}
	};
#define MAKE_SUBSCRIPT_EXPRESSION(array, index) SubscriptExpression{ make_handle(ast, array), make_handle(ast, index) }

	struct MemberAccessExpression : public ExpressionBase
	{
	public:
		ExpressionHandle object;
		ExpressionReference<IdentifierExpression> member;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		MemberAccessExpression(ExpressionHandle object, ExpressionReference<IdentifierExpression> member)
			: object{ object }, member{ member }
		{
		}
	};
#define MAKE_MEMBER_ACCESS_EXPRESSION(object, member) MemberAccessExpression{ make_handle(ast, object), make_reference(ast, member) }

	struct MemberAccessThroughPointerExpression : public ExpressionBase
	{
	public:
		ExpressionHandle object;
		ExpressionReference<IdentifierExpression> member;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		MemberAccessThroughPointerExpression(ExpressionHandle object, ExpressionReference<IdentifierExpression> member)
			: object{ object }, member{ member }
		{
		}
	};
#define MAKE_MEMBER_ACCESS_THROUGH_POINTER_EXPRESSION(object, member) MemberAccessThroughPointerExpression{ make_handle(ast, object), make_handle(ast, member) }

	struct CompoundLiteralExpression : public ExpressionBase
	{
	public:
		struct Initializer
		{
			// optional can be .identifier for struct/union or [index] for array
			ExpressionHandle designator;
			ExpressionHandle value;
		};

	public:
		TypeHandle type;
		std::vector<Initializer> initializers;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		CompoundLiteralExpression(TypeHandle type, std::vector<Initializer> initializers)
			: type{ type }, initializers{ std::move(initializers) }
		{
		}
	};
#define MAKE_COMPOUND_LITERAL_EXPRESSION(type, initializers) CompoundLiteralExpression{ type, initializers }

	using PostfixExpressionTypes = SumTypeList
	<
#define X(enumerator, associativity, precedence, type, name) type,
		POSTFIX_EXPRESSION_KINDS(X)
#undef X
		void
	>;


	//////////////////////////////////////////////////////
	// Unary Expressions
	//////////////////////////////////////////////////////

	struct PrefixIncrementExpression : public ExpressionBase
	{
	public:
		// restricted to lvalues (unary expression only), checked at AST construction
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		PrefixIncrementExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_PREFIX_INCREMENT_EXPRESSION(operand) PrefixIncrementExpression{ make_handle(ast, operand) }

	struct PrefixDecrementExpression : public ExpressionBase
	{
	public:
		// restricted to lvalues (unary expression only), checked at AST construction
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		PrefixDecrementExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_PREFIX_DECREMENT_EXPRESSION(operand) PrefixDecrementExpression{ make_handle(ast, operand) }

	struct PlusExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		PlusExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_PLUS_EXPRESSION(operand) PlusExpression{ make_handle(ast, operand) }

	struct MinusExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		MinusExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_MINUS_EXPRESSION(operand) MinusExpression{ make_handle(ast, operand) }

	struct BitwiseNotExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		BitwiseNotExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_BITWISE_NOT_EXPRESSION(operand) BitwiseNotExpression{ make_handle(ast, operand) }

	struct LogicalNotExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		LogicalNotExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_LOGICAL_NOT_EXPRESSION(operand) LogicalNotExpression{ make_handle(ast, operand)

	struct CastExpression : public ExpressionBase
	{
	public:
		ExpressionHandle target;
		TypeHandle type;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		CastExpression(ExpressionHandle target, TypeHandle type)
			: target{ target }, type{ type }
		{
		}
	};
#define MAKE_CAST_EXPRESSION(target, type) CastExpression{ make_handle(ast, target), type }

	struct AddressOfExpression : public ExpressionBase
	{
	public:
		// operand must be an addressable lvalue or function designator
		// this is checked just before code generation rather than at AST construction
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		AddressOfExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_ADDRESS_OF_EXPRESSION(operand) AddressOfExpression{ make_handle(ast, operand) }

	struct DereferenceExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		DereferenceExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_DEREFERENCE_EXPRESSION(operand) DereferenceExpression{ make_handle(ast, operand) }

	struct SizeOfExpressionExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		SizeOfExpressionExpression(ExpressionHandle operand)
			: operand{ operand }
		{
		}
	};
#define MAKE_SIZEOF_EXPRESSION_EXPRESSION(operand) SizeOfExpressionExpression{ make_handle(ast, operand) }

	struct SizeOfTypeExpression : public ExpressionBase
	{
	public:
		TypeHandle type;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		SizeOfTypeExpression(TypeHandle type)
			: type{ type }
		{
		}
	};
#define MAKE_SIZEOF_TYPE_EXPRESSION(type) SizeOfTypeExpression{ type }

	using UnaryExpressionTypes = SumTypeList
	<
#define X(enumerator, associativity, precedence, type, name) type,
		UNARY_EXPRESSION_KINDS(X)
#undef X
		void
	>;


	//////////////////////////////////////////////////////
	// Binary Expressions
	//////////////////////////////////////////////////////

	struct MultiplyExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		MultiplyExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_MULTIPLY_EXPRESSION(left, right) MultiplyExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct DivideExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		DivideExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_DIVIDE_EXPRESSION(left, right) DivideExpression{ make_handle(ast, left), make_handle(ast, right) }
	
	struct ModuloExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		ModuloExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_MODULO_EXPRESSION(left, right) ModuloExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct AddExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		AddExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_ADD_EXPRESSION(left, right) AddExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct SubtractExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		SubtractExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_SUBTRACT_EXPRESSION(left, right) SubtractExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct LeftShiftExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		LeftShiftExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_LEFT_SHIFT_EXPRESSION(left, right) LeftShiftExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct RightShiftExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		RightShiftExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_RIGHT_SHIFT_EXPRESSION(left, right) RightShiftExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct LessThanExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		LessThanExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_LESS_THAN_EXPRESSION(left, right) LessThanExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct LessThanOrEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		LessThanOrEqualToExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_LESS_THAN_OR_EQUAL_TO_EXPRESSION(left, right) LessThanOrEqualToExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct GreaterThanExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		GreaterThanExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_GREATER_THAN_EXPRESSION(left, right) GreaterThanExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct GreaterThanOrEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		GreaterThanOrEqualToExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_GREATER_THAN_OR_EQUAL_TO_EXPRESSION(left, right) GreaterThanOrEqualToExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct EqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		EqualToExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_EQUAL_TO_EXPRESSION(left, right) EqualToExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct NotEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		NotEqualToExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_NOT_EQUAL_TO_EXPRESSION(left, right) NotEqualToExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseAndExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		BitwiseAndExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_AND_EXPRESSION(left, right) BitwiseAndExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseXorExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		BitwiseXorExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_XOR_EXPRESSION(left, right) BitwiseXorExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseOrExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		BitwiseOrExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_OR_EXPRESSION(left, right) BitwiseOrExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct LogicalAndExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		LogicalAndExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_LOGICAL_AND_EXPRESSION(left, right) LogicalAndExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct LogicalOrExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		LogicalOrExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_LOGICAL_OR_EXPRESSION(left, right) LogicalOrExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct TernaryExpression : public ExpressionBase
	{
	public:
		ExpressionHandle condition;
		ExpressionHandle then;
		ExpressionHandle otherwise;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		TernaryExpression(ExpressionHandle condition, ExpressionHandle then, ExpressionHandle otherwise)
			: condition{ condition }, then{ then }, otherwise{ otherwise }
		{
		}
	};
#define MAKE_TERNARY_EXPRESSION(condition, then, otherwise) TernaryExpression{ make_handle(ast, condition), make_handle(ast, then), make_handle(ast, otherwise) }

	struct AssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		AssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_ASSIGNMENT_EXPRESSION(left, right) AssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct AddAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		AddAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_ADD_ASSIGNMENT_EXPRESSION(left, right) AddAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct SubtractAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		SubtractAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_SUBTRACT_ASSIGNMENT_EXPRESSION(left, right) SubtractAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct MultiplyAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		MultiplyAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_MULTIPLY_ASSIGNMENT_EXPRESSION(left, right) MultiplyAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct DivideAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		DivideAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_DIVIDE_ASSIGNMENT_EXPRESSION(left, right) DivideAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct ModuloAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		ModuloAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_MODULO_ASSIGNMENT_EXPRESSION(left, right) ModuloAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct LeftShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		LeftShiftAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_LEFT_SHIFT_ASSIGNMENT_EXPRESSION(left, right) LeftShiftAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct RightShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		RightShiftAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_RIGHT_SHIFT_ASSIGNMENT_EXPRESSION(left, right) RightShiftAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseAndAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		BitwiseAndAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_AND_ASSIGNMENT_EXPRESSION(left, right) BitwiseAndAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseXorAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		BitwiseXorAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_XOR_ASSIGNMENT_EXPRESSION(left, right) BitwiseXorAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseOrAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		BitwiseOrAssignmentExpression(ExpressionHandle left, ExpressionHandle right)
			: left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_OR_ASSIGNMENT_EXPRESSION(left, right) BitwiseOrAssignmentExpression{ make_handle(ast, left), make_handle(ast, right) }

	struct CommaExpression : public ExpressionBase
	{
	public:
		std::vector<ExpressionHandle> expressions;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		CommaExpression(std::vector<ExpressionHandle> expressions)
			: expressions{ std::move(expressions) }
		{
		}
	};
#define MAKE_COMMA_EXPRESSION(expressions) CommaExpression{ expressions }

	using BinaryExpressionTypes = SumTypeList
	<
#define X(enumerator, associativity, precedence, type, name) type,
		BINARY_EXPRESSION_KINDS(X)
#undef X
		void
	>;


	//////////////////////////////////////////////////////
	// All Expressions
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept IsExpression = SumTuple<SumMemberT, SumStorageT, ExpressionASTMethods<SumStorageT>>;

	using ExpressionKinds = SumTypeList
	<
#define X(enumerator, associativity, precedence, type, name) type,
		PRIMARY_EXPRESSION_KINDS(X)
		POSTFIX_EXPRESSION_KINDS(X)
		UNARY_EXPRESSION_KINDS(X)
		BINARY_EXPRESSION_KINDS(X)
#undef X
		void
	>;

	using ExpressionSumImplementation = MakeSum<ExpressionASTMethods, ExpressionKinds>::Type;

	struct ExpressionSumStorage : public ExpressionSumImplementation::Storage
	{
		using ExpressionSumImplementation::Storage::Storage;

		using Type = ExpressionSumImplementation::Type;
		using Anchor = ExpressionSumImplementation::Anchor;
	};
}

#endif
