#ifndef HAZE_LINKERTYPE_H
#define HAZE_LINKERTYPE_H

#define AS_COMPILER_LINKER(x) static_cast<CompilerLinker*>(x)
#define AS_ASSEMBLER_LINKER(x) static_cast<AssemblerLinker*>(x)

namespace hz
{
	enum class LinkerType
	{
		COMPILER,
		ASSEMBLER,
	};
}

#endif
