#ifndef HAZE_INLINEASMSTATEMENT_H
#define HAZE_INLINEASMSTATEMENT_H

#include "Statement.h"
#include "Command.h"

#include <vector>

namespace hz
{
	class AssemblerParser;

	class InlineAsmStatement : public Statement
	{
	private:
		std::vector<Node*> commands;
		AssemblerParser* assembler_parser;

	public:
		InlineAsmStatement(std::vector<Node*>&& commands, AssemblerParser* assembler_parser)
			: commands{ std::move(commands) }, assembler_parser{ assembler_parser }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual std::string string() const final override;
		virtual InlineAsmStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
	};
}

#endif
