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


#define AUTOENUM_ROUTER(X, enumerator, name, extension) X(enumerator, FakeType, name, ToolchainKind)

	DEFINE_ENUM(TOOLCHAIN_KINDS, ToolchainKind, toolchain kind)

#undef AUTOENUM_ROUTER
}

#endif 
