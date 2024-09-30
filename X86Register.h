#ifndef HAZE_X86REGISTER_H
#define HAZE_X86REGISTER_H

// Haze X86Register.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum X86Register : std::uint8_t
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
}

#endif
