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
		// push imm8
		static byterange push_i8(std::uint8_t);
		// push imm32
		static byterange push_i32(std::uint32_t);
		// push DWORD PTR
		static byterange push_m(std::uint32_t);

	public:
		// pop destination
		static byterange pop_r(std::uint8_t);

	public:
		// mov destination, source
		static byterange mov_rr(std::uint8_t, std::uint8_t);

	public:
		// mov destination, [source + offset]
		static byterange mov_ro(std::uint8_t, std::int32_t);
		// mov [source + offset], destination
		static byterange mov_or(std::int32_t, std::uint8_t);

	public:
		// mov destination, [pointer]
		static byterange mov_rm(std::uint8_t, std::uint32_t);
		// mov [pointer], source
		static byterange mov_mr(std::uint32_t, std::uint8_t);

	public:
		// mov destination, immediate
		static byterange mov_ri(std::uint8_t, std::uint32_t);

	public:
		// mov [base + offset], immediate
		static byterange mov_mi(std::uint8_t, std::uint8_t, std::uint32_t);

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
		static byterange call_absolute(std::uint32_t);
		// call disp32
		static byterange call_relative(std::uint32_t);

	public:
		// jmp disp32
		static byterange jmp_relative(std::uint32_t);
		// je disp32
		static byterange je_relative(std::uint32_t);
		// jne disp32
		static byterange jne_relative(std::uint32_t);

	public:
		// sete destination
		static byterange sete(std::uint8_t);
		// setl destination
		static byterange setl(std::uint8_t);
		// setg destination
		static byterange setg(std::uint8_t);

	public:
		// movzx destination, source
		static byterange movzx(std::uint8_t, std::uint8_t);

	public:
		// ret
		static byterange ret();
		// ret imm16
		static byterange ret(std::uint16_t);

	public:
		// leave
		static byterange leave();
	};
}

#endif 
