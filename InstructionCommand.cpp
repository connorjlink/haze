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

	InstructionCommand::InstructionCommand(Opcode opcode, Register dst, Register src, std::uint8_t imm, std::uint16_t mem, std::string branch_target)
		: opcode{ opcode }, dst{ dst }, src{ src }, imm{ imm }, mem{ mem }, marked_for_deletion{ false }, branch_target{ std::move(branch_target) }
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

	CommandType InstructionCommand::ctype() const
	{
		return CommandType::INSTRUCTION;
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
			case MOVE: _generator->make_move(dst, src); break;
			case LOAD: _generator->make_load(dst, mem); break;
			case COPY: _generator->make_copy(dst, imm); break;
			case SAVE: _generator->make_save(mem, src); break;
			case IADD: _generator->make_iadd(dst, src); break;
			case ISUB: _generator->make_isub(dst, src); break;
			case BAND: _generator->make_band(dst, src); break;
			case BIOR: _generator->make_bior(dst, src); break;
			case BXOR: _generator->make_bxor(dst, src); break;
			case CALL: _generator->make_call(mem);      break;
			case EXIT: _generator->make_exit();         break;
			case PUSH: _generator->make_push(src);      break;
			case PULL: _generator->make_pull(dst);      break;
			case BRNZ: _generator->make_brnz(mem, src); break;
			case BOOL: _generator->make_bool(src);      break;
			case STOP: _generator->make_stop();         break;
		}
	}

	InstructionCommand* InstructionCommand::optimize()
	{
		return nullptr;
	}

	Node* InstructionCommand::evaluate(Context* context) const
	{
		Log::error("Instruction assembly commands cannot be evaluated in an interpreted context");
	}
}