import std;

#include <command/InstructionCommand.h>

// Haze InstructionCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	InstructionCommand::InstructionCommand(Token token, byterange&& object_code, const std::string& branch_target)
		: Command{ token }, object_code{ std::move(object_code) }, marked_for_deletion{false}, branch_target{std::move(branch_target)}
	{
	}

	CommandType InstructionCommand::ctype() const
	{
		return CommandType::INSTRUCTION;
	}

	void InstructionCommand::generate(Allocation*)
	{
		USE_SAFE(ErrorReporter)->post_error("unsupported generation command type `instruction`", _token);
	}

	InstructionCommand* InstructionCommand::optimize()
	{
		return nullptr;
	}

	Node* InstructionCommand::evaluate(Context* context) const
	{
		USE_SAFE(ErrorReporter)->post_error("unsupported interpreter command type `instruction`", _token);
		return nullptr;
	}
}