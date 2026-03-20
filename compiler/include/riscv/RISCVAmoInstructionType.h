#ifndef HAZE_RISCVAMOINSTRUCTIONTYPE_H
#define HAZE_RISCVAMOINSTRUCTIONTYPE_H

// Haze RISCVAmoInstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVAmoInstructionType
	{
		AMOSWAP, AMOADD, AMOXOR, AMOAND, AMOOR, AMOMIN, AMOMAX, AMOMINU, AMOMAXU,
	};
}

#endif
