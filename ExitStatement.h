#ifndef HAZE_EXITSTATEMENT_H
#define HAZE_EXITSTATEMENT_H

#include "Statement.h"

// Haze ExitStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Expression;

	class ExitStatement : public Statement
	{
	private:
		Expression* _code;

	public:
		ExitStatement(Expression* code, Token token)
			: Statement{ token }, _code{ code }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual ExitStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
