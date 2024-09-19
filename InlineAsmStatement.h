#ifndef HAZE_INLINEASMSTATEMENT_H
#define HAZE_INLINEASMSTATEMENT_H

#include "Statement.h"
#include "Command.h"

// Haze InlineAsmStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AssemblerParser;

	class InlineAsmStatement : public Statement
	{
	private:
		std::vector<Node*> commands;
		AssemblerParser* assembler_parser;

	public:
		InlineAsmStatement(std::vector<Node*>&& commands, AssemblerParser* assembler_parser, Token token)
			: Statement{ token }, commands{ std::move(commands) }, assembler_parser{ assembler_parser }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual InlineAsmStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
