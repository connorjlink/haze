#ifndef HAZE_STATEMENT_H
#define HAZE_STATEMENT_H

#include "Node.h"

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
	class Statement : public Node
	{
	public:
		enum class Type
		{
			COMPOUND,
			VARIABLE,
			RETURN,
			INLINEASM,
			WHILE,
			FOR,
			IF,
			EXPRESSION,
		};

	public:
		virtual Node::Type ntype() const final override;
		virtual Statement::Type stype() const = 0;
	};
}

#endif
