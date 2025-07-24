import std;

#include <command/LabelCommand.h>

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
		USE_SAFE(ErrorReporter)->post_error("unsupported compiler command type `label`", _token);
	}

	LabelCommand* LabelCommand::optimize()
	{
		// No optimizations possible for a label command
		return nullptr;
	}

	Node* LabelCommand::evaluate(Context*) const
	{
		USE_SAFE(ErrorReporter)->post_error("unsupported interpreter command type `label`", _token);
		return nullptr;
	}
}