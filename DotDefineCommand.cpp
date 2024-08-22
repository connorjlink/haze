#include "DotDefineCommand.h"
#include "Log.h"

#include <format>

namespace hz
{
	CommandType DotDefineCommand::ctype() const
	{
		return CommandType::DOTDEFINE;
	}

	std::string DotDefineCommand::string() const
	{
		return std::format(".define command");
	}

	DotDefineCommand* DotDefineCommand::copy() const
	{
		return new DotDefineCommand{ *this };
	}

	void DotDefineCommand::generate(Allocation*)
	{
		Log::error(".define commands cannot be generated in a compiled context");
	}

	Command* DotDefineCommand::optimize()
	{
		return nullptr;
	}

	Node* DotDefineCommand::evaluate(Context* context) const
	{
		context->define_variable(identifier, value);
		return nullptr;
	}
}
