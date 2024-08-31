#include "DotOrgCommand.h"
#include "ErrorReporter.h"

// Haze DotOrgCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	CommandType DotOrgCommand::ctype() const
	{
		return CommandType::DOTORG;
	}

	DotOrgCommand* DotOrgCommand::copy() const
	{
		return new DotOrgCommand{ *this };
	}

	void DotOrgCommand::generate(Allocation*)
	{
		_error_reporter->post_error("unsupported compiler command type `.org`", _token);
	}

	DotOrgCommand* DotOrgCommand::optimize()
	{
		return nullptr;
	}

	Node* DotOrgCommand::evaluate(Context* context) const
	{
		_error_reporter->post_error("unsupported interpreter command type `.org`", _token);
		return nullptr;
	}
}