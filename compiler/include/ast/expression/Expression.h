#ifndef HAZE_EXPRESSION_AST_H
#define HAZE_EXPRESSION_AST_H

#include <ast/AST.h>
#include <ast/expression/defs/ExpressionKind.h>
#include <command/Command.h>
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

	FORWARD_DECLARE_SUM(Expression)

#define EXPRESSION_AST_METHODS(X, handlet) \
	BASE_AST_METHODS(X, handlet) \
	X(get_type, TypeHandle)

	DEFINE_SUM(Expression, EXPRESSION_AST_METHODS)


	struct ExpressionBase
		: public ExpressionFacade
		, public InjectService<Generator>
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = ExpressionStorage;

	public:
		Token token;

	public:
		template<typename Self>
		bool check_types(this Self&& self)
		{
			return self.get_type().is_valid();
		}

	public:
		ExpressionBase(const Token& token)
			: token{ token }
		{
		}
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
		std::string_view name;
		Kind identifier_kind;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		IdentifierExpression(const Token& token, Kind identifier_kind, std::string_view name)
			: ExpressionBase{ token }, identifier_kind{ identifier_kind }, name{ name }
		{
		}
	};
#define MAKE_IDENTIFIER_EXPRESSION(token, identifier_kind, name) MAKE_REFERENCE(IdentifierExpression, Expression, EXPAND(IdentifierExpression{ token, identifier_kind, name }), expression_storage)

	struct IntegerLiteralExpression : public ExpressionBase
	{
	public:
		BigInteger value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		IntegerLiteralExpression(const Token& token, BigInteger value)
			: ExpressionBase{ token }, value{ value }
		{
		}
	};
#define MAKE_INTEGER_LITERAL_EXPRESSION(token, value) MAKE_REFERENCE(IntegerLiteralExpression, Expression, EXPAND(IntegerLiteralExpression{ token, value }), expression_storage)

	struct FloatLiteralExpression : public ExpressionBase
	{
	public:
		BigFloat value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		FloatLiteralExpression(const Token& token, BigFloat value)
			: ExpressionBase{ token }, value{ value }
		{
		}
	};
#define MAKE_FLOAT_LITERAL_EXPRESSION(token, value) FloatLiteralExpression{ token, value }

	struct StringLiteralExpression : public ExpressionBase
	{
	public:
		std::string_view value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		StringLiteralExpression(const Token& token, std::string_view value)
			: ExpressionBase{ token }, value{ value }
		{
		}
	};
#define MAKE_STRING_LITERAL_EXPRESSION(token, value) MAKE_REFERENCE(StringLiteralExpression, Expression, EXPAND(StringLiteralExpression{ token, value }), expression_storage)

	struct CharacterLiteralExpression : public ExpressionBase
	{
	public:
		char value;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		CharacterLiteralExpression(const Token& token, char value)
			: ExpressionBase{ token }, value{ value }
		{
		}
	};
#define MAKE_CHARACTER_LITERAL_EXPRESSION(token, value) CharacterLiteralExpression{ token, value }

	struct ParenthesizedExpression : public ExpressionBase
	{
	public:
		ExpressionHandle inner;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		ParenthesizedExpression(const Token& token, ExpressionHandle inner)
			: ExpressionBase{ token }, inner{ inner }
		{
		}
	};
#define MAKE_PARENTHESIZED_EXPRESSION(token, inner) ParenthesizedExpression{ token, make_handle(ast, inner) }

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
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		PostfixIncrementExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_POSTFIX_INCREMENT_EXPRESSION(token, operand) PostfixIncrementExpression{ token, make_handle(ast, operand) }

	struct PostfixDecrementExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		PostfixDecrementExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_POSTFIX_DECREMENT_EXPRESSION(token, operand) PostfixDecrementExpression{ token, make_handle(ast, operand) }

	struct FunctionCallExpression : public ExpressionBase
	{
	public:
		ExpressionReference<IdentifierExpression> identifier;
		std::vector<ExpressionHandle> arguments;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		FunctionCallExpression(const Token& token, ExpressionReference<IdentifierExpression> identifier, std::vector<ExpressionHandle> arguments)
			: ExpressionBase{ token }, identifier{ identifier }, arguments{ std::move(arguments) }
		{
		}
	};
#define MAKE_FUNCTION_CALL_EXPRESSION(token, identifier, arguments) FunctionCallExpression{ token, make_handle(ast, identifier), arguments }

	struct SubscriptExpression : public ExpressionBase
	{
	public:
		ExpressionHandle array;
		ExpressionHandle index;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		SubscriptExpression(const Token& token, ExpressionHandle array, ExpressionHandle index)
			: ExpressionBase{ token }, array{ array }, index{ index }
		{
		}
	};
#define MAKE_SUBSCRIPT_EXPRESSION(token, array, index) SubscriptExpression{ token, make_handle(ast, array), make_handle(ast, index) }

	struct MemberAccessExpression : public ExpressionBase
	{
	public:
		ExpressionHandle object;
		ExpressionReference<IdentifierExpression> member;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		MemberAccessExpression(const Token& token, ExpressionHandle object, ExpressionReference<IdentifierExpression> member)
			: ExpressionBase{ token }, object{ object }, member{ member }
		{
		}
	};
#define MAKE_MEMBER_ACCESS_EXPRESSION(token, object, member) MemberAccessExpression{ token, make_handle(ast, object), make_reference(ast, member) }

	struct MemberAccessThroughPointerExpression : public ExpressionBase
	{
	public:
		ExpressionHandle object;
		ExpressionReference<IdentifierExpression> member;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		MemberAccessThroughPointerExpression(const Token& token, ExpressionHandle object, ExpressionReference<IdentifierExpression> member)
			: ExpressionBase{ token }, object{ object }, member{ member }
		{
		}
	};
#define MAKE_MEMBER_ACCESS_THROUGH_POINTER_EXPRESSION(token, object, member) MemberAccessThroughPointerExpression{ token, make_handle(ast, object), make_reference(ast, member) }

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
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		CompoundLiteralExpression(const Token& token, TypeHandle type, std::vector<Initializer> initializers)
			: ExpressionBase{ token }, type{ type }, initializers{ std::move(initializers) }
		{
		}
	};
#define MAKE_COMPOUND_LITERAL_EXPRESSION(token, type, initializers) CompoundLiteralExpression{ token, type, initializers }

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
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		PrefixIncrementExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_PREFIX_INCREMENT_EXPRESSION(token, operand) PrefixIncrementExpression{ token, make_handle(ast, operand) }

	struct PrefixDecrementExpression : public ExpressionBase
	{
	public:
		// restricted to lvalues (unary expression only), checked at AST construction
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		PrefixDecrementExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_PREFIX_DECREMENT_EXPRESSION(token, operand) PrefixDecrementExpression{ token, make_handle(ast, operand) }

	struct PlusExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		PlusExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_PLUS_EXPRESSION(token, operand) PlusExpression{ token, make_handle(ast, operand) }

	struct MinusExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		MinusExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_MINUS_EXPRESSION(token, operand) MinusExpression{ token, make_handle(ast, operand) }

	struct BitwiseNotExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		BitwiseNotExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_BITWISE_NOT_EXPRESSION(token, operand) BitwiseNotExpression{ token, make_handle(ast, operand) }

	struct LogicalNotExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		LogicalNotExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_LOGICAL_NOT_EXPRESSION(token, operand) LogicalNotExpression{ token, make_handle(ast, operand) }

	struct CastExpression : public ExpressionBase
	{
	public:
		ExpressionHandle target;
		TypeHandle type;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		CastExpression(const Token& token, ExpressionHandle target, TypeHandle type)
			: ExpressionBase{ token }, target{ target }, type{ type }
		{
		}
	};
#define MAKE_CAST_EXPRESSION(token, target, type) CastExpression{ token, make_handle(ast, target), type }

	struct AddressOfExpression : public ExpressionBase
	{
	public:
		// operand must be an addressable lvalue or function designator
		// this is checked just before code generation rather than at AST construction
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		AddressOfExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_ADDRESS_OF_EXPRESSION(token, operand) AddressOfExpression{ token, make_handle(ast, operand) }

	struct DereferenceExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		DereferenceExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_DEREFERENCE_EXPRESSION(token, operand) DereferenceExpression{ token, make_handle(ast, operand) }

	struct SizeOfExpressionExpression : public ExpressionBase
	{
	public:
		ExpressionHandle operand;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		SizeOfExpressionExpression(const Token& token, ExpressionHandle operand)
			: ExpressionBase{ token }, operand{ operand }
		{
		}
	};
#define MAKE_SIZEOF_EXPRESSION_EXPRESSION(token, operand) SizeOfExpressionExpression{ token, make_handle(ast, operand) }

	struct SizeOfTypeExpression : public ExpressionBase
	{
	public:
		TypeHandle type;

	public:
		ExpressionKind expression_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		SizeOfTypeExpression(const Token& token, TypeHandle type)
			: ExpressionBase{ token }, type{ type }
		{
		}
	};
#define MAKE_SIZEOF_TYPE_EXPRESSION(token, type) SizeOfTypeExpression{ token, type }

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
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		MultiplyExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_MULTIPLY_EXPRESSION(token, left, right) MultiplyExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct DivideExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const ExpressionStorage&) const;
		ExpressionHandle evaluate(const ExpressionStorage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		DivideExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_DIVIDE_EXPRESSION(token, left, right) DivideExpression{ token, make_handle(ast, left), make_handle(ast, right) }
	
	struct ModuloExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		ModuloExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_MODULO_EXPRESSION(token, left, right) ModuloExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct AddExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		AddExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_ADD_EXPRESSION(token, left, right) AddExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct SubtractExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		SubtractExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_SUBTRACT_EXPRESSION(token, left, right) SubtractExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct LeftShiftExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		LeftShiftExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LEFT_SHIFT_EXPRESSION(token, left, right) LeftShiftExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct RightShiftExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		RightShiftExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_RIGHT_SHIFT_EXPRESSION(token, left, right) RightShiftExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct LessThanExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		LessThanExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LESS_THAN_EXPRESSION(token, left, right) LessThanExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct LessThanOrEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		LessThanOrEqualToExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LESS_THAN_OR_EQUAL_TO_EXPRESSION(token, left, right) LessThanOrEqualToExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct GreaterThanExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		GreaterThanExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_GREATER_THAN_EXPRESSION(token, left, right) GreaterThanExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct GreaterThanOrEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		GreaterThanOrEqualToExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_GREATER_THAN_OR_EQUAL_TO_EXPRESSION(token, left, right) GreaterThanOrEqualToExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct EqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		EqualToExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_EQUAL_TO_EXPRESSION(token, left, right) EqualToExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct NotEqualToExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		NotEqualToExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_NOT_EQUAL_TO_EXPRESSION(token, left, right) NotEqualToExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseAndExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		BitwiseAndExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_AND_EXPRESSION(token, left, right) BitwiseAndExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseXorExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		BitwiseXorExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_XOR_EXPRESSION(token, left, right) BitwiseXorExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseOrExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		BitwiseOrExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_OR_EXPRESSION(token, left, right) BitwiseOrExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct LogicalAndExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		LogicalAndExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LOGICAL_AND_EXPRESSION(token, left, right) LogicalAndExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct LogicalOrExpression : public ExpressionBase
	{
	public:
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		LogicalOrExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LOGICAL_OR_EXPRESSION(token, left, right) LogicalOrExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct TernaryExpression : public ExpressionBase
	{
	public:
		ExpressionHandle condition;
		ExpressionHandle then;
		ExpressionHandle otherwise;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		TernaryExpression(const Token& token, ExpressionHandle condition, ExpressionHandle then, ExpressionHandle otherwise)
			: ExpressionBase{ token }, condition{ condition }, then{ then }, otherwise{ otherwise }
		{
		}
	};
#define MAKE_TERNARY_EXPRESSION(token, condition, then, otherwise) TernaryExpression{ token, make_handle(ast, condition), make_handle(ast, then), make_handle(ast, otherwise) }

	struct AssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		AssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_ASSIGNMENT_EXPRESSION(token, left, right) AssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct AddAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		AddAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_ADD_ASSIGNMENT_EXPRESSION(token, left, right) AddAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct SubtractAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		SubtractAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_SUBTRACT_ASSIGNMENT_EXPRESSION(token, left, right) SubtractAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct MultiplyAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		MultiplyAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_MULTIPLY_ASSIGNMENT_EXPRESSION(token, left, right) MultiplyAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct DivideAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		DivideAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_DIVIDE_ASSIGNMENT_EXPRESSION(token, left, right) DivideAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct ModuloAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		ModuloAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_MODULO_ASSIGNMENT_EXPRESSION(token, left, right) ModuloAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct LeftShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		LeftShiftAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LEFT_SHIFT_ASSIGNMENT_EXPRESSION(token, left, right) LeftShiftAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct RightShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		RightShiftAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_RIGHT_SHIFT_ASSIGNMENT_EXPRESSION(token, left, right) RightShiftAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseAndAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		BitwiseAndAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_AND_ASSIGNMENT_EXPRESSION(token, left, right) BitwiseAndAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseXorAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		BitwiseXorAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_XOR_ASSIGNMENT_EXPRESSION(token, left, right) BitwiseXorAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct BitwiseOrAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at AST construction and before code generation
		ExpressionHandle left;
		ExpressionHandle right;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		BitwiseOrAssignmentExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_BITWISE_OR_ASSIGNMENT_EXPRESSION(token, left, right) BitwiseOrAssignmentExpression{ token, make_handle(ast, left), make_handle(ast, right) }

	struct CommaExpression : public ExpressionBase
	{
	public:
		std::vector<ExpressionHandle> expressions;

	public:
		ExpressionKind expression_kind() const;
		std::string format() const;
		void generate(const CommandStorage&) const;
		ExpressionHandle evaluate(const Storage&, const Context&) const;
		ExpressionHandle optimize(const ExpressionStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		CommaExpression(const Token& token, std::vector<ExpressionHandle> expressions)
			: ExpressionBase{ token }, expressions{ std::move(expressions) }
		{
		}
	};
#define MAKE_COMMA_EXPRESSION(token, expressions) CommaExpression{ token, expressions }

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

	template<typename SumMemberT, typename StorageT>
	concept IsExpression = SumTuple<SumMemberT, StorageT, ExpressionMethods<typename StorageT::Anchor>>;

	using ExpressionKinds = SumTypeList
	<
#define X(enumerator, associativity, precedence, type, name) type,
		EXPRESSION_KINDS(X)
#undef X
		void
	>;


	using ExpressionSumImplementation = MakeSum<ExpressionMethods, ExpressionKinds>::Type;

	struct ExpressionStorage : public ExpressionSumImplementation::Storage
	{
		using ExpressionSumImplementation::Storage::Storage;

		using Type = ExpressionSumImplementation::Type;
		using Anchor = ExpressionSumImplementation::Anchor;
	};
}

#endif
