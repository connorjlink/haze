#ifndef HAZE_INLINEASMSTATEMENT_H
#define HAZE_INLINEASMSTATEMENT_H

#include <ast/Statement.h>

// Haze InlineAsmStatement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AssemblerParser;

	class InlineAsmStatement : public Statement
	{
	private:
		const std::string& enclosing_file;
		std::vector<Command*> commands;
		AssemblerParser* assembler_parser;

	public:
		InlineAsmStatement(const std::vector<Command*>& commands, AssemblerParser* assembler_parser, const std::string& enclosing_file, Token token)
			: Statement{ token }, commands{ commands }, assembler_parser{ assembler_parser }, enclosing_file{ enclosing_file }
		{
		}

	public:
		virtual StatementType stype() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
