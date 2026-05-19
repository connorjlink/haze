#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

#include <ast/AST.h>
#include <type/Type.h>

// Haze Statement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class NullStatement;
	class ExpressionStatement;
	class ReturnStatement;
	class IfStatement;
	class WhileStatement;
	class ForStatement;
	class CompoundStatement;
	class FunctionDeclarationStatement;
	class StructDeclarationStatement;
	class VariableDeclarationStatement;
	class InlineAssemblyStatement;

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
