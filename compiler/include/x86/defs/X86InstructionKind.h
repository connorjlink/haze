#ifndef HAZE_X86INSTRUCTIONKIND_H
#define HAZE_X86INSTRUCTIONKIND_H

#include <utility/AutoEnum.h>
#include <utility/Formatter.h>

// Haze X86InstructionKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define X86_INSTRUCTION_KINDS(X) \
	X(PUSH,    push) \
	X(POP,     pop) \
	X(MOV,     mov) \
	X(MOVZX,   movzx) \
	X(ADD,     add) \
	X(SUB,     sub) \
	X(OR,      or ) \
	X(AND,     and) \
	X(XOR,     xor) \
	X(INC,     inc) \
	X(DEC,     dec) \
	X(SAL,     sal) \
	X(SAR,     sar) \
	X(TEST,    test) \
	X(CMP,     cmp) \
	X(CALL,    call) \
	X(APICALL, apicall) \
	X(JMP,     jmp) \
	X(JE,      je) \
	X(JNE,     jne) \
	X(JL,      jl) \
	X(JLE,     jle) \
	X(JG,      jg) \
	X(JGE,     jge) \
	X(JA,      ja) \
	X(JAE,     jae) \
	X(JB,      jb) \
	X(JBE,     jbe) \
	X(SETE,    sete) \
	X(SETNE,   setne) \
	X(SETL,    setl) \
	X(SETLE,   setle) \
	X(SETG,    setg) \
	X(SETGE,   setge) \
	X(SETA,    seta) \
	X(SETAE,   setae) \
	X(SETB,    setb) \
	X(SETBE,   setbe) \
	X(NOP,     nop) \
	X(RET,     ret) \
	X(LEAVE,   leave)


#define ENUM_MEMBER(enumerator, name) enumerator,
#define SWITCH_CASE(enumerator, name) case X86InstructionKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name) Mapping{ #name, X86InstructionKind::enumerator },
#define FORWARD_DECLARATION(enumerator, name) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, X86_INSTRUCTION_KINDS, X86InstructionKind, x86 instruction kind)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif
