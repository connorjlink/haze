#ifndef HAZE_STATEMENTKIND_X
#define HAZE_STATEMENTKIND_X

// Haze StatementKind.x
// (c) Connor J. Link. All Rights Reserved.

#define STATEMENT_KINDS(X) \
	X(NULL, NullStatement, null) \
	X(EXPRESSION, ExpressionStatement, expression) \
	X(RETURN, ReturnStatement, return) \
	X(IF, IfStatement, if) \
	X(WHILE, WhileStatement, while) \
	X(DO, DoStatement, do) \
	X(FOR, ForStatement, for) \
	X(GOTO, GotoStatement, goto) \
	X(CONTINUE, ContinueStatement, continue) \
	X(BREAK, BreakStatement, break) \
	X(SWITCH, SwitchStatement, switch) \
	X(COMPOUND, CompoundStatement, compound) \
	X(LABELED, LabeledStatement, labeled) \
	X(DECLARATION, DeclarationStatement, declaration) \
	X(ASM, InlineAssemblyStatement, asm)

#endif
