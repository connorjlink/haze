#ifndef HAZE_DOTDEFINECOMMAND_H
#define HAZE_DOTDEFINECOMMAND_H

#include "Command.h"
#include "Context.h"

// Haze DotDefineCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class DotDefineCommand : public Command
	{
	private:
		std::string identifier;
		variable_t value;

	public:
		DotDefineCommand(std::string identifier, variable_t value, Token token)
			: Command{ token }, identifier{ std::move(identifier) }, value{ value }
		{
		}

	public:
		virtual CommandType ctype() const final override;
		virtual DotDefineCommand* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Command* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
