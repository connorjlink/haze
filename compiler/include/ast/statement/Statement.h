#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

#include <ast/AST.h>
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
	};
}

// NOTE: this secondary include block is mandatory to allow the incomplete type facade
#include <ast/statement/NullStatement.h>
#include <ast/statement/ExpressionStatement.h>
#include <ast/statement/ReturnStatement.h>
#include <ast/statement/IfStatement.h>
#include <ast/statement/WhileStatement.h>
#include <ast/statement/DoStatement.h>
#include <ast/statement/ForStatement.h>
#include <ast/statement/CompoundStatement.h>
#include <ast/statement/FunctionDeclarationStatement.h>
#include <ast/statement/StructDeclarationStatement.h>
#include <ast/statement/VariableDeclarationStatement.h>
#include <ast/statement/InlineAssemblyStatement.h>


namespace hz
{
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
		ForStatement,
		CompoundStatement,
		FunctionDeclarationStatement,
		StructDeclarationStatement,
		VariableDeclarationStatement,
		InlineAssemblyStatement
	>;

	using StatementSum = MakeSum<ASTMethods, StatementTypes>::Type;
	using StatementBase = SumMemberBase<StatementSum>;

	template<typename T>
	using StatementReference = StatementSum::template Reference<T>;

	using StatementHandle = StatementSum::Handle;
}

#endif
