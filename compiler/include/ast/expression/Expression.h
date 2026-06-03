#ifndef HAZE_EXPRESSION_AST_H
#define HAZE_EXPRESSION_AST_H

#include <ast/AST.h>
#include <ast/expression/defs/ExpressionKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Context.h>
#include <toolchain/Generator.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Expression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Expression)

	template<typename MethodsT>
	using ExpressionASTMethods = ASTMethods<MethodsT, ExpressionHandle>;

	DEFINE_SUM(Expression, AST_METHODS)


	class ExpressionBase
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

	class IdentifierExpression : public ExpressionBase
	{
	public:
		enum class Kind
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
		std::string format(void) const;
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

	class IntegerLiteralExpression : public ExpressionBase
	{
	public:
		BigInteger value;

	public:
		std::string format(void) const;
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

	class FloatLiteralExpression : public ExpressionBase
	{
	public:
		BigFloat value;

	public:
		std::string format(void) const;
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

	class StringLiteralExpression : public ExpressionBase
	{
	public:
		std::string value;

	public:
		std::string format(void) const;
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

	class CharacterLiteralExpression : public ExpressionBase
	{
	public:
		char value;

	public:
		std::string format(void) const;
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

	class ParenthesizedExpression : public ExpressionBase
	{
	public:
		ExpressionHandle inner;

	public:
		std::string format(void) const;
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

	class PostfixIncrementExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class PostfixDecrementExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class FunctionCallExpression : public ExpressionBase
	{
	public:
		ExpressionReference<IdentifierExpression> identifier;
		std::vector<ExpressionHandle> arguments;

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		FunctionCallExpression(ExpressionReference<IdentifierExpression> identifier, const std::vector<ExpressionHandle>& arguments)
			: identifier{ identifier }, arguments{ arguments }
		{
		}
	};
#define MAKE_FUNCTION_CALL_EXPRESSION(identifier, arguments) FunctionCallExpression{ make_handle(ast, identifier), arguments }

	class SubscriptExpression : public ExpressionBase
	{
	public:
		ExpressionHandle array;
		ExpressionHandle index;

	public:
		std::string format(void) const;
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
#define MAKE_SUBSCRIPT_EXPRESSION(array, index) SubscriptExpression{ make_handle(ast, array), make_handle(ast, index)

	class MemberAccessExpression : public ExpressionBase
	{
	public:
		ExpressionHandle object;
		ExpressionReference<IdentifierExpression> member;

	public:
		std::string format(void) const;
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

	class MemberAccessThroughPointerExpression : public ExpressionBase
	{
	public:
		ExpressionHandle object;
		ExpressionReference<IdentifierExpression> member;

	public:
		std::string format(void) const;
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

	class CompoundLiteralExpression : public ExpressionBase
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
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		CompoundLiteralExpression(TypeHandle type, const std::vector<Initializer>& initializers)
			: type{ type }, initializers{ initializers }
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

	class PrefixIncrementExpression : public ExpressionBase
	{
	public:
		// restricted to lvalues (unary expression only), checked at AST construction
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class PrefixDecrementExpression : public ExpressionBase
	{
	public:
		// restricted to lvalues (unary expression only), checked at AST construction
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class PlusExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class MinusExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class BitwiseNotExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class LogicalNotExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class CastExpression : public ExpressionBase
	{
	public:
		ExpressionHandle target;
		TypeHandle type;

	public:
		std::string format(void) const;
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

	class AddressOfExpression : public ExpressionBase
	{
	public:
		// operand must be an addressable lvalue or function designator
		// this is checked just before code generation rather than at AST construction
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class DereferenceExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class SizeOfExpressionExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		std::string format(void) const;
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

	class SizeOfTypeExpression : public ExpressionBase
	{
	public:
		TypeHandle type;

	public:
		std::string format(void) const;
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

	class MultiplyExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class DivideExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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
	
	class ModuloExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class AddExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class SubtractExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class LeftShiftExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class RightShiftExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class LessThanExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class LessThanOrEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class GreaterThanExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class GreaterThanOrEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class EqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class NotEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class BitwiseAndExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class BitwiseXorExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class BitwiseOrExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class LogicalAndExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class LogicalOrExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class TernaryExpression : public ExpressionBase
	{
	public:
		ExpressionHandle condition;
		ExpressionHandle then;
		ExpressionHandle otherwise;

	public:
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

	class AssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class AddAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class SubtractAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class MultiplyAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class DivideAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class ModuloAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class LeftShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class RightShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class BitwiseAndAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class BitwiseXorAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class BitwiseOrAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
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

	class CommaExpression : public ExpressionBase
	{
	public:
		std::vector<ExpressionHandle> expressions;

	public:
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		CommaExpression(const std::vector<ExpressionHandle>& expressions)
			: expressions{ expressions }
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
	concept ExpressionConcept = SumTuple<SumMemberT, SumStorageT, ExpressionASTMethods<SumStorageT>>;

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


	template<typename Self>
	ExpressionKind ExpressionBase::expression_kind(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, associativity, precedence, type, name) case TypeIndexV<type, typename Storage::Type>: return ExpressionKind::enumerator;
			PRIMARY_EXPRESSION_KINDS(X)
			POSTFIX_EXPRESSION_KINDS(X)
			UNARY_EXPRESSION_KINDS(X)
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid expression tag `{}`", self.tag_type()), self.token);

		// error recovery does not care about expression kind
		return ExpressionKind::INTEGER_LITERAL;
	}
}

#endif
