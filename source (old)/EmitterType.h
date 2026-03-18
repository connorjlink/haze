#ifndef HAZE_EMITTERTYPE_H
#define HAZE_EMITTERTYPE_H

// Haze EmitterType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class EmitterType
	{
		X86,
		RISCV,
	};

	static const std::unordered_map<EmitterType, std::string> emitter_type_to_string
	{
		{ EmitterType::X86, "x86" },
		{ EmitterType::RISCV, "riscv" },
	};
}

#endif
