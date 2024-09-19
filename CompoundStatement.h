#ifndef HAZE_COMPOUNDSTATEMENT_H
#define HAZE_COMPOUNDSTATEMENT_H

#include "Statement.h"

// Haze CompoundStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CompoundStatement : public Statement
	{
	private:
		std::vector<Statement*> substatements;

	public:
		CompoundStatement(std::vector<Statement*>&& substatements, Token token)
			: Statement{ token }, substatements(std::move(substatements))
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual CompoundStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
