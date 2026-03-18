import std;

#include <command/InstructionCommand.h>

// Haze InstructionCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	InstructionCommand::InstructionCommand(Token token, Opcode opcode, register_t dst, register_t src, native_uint imm, native_uint mem, native_int rel, const std::string& branch_target)
		: Command{ token }, opcode{ opcode }, destination{ dst }, source{ src }, marked_for_deletion{ false }, branch_target{ std::move(branch_target) }
	{
		this->immediate = static_cast<decltype(immediate)>(imm);
		this->absolute = static_cast<decltype(absolute)>(mem);
		this->relative = static_cast<decltype(relative)>(mem);

		embedded_object_code = {};
		approximate_embedded_size = 0;
	}

	CommandType InstructionCommand::ctype() const
	{
		return CommandType::INSTRUCTION;
	}

	InstructionCommand* InstructionCommand::copy() const
	{
		return new InstructionCommand{ *this };
	}

	void InstructionCommand::generate([[maybe_unused]] Allocation* allocation)
	{
#pragma message("TODO: FIX THIS METHOD to generate for allocations")
		/*switch (opcode)
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
		}*/
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