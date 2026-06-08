#ifndef HAZE_TOOLCHAINKIND_H
#define HAZE_TOOLCHAINKIND_H

#include <utility/AutoEnum.h>

// Haze ToolchainKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define TOOLCHAIN_KINDS(X) \
	X(ASSEMBLER, assembler,    .s) \
	X(HAZE,      intermediate, .hz) \
	X(COMPILER,  compiler,     .cjl)


#define ENUM_MEMBER(enumerator, name, extension) enumerator,
#define SWITCH_CASE(enumerator, name, extension) case ToolchainKind::enumerator: return #name;
#define MAP_MEMBER(enumerator, name, extension) Mapping{ #name, ToolchainKind::enumerator },
#define FORWARD_DECLARATION(enumerator, name, extension) /* elide forward declarations */

	DEFINE_ENUM(ENUM_MEMBER, SWITCH_CASE, MAP_MEMBER, FORWARD_DECLARATION, TOOLCHAIN_KINDS, ToolchainKind, toolchain kind)

#undef FORWARD_DECLARATION
#undef MAP_MEMBER
#undef SWITCH_CASE
#undef ENUM_MEMBER
}

#endif 
