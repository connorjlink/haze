#ifndef HAZE_HOOKSTATEMENT_H
#define HAZE_HOOKSTATEMENT_H

#include "Statement.h"

namespace hz
{
	class HookStatement : public Statement
	{
	private:
		// if true, hook; if false, unhook
		bool hook;

	public:
		HookStatement(bool hook)
			: hook{ hook }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual std::string string() const final override;
		virtual HookStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
