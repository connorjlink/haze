#ifndef HAZE_RISCVZICSREXTENSIONINSTRUCTIONTYPE_H
#define HAZE_RISCVZICSREXTENSIONINSTRUCTIONTYPE_H

// Haze RISCVZicsrExtensionInstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVZicsrExtensionInstructionType
	{
		CSRRW, CSRRS, CSRRC, CSRRWI, CSRRSI, CSRRCI,
	};
}

#endif
