#include "DotOrgCommand.h"

#include <format>

namespace hz
{
	Command::Type DotOrgCommand::ctype() const
	{
		return Command::Type::DOTORG;
	}

	std::string DotOrgCommand::string() const
	{
		return std::format(".org command {:04X}", address);
	}

	DotOrgCommand* DotOrgCommand::copy() const
	{
		return new DotOrgCommand{ *this };
	}

	void DotOrgCommand::generate(Allocation*)
	{
		//TODO: nothing to do here??
	}

	DotOrgCommand* DotOrgCommand::optimize()
	{
		return nullptr;
	}
}