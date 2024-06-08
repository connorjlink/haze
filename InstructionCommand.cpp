#include "InstructionCommand.h"
#include "Generator.h"

namespace hz
{
	Opcode InstructionCommand::itype() const
	{
		return instruction->opcode;
	}

	std::string InstructionCommand::string() const
	{
		return instruction->string();
	}

	void InstructionCommand::generate(Allocation*)
	{
		generator->raw(instruction);
	}

	InstructionCommand* InstructionCommand::optimize()
	{
		return nullptr;
	}
}