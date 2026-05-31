#ifndef HAZE_RISCVREGISTER_H
#define HAZE_RISCVREGISTER_H

#include <utility/AutoEnum.h>
#include <utility/Constants.h>

// Haze RISCVRegister.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define RISCV_REGISTERS(X) \
	X(X0, x0, 0) \
	X(ZERO, zero, 0) \
	X(X1, x1, 1) \
	X(RA, ra, 1) \
	X(X2, x2, 2) \
	X(SP, sp, 2) \
	X(X3, x3, 3) \
	X(GP, gp, 3) \
	X(X4, x4, 4) \
	X(TP, tp, 4) \
	X(X5, x5, 5) \
	X(T0, t0, 5) \
	X(X6, x6, 6) \
	X(T1, t1, 6) \
	X(X7, x7, 7) \
	X(T2, t2, 7) \
	X(X8, x8, 8) \
	X(FP, s0, 8) \
	X(X9, x9, 9) \
	X(S1, s1, 9) \
	X(X10, x10, 10) \
	X(A0, a0, 10) \
	X(X11, x11, 11) \
	X(A1, a1, 11) \
	X(X12, x12, 12) \
	X(A2, a2, 12) \
	X(X13, x13, 13) \
	X(A3, a3, 13) \
	X(X14, x14, 14) \
	X(A4, a4, 14) \
	X(X15, x15, 15) \
	X(A5, a5, 15) \
	X(X16, x16, 16) \
	X(A6, a6, 16) \
	X(X17, x17, 17) \
	X(A7, a7, 17) \
	X(X18, x18, 18) \
	X(S2, s2, 18) \
	X(X19, x19, 19) \
	X(S3, s3, 19) \
	X(X20, x20, 20) \
	X(S4, s4, 20) \
	X(X21, x21, 21) \
	X(S5, s5, 21) \
	X(X22, x22, 22) \
	X(S6, s6, 22) \
	X(X23, x23, 23) \
	X(S7, s7, 23) \
	X(X24, x24, 24) \
	X(S8, s8, 24) \
	X(X25, x25, 25) \
	X(S9, s9, 25) \
	X(X26, x26, 26) \
	X(S10, s10, 26) \
	X(X27, x27, 27) \
	X(S11, s11, 27) \
	X(X28, x28, 28) \
	X(T3, t3, 28) \
	X(X29, x29, 29) \
	X(T4, t4, 29) \
	X(X30, x30, 30) \
	X(T5, t5, 30) \
	X(X31, x31, 31) \
	X(T6, t6, 31)

#define ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define SWITCH_CASE(enumerator, name, value) case RISCVRegister::enumerator: return #name;

	DEFINE_ENUM_BACKED(ENUM_MEMBER, SWITCH_CASE, RISCV_REGISTERS, RISCVRegister, RISC-V register, : Register)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
