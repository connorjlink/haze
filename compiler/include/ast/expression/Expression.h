#ifndef HAZE_EXPRESSION_AST_H
#define HAZE_EXPRESSION_AST_H

#include <ast/AST.h>
#include <runtime/Context.h>
#include <type/Type.h>
#include <toolchain/Generator.h>

// Haze Expression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AdjustExpression;
	class ArgumentExpression;
	class FunctionArgumentExpression;
	class FunctionCallExpression;
	class CastExpression;
	class ConditionalExpression;
	class IdentifierExpression;
	class IntegerLiteralExpression;
	class FloatLiteralExpression;
	class StringLiteralExpression;
	class StructOrUnionLiteralExpression;
	class UnaryExpression;
	class BinaryExpression;
	class TernaryExpression;
	class MemberAccessExpression;
	class ArrayAccessExpression;

	using ExpressionTypes = SumTypeList
	<
		AdjustExpression,
		ArgumentExpression,
		FunctionArgumentExpression,
		FunctionCallExpression,
		CastExpression,
		ConditionalExpression,
		IdentifierExpression,
		IntegerLiteralExpression,
		FloatLiteralExpression,
		StringLiteralExpression,
		StructOrUnionLiteralExpression,
		UnaryExpression,
		BinaryExpression,
		TernaryExpression,
		MemberAccessExpression,
		ArrayAccessExpression
	>;

	using ExpressionSum = MakeSum<ASTMethods, ExpressionTypes>::Type;
	using ExpressionSumBase = SumMemberBase<ExpressionSum>;


	class ExpressionBase
		: public ExpressionSumBase
		, public InjectService<Generator>
	{
	public:
		template<typename Self>
		bool check_types(this Self&& self, const Storage& ast)
		{
			return self.get_type().is_valid();
		}
	};


	class IntegerLiteralExpression : public ExpressionBase
	{
	public:
		BigInteger value;

	public:
		IntegerLiteralExpression(BigInteger value)
			: value{ value }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		SumHandle evaluate(const Storage&, Context&) const;
		SumHandle optimize(const Storage&) const;
		SumHandle get_type(const Storage&) const;
	};
#define MAKE_INTEGER_LITERAL_EXPRESSION(value) IntegerLiteralExpression{ MAKE_HANDLE(ast, value) }


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
		Kind identifier_kind;
		std::string name;

	public:
		IdentifierExpression(Kind identifier_kind, const std::string& name)
			: identifier_kind{ identifier_kind }, name{ name }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		SumHandle evaluate(const Storage&, Context&) const;
		SumHandle optimize(const Storage&) const;
		SumHandle get_type(const Storage&) const;
	};
#define MAKE_IDENTIFIER_EXPRESSION(identifier_kind, name) IdentifierExpression{ identifier_kind, name }

	class AdjustExpression : public ExpressionBase
	{
	public:
		enum class Kind
		{
			PRE_INCREMENT,
			POST_INCREMENT,
			PRE_DECREMENT,
			POST_DECREMENT,
		};

	public:
		Kind adjust_kind;
		SumHandle target;

	public:
		AdjustType(Type adjust_kind, SumHandle target)
			: adjust_kind{ adjust_kind }, target{ target }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		SumHandle evaluate(const Storage&, Context&) const;
		SumHandle optimize(const Storage&) const;
		SumHandle get_type(const Storage&) const;
	};
#define MAKE_ADJUST_EXPRESSION(target, adjust_kind) AdjustExpression{ adjust_kind, MAKE_HANDLE(ast, target) }

	class ArgumentExpression : public ExpressionBase
	{
	public:
		Type type;
		IdentifierExpression identifier;

	public:
		ArgumentExpression(Type type, IdentifierExpression identifier)
			: type{ type }, identifier{ identifier }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		SumHandle evaluate(const Storage&, Context&) const;
		SumHandle optimize(const Storage&) const;
		SumHandle get_type(const Storage&) const;
	};
#define MAKE_ARGUMENT_EXPRESSION(type, identifier) ArgumentExpression{ type, identifier }

	class FunctionArgumentExpression : public ExpressionBase
	{
	public:
		Type type;
		SumReference<IdentifierExpression> identifier;
		SumHandle value;

	public:
		FunctionArgumentExpression(Type type, SumReference<IdentifierExpression> identifier, SumHandle value)
			: type{ type }, identifier{ identifier }, value{ value }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		SumHandle evaluate(const Storage&, Context&) const;
		SumHandle optimize(const Storage&) const;
		bool check_types(const Storage&) const;
	};
#define MAKE_FUNCTION_ARGUMENT_EXPRESSION(type, identifier, value) FunctionArgumentExpression{ type, MAKE_REFERENCE(ast, identifier), MAKE_HANDLE(ast, value) }

	class FunctionCallExpression : public ExpressionBase
	{
	public:
		SumReference<IdentifierExpression> identifier;
		std::vector<SumHandle<Expression>> arguments;

	public:
		FunctionCallExpression(SumHandle<IdentifierExpression> identifier, std::vector<SumHandle<Expression>> arguments)
			: identifier{ identifier }, arguments{ arguments }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		SumHandle evaluate(const Storage&, Context&) const;
		SumHandle optimize(const Storage&) const;
		bool check_types(const Storage&) const;
	};
#define MAKE_FUNCTION_CALL_EXPRESSION(identifier, arguments) FunctionCallExpression{ MAKE_HANDLE(ast, identifier), arguments }

	class TernaryExpression : public ExpressionBase
	{
	public:
		SumHandle condition;
		SumHandle true_branch;
		SumHandle false_branch;

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		SumHandle evaluate(const Storage&, Context&) const;
		SumHandle optimize(const Storage&) const;
		bool check_types(const Storage&) const;
	};
#define MAKE_TERNARY_EXPRESSION(condition, true_expression, false_expression) TernaryExpression{ MAKE_HANDLE(ast, condition), MAKE_HANDLE(ast, true_expression), MAKE_HANDLE(ast, false_expression) }
}

#endif
