import std;

#include "DotDefineCommand.h"
#include "ErrorReporter.h"

// Haze DotDefineCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	CommandType DotDefineCommand::ctype() const
	{
		return CommandType::DOTDEFINE;
	}

	DotDefineCommand* DotDefineCommand::copy() const
	{
		return new DotDefineCommand{ *this };
	}

	void DotDefineCommand::generate(Allocation*)
	{
		_error_reporter->post_error("unsupported compiler command type `.define`", _token);
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
