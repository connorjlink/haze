#ifndef HAZE_X86BUILDER_H
#define HAZE_X86BUILDER_H

#include "InstructionEncoding.h"
#include "Constants.h"

#include <cstdint>

// Haze X86Builder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Builder
	{
	public:
		// registers
		enum
		{
			EAX = 0,
			ECX,
			EDX,
			EBX,
			ESP,
			EBP,
			ESI,
			EDI,
		};

	public:
		static std::uint8_t modrm(std::uint8_t, std::uint8_t, std::uint8_t);
		static std::uint8_t modrm_rr(Register source, Register destination);

	public:
		static std::uint8_t sib(std::uint8_t, std::uint8_t, std::uint8_t);

	public:
		static byterange push_r(std::uint8_t);
		static byterange push_i(std::uint8_t);
		static byterange push_m(std::uint32_t);

	public:
		static byterange pop_r(std::uint8_t);
	};
}

#endif 
