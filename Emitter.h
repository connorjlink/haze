#ifndef HAZE_EMITTER_H
#define HAZE_EMITTER_H

#include "InstructionCommand.h"

#include <cstdint>
#include <vector>

namespace hz
{
	class Emitter
	{
	public:
		enum class Type
		{
			HAZE,  // NOTE: current implementation
			LIVE,  // NOTE: interpreter for geo
			X86,   // NOTE: near future
			ARM64, // NOTE: far future
		};

	private:
		std::vector<InstructionCommand*> image;

	public:
		virtual void emit_move(Register, Register) = 0;
		virtual void emit_load(Register, std::uint16_t) = 0;
		virtual void emit_copy(Register, std::uint8_t) = 0;
		virtual void emit_save(std::uint16_t, Register) = 0;
		virtual void emit_iadd(Register, Register) = 0;
		virtual void emit_isub(Register, Register) = 0;
		virtual void emit_band(Register, Register) = 0;
		virtual void emit_bior(Register, Register) = 0;
		virtual void emit_bxor(Register, Register) = 0;
		virtual void emit_call(std::uint16_t) = 0;
		virtual void emit_exit() = 0;
		virtual void emit_push(Register) = 0;
		virtual void emit_pull(Register) = 0;
		virtual void emit_brnz(std::uint16_t, Register) = 0;
		virtual void emit_bool(Register) = 0;
	};
}

#endif 

