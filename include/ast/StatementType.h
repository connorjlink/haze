#ifndef HAZE_STATEMENTTYPE_H
#define HAZE_STATEMENTTYPE_H

// Haze StatementType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_STATEMENT(x) static_cast<Statement*>(x)

#define AS_COMPOUND_STATEMENT(x) static_cast<CompoundStatement*>(x)
#define AS_VARIABLE_STATEMENT(x) static_cast<VariableStatement*>(x)
#define AS_RETURN_STATEMENT(x) static_cast<ReturnStatement*>(x)
#define AS_INLINEASM_STATEMENT(x) static_cast<InlineAsmStatement*>(x)
#define AS_WHILE_STATEMENT(x) static_cast<WhileStatement*>(x)
#define AS_FOR_STATEMENT(x) static_cast<ForStatement*>(x)
#define AS_IF_STATEMENT(x) static_cast<IfStatement*>(x)
#define AS_EXPRESSION_STATEMENT(x) static_cast<ExpressionStatement*>(x)

namespace hz
{
	enum class StatementType
	{
		COMPOUND,
		VARIABLE,
		RETURN,
		INLINEASM,
		WHILE,
		FOR,
		IF,
		EXPRESSION,
		PRINT,
		HOOK,
		NONE, // TODO: this is the NullStatement, let's fix that later
		EXIT,
		STRUCT,
	};

	static const std::unordered_map<StatementType, std::string> _statement_type_map
	{
		{ StatementType::COMPOUND, "compound" },
		{ StatementType::VARIABLE, "variable" },
		{ StatementType::RETURN, "return" },
		{ StatementType::INLINEASM, "asm" },
		{ StatementType::WHILE, "while" },
		{ StatementType::FOR, "for" },
		{ StatementType::IF, "if" },
		{ StatementType::EXPRESSION, "expression" },
		{ StatementType::PRINT, "print" },
		{ StatementType::HOOK, "hook" },
		{ StatementType::NONE, "null" },
		{ StatementType::EXIT, "exit" },
		{ StatementType::STRUCT, "struct" },
	};
}

#endif