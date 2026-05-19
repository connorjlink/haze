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
	// forward declare sum storage and self-referential types for facade

	struct ExpressionSumStorage;

	using ExpressionHandle = SumHandle<ExpressionSumStorage>;

	template<typename T>
	using ExpressionReference = SumReference<T, ExpressionSumStorage>;

	using ExpressionBase = SumMemberBase<ExpressionSumStorage>;
}

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

	template<typename T>
	using ExpressionReference = ExpressionSum::template Reference<T>;

	using ExpressionHandle = ExpressionSum::Handle;


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
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		ExpressionHandle get_type(const Storage&) const;
	};
#define MAKE_INTEGER_LITERAL_EXPRESSION(value) IntegerLiteralExpression{ value }

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
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		ExpressionHandle get_type(const Storage&) const;
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
		ExpressionHandle target;

	public:
		AdjustExpression(Kind adjust_kind, ExpressionHandle target)
			: adjust_kind{ adjust_kind }, target{ target }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		ExpressionHandle get_type(const Storage&) const;
	};
#define MAKE_ADJUST_EXPRESSION(target, adjust_kind) AdjustExpression{ adjust_kind, MAKE_HANDLE(ast, target) }

	class ArgumentExpression : public ExpressionBase
	{
	public:
		TypeHandle type;
		ExpressionReference<IdentifierExpression> identifier;

	public:
		ArgumentExpression(TypeHandle type, ExpressionReference<IdentifierExpression> identifier)
			: type{ type }, identifier{ identifier }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		ExpressionHandle get_type(const Storage&) const;
	};
#define MAKE_ARGUMENT_EXPRESSION(type, identifier) ArgumentExpression{ type, identifier }

	class FunctionArgumentExpression : public ExpressionBase
	{
	public:
		TypeHandle type;
		ExpressionReference<IdentifierExpression> identifier;
		ExpressionHandle value;

	public:
		FunctionArgumentExpression(TypeHandle type, ExpressionReference<IdentifierExpression> identifier, ExpressionHandle value)
			: type{ type }, identifier{ identifier }, value{ value }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		ExpressionHandle get_type(const Storage&) const;
	};
#define MAKE_FUNCTION_ARGUMENT_EXPRESSION(type, identifier, value) FunctionArgumentExpression{ type, MAKE_REFERENCE(ast, identifier), MAKE_HANDLE(ast, value) }

	class FunctionCallExpression : public ExpressionBase
	{
	public:
		ExpressionReference<IdentifierExpression> identifier;
		std::vector<ExpressionHandle> arguments;

	public:
		FunctionCallExpression(ExpressionReference<IdentifierExpression> identifier, std::vector<ExpressionHandle> arguments)
			: identifier{ identifier }, arguments{ arguments }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		ExpressionHandle get_type(const Storage&) const;
	};
#define MAKE_FUNCTION_CALL_EXPRESSION(identifier, arguments) FunctionCallExpression{ MAKE_HANDLE(ast, identifier), arguments }

	class TernaryExpression : public ExpressionBase
	{
	public:
		ExpressionHandle condition;
		ExpressionHandle true_branch;
		ExpressionHandle false_branch;

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		ExpressionHandle get_type(const Storage&) const;
	};
#define MAKE_TERNARY_EXPRESSION(condition, true_expression, false_expression) TernaryExpression{ MAKE_HANDLE(ast, condition), MAKE_HANDLE(ast, true_expression), MAKE_HANDLE(ast, false_expression) }
}

#endif
