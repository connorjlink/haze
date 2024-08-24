#ifndef HAZE_EMITTER_H
#define HAZE_EMITTER_H

#include "EmitterType.h"

#include "InstructionCommand.h"

#include <cstdint>
#include <vector>

namespace hz
{
	class Emitter
	{
	protected:
		std::vector<InstructionCommand*> image;

	public:
		Emitter(std::vector<InstructionCommand*>&& image)
			: image{ std::move(image) }
		{
		}

	private:
		virtual std::vector<std::uint8_t> emit_move(Register, Register) = 0;
		virtual std::vector<std::uint8_t> emit_load(Register, std::uint16_t) = 0;
		virtual std::vector<std::uint8_t> emit_copy(Register, std::uint8_t) = 0;
		virtual std::vector<std::uint8_t> emit_save(std::uint16_t, Register) = 0;
		virtual std::vector<std::uint8_t> emit_iadd(Register, Register) = 0;
		virtual std::vector<std::uint8_t> emit_isub(Register, Register) = 0;
		virtual std::vector<std::uint8_t> emit_band(Register, Register) = 0;
		virtual std::vector<std::uint8_t> emit_bior(Register, Register) = 0;
		virtual std::vector<std::uint8_t> emit_bxor(Register, Register) = 0;
		virtual std::vector<std::uint8_t> emit_call(std::uint16_t) = 0;
		virtual std::vector<std::uint8_t> emit_exit() = 0;
		virtual std::vector<std::uint8_t> emit_push(Register) = 0;
		virtual std::vector<std::uint8_t> emit_pull(Register) = 0;
		virtual std::vector<std::uint8_t> emit_brnz(std::uint16_t, Register) = 0;
		virtual std::vector<std::uint8_t> emit_bool(Register) = 0;

	public:
		virtual EmitterType etype() const = 0;

	public:
		virtual std::vector<std::uint8_t> emit() = 0;
	};
}

#endif 

