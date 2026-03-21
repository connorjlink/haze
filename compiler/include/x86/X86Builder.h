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
		static std::uint8_t modrm(std::uint8_t, std::uint8_t, std::uint8_t);
		// register-register mod R/M byte
		static std::uint8_t modrm_rr(std::uint8_t, std::uint8_t);

	public:
		// scale-index byte
		static std::uint8_t sib(std::uint8_t, std::uint8_t, std::uint8_t);
	};
}

#endif 
