#ifndef HAZE_LINKERTYPE_H
#define HAZE_LINKERTYPE_H

// Haze LinkerType.h
// (c) Connor J. Link. All Rights Reserved.

#define AS_COMPILER_LINKER(x) static_cast<CompilerLinker*>(x)
#define AS_ASSEMBLER_LINKER(x) static_cast<AssemblerLinker*>(x)

namespace hz
{
	enum class LinkerType
	{
		COMPILER,
		ASSEMBLER,
	};

	static const std::unordered_map<LinkerType, std::string> _linker_type_map
	{
		{ LinkerType::COMPILER, "compiler" },
		{ LinkerType::ASSEMBLER, "assembler" },
	};
}

#endif
