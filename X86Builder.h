#ifndef HAZE_X86BUILDER_H
#define HAZE_X86BUILDER_H

#include "Constants.h"
#include "InstructionEncoding.h"
#include "X86Register.h"

// Haze X86Builder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
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
		// mov destination, [base + displacement]
		static byterange mov_rbd(std::uint8_t, std::uint8_t, std::int32_t);
		// mov [base + displacement], source
		static byterange mov_bdr(std::uint8_t, std::int32_t, std::uint8_t);
		// mov destination, [EBP + displacement]
		static byterange mov_rbo(std::uint8_t, std::int32_t);
		// mov [EBP + displacement], destination
		static byterange mov_obr(std::int32_t, std::uint8_t);

	public:
		// mov destination, [pointer]
		static byterange mov_rm(std::uint8_t, std::uint32_t);
		// mov [pointer], source
		static byterange mov_mr(std::uint32_t, std::uint8_t);

	public:
		// mov destination, immediate
		static byterange mov_ri(std::uint8_t, std::uint32_t);

	public:
		// mov [base + disp8], imm32
		static byterange mov_bdi(std::uint8_t, std::int8_t, std::uint32_t);

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
		// sal destination, immediate
		static byterange sal_imm(std::uint8_t, std::uint8_t);
		// sar destination, immediate
		static byterange sar_imm(std::uint8_t, std::uint8_t);

	public:
		// test lhs, rhs
		static byterange test_rr(std::uint8_t, std::uint8_t);
		// cmp lhs, rhs
		static byterange cmp_rr(std::uint8_t, std::uint8_t);
		// cmp lhs, imm32
		static byterange cmp_ri(std::uint8_t, std::int32_t);

	public:
		// call [pointer]
		static byterange call_absolute(std::uint32_t);
		// call disp32
		static byterange call_relative(std::int32_t);

	public:
		// jmp disp32
		static byterange jmp_relative(std::int32_t);
		// je disp32
		static byterange je_relative(std::int32_t);
		// jne disp32
		static byterange jne_relative(std::int32_t);
		// jl disp32
		static byterange jl_relative(std::int32_t);
		// jle disp32
		static byterange jle_relative(std::int32_t);
		// jg disp32
		static byterange jg_relative(std::int32_t);
		// jge disp32
		static byterange jge_relative(std::int32_t);
		// ja disp32
		static byterange ja_relative(std::int32_t);
		// jae disp32
		static byterange jae_relative(std::int32_t);
		// jb disp32
		static byterange jb_relative(std::int32_t);
		// jbe disp32
		static byterange jbe_relative(std::int32_t);

	public:
		// sete destination
		static byterange sete_r(std::uint8_t);
		// setne destination
		static byterange setne_r(std::uint8_t);
		// setl destination
		static byterange setl_r(std::uint8_t);
		// setle destination
		static byterange setle_r(std::uint8_t);
		// setg destination
		static byterange setg_r(std::uint8_t);
		// setge destination
		static byterange setge_r(std::uint8_t);
		// seta destination
		static byterange seta_r(std::uint8_t);
		// setae destination
		static byterange setae_r(std::uint8_t);
		// setb destination
		static byterange setb_r(std::uint8_t);
		// setbe destination
		static byterange setbe_r(std::uint8_t);

	public:
		// movzx destination, source
		static byterange movzx_rr(std::uint8_t, std::uint8_t);

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
