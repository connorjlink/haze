#ifndef HAZE_EMITTER_H
#define HAZE_EMITTER_H

#include "EmitterType.h"
#include "Constants.h"
#include "InstructionEncoding.h"

#include <string>
#include <vector>
#include <cstdint>

// Haze Emitter.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class InstructionCommand;

	class Emitter
	{
	protected:
		std::vector<InstructionCommand*> image;

	public:
		Emitter(std::vector<InstructionCommand*>&& image, const std::string& filepath);
		virtual ~Emitter();

	public:
		virtual byterange emit_move(register_t, register_t) = 0;
		virtual byterange emit_load(register_t, std::uint32_t) = 0;
		virtual byterange emit_copy(register_t, std::uint8_t) = 0;
		virtual byterange emit_save(std::uint32_t, register_t) = 0;
		virtual byterange emit_iadd(register_t, register_t) = 0;
		virtual byterange emit_isub(register_t, register_t) = 0;
		virtual byterange emit_band(register_t, register_t) = 0;
		virtual byterange emit_bior(register_t, register_t) = 0;
		virtual byterange emit_bxor(register_t, register_t) = 0;
		virtual byterange emit_call(std::uint32_t) = 0;
		virtual byterange emit_exit() = 0;
		virtual byterange emit_push(register_t) = 0;
		virtual byterange emit_pull(register_t) = 0;
		virtual byterange emit_make(register_t) = 0;
		virtual byterange emit_take(register_t) = 0;
		virtual byterange emit_brnz(std::uint32_t, register_t) = 0;
		virtual byterange emit_bool(register_t) = 0;
		virtual byterange emit_stop() = 0;

	public:
		virtual EmitterType etype() const = 0;

	public:
		virtual byterange emit() = 0;
	};

	extern Emitter* _emitter;
}

#endif 

