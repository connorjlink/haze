#ifndef HAZE_DOTDEFINECOMMAND_H
#define HAZE_DOTDEFINECOMMAND_H

#include <command/Command.h>

// Haze DotDefineCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Context;
	class IntegerLiteral;

	class DotDefineCommand : public Command
	{
	private:
		std::string identifier;
		IntegerLiteral* value;

	public:
		DotDefineCommand(const std::string& identifier, IntegerLiteral* value, Token token)
			: Command{ token }, identifier{ identifier }, value{ value }
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
