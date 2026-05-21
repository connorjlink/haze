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
		std::float64_t value;

	public:
		FloatLiteralExpression(std::float64_t value)
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

	enum class BinaryExpressionKind
	{
		ADD,
		ADD_ASSIGNMENT,
		SUBTRACT,
		SUBTRACT_ASSIGNMENT,
		MULTIPLY,
		MULTIPLY_ASSIGNMENT,
		DIVIDE,
		DIVIDE_ASSIGNMENT,
		MODULO,
		MODULO_ASSIGNMENT,
		LEFT_SHIFT,
		LEFT_SHIFT_ASSIGNMENT,
		RIGHT_SHIFT,
		RIGHT_SHIFT_ASSIGNMENT,
		BITWISE_AND,
		BITWISE_AND_ASSIGNMENT,
		BITWISE_OR,
		BITWISE_OR_ASSIGNMENT,
		BITWISE_XOR,
		BITWISE_XOR_ASSIGNMENT,
		LOGICAL_AND,
		LOGICAL_OR,
		EQUAL,
		NOT_EQUAL,
		LESS,
		LESS_EQUAL,
		GREATER,
		GREATER_EQUAL,
		DOT,
		ARROW,
		COMMA,
	};

	static const std::unordered_map<BinaryExpressionKind, std::string_view> binary_expression_kind_map
	{
		{ BinaryExpressionKind::ADD, " + " },
		{ BinaryExpressionKind::ADD_ASSIGNMENT, " += " },
		{ BinaryExpressionKind::SUBTRACT, " - " },
		{ BinaryExpressionKind::SUBTRACT_ASSIGNMENT, " -= " },
		{ BinaryExpressionKind::MULTIPLY, " * " },
		{ BinaryExpressionKind::MULTIPLY_ASSIGNMENT, " *= " },
		{ BinaryExpressionKind::DIVIDE, " / " },
		{ BinaryExpressionKind::DIVIDE_ASSIGNMENT, " /= " },
		{ BinaryExpressionKind::MODULO, " % " },
		{ BinaryExpressionKind::MODULO_ASSIGNMENT, " %= " },
		{ BinaryExpressionKind::LEFT_SHIFT, " << " },
		{ BinaryExpressionKind::LEFT_SHIFT_ASSIGNMENT, " <<= " },
		{ BinaryExpressionKind::RIGHT_SHIFT, " >> " },
		{ BinaryExpressionKind::RIGHT_SHIFT_ASSIGNMENT, " >>= " },
		{ BinaryExpressionKind::BITWISE_AND, " & " },
		{ BinaryExpressionKind::BITWISE_AND_ASSIGNMENT, " &= " },
		{ BinaryExpressionKind::BITWISE_OR, " | " },
		{ BinaryExpressionKind::BITWISE_OR_ASSIGNMENT, " |= " },
		{ BinaryExpressionKind::BITWISE_XOR, " ^ " },
		{ BinaryExpressionKind::BITWISE_XOR_ASSIGNMENT, " ^= " },
		{ BinaryExpressionKind::LOGICAL_AND, " && " },
		{ BinaryExpressionKind::LOGICAL_OR, " || " },
		{ BinaryExpressionKind::EQUAL, " == " },
		{ BinaryExpressionKind::NOT_EQUAL, " != " },
		{ BinaryExpressionKind::LESS, " < " },
		{ BinaryExpressionKind::LESS_EQUAL, " <= " },
		{ BinaryExpressionKind::GREATER, " > " },
		{ BinaryExpressionKind::GREATER_EQUAL, " >= " },
		{ BinaryExpressionKind::DOT, "." },
		{ BinaryExpressionKind::ARROW, "->" },
		{ BinaryExpressionKind::COMMA, ", " }
	};

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
}

#endif
