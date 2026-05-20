#ifndef HAZE_INLINEASSEMBLYSTATEMENT_H
#define HAZE_INLINEASSEMBLYSTATEMENT_H

#include <data/DependencyInjector.h>
#include <toolchain/AssemblerParser.h>

// Haze InlineAssemblyStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class InlineAssemblyStatement 
		: public StatementBase
		, public InjectService<AssemblerParser>
	{
	private:
		std::vector<Command*> commands;

	public:
		InlineAssemblyStatement(const std::vector<Command*>& commands, const Token& token)
			: StatementBase{ token }, commands{ commands }
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
