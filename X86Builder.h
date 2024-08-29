#ifndef HAZE_X86BUILDER_H
#define HAZE_X86BUILDER_H

#include <cstdint>

// Haze X86Builder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86Builder
	{
	public:
		// registers
		enum
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

	public:
		static std::uint8_t modrm(std::uint8_t, std::uint8_t, std::uint8_t);
	};
}

#endif 
