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
		const std::string& _enclosing_file;
		std::vector<Node*> _commands;
		AssemblerParser* _assembler_parser;

	public:
		InlineAsmStatement(std::vector<Node*>&& commands, AssemblerParser* assembler_parser, const std::string& enclosing_file, Token token)
			: Statement{ token }, _commands{ std::move(commands) }, _assembler_parser{ assembler_parser }, _enclosing_file{ enclosing_file }
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
