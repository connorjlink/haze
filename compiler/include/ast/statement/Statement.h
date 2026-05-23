#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

#include <allocator/Value.h>
#include <ast/AST.h>
#include <ast/declaration/Declaration.h>
#include <data/DependencyInjector.h>
#include <toolchain/models/Token.h>
#include <toolchain/RISCVAssemblerParser.h>
#include <type/Type.h>

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

	public:
		template<typename Self>
		StatementKind statement_kind(this Self&& self)
		{
			switch (self.ttype())
			{
#define X(enumerator, type, name) case TypeIndexV<type, typename StatementSumStorage::Type>: return StatementKind::enumerator;
#include <ast/statement/defs/StatementKind.def>
#undef X
			}

			return "<unknown statement kind>";
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

	class GotoStatement : public StatementBase
	{
	private:
		std::string label;

	public:
		GotoStatement(const std::string& label, const Token& token)
			: StatementBase{ token }, label{ label }
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
#define MAKE_GOTO_STATEMENT(label, token) GotoStatement{ label, token }

	class ContinueStatement : public StatementBase
	{
	public:
		ContinueStatement(const Token& token)
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
#define MAKE_CONTINUE_STATEMENT(token) ContinueStatement{ token }

	class BreakStatement : public StatementBase
	{
	public:
		BreakStatement(const Token& token)
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
#define MAKE_BREAK_STATEMENT(token) BreakStatement{ token }

	class SwitchStatement : public StatementBase
	{
	public:
		struct Case
		{
			ExpressionHandle condition;
			StatementHandle statement;
		};

	private:
		ExpressionHandle condition;
		std::vector<Case> cases;
		StatementHandle fallback;

	public:
		SwitchStatement(ExpressionHandle condition, const std::vector<Case>& cases, StatementHandle fallback, const Token& token)
			: StatementBase{ token }, condition{ condition }, cases{ cases }, fallback{ fallback }
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
#define MAKE_SWITCH_STATEMENT(condition, cases, fallback, token) SwitchStatement{ MAKE_HANDLE(ast, condition), cases, fallback, token }

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

	class LabeledStatement : public StatementBase
	{
	private:
		std::string label;
		StatementHandle statement;

	public:
		LabeledStatement(const std::string& label, StatementHandle statement, const Token& token)
			: StatementBase{ token }, label{ label }, statement{ statement }
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
#define MAKE_LABELED_STATEMENT(label, statement, token) LabeledStatement{ label, statement, token }

	class DeclarationStatement : public StatementBase
	{
	private:
		DeclarationHandle declaration;

	public:
		DeclarationStatement(DeclarationHandle declaration, const Token& token)
			: StatementBase{ token }, declaration{ declaration }
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
#define MAKE_DECLARATION_STATEMENT(declaration, token) DeclarationStatement{ declaration, token }

	class InlineAssemblyStatement
		: public StatementBase
		, public InjectService<AssemblerParser>
	{
	private:
		std::vector<CommandHandle> commands;

	public:
		InlineAssemblyStatement(const std::vector<CommandHadle>& commands, const Token& token)
			: StatementBase{ token }, commands{ commands }
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
#define MAKE_INLINE_ASSEMBLY_STATEMENT(commands, token) InlineAssemblyStatement{ commands, token }


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
		GotoStatement,
		ContinueStatement,
		BreakStatement,
		ReturnStatement,
		CompoundStatement,
		VariableDeclarationStatement,
		InlineAssemblyStatement,
		LabeledStatement,
	>;

	using StatementSum = MakeSum<ASTMethods, StatementTypes>::Type;

	template<typename T>
	using StatementReference = StatementSum::template Reference<T>;

	using StatementHandle = StatementSum::Handle;
}

#endif
