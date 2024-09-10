#ifndef HAZE_HAZEEMITTER_H
#define HAZE_HAZEEMITTER_H

#include "Emitter.h"

// Haze HazeEmitter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class HazeEmitter : public Emitter
	{
	public:
		using Emitter::Emitter;

	private:
		virtual byterange emit_move(Register, Register) final override;
		virtual byterange emit_load(Register, std::uint32_t) final override;
		virtual byterange emit_copy(Register, std::uint8_t) final override;
		virtual byterange emit_save(std::uint32_t, Register) final override;
		virtual byterange emit_iadd(Register, Register) final override;
		virtual byterange emit_isub(Register, Register) final override;
		virtual byterange emit_band(Register, Register) final override;
		virtual byterange emit_bior(Register, Register) final override;
		virtual byterange emit_bxor(Register, Register) final override;
		virtual byterange emit_call(std::uint32_t) final override;
		virtual byterange emit_exit() final override;
		virtual byterange emit_push(Register) final override;
		virtual byterange emit_pull(Register) final override;
		virtual byterange emit_brnz(std::uint32_t, Register) final override;
		virtual byterange emit_make(Register) final override;
		virtual byterange emit_take(Register) final override;
		virtual byterange emit_bool(Register) final override;
		virtual byterange emit_stop() final override;

	public:
		virtual EmitterType etype() const final override;

	public:
		byterange emit() final override;
	};
}

#endif
