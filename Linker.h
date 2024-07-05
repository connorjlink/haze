#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Linkable.h"
#include "InstructionCommand.h"

#include <vector>
#include <cstdint>

#define AS_COMPILER_LINKER(x) static_cast<CompilerLinker*>(x)
#define AS_ASSEMBLER_LINKER(x) static_cast<AssemblerLinker*>(x)

namespace hz
{
	class Linker
	{
	public:
		enum class Type
		{
			COMPILER,
			ASSEMBLER,
		};

	public:
		virtual Linker::Type ltype() const = 0;
		virtual bool optimize() = 0;
		virtual std::vector<InstructionCommand*> link(std::uint16_t) = 0;
	};
}

#endif
