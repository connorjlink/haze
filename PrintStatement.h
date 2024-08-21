#ifndef HAZE_PRINTSTATEMENT_H
#define HAZE_PRINTSTATEMENT_H

#include "Statement.h"
#include "Context.h"

#include <string>

namespace hz
{
	class PrintStatement : public Statement
	{
	private:
		std::string message;

	public:
		PrintStatement(std::string message)
			: message{ std::move(message) }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual std::string string() const final override;
		virtual PrintStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
