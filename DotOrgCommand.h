#ifndef HAZE_DOTORGCOMMAND_H
#define HAZE_DOTORGCOMMAND_H

#include "Command.h"

namespace hz
{
	class DotOrgCommand : public Command
	{
	public:
		std::uint32_t address;

	public:
		DotOrgCommand(std::uint32_t address)
			: address{ address }
		{
		}

	public:
		virtual CommandType ctype() const final override;
		virtual std::string string() const final override;
		virtual DotOrgCommand* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual DotOrgCommand* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
