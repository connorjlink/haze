#include "Label.h"

#include "Log.h"

#include <format>

namespace hz
{
	Command::Type Label::ctype() const
	{
		return Command::Type::LABEL;
	}

	std::string Label::string() const
	{
		return std::format("label ({})", identifier);
	}

	Label* Label::copy() const
	{
		return new Label{ *this };
	}

	void Label::generate(Allocation*)
	{
		Log::error("invalid generate() for type `Label`");
	}

	Label* Label::optimize()
	{
		return nullptr;
	}
}