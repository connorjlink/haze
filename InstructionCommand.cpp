#include "InstructionCommand.h"
#include "Disassembler.h"
#include "Generator.h"
#include "Utility.h"

#include <format>

namespace hz
{
	InstructionCommand::InstructionCommand(std::uint32_t bytes)
	{
		mem = bytes & 0xFFFF;
		imm = (bytes & 0xFF00) >> 8;
		src = static_cast<Register>((bytes & (0x03 << 16)) >> 16);
		dst = static_cast<Register>((bytes & (0x0C << 16)) >> 18);
		opcode = static_cast<Opcode>((bytes & (0xF0 << 16)) >> 20);
		marked_for_deletion = false;
	}

	InstructionCommand::InstructionCommand(Opcode opcode, Register op1, Register op2, std::uint8_t imm, std::uint16_t mem, std::string branch_target)
		: opcode{ opcode }, dst{ dst }, src{ src }, imm{ imm }, mem{ mem}, marked_for_deletion{ false }, branch_target{ std::move(branch_target) }
	{
		embedded_object_code = {};
		approximate_embedded_size = 0;
	}


	std::uint32_t InstructionCommand::bytes() const
	{
		std::uint32_t instruction{};

		instruction |= mem;
		instruction |= imm << 8;
		instruction |= (src & 0b11) << 16;
		instruction |= (dst & 0b11) << 18;
		instruction |= opcode << 20;

		return instruction;
	}

	Command::Type InstructionCommand::ctype() const
	{
		return Command::Type::INSTRUCTION;
	}

	std::string InstructionCommand::string() const
	{
		const auto data = bytes();
		const auto bytes = extract(data);

		return std::format("{} ; #{:02X} {:02X} {:02X}", Disassembler::disassemble_instruction(data), bytes[0], bytes[1], bytes[2]);
	}

	InstructionCommand* InstructionCommand::copy() const
	{
		return new InstructionCommand{ *this };
	}

	void InstructionCommand::generate(Allocation*)
	{
		switch (opcode)
		{
			case MOVE: generator->make_move(dst, src); break;
			case LOAD: generator->make_load(dst, mem); break;
			case COPY: generator->make_copy(dst, imm); break;
			case SAVE: generator->make_save(mem, src); break;
			case IADD: generator->make_iadd(dst, src); break;
			case ISUB: generator->make_isub(dst, src); break;
			case BAND: generator->make_band(dst, src); break;
			case BIOR: generator->make_bior(dst, src); break;
			case BXOR: generator->make_bxor(dst, src); break;
			case CALL: generator->make_call(mem);      break;
			case EXIT: generator->make_exit();         break;
			case PUSH: generator->make_push(src);      break;
			case PULL: generator->make_pull(dst);      break;
			case BRNZ: generator->make_brnz(mem, src); break;
			case BOOL: generator->make_bool(src);      break;
		}
	}

	InstructionCommand* InstructionCommand::optimize()
	{
		return nullptr;
	}

}