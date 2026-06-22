#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

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

#define EXPRESSION_expression_storage_METHODS(X, handlet) \
	BASE_expression_storage_METHODS(X, handlet) \
	X(get_type, TypeHandle)

	DEFINE_SUM(Expression, EXPRESSION_expression_storage_METHODS)


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
#define MAKE_IDENTIFIER_EXPRESSION(token, identifier_kind, name) \
	MAKE_REFERENCE(IdentifierExpression, Expression, EXPAND(IdentifierExpression{ token, identifier_kind, name }), expression_storage)

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
#define MAKE_INTEGER_LITERAL_EXPRESSION(token, value) \
	MAKE_REFERENCE(IntegerLiteralExpression, Expression, EXPAND(IntegerLiteralExpression{ token, value }), expression_storage)

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
#define MAKE_FLOAT_LITERAL_EXPRESSION(token, value) \
	MAKE_REFERENCE(FloatLiteralExpression, Expression, EXPAND(FloatLiteralExpression{ token, value }), expression_storage)

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
#define MAKE_STRING_LITERAL_EXPRESSION(token, value) \
	MAKE_REFERENCE(StringLiteralExpression, Expression, EXPAND(StringLiteralExpression{ token, value }), expression_storage)

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
#define MAKE_CHARACTER_LITERAL_EXPRESSION(token, value) \
	MAKE_REFERENCE(CharacterLiteralExpression, Expression, EXPAND(CharacterLiteralExpression{ token, value }), expression_storage)

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
#define MAKE_PARENTHESIZED_EXPRESSION(token, inner) \
	MAKE_REFERENCE(ParenthesizedExpression, Expression, EXPAND(ParenthesizedExpression{ token, inner }), expression_storage)

	using PrimaryExpressionTypes = SumTypeList
	<
#define X(enumerator, token, associativity, precedence, type, name) type,
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
#define MAKE_POSTFIX_INCREMENT_EXPRESSION(token, operand) \
	MAKE_REFERENCE(PostfixIncrementExpression, Expression, EXPAND(PostfixIncrementExpression{ token, operand }), expression_storage)

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
#define MAKE_POSTFIX_DECREMENT_EXPRESSION(token, operand) \
	MAKE_REFERENCE(PostfixDecrementExpression, Expression, EXPAND(PostfixDecrementExpression{ token, operand }), expression_storage)

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
#define MAKE_FUNCTION_CALL_EXPRESSION(token, identifier, arguments) \
	MAKE_REFERENCE(FunctionCallExpression, Expression, EXPAND(FunctionCallExpression{ token, identifier, arguments }), expression_storage)

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
#define MAKE_SUBSCRIPT_EXPRESSION(token, array, index) \
	MAKE_REFERENCE(SubscriptExpression, Expression, EXPAND(SubscriptExpression{ token, array, index }), expression_storage)

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
#define MAKE_MEMBER_ACCESS_EXPRESSION(token, object, member) \
	MAKE_REFERENCE(MemberAccessExpression, Expression, EXPAND(MemberAccessExpression{ token, object, member }), expression_storage)

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
#define MAKE_MEMBER_ACCESS_THROUGH_POINTER_EXPRESSION(token, object, member) \
	MAKE_REFERENCE(MemberAccessThroughPointerExpression, Expression, EXPAND(MemberAccessThroughPointerExpression{ token, object, member }), expression_storage)

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
#define MAKE_COMPOUND_LITERAL_EXPRESSION(token, type, initializers) \
	MAKE_REFERENCE(CompoundLiteralExpression, Expression, EXPAND(CompoundLiteralExpression{ token, type, initializers }), expression_storage)

	using PostfixExpressionTypes = SumTypeList
	<
#define X(enumerator, token, associativity, precedence, type, name) type,
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
		// restricted to lvalues (unary expression only), checked at expression_storage construction
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
#define MAKE_PREFIX_INCREMENT_EXPRESSION(token, operand) \
	MAKE_REFERENCE(PrefixIncrementExpression, Expression, EXPAND(PrefixIncrementExpression{ token, operand }), expression_storage)

	struct PrefixDecrementExpression : public ExpressionBase
	{
	public:
		// restricted to lvalues (unary expression only), checked at expression_storage construction
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
#define MAKE_PREFIX_DECREMENT_EXPRESSION(token, operand) \
	MAKE_REFERENCE(PrefixDecrementExpression, Expression, EXPAND(PrefixDecrementExpression{ token, operand }), expression_storage)

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
#define MAKE_PLUS_EXPRESSION(token, operand) \
	MAKE_REFERENCE(PlusExpression, Expression, EXPAND(PlusExpression{ token, operand }), expression_storage)

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
#define MAKE_MINUS_EXPRESSION(token, operand) \
	MAKE_REFERENCE(MinusExpression, Expression, EXPAND(MinusExpression{ token, operand }), expression_storage)

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
#define MAKE_BITWISE_NOT_EXPRESSION(token, operand) \
	MAKE_REFERENCE(BitwiseNotExpression, Expression, EXPAND(BitwiseNotExpression{ token, operand }), expression_storage)

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
#define MAKE_LOGICAL_NOT_EXPRESSION(token, operand) \
	MAKE_REFERENCE(LogicalNotExpression, Expression, EXPAND(LogicalNotExpression{ token, operand }), expression_storage)

	struct Cexpression_storageExpression : public ExpressionBase
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
		Cexpression_storageExpression(const Token& token, ExpressionHandle target, TypeHandle type)
			: ExpressionBase{ token }, target{ target }, type{ type }
		{
		}
	};
#define MAKE_Cexpression_storage_EXPRESSION(token, target, type) \
	MAKE_REFERENCE(Cexpression_storageExpression, Expression, EXPAND(Cexpression_storageExpression{ token, target, type }), expression_storage)

	struct AddressOfExpression : public ExpressionBase
	{
	public:
		// operand must be an addressable lvalue or function designator
		// this is checked just before code generation rather than at expression_storage construction
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
#define MAKE_ADDRESS_OF_EXPRESSION(token, operand) \
	MAKE_REFERENCE(AddressOfExpression, Expression, EXPAND(AddressOfExpression{ token, operand }), expression_storage)

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
#define MAKE_DEREFERENCE_EXPRESSION(token, operand) \
	MAKE_REFERENCE(DereferenceExpression, Expression, EXPAND(DereferenceExpression{ token, operand }), expression_storage)

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
#define MAKE_SIZEOF_EXPRESSION_EXPRESSION(token, operand) \
	MAKE_REFERENCE(SizeOfExpressionExpression, Expression, EXPAND(SizeOfExpressionExpression{ token, operand }), expression_storage)

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
#define MAKE_SIZEOF_TYPE_EXPRESSION(token, type) \
	MAKE_REFERENCE(SizeOfTypeExpression, Expression, EXPAND(SizeOfTypeExpression{ token, type }), expression_storage)

	using UnaryExpressionTypes = SumTypeList
	<
#define X(enumerator, token, associativity, precedence, type, name) type,
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
#define MAKE_MULTIPLY_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(MultiplyExpression, Expression, EXPAND(MultiplyExpression{ token, left, right }), expression_storage)

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
#define MAKE_DIVIDE_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(DivideExpression, Expression, EXPAND(DivideExpression{ token, left, right }), expression_storage)

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
#define MAKE_MODULO_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(ModuloExpression, Expression, EXPAND(ModuloExpression{ token, left, right }), expression_storage)

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
#define MAKE_ADD_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(AddExpression, Expression, EXPAND(AddExpression{ token, left, right }), expression_storage)

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
#define MAKE_SUBTRACT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(SubtractExpression, Expression, EXPAND(SubtractExpression{ token, left, right }), expression_storage)

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
#define MAKE_LEFT_SHIFT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(LeftShiftExpression, Expression, EXPAND(LeftShiftExpression{ token, left, right }), expression_storage)

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
#define MAKE_RIGHT_SHIFT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(RightShiftExpression, Expression, EXPAND(RightShiftExpression{ token, left, right }), expression_storage)

	struct LessExpression : public ExpressionBase
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
		LessExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LESS_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(LessExpression, Expression, EXPAND(LessExpression{ token, left, right }), expression_storage)

	struct LessEqualExpression : public ExpressionBase
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
		LessEqualExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_LESS_EQUAL_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(LessEqualExpression, Expression, EXPAND(LessEqualExpression{ token, left, right }), expression_storage)

	struct GreaterExpression : public ExpressionBase
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
		GreaterExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_GREATER_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(GreaterExpression, Expression, EXPAND(GreaterExpression{ token, left, right }), expression_storage)

	struct GreaterEqualExpression : public ExpressionBase
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
		GreaterEqualExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_GREATER_EQUAL_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(GreaterEqualExpression, Expression, EXPAND(GreaterEqualExpression{ token, left, right }), expression_storage)

	struct EqualExpression : public ExpressionBase
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
		EqualExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_EQUAL_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(EqualExpression, Expression, EXPAND(EqualExpression{ token, left, right }), expression_storage)

	struct NotEqualExpression : public ExpressionBase
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
		NotEqualExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_NOT_EQUAL_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(NotEqualExpression, Expression, EXPAND(NotEqualExpression{ token, left, right }), expression_storage)

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
#define MAKE_BITWISE_AND_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(BitwiseAndExpression, Expression, EXPAND(BitwiseAndExpression{ token, left, right }), expression_storage)

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
#define MAKE_BITWISE_XOR_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(BitwiseXorExpression, Expression, EXPAND(BitwiseXorExpression{ token, left, right }), expression_storage)

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
#define MAKE_BITWISE_OR_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(BitwiseOrExpression, Expression, EXPAND(BitwiseOrExpression{ token, left, right }), expression_storage)

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
#define MAKE_LOGICAL_AND_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(LogicalAndExpression, Expression, EXPAND(LogicalAndExpression{ token, left, right }), expression_storage)

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
#define MAKE_LOGICAL_OR_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(LogicalOrExpression, Expression, EXPAND(LogicalOrExpression{ token, left, right }), expression_storage)

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
#define MAKE_TERNARY_EXPRESSION(token, condition, then, otherwise) \
	MAKE_REFERENCE(TernaryExpression, Expression, EXPAND(TernaryExpression{ token, condition, then, otherwise }), expression_storage)

	struct AssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(AssignmentExpression, Expression, EXPAND(AssignmentExpression{ token, left, right }), expression_storage)

	struct AddAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_ADD_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(AddAssignmentExpression, Expression, EXPAND(AddAssignmentExpression{ token, left, right }), expression_storage)

	struct SubtractAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_SUBTRACT_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(SubtractAssignmentExpression, Expression, EXPAND(SubtractAssignmentExpression{ token, left, right }), expression_storage)

	struct MultiplyAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_MULTIPLY_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(MultiplyAssignmentExpression, Expression, EXPAND(MultiplyAssignmentExpression{ token, left, right }), expression_storage)

	struct DivideAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_DIVIDE_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(DivideAssignmentExpression, Expression, EXPAND(DivideAssignmentExpression{ token, left, right }), expression_storage)

	struct ModuloAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_MODULO_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(ModuloAssignmentExpression, Expression, EXPAND(ModuloAssignmentExpression{ token, left, right }), expression_storage)

	struct LeftShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_LEFT_SHIFT_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(LeftShiftAssignmentExpression, Expression, EXPAND(LeftShiftAssignmentExpression{ token, left, right }), expression_storage)

	struct RightShiftAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_RIGHT_SHIFT_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(RightShiftAssignmentExpression, Expression, EXPAND(RightShiftAssignmentExpression{ token, left, right }), expression_storage)

	struct BitwiseAndAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_BITWISE_AND_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(BitwiseAndAssignmentExpression, Expression, EXPAND(BitwiseAndAssignmentExpression{ token, left, right }), expression_storage)

	struct BitwiseXorAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_BITWISE_XOR_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(BitwiseXorAssignmentExpression, Expression, EXPAND(BitwiseXorAssignmentExpression{ token, left, right }), expression_storage)

	struct BitwiseOrAssignmentExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
#define MAKE_BITWISE_OR_ASSIGNMENT_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(BitwiseOrAssignmentExpression, Expression, EXPAND(BitwiseOrAssignmentExpression{ token, left, right }), expression_storage)

	struct CommaExpression : public ExpressionBase
	{
	public:
		// restricted to assignable lvalues, checked at expression_storage construction and before code generation
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
		CommaExpression(const Token& token, ExpressionHandle left, ExpressionHandle right)
			: ExpressionBase{ token }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_COMMA_EXPRESSION(token, left, right) \
	MAKE_REFERENCE(CommaExpression, Expression, EXPAND(CommaExpression{ token, left, right }), expression_storage)

	using BinaryExpressionTypes = SumTypeList
	<
#define X(enumerator, token, associativity, precedence, type, name) type,
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
#define X(enumerator, token, associativity, precedence, type, name) type,
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
