#ifndef HAZE_STATEMENTDECLARATOR_H
#define HAZE_STATEMENTDECLARATOR_H

#include "Declarator.h"
#include "Statement.h"

namespace hz
{
	class StatementDeclarator : public Declarator
	{
	public:
		enum class Type
		{
			COMPOUND,
			VARIABLE,
			RETURN,
			// INLINEASM,
			WHILE,
			FOR,
			IF,
			EXPRESSION,
		};

	public:
		Statement* statement;

	public:
		StatementDeclarator(Statement* statement)
			: statement{ statement }
		{
		}

	public:
		virtual Declarator::Type dtype() const final override;
		virtual StatementDeclarator::Type stype() const = 0;
	};
}

#endif
