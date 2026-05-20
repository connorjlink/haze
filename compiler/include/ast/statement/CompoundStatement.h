#ifndef HAZE_COMPOUNDSTATEMENT_H
#define HAZE_COMPOUNDSTATEMENT_H

#include <ast/statement/Statement.h>

// Haze CompoundStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class CompoundStatement : public StatementBase
	{
	public:
		std::vector<StatementHandle> substatements;

	public:
		CompoundStatement(const std::vector<StatementHandle>& substatements, const Token& token)
			: StatementBase{ token }, substatements{ substatements }
		{
		}

	public:
		StatementType tag_type(void) const;
		std::string format(void) const;
		void generate(const Storage&) const;
		StatementHandle evaluate(const Storage&, Context&) const;
		StatementHandle optimize(const Storage&) const;
		StatementHandle get_type(const Storage&) const;
	};
}

#endif
