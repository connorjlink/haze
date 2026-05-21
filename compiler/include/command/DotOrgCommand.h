#ifndef HAZE_DOTORGCOMMAND_H
#define HAZE_DOTORGCOMMAND_H

#include <command/Command.h>

// Haze DotOrgCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class DotOrgCommand : public Command
	{
	public:
		Address address;

	public:
		DotOrgCommand(Address address, const Token& token)
			: Command{ token }, address{ address }
		{
		}

	public:
		virtual CommandType ctype() const final override;
		virtual void generate(Allocation*) final override;
		virtual DotOrgCommand* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
