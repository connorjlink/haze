import std;

#include "LabelCommand.h"
#include "ErrorReporter.h"

// Haze LabelCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	CommandType LabelCommand::ctype() const
	{
		return CommandType::LABEL;
	}

	LabelCommand* LabelCommand::copy() const
	{
		return new LabelCommand{ *this };
	}

	void LabelCommand::generate(Allocation*)
	{
		_error_reporter->post_error("unsupported compiler command type `label`", _token);
	}

	LabelCommand* LabelCommand::optimize()
	{
		// No optimizations possible for a label command
		return nullptr;
	}

	Node* LabelCommand::evaluate(Context*) const
	{
		_error_reporter->post_error("unsupported interpreter command type `label`", _token);
		return nullptr;
	}
}