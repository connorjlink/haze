#ifndef HAZE_ASMSTATEMENT_H
#define HAZE_ASMSTATEMENT_H

#include "Statement.h"
#include "Instruction.h"

#include <vector>

namespace hz
{
	class AsmStatement : public Statement
	{
	private:
		std::vector<IndexedInstruction> iis;

	public:
		AsmStatement(std::vector<IndexedInstruction>&& iis)
			: iis(std::move(iis))
		{
		}

	public:
		virtual Statement::Type stype() const final override;
		virtual std::string string() const final override;
		virtual AsmStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
	};
}

#endif
