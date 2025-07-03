#ifndef HAZE_x86EMITTER_H
#define HAZE_x86EMITTER_H

#include "Emitter.h"

// Haze X86Emitter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Emitter : public Emitter
	{
	public:
		using Emitter::Emitter;

	private:
		byterange build_stop(std::uint8_t);

	public:
		static byterange emit_init();

	public:
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
		virtual byterange emit_make(register_t) final override;
		virtual byterange emit_take(register_t) final override;
		virtual byterange emit_brnz(std::uint32_t, register_t) final override;
		virtual byterange emit_bool(register_t) final override;
		virtual byterange emit_stop() final override;

	public:
		virtual EmitterType etype() const final override;

	public:
		byterange emit() final override;
	};
}

#endif
