#ifndef HAZE_X86BUILDER_H
#define HAZE_X86BUILDER_H

#include "Constants.h"
#include "InstructionEncoding.h"

// Haze X86Builder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// registers
	enum : register_t
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

	class X86Builder
	{
	public:
		static std::uint8_t modrm(std::uint8_t, std::uint8_t, std::uint8_t);
		static std::uint8_t modrm_rr(std::uint8_t, std::uint8_t);

	public:
		static std::uint8_t sib(std::uint8_t, std::uint8_t, std::uint8_t);

	public:
		static byterange push_r(std::uint8_t);
		static byterange push_i(std::uint8_t);
		static byterange push_m(std::uint32_t);

	public:
		static byterange pop_r(std::uint8_t);

	public:
		static byterange mov_rr(std::uint8_t, std::uint8_t);
		static byterange mov_ro(std::uint8_t, std::int32_t);

	public:
		static byterange add_rr(std::uint8_t, std::uint8_t);
		static byterange add_ri(std::uint8_t, std::int32_t);

	public:
		static byterange sub_rr(std::uint8_t, std::uint8_t);
		static byterange sub_ri(std::uint8_t, std::int32_t);

	public:
		static byterange or_rr(std::uint8_t, std::uint8_t);
		static byterange or_ri(std::uint8_t, std::int32_t);

	public:
		static byterange and_rr(std::uint8_t, std::uint8_t);
		static byterange and_ri(std::uint8_t, std::int32_t);

	public:
		static byterange xor_rr(std::uint8_t, std::uint8_t);
		static byterange xor_ri(std::uint8_t, std::int32_t);

	public:
		static byterange inc_r(std::uint8_t);
		static byterange dec_r(std::uint8_t);
	};
}

#endif 
