#ifndef HAZE_X86SETCCINSTRUCTIONTYPE_H
#define HAZE_X86SETCCINSTRUCTIONTYPE_H

// Haze X86SetccInstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class X86SetccInstructionType
	{
		SETE,
		SETNE,
		SETL,
		SETLE,
		SETG,
		SETGE,
		SETA,
		SETAE,
		SETB,
		SETBE,
	};
}

#endif
