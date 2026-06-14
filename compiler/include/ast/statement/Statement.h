#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

#include <allocator/Value.h>
#include <ast/AST.h>
#include <ast/declaration/Declaration.h>
#include <ast/statement/defs/StatementKind.h>
#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/CompilerParser.h>
#include <toolchain/Generator.h>
#include <toolchain/RISCVAssemblerParser.h>
#include <toolchain/models/Token.h>
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


	struct StatementBase 
		: public StatementFacade
		, public InjectSingleton<CommandLineOptions, ErrorReporter>
		, public InjectService<Generator>
	{
	public:
		using Storage = StatementSumStorage;
		using VariableStorage = VariableSumStorage;

	public:
		Token token;

	public:
		template<typename Self>
		StatementKind statement_kind(this Self&& self)
		{
			return self.statement_kind();
		}

	public:
		StatementBase(const Token& token)
			: token{ token }
		{
		}
	};
}

namespace hz
{
	//////////////////////////////////////////////////////
	// C Statement Categories
	//////////////////////////////////////////////////////

	struct NullStatement : public StatementBase
	{
	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		NullStatement(const Token& token)
			: StatementBase{ token }
		{
		}
	};
#define MAKE_NULL_STATEMENT(token) NullStatement{ token }

	struct ExpressionStatement : public StatementBase
	{
	private:
		ExpressionHandle expression;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		ExpressionStatement(ExpressionHandle expression, const Token& token)
			: StatementBase{ token }, expression{ expression }
		{
		}
	};
#define MAKE_EXPRESSION_STATEMENT(expression, token) ExpressionStatement{ make_handle(ast, expression), token }

	struct ReturnStatement 
		: public StatementBase
		, public InjectService<CompilerParser>
	{
	private:
		std::string enclosing_function;
		ExpressionHandle expression;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		ReturnStatement(const std::string& enclosing_function, ExpressionHandle expression, const Token& token)
			: StatementBase{ token }, enclosing_function{ enclosing_function }, expression{ expression }
		{
		}
	};
#define MAKE_RETURN_STATEMENT(enclosing_function, expression, value, token) ReturnStatement{ enclosing_function, make_handle(ast, expression), value, token }

	struct IfStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle if_body;
		StatementHandle else_body;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		IfStatement(ExpressionHandle condition, StatementHandle if_body, StatementHandle else_body, const Token& token)
			: StatementBase{ token }, condition{ condition }, if_body{ if_body }, else_body{ else_body }
		{
		}
	};
#define MAKE_IF_STATEMENT(condition, if_body, else_body, token) IfStatement{ make_handle(ast, condition), if_body, else_body, token }

	struct WhileStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle body;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		WhileStatement(ExpressionHandle condition, StatementHandle body, const Token& token)
			: StatementBase{ token }, condition{ condition }, body{ body }
		{
		}
	};
#define MAKE_WHILE_STATEMENT(condition, body, token) WhileStatement{ make_handle(ast, condition), body, token }

	struct DoStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle body;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		DoStatement(ExpressionHandle condition, StatementHandle body, const Token& token)
			: StatementBase{ token }, condition{ condition }, body{ body }
		{
		}
	};
#define MAKE_DO_STATEMENT(condition, body, token) DoStatement{ make_handle(ast, condition), body, token }

	struct ForStatement : public StatementBase
	{
	private:
		StatementHandle initialization;
		ExpressionHandle condition;
		ExpressionHandle increment;
		StatementHandle body;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		ForStatement(StatementHandle initialization, ExpressionHandle condition, ExpressionHandle increment, StatementHandle body, const Token& token)
			: StatementBase{ token }, initialization{ initialization }, condition{ condition }, increment{ increment }, body{ body }
		{
		}
	};
#define MAKE_FOR_STATEMENT(initialization, condition, increment, body, token) ForStatement{ initialization, make_handle(ast, condition), make_handle(ast, increment), body, token }

	struct GotoStatement : public StatementBase
	{
	private:
		std::string label;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;
	
	public:
		GotoStatement(const std::string& label, const Token& token)
			: StatementBase{ token }, label{ label }
		{
		}
	};
#define MAKE_GOTO_STATEMENT(label, token) GotoStatement{ label, token }

	struct ContinueStatement : public StatementBase
	{
	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		ContinueStatement(const Token& token)
			: StatementBase{ token }
		{
		}
	};
#define MAKE_CONTINUE_STATEMENT(token) ContinueStatement{ token }

	struct BreakStatement : public StatementBase
	{
	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		BreakStatement(const Token& token)
			: StatementBase{ token }
		{
		}
	};
#define MAKE_BREAK_STATEMENT(token) BreakStatement{ token }

	struct SwitchStatement : public StatementBase
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
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		SwitchStatement(ExpressionHandle condition, const std::vector<Case>& cases, StatementHandle fallback, const Token& token)
			: StatementBase{ token }, condition{ condition }, cases{ cases }, fallback{ fallback }
		{
		}
	};
#define MAKE_SWITCH_STATEMENT(condition, cases, fallback, token) SwitchStatement{ make_handle(ast, condition), cases, fallback, token }

	struct CompoundStatement : public StatementBase
	{
	public:
		std::vector<StatementHandle> substatements;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		CompoundStatement(const std::vector<StatementHandle>& substatements, const Token& token)
			: StatementBase{ token }, substatements{ substatements }
		{
		}
	};
#define MAKE_COMPOUND_STATEMENT(substatements, token) CompoundStatement{ substatements, token }

	struct LabeledStatement : public StatementBase
	{
	private:
		std::string label;
		StatementHandle statement;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		LabeledStatement(const std::string& label, StatementHandle statement, const Token& token)
			: StatementBase{ token }, label{ label }, statement{ statement }
		{
		}
	};
#define MAKE_LABELED_STATEMENT(label, statement, token) LabeledStatement{ label, statement, token }

	struct DeclarationStatement : public StatementBase
	{
	private:
		DeclarationHandle declaration;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		DeclarationStatement(DeclarationHandle declaration, const Token& token)
			: StatementBase{ token }, declaration{ declaration }
		{
		}
	};
#define MAKE_DECLARATION_STATEMENT(declaration, token) DeclarationStatement{ declaration, token }

	struct InlineAssemblyStatement
		: public StatementBase
		, public InjectService<AssemblerParser>
	{
	private:
		std::vector<CommandHandle> commands;

	public:
		std::string format(std::uint32_t) const;
		void generate(const Storage&) const;
		ASTTask evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		TypeHandle get_type(const TypeSumStorage&) const;

	public:
		InlineAssemblyStatement(const std::vector<CommandHandle>& commands, const Token& token)
			: StatementBase{ token }, commands{ commands }
		{
		}
	};
#define MAKE_INLINE_ASSEMBLY_STATEMENT(commands, token) InlineAssemblyStatement{ commands, token }


	//////////////////////////////////////////////////////
	// All Statements
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept IsStatement = SumTuple<SumMemberT, SumStorageT, TypeMethods<SumStorageT>>;

	using StatementKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		STATEMENT_KINDS(X)
#undef X
		void
	>;

	using StatementSumImplementation = MakeSum<StatementASTMethods, StatementKinds>::Type;

	struct StatementStorage : public StatementSumImplementation::Storage
	{
		using StatementSumImplementation::Storage::Storage;

		using Type = StatementSumImplementation::Type;
		using Anchor = StatementSumImplementation::Anchor;
	};
}

#endif
