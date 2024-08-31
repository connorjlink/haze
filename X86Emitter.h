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
		std::vector<std::uint8_t> build_stop(std::uint8_t);

	public:
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
