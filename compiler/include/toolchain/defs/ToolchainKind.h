#ifndef HAZE_TOOLCHAINKIND_H
#define HAZE_TOOLCHAINKIND_H

#include <utility/AutoEnum.h>

// Haze ToolchainKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TOOLCHAIN_KINDS(X) \
	X(ASSEMBLER, assembler, .s) \
	X(COMPILER, compiler, .c)


#define ENUM_MEMBER(enumerator, name, extension) enumerator,
#define SWITCH_CASE(enumerator, name, extension) case ToolchainKind::enumerator: return #name;

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, TOOLCHAIN_KINDS, ToolchainKind, toolchain kind)

#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif 
