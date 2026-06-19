#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

#include <allocator/Value.h>
#include <ast/AST.h>
#include <ast/declaration/Declaration.h>
#include <ast/statement/defs/StatementKind.h>
#include <cli/CommandLineOptions.h>
#include <command/Command.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/models/Symbol.h>
#include <toolchain/CompilerParser.h>
#include <toolchain/Generator.h>
#include <toolchain/models/Token.h>
#include <toolchain/RISCVAssemblerParser.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Statement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct QueueEntry
	{
		Symbol symbol;
		Token token;
	};

	FORWARD_DECLARE_SUM(Statement)

#define STATEMENT_AST_METHODS(X, handlet) \
	BASE_AST_METHODS(X, handlet) \
	X(statement_kind, StatementKind)

	DEFINE_SUM(Statement, STATEMENT_AST_METHODS)


	struct StatementBase 
		: public StatementFacade
		, public InjectSingleton<CommandLineOptions, ErrorReporter>
		, public InjectService<Generator>
	{
	public:
		using Storage = StatementStorage;
		using VariableStorage = VariableStorage;

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
	//////////////////////////////////////////////////////
	// C Statement Categories
	//////////////////////////////////////////////////////

	struct NullStatement : public StatementBase
	{
	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

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
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		ExpressionStatement(const Token& token, ExpressionHandle expression)
			: StatementBase{ token }, expression{ expression }
		{
		}
	};
#define MAKE_EXPRESSION_STATEMENT(token, expression) ExpressionStatement{ token, make_handle(ast, expression) }

	struct ReturnStatement 
		: public StatementBase
		, public InjectService<CompilerParser>
	{
	private:
		std::string_view enclosing_function;
		ExpressionHandle expression;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		ReturnStatement(const Token& token, std::string_view enclosing_function, ExpressionHandle expression)
			: StatementBase{ token }, enclosing_function{ enclosing_function }, expression{ expression }
		{
		}
	};
#define MAKE_RETURN_STATEMENT(token, enclosing_function, expression, value) ReturnStatement{ token, enclosing_function, make_handle(ast, expression), value }

	struct IfStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle if_body;
		StatementHandle else_body;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		IfStatement(const Token& token, ExpressionHandle condition, StatementHandle if_body, StatementHandle else_body)
			: StatementBase{ token }, condition{ condition }, if_body{ if_body }, else_body{ else_body }
		{
		}
	};
#define MAKE_IF_STATEMENT(token, condition, if_body, else_body) IfStatement{ token, make_handle(ast, condition), if_body, else_body }

	struct WhileStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle body;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		WhileStatement(const Token& token, ExpressionHandle condition, StatementHandle body)
			: StatementBase{ token }, condition{ condition }, body{ body }
		{
		}
	};
#define MAKE_WHILE_STATEMENT(token, condition, body) WhileStatement{ token, make_handle(ast, condition), body }

	struct DoStatement : public StatementBase
	{
	private:
		ExpressionHandle condition;
		StatementHandle body;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		DoStatement(const Token& token, ExpressionHandle condition, StatementHandle body)
			: StatementBase{ token }, condition{ condition }, body{ body }
		{
		}
	};
#define MAKE_DO_STATEMENT(token, condition, body) DoStatement{ token, make_handle(ast, condition), body }

	struct ForStatement : public StatementBase
	{
	private:
		StatementHandle initialization;
		ExpressionHandle condition;
		ExpressionHandle increment;
		StatementHandle body;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		ForStatement(const Token& token, StatementHandle initialization, ExpressionHandle condition, ExpressionHandle increment, StatementHandle body)
			: StatementBase{ token }, initialization{ initialization }, condition{ condition }, increment{ increment }, body{ body }
		{
		}
	};
#define MAKE_FOR_STATEMENT(token, initialization, condition, increment, body) ForStatement{ token, initialization, make_handle(ast, condition), make_handle(ast, increment), body }

	struct GotoStatement : public StatementBase
	{
	private:
		std::string_view label;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;
	
	public:
		GotoStatement(const Token& token, std::string_view label)
			: StatementBase{ token }, label{ label }
		{
		}
	};
#define MAKE_GOTO_STATEMENT(label, token) GotoStatement{ label, token }

	struct ContinueStatement : public StatementBase
	{
	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

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
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

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
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		SwitchStatement(const Token& token, ExpressionHandle condition, std::vector<Case> cases, StatementHandle fallback)
			: StatementBase{ token }, condition{ condition }, cases{ std::move(cases) }, fallback{ fallback }
		{
		}
	};
#define MAKE_SWITCH_STATEMENT(token, condition, cases, fallback) SwitchStatement{ token, make_handle(ast, condition), cases, fallback }

	struct CompoundStatement : public StatementBase
	{
	public:
		std::vector<StatementHandle> substatements;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		CompoundStatement(const Token& token, std::vector<StatementHandle> substatements)
			: StatementBase{ token }, substatements{ std::move(substatements) }
		{
		}
	};
#define MAKE_COMPOUND_STATEMENT(token, substatements) CompoundStatement{ token, substatements }

	struct LabeledStatement : public StatementBase
	{
	private:
		std::string_view label;
		StatementHandle statement;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		LabeledStatement(const Token& token, std::string_view label, StatementHandle statement)
			: StatementBase{ token }, label{ label }, statement{ statement }
		{
		}
	};
#define MAKE_LABELED_STATEMENT(token, label, statement) LabeledStatement{ token, label, statement }

	struct DeclarationStatement : public StatementBase
	{
	private:
		DeclarationHandle declaration;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		DeclarationStatement(const Token& token, DeclarationHandle declaration)
			: StatementBase{ token }, declaration{ declaration }
		{
		}
	};
#define MAKE_DECLARATION_STATEMENT(token, declaration) DeclarationStatement{ token, declaration }

	struct InlineAssemblyStatement
		: public StatementBase
		, public InjectService<AssemblerParser>
	{
	private:
		std::vector<CommandHandle> commands;

	public:
		StatementKind statement_kind() const;
		std::string format(std::uint32_t) const;
		void generate(const CommandStorage&) const;
		ASTTask evaluate(const VariableStorage&, Context&) const;
		StatementHandle optimize(const StatementStorage&) const;
		bool is_pure() const;
		bool is_constant() const;

	public:
		InlineAssemblyStatement(const Token& token, std::vector<CommandHandle> commands)
			: StatementBase{ token }, commands{ std::move(commands) }
		{
		}
	};
#define MAKE_INLINE_ASSEMBLY_STATEMENT(token, commands) InlineAssemblyStatement{ token, commands }


	//////////////////////////////////////////////////////
	// All Statements
	//////////////////////////////////////////////////////

	template<typename SumMemberT, typename StorageT>
	concept IsStatement = SumTuple<SumMemberT, StorageT, StatementMethods<typename StorageT::Anchor>>;

	using StatementKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		STATEMENT_KINDS(X)
#undef X
		void
	>;


	using StatementSumImplementation = MakeSum<StatementMethods, StatementKinds>::Type;

	struct StatementStorage : public StatementSumImplementation::Storage
	{
		using StatementSumImplementation::Storage::Storage;

		using Type = StatementSumImplementation::Type;
		using Anchor = StatementSumImplementation::Anchor;
	};
}

#endif
