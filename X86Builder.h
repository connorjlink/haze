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
		// mod R/M byte
		static std::uint8_t modrm(std::uint8_t, std::uint8_t, std::uint8_t);
		// register-register mod R/M byte
		static std::uint8_t modrm_rr(std::uint8_t, std::uint8_t);

	public:
		// scale-index byte
		static std::uint8_t sib(std::uint8_t, std::uint8_t, std::uint8_t);

	public:
		// push source
		static byterange push_r(std::uint8_t);
		// push immediate
		static byterange push_i(std::uint8_t);
		// push effective address
		static byterange push_ea(std::uint32_t);
		// push DWORD PTR
		static byterange push_m(std::uint32_t);

	public:
		// pop destination
		static byterange pop_r(std::uint8_t);

	public:
		// mov destination, source
		static byterange mov_rr(std::uint8_t, std::uint8_t);
		// mov destination, [source + offset]
		static byterange mov_ro(std::uint8_t, std::int32_t);

	public:
		// mov destination, [pointer]
		static byterange mov_rm(std::uint8_t, std::uint32_t);
		// mov [pointer], source
		static byterange mov_mr(std::uint32_t, std::uint8_t);

	public:
		// mov destination, immediate
		static byterange mov_ri(std::uint8_t, std::uint32_t);

	public:
		// add destination, source
		static byterange add_rr(std::uint8_t, std::uint8_t);
		// add destination, immediate
		static byterange add_ri(std::uint8_t, std::int32_t);

	public:
		// sub destination, source
		static byterange sub_rr(std::uint8_t, std::uint8_t);
		// sub destination, immediate
		static byterange sub_ri(std::uint8_t, std::int32_t);

	public:
		// or destination, source
		static byterange or_rr(std::uint8_t, std::uint8_t);
		// or destination, immediate
		static byterange or_ri(std::uint8_t, std::int32_t);

	public:
		// and destination, source
		static byterange and_rr(std::uint8_t, std::uint8_t);
		// and destination, immediate
		static byterange and_ri(std::uint8_t, std::int32_t);

	public:
		// xor destination, source
		static byterange xor_rr(std::uint8_t, std::uint8_t);
		// xor destination, immediate
		static byterange xor_ri(std::uint8_t, std::int32_t);

	public:
		// inc destination
		static byterange inc_r(std::uint8_t);
		// inc destination
		static byterange dec_r(std::uint8_t);

	public:
		// test lhs, rhs
		static byterange test_rr(std::uint8_t, std::uint8_t);
		// cmp lhs, rhs
		static byterange cmp_rr(std::uint8_t, std::uint8_t);

	public:
		// call [pointer]
		static byterange call(std::uint32_t);

	public:
		// ret
		static byterange ret();
	};
}

#endif 
