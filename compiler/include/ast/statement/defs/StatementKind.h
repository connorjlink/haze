#ifndef HAZE_STATEMENTKIND_H
#define HAZE_STATEMENTKIND_H

#include <utility/AutoEnum.h>

// Haze StatementKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define STATEMENT_KINDS(X) \
	X(NULL,        NullStatement,           null) \
	X(EXPRESSION,  ExpressionStatement,     expression) \
	X(RETURN,      ReturnStatement,         return) \
	X(IF,          IfStatement,             if) \
	X(WHILE,       WhileStatement,          while) \
	X(DO,          DoStatement,             do) \
	X(FOR,         ForStatement,            for) \
	X(GOTO,        GotoStatement,           goto) \
	X(CONTINUE,    ContinueStatement,       continue) \
	X(BREAK,       BreakStatement,          break) \
	X(SWITCH,      SwitchStatement,         switch) \
	X(COMPOUND,    CompoundStatement,       compound) \
	X(LABELED,     LabeledStatement,        labeled) \
	X(DECLARATION, DeclarationStatement,    declaration) \
	X(ASM,         InlineAssemblyStatement, asm)

#undef NULL

#define AUTOENUM_ROUTER(X, enumerator, type, name) X(enumerator, type, name, StatementKind)

	DEFINE_ENUM(STATEMENT_KINDS, StatementKind, statement kind)

#undef AUTOENUM_ROUTER
}

#endif