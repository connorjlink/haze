#ifndef HAZE_RISCVMEXTENSIONINSTRUCTIONTYPE_H
#define HAZE_RISCVMEXTENSIONINSTRUCTIONTYPE_H

// Haze RISCVMExtensionInstructionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class RISCVMExtensionInstructionType
	{
		MUL, MULH, MULHU, MULHSU, DIV, DIVU, REM, REMU,
	};
}

#endif
