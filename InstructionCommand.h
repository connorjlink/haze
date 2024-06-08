#ifndef HAZE_INSTRUCTIONCOMMAND_H
#define HAZE_INSTRUCTIONCOMMAND_H

#include "Command.h"
#include "Instruction.h"

namespace hz
{
	class InstructionCommand : public Command
	{
	private:
		Instruction* instruction;

	public:
		Opcode itype() const;
		virtual std::string string() const final override;
		virtual InstructionCommand* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual InstructionCommand* optimize() final override;
	};
}

#endif
