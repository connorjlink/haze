#ifndef HAZE_HAZEEMITTER_H
#define HAZE_HAZEEMITTER_H

#include "Emitter.h"

namespace hz
{
	class HazeEmitter : public Emitter
	{
	public:
		using Emitter::Emitter;

	private:
		virtual std::vector<std::uint8_t> emit_move(Register, Register) final override;
		virtual std::vector<std::uint8_t> emit_load(Register, std::uint32_t) final override;
		virtual std::vector<std::uint8_t> emit_copy(Register, std::uint8_t) final override;
		virtual std::vector<std::uint8_t> emit_save(std::uint32_t, Register) final override;
		virtual std::vector<std::uint8_t> emit_iadd(Register, Register) final override;
		virtual std::vector<std::uint8_t> emit_isub(Register, Register) final override;
		virtual std::vector<std::uint8_t> emit_band(Register, Register) final override;
		virtual std::vector<std::uint8_t> emit_bior(Register, Register) final override;
		virtual std::vector<std::uint8_t> emit_bxor(Register, Register) final override;
		virtual std::vector<std::uint8_t> emit_call(std::uint32_t) final override;
		virtual std::vector<std::uint8_t> emit_exit() final override;
		virtual std::vector<std::uint8_t> emit_push(Register) final override;
		virtual std::vector<std::uint8_t> emit_pull(Register) final override;
		virtual std::vector<std::uint8_t> emit_brnz(std::uint32_t, Register) final override;
		virtual std::vector<std::uint8_t> emit_bool(Register) final override;
		virtual std::vector<std::uint8_t> emit_stop() final override;

	public:
		virtual EmitterType etype() const final override;

	public:
		std::vector<std::uint8_t> emit() final override;
	};
}

#endif
