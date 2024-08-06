#ifndef HAZE_NULLSTATEMENT_H
#define HAZE_NULLSTATEMENT_H

#include "Statement.h"

namespace hz
{
	class NullStatement : public Statement
	{
	private:
		// No class members

	public:
		NullStatement()
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual std::string string() const final override;
		virtual NullStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
