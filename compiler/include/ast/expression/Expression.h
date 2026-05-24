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

	using ExpressionFacade = SumMemberBase<ExpressionSumStorage>;

	class ExpressionBase
		: public ExpressionFacade
		, public InjectService<Generator>
	{
	public:
		template<typename Self>
		bool check_types(this Self&& self, const Storage& ast)
		{
			return self.get_type().is_valid();
		}

		template<typename Self>
		ExpressionKind expression_kind(this Self&& self)
		{
			switch (self.ttype())
			{

			}

			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid expression tag `{}`", self.ttype()), self.token);

			return ExpressionKind::;
		}
	};
}

namespace hz
{
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
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_INTEGER_LITERAL_EXPRESSION(value) IntegerLiteralExpression{ value }

	class FloatLiteralExpression : public ExpressionBase
	{
	public:
		BigFloat value;

	public:
		FloatLiteralExpression(BigFloat value)
			: value{ value }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_FLOAT_LITERAL_EXPRESSION(value) FloatLiteralExpression{ value }

	class StringLiteralExpression : public ExpressionBase
	{
	public:
		std::string value;

	public:
		StringLiteralExpression(const std::string& value)
			: value{ value }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_STRING_LITERAL_EXPRESSION(value) StringLiteralExpression{ value }

	class CharacterLiteralExpression : public ExpressionBase
	{
	public:
		char value;

	public:
		CharacterLiteralExpression(char value)
			: value{ value }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_CHARACTER_LITERAL_EXPRESSION(value) CharacterLiteralExpression{ value }

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
		TypeHandle get_type(const Storage&) const;
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
		TypeHandle get_type(const Storage&) const;
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
		TypeHandle get_type(const Storage&) const;
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
		TypeHandle get_type(const Storage&) const;
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
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_FUNCTION_CALL_EXPRESSION(identifier, arguments) FunctionCallExpression{ MAKE_HANDLE(ast, identifier), arguments }

	



	class BinaryExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;
		BinaryExpressionKind kind;

	public:
		std::string format() const;
		void generate(const Storage& ast) const;
		ExpressionHandle evaluate(const Storage& ast, const Context& context) const;
		ExpressionHandle optimize(const Storage& ast) const;
		ExpressionHandle get_type(const Storage& ast) const;

	public:
		BinaryExpression(ExpressionHandle left, ExpressionHandle right, BinaryExpressionKind kind)
			: left{ left }, right{ right }, kind{ kind }
		{
		}
	};
#define MAKE_BINARY_EXPRESSION(left, right, kind) BinaryExpression{ MAKE_HANDLE(ast, left), MAKE_HANDLE(ast, right), kind }

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
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_TERNARY_EXPRESSION(condition, true_expression, false_expression) TernaryExpression{ MAKE_HANDLE(ast, condition), MAKE_HANDLE(ast, true_expression), MAKE_HANDLE(ast, false_expression) }

	class CastExpression : public ExpressionBase
	{
	public:
		ExpressionHandle target;
		TypeHandle type;

	public:
		CastExpression(ExpressionHandle target, TypeHandle type)
			: target{ target }, type{ type }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		ExpressionHandle evaluate(const Storage&, Context&) const;
		ExpressionHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_CAST_EXPRESSION(target, type) CastExpression{ MAKE_HANDLE(ast, target), type }


	using ExpressionTypes = SumTypeList
	<
		AdjustExpression,
		ArgumentExpression,
		FunctionArgumentExpression,
		FunctionCallExpression,
		CastExpression,
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

	template<typename T>
	using ExpressionReference = ExpressionSum::template Reference<T>;

	using ExpressionHandle = ExpressionSum::Handle;
}

#endif
