#ifndef HAZE_DOTDEFINECOMMAND_H
#define HAZE_DOTDEFINECOMMAND_H

#include "Command.h"
#include "Context.h"

namespace hz
{


	class DotDefineCommand : public Command
	{
	private:
		std::string identifier;
		variable_t value;

	public:
		DotDefineCommand(std::string identifier, variable_t value)
			: identifier{ std::move(identifier) }, value{ value }
		{
		}

	public:
		virtual CommandType ctype() const final override;
		virtual std::string string() const final override;
		virtual DotDefineCommand* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Command* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif 
