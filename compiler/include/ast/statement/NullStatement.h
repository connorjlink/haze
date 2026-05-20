#ifndef HAZE_NULLSTATEMENT_H
#define HAZE_NULLSTATEMENT_H

// Haze NullStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class NullStatement : public StatementBase
	{
	public:
		NullStatement(const Token& token)
			: StatementBase{ token }
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
