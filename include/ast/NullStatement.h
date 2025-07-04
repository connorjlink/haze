#ifndef HAZE_NULLSTATEMENT_H
#define HAZE_NULLSTATEMENT_H

#include <ast/Statement.h>

// Haze NullStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class NullStatement : public Statement
	{
	public:
		NullStatement(Token token)
			: Statement{ token }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual NullStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
