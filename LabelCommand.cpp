#include "LabelCommand.h"

#include "Log.h"

#include <format>

namespace hz
{
	Command::Type LabelCommand::ctype() const
	{
		return Command::Type::LABEL;
	}

	std::string LabelCommand::string() const
	{
		return std::format("LabelCommand ({})", identifier);
	}

	LabelCommand* LabelCommand::copy() const
	{
		return new LabelCommand{ *this };
	}

	void LabelCommand::generate(Allocation*)
	{
		Log::error("invalid generate() for type `LabelCommand`");
	}

	LabelCommand* LabelCommand::optimize()
	{
		return nullptr;
	}
}