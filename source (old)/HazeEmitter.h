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
		virtual byterange emit_move(register_t, register_t) final override;
		virtual byterange emit_load(register_t, std::uint32_t) final override;
		virtual byterange emit_copy(register_t, std::uint8_t) final override;
		virtual byterange emit_save(std::uint32_t, register_t) final override;
		virtual byterange emit_iadd(register_t, register_t) final override;
		virtual byterange emit_isub(register_t, register_t) final override;
		virtual byterange emit_band(register_t, register_t) final override;
		virtual byterange emit_bior(register_t, register_t) final override;
		virtual byterange emit_bxor(register_t, register_t) final override;
		virtual byterange emit_call(std::uint32_t) final override;
		virtual byterange emit_exit() final override;
		virtual byterange emit_push(register_t) final override;
		virtual byterange emit_pull(register_t) final override;
		virtual byterange emit_brnz(std::uint32_t, register_t) final override;
		virtual byterange emit_make(register_t) final override;
		virtual byterange emit_take(register_t) final override;
		virtual byterange emit_bool(register_t) final override;
		virtual byterange emit_stop() final override;

	public:
		virtual EmitterType etype() const final override;

	public:
		byterange emit() final override;
	};
}

#endif
