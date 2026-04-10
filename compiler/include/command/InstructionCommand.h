#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <command/Command.h>
#include <utility/Constants.h>

// Haze InstructionCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Allocation;
	class Context;

	class InstructionCommand : public Command
	{
	public:
		bool marked_for_deletion;
		std::string branch_target;
		ByteRange object_code;

	public:
		InstructionCommand() = delete;
		InstructionCommand(Token, ByteRange&&, const std::string& = "");

	public:
		inline std::size_t length() const
		{
			return object_code.size();
		}


	public:
		virtual CommandType ctype() const final override;
		virtual void generate(Allocation*) final override;
		virtual InstructionCommand* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
