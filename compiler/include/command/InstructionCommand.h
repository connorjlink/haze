#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <command/Command.h>
#include <toolchain/models/InstructionEncoding.h>
#include <utility/PlatformVariables.h>

// Haze InstructionCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Allocation;
	class Context;

	class InstructionCommand : public Command
	{
	public:
		Opcode opcode;
		register_t destination, source;
		std::uint8_t immediate;
		std::uint32_t absolute;
		std::int32_t relative;
		bool marked_for_deletion;
		std::string branch_target;

	public:
		std::vector<InstructionCommand*> embedded_object_code;
		std::uint32_t approximate_embedded_size;

	public:
		InstructionCommand() = delete;
		InstructionCommand(Token, Opcode, register_t, register_t, native_uint = 0, native_uint = 0, native_int = 0, const std::string& = "");

	public:
		virtual CommandType ctype() const final override;
		virtual InstructionCommand* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual InstructionCommand* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
