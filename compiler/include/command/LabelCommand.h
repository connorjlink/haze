#ifndef HAZE_LABELCOMMAND_H
#define HAZE_LABELCOMMAND_H

#include "Command.h"

// Haze LabelCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class LabelCommand : public Command
	{
	public:
		std::string identifier;
		std::uint32_t address;

	public:
		LabelCommand(const std::string& identifier, Token token)
			: Command{ token }, identifier{ identifier }
		{
		}

	public:
		virtual CommandType ctype() const final override;
		virtual LabelCommand* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual LabelCommand* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
