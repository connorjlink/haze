#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

#include <allocator/Value.h>
#include <ast/AST.h>
#include <ast/declaration/Declaration.h>
#include <ast/statement/defs/StatementKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/models/Token.h>
#include <toolchain/RISCVAssemblerParser.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Statement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Statement)

	template<typename MethodsT>
	using StatementASTMethods = ASTMethods<MethodsT, StatementHandle>;

	DEFINE_SUM(Statement, AST_METHODS)


	class StatementBase 
		: public StatementFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		using Storage = StatementSumStorage;

	public:
		Token token;

	public:
		StatementBase(const Token& token)
			: token{ token }
		{
		}

	public:
		template<typename Self>
		StatementKind statement_kind(this Self&&);
	};
}

namespace hz
{
	//////////////////////////////////////////////////////
	// C Statement Categories
	//////////////////////////////////////////////////////

	class NullStatement : public StatementBase
	{
	public:
		NullStatement(const Token& token)
			: StatementBase{ token }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_EXPRESSION_STATEMENT(expression, token) ExpressionStatement{ make_handle(ast, expression), token }

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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_RETURN_STATEMENT(enclosing_function, expression, value, token) ReturnStatement{ enclosing_function, make_handle(ast, expression), value, token }

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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_IF_STATEMENT(condition, if_body, else_body, token) IfStatement{ make_handle(ast, condition), if_body, else_body, token }

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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_WHILE_STATEMENT(condition, body, token) WhileStatement{ make_handle(ast, condition), body, token }

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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_DO_STATEMENT(condition, body, token) DoStatement{ make_handle(ast, condition), body, token }

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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_FOR_STATEMENT(initialization, condition, induction, body, token) ForStatement{ initialization, make_handle(ast, condition), make_handle(ast, induction), body, token }

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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_SWITCH_STATEMENT(condition, cases, fallback, token) SwitchStatement{ make_handle(ast, condition), cases, fallback, token }

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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
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
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_DECLARATION_STATEMENT(declaration, token) DeclarationStatement{ declaration, token }

	class InlineAssemblyStatement
		: public StatementBase
		, public InjectService<AssemblerParser>
	{
	private:
		std::vector<CommandHandle> commands;

	public:
		InlineAssemblyStatement(const std::vector<CommandHandle>& commands, const Token& token)
			: StatementBase{ token }, commands{ commands }
		{
		}

	public:
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	};
#define MAKE_INLINE_ASSEMBLY_STATEMENT(commands, token) InlineAssemblyStatement{ commands, token }


	//////////////////////////////////////////////////////
	// All Statements
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept TypeConcept = SumTuple<SumMemberT, SumStorageT, TypeMethods<SumStorageT>>;

	using StatementKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		STATEMENT_KINDS(X)
#undef X
		void
	>;

	using StatementSumImplementation = MakeSum<StatementASTMethods, StatementKinds>::Type;

	struct StatementSumStorage : public StatementSumImplementation::Storage
	{
		using StatementSumImplementation::Storage::Storage;

		using Type = StatementSumImplementation::Type;
		using Anchor = StatementSumImplementation::Anchor;
	};


	template<typename Self>
	StatementKind StatementBase::statement_kind(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, type, name) case TypeIndexV<type, typename StatementSumStorage::Type>: return StatementKind::enumerator;
			STATEMENT_KINDS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid statement tag `{}`", self.tag_type()), self.token);

		// error recovery does not care about statement kind
		return StatementKind::NULL;
	}
}

#endif
