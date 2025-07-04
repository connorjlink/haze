import std;

#include <command/DotDefineCommand.h>
#include <command/models/IntegerLiteral.h>
#include <error/CommonErrors.h>
#include <runtime/Context.h>

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
		CommonErrors::invalid_command_type(ctype(), _token);
	}

	Command* DotDefineCommand::optimize()
	{
		// No optimizations possible for a .define command
		return nullptr;
	}

	Node* DotDefineCommand::evaluate(Context* context) const
	{
		context->define_variable(identifier, integer_literal_to_variable(value));
		return nullptr;
	}
}
