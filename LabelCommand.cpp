#include "LabelCommand.h"

#include "Log.h"

#include <format>

namespace hz
{
	CommandType LabelCommand::ctype() const
	{
		return CommandType::LABEL;
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

	Node* LabelCommand::evaluate(Context* context) const
	{
		Log::error("Label assembly commands cannot be evaluated in an interpreted context");
	}
}