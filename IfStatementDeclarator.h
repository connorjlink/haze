#ifndef HAZE_IFSTATEMENTDECLARATOR_H
#define HAZE_IFSTATEMENTDECLARATOR_H

#include "StatementDeclarator.h"
#include "IfStatement.h"

namespace hz
{
	class IfStatementDeclarator : public StatementDeclarator
	{
	public:
		IfStatementDeclarator(Statement* statement)
			: StatementDeclarator{ statement }
		{
		}

	public:
		virtual StatementDeclarator::Type stype() const final override;
		virtual std::string string() const final override;
		virtual IfStatementDeclarator* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual StatementDeclarator* optimize() final override;

	public:
		using StatementDeclarator::StatementDeclarator;
	};
}

#endif
