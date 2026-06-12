#ifndef HAZE_RISCVREGISTER_H
#define HAZE_RISCVREGISTER_H

#include <utility/AutoEnum.h>
#include <utility/Constants.h>

// Haze RISCVRegister.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define RISCV_REGISTERS(X) \
	X(X0,   x0,    0, ALIAS) \
	X(ZERO, zero,  0, MAIN) \
	X(X1,   x1,    1, ALIAS) \
	X(RA,   ra,    1, MAIN) \
	X(X2,   x2,    2, ALIAS) \
	X(SP,   sp,    2, MAIN) \
	X(X3,   x3,    3, ALIAS) \
	X(GP,   gp,    3, MAIN) \
	X(X4,   x4,    4, ALIAS) \
	X(TP,   tp,    4, MAIN) \
	X(X5,   x5,    5, ALIAS) \
	X(T0,   t0,    5, MAIN) \
	X(X6,   x6,    6, ALIAS) \
	X(T1,   t1,    6, MAIN) \
	X(X7,   x7,    7, ALIAS) \
	X(T2,   t2,    7, MAIN) \
	X(X8,   x8,    8, ALIAS) \
	X(FP,   s0,    8, MAIN) \
	X(X9,   x9,    9, ALIAS) \
	X(S1,   s1,    9, MAIN) \
	X(X10,  x10,  10, ALIAS) \
	X(A0,   a0,   10, MAIN) \
	X(X11,  x11,  11, ALIAS) \
	X(A1,   a1,   11, MAIN) \
	X(X12,  x12,  12, ALIAS) \
	X(A2,   a2,   12, MAIN) \
	X(X13,  x13,  13, ALIAS) \
	X(A3,   a3,   13, MAIN) \
	X(X14,  x14,  14, ALIAS) \
	X(A4,   a4,   14, MAIN) \
	X(X15,  x15,  15, ALIAS) \
	X(A5,   a5,   15, MAIN) \
	X(X16,  x16,  16, ALIAS) \
	X(A6,   a6,   16, MAIN) \
	X(X17,  x17,  17, ALIAS) \
	X(A7,   a7,   17, MAIN) \
	X(X18,  x18,  18, ALIAS) \
	X(S2,   s2,   18, MAIN) \
	X(X19,  x19,  19, ALIAS) \
	X(S3,   s3,   19, MAIN) \
	X(X20,  x20,  20, ALIAS) \
	X(S4,   s4,   20, MAIN) \
	X(X21,  x21,  21, ALIAS) \
	X(S5,   s5,   21, MAIN) \
	X(X22,  x22,  22, ALIAS) \
	X(S6,   s6,   22, MAIN) \
	X(X23,  x23,  23, ALIAS) \
	X(S7,   s7,   23, MAIN) \
	X(X24,  x24,  24, ALIAS) \
	X(S8,   s8,   24, MAIN) \
	X(X25,  x25,  25, ALIAS) \
	X(S9,   s9,   25, MAIN) \
	X(X26,  x26,  26, ALIAS) \
	X(S10,  s10,  26, MAIN) \
	X(X27,  x27,  27, ALIAS) \
	X(S11,  s11,  27, MAIN) \
	X(X28,  x28,  28, ALIAS) \
	X(T3,   t3,   28, MAIN) \
	X(X29,  x29,  29, ALIAS) \
	X(T4,   t4,   29, MAIN) \
	X(X30,  x30,  30, ALIAS) \
	X(T5,   t5,   30, MAIN) \
	X(X31,  x31,  31, ALIAS) \
	X(T6,   t6,   31, MAIN)

#define ENUM_MEMBER(enumerator, name, value) enumerator = value,
#define ENUM_MEMBER_ADAPTER(enumerator, name, value, category) ENUM_MEMBER(enumerator, name, value)

#define SWITCH_CASE(enumerator, name, value) case RISCVRegister::enumerator: return #name;
#define SWITCH_CASE_ROUTE_MAIN(enumerator, name, value) SWITCH_CASE(enumerator, name, value)
#define SWITCH_CASE_ROUTE_ALIAS(enumerator, name, value) /* elide switch cases for alternatives */
#define SWITCH_CASE_ADAPTER(enumerator, name, value, category) SWITCH_CASE_ROUTE_##category(enumerator, name, value)

#define MAP_MEMBER(enumerator, name, value) Mapping{ #name, RISCVRegister::enumerator },
#define MAP_MEMBER_ROUTE_MAIN(enumerator, name, value) MAP_MEMBER(enumerator, name, value)
#define MAP_MEMBER_ROUTE_ALIAS(enumerator, name, value) /* elide map members for alternatives */
#define MAP_MEMBER_ADAPTER(enumerator, name, value, category) MAP_MEMBER_ROUTE_##category(enumerator, name, value)

#define FORWARD_DECLARATION(enumerator, name, value, category) /* elide forward declarations */

	DEFINE_ENUM_INTERNAL(ENUM_MEMBER_ADAPTER, SWITCH_CASE_ADAPTER, MAP_MEMBER_ADAPTER, FORWARD_DECLARATION, RISCV_REGISTERS, RISCVRegister, RISC-V register, /* fake */)

#undef FORWARD_DECLARATION

#undef MAP_MEMBER_ADAPTER
#undef MAP_MEMBER_ROUTE_ALIAS
#undef MAP_MEMBER_ROUTE_MAIN
#undef MAP_MEMBER

#undef SWITCH_CASE_ADAPTER
#undef SWITCH_CASE_ROUTE_ALIAS
#undef SWITCH_CASE_ROUTE_MAIN
#undef SWITCH_CASE

#undef ENUM_MEMBER_ADAPTER
#undef ENUM_MEMBER

	inline Register to_register(RISCVRegister $register)
	{
		return static_cast<Register>($register);
	}
}

#endif
