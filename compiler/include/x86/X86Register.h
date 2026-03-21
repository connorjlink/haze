#ifndef HAZE_X86REGISTER_H
#define HAZE_X86REGISTER_H

// Haze X86Register.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum X86Register : std::int8_t
	{
		EAX = 0,
		ECX = 1,
		EDX = 2,
		EBX = 3,
		ESP = 4,
		EBP = 5,
		ESI = 6,
		EDI = 7,
	};
}

#endif
