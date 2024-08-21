#ifndef HAZE_STATEMENTTYPE_H
#define HAZE_STATEMENTTYPE_H

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
	};
}

#endif