#include "InstructionCommand.h"
#include "Disassembler.h"
#include "Generator.h"
#include "ErrorReporter.h"

// Haze InstructionCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	InstructionCommand::InstructionCommand(Token token, std::uint32_t bytes)
		: Command{ token }
	{
		mem = bytes & 0xFFFF;
		imm = (bytes & 0xFF00) >> 8;
		src = ((bytes & (0x03 << 16)) >> 16);
		dst = ((bytes & (0x0C << 16)) >> 18);
		opcode = static_cast<Opcode>((bytes & (0xF0 << 16)) >> 20);
		marked_for_deletion = false;
	}

	InstructionCommand::InstructionCommand(Token token, Opcode opcode, register_t dst, register_t src, std::uint8_t imm, std::uint32_t mem, std::string branch_target)
		: Command{ token }, opcode{ opcode }, dst{ dst }, src{ src }, imm{ imm }, mem{ mem }, marked_for_deletion{ false }, branch_target{ std::move(branch_target) }
	{
		embedded_object_code = {};
		approximate_embedded_size = 0;
	}


	/*std::uint32_t InstructionCommand::bytes() const
	{
		std::uint32_t instruction{};

		instruction |= mem;
		instruction |= imm << 8;
		instruction |= (src & 0b11) << 16;
		instruction |= (dst & 0b11) << 18;
		instruction |= opcode << 20;

		return instruction;
	}*/

	CommandType InstructionCommand::ctype() const
	{
		return CommandType::INSTRUCTION;
	}

	InstructionCommand* InstructionCommand::copy() const
	{
		return new InstructionCommand{ *this };
	}

	void InstructionCommand::generate(Allocation*)
	{
		switch (opcode)
		{
			case Opcode::MOVE: _generator->make_move(dst, src); break;
			case Opcode::LOAD: _generator->make_load(dst, mem); break;
			case Opcode::COPY: _generator->make_copy(dst, imm); break;
			case Opcode::SAVE: _generator->make_save(mem, src); break;
			case Opcode::IADD: _generator->make_iadd(dst, src); break;
			case Opcode::ISUB: _generator->make_isub(dst, src); break;
			case Opcode::BAND: _generator->make_band(dst, src); break;
			case Opcode::BIOR: _generator->make_bior(dst, src); break;
			case Opcode::BXOR: _generator->make_bxor(dst, src); break;
			case Opcode::CALL: _generator->make_call(mem);      break;
			case Opcode::EXIT: _generator->make_exit(src);      break;
			case Opcode::PUSH: _generator->make_push(src);      break;
			case Opcode::PULL: _generator->make_pull(dst);      break;
			case Opcode::MAKE: _generator->make_make(src);      break;
			case Opcode::TAKE: _generator->make_take(dst);      break;
			case Opcode::BRNZ: _generator->make_brnz(mem, src); break;
			case Opcode::BOOL: _generator->make_bool(src);      break;
			case Opcode::STOP: _generator->make_stop();         break;
		}
	}

	InstructionCommand* InstructionCommand::optimize()
	{
		return nullptr;
	}

	Node* InstructionCommand::evaluate(Context* context) const
	{
		_error_reporter->post_error("unsupported interpreter command type `instruction`", _token);
		return nullptr;
	}
}