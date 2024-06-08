#ifndef HAZE_INLINEASMSTATEMENT_H
#define HAZE_INLINEASMSTATEMENT_H

#include "Statement.h"
#include "Command.h"

#include <vector>

namespace hz
{
	class InlineAsmStatement : public Statement
	{
	private:
		std::vector<Node*> commands;

	public:
		InlineAsmStatement(std::vector<Node*>&& commands)
			: commands(std::move(commands))
		{
		}

	public:
		virtual Statement::Type stype() const final override;
		virtual std::string string() const final override;
		virtual InlineAsmStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
	};
}

#endif
