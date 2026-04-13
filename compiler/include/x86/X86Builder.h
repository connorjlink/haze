#ifndef HAZE_X86BUILDER_H
#define HAZE_X86BUILDER_H

#include <utility/Constants.h>

// Haze X86Builder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Builder
	{
	public:
		// mod R/M byte
		static Register modrm(Register, Register, Register);
		// register-register mod R/M byte
		static Register modrm_rr(Register, Register);

	public:
		// scale-index byte
		static Register sib(Register, Register, Register);
	};
}

#endif 
