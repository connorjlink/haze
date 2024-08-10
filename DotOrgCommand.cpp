#include "DotOrgCommand.h"
#include "Log.h"

#include <format>

namespace hz
{
	CommandType DotOrgCommand::ctype() const
	{
		return CommandType::DOTORG;
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

	Node* DotOrgCommand::evaluate(Context* context) const
	{
		Log::error(".org assembly directives cannot be evaluated in an interpreted context");
	}
}