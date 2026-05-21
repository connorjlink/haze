#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

#include <ast/AST.h>
#include <type/Type.h>
#include <allocator/Value.h>

// Haze Statement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	struct StatementSumStorage;

	using StatementHandle = SumHandle<StatementSumStorage>;

	template<typename T>
	using StatementReference = SumReference<T, StatementSumStorage>;

	using StatementFacade = SumMemberBase<StatementSumStorage>;

	struct StatementBase : public StatementFacade
	{
	public:
		Token token;

	public:
		StatementBase(const Token& token)
			: token{ token }
		{
		}
	};
}

namespace hz
{
	class NullStatement : public StatementBase
	{
	public:
		NullStatement(const Token& token)
			: StatementBase{ token }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_NULL_STATEMENT(token) NullStatement{ token }

	class ExpressionStatement : public StatementBase
	{
	private:
		ExpressionHandle expression;

	public:
		ExpressionStatement(ExpressionHandle expression, const Token& token)
			: StatementBase{ token }, expression{ expression }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_EXPRESSION_STATEMENT(expression, token) ExpressionStatement{ MAKE_HANDLE(ast, expression), token }

	class ReturnStatement : public StatementBase
	{
	private:
		std::string enclosing_function;
		ExpressionHandle expression;
		ValueHandle value;

	public:
		ReturnStatement(const std::string& enclosing_function, ExpressionHandle expression, ValueHandle value, const Token& token)
			: StatementBase{ token }, enclosing_function{ enclosing_function }, expression{ expression }, value{ value }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_RETURN_STATEMENT(enclosing_function, expression, value, token) ReturnStatement{ enclosing_function, MAKE_HANDLE(ast, expression), value, token }

	class IfStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle if_body;
		StatementHandle else_body;

	public:
		IfStatement(ExpressionHandle condition, StatementHandle if_body, StatementHandle else_body, const Token& token)
			: StatementBase{ token }, condition{ condition }, if_body{ if_body }, else_body{ else_body }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_IF_STATEMENT(condition, if_body, else_body, token) IfStatement{ MAKE_HANDLE(ast, condition), if_body, else_body, token }

	class WhileStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle body;

	public:
		WhileStatement(ExpressionHandle condition, StatementHandle body, const Token& token)
			: StatementBase{ token }, condition{ condition }, body{ body }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_WHILE_STATEMENT(condition, body, token) WhileStatement{ MAKE_HANDLE(ast, condition), body, token }

	class DoStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle body;

	public:
		DoStatement(ExpressionHandle condition, StatementHandle body, const Token& token)
			: StatementBase{ token }, condition{ condition }, body{ body }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_DO_STATEMENT(condition, body, token) DoStatement{ MAKE_HANDLE(ast, condition), body, token }

	class ForStatement : public StatementBase
	{
	private:
		StatementHandle initialization;
		ExpressionHandle condition;
		ExpressionHandle induction;
		StatementHandle body;

	public:
		ForStatement(StatementHandle initialization, ExpressionHandle condition, ExpressionHandle induction, StatementHandle body, const Token& token)
			: StatementBase{ token }, initialization{ initialization }, condition{ condition }, induction{ induction }, body{ body }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_FOR_STATEMENT(initialization, condition, induction, body, token) ForStatement{ initialization, MAKE_HANDLE(ast, condition), MAKE_HANDLE(ast, induction), body, token }

	class CompoundStatement : public StatementBase
	{
	public:
		std::vector<StatementHandle> substatements;

	public:
		CompoundStatement(const std::vector<StatementHandle>& substatements, const Token& token)
			: StatementBase{ token }, substatements{ substatements }
		{
		}

	public:
		StatementKind tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const Storage&) const;
	};
#define MAKE_COMPOUND_STATEMENT(substatements, token) CompoundStatement{ substatements, token }

	class FunctionDeclarationStatement : public StatementBase
	{
	public:
		Function function;
	};

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept Type = SumTuple<SumMemberT, SumStorageT, TypeMethods<SumStorageT>>;

	// statements use the base AST node methods

	using StatementTypes = SumTypeList
	<
		NullStatement,
		ExpressionStatement,
		ReturnStatement,
		IfStatement,
		WhileStatement,
		DoStatement,
		ForStatement,
		CompoundStatement,
		FunctionDeclarationStatement,
		StructDeclarationStatement,
		VariableDeclarationStatement,
		InlineAssemblyStatement
	>;

	using StatementSum = MakeSum<ASTMethods, StatementTypes>::Type;

	template<typename T>
	using StatementReference = StatementSum::template Reference<T>;

	using StatementHandle = StatementSum::Handle;
}

#endif
