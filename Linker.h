#ifndef HAZE_LINKER_H
#define HAZE_LINKER_H

#include "Linkable.h"
#include "Instruction.h"

#include <vector>
#include <cstdint>

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
		virtual std::vector<std::uint8_t> link() = 0;
	};
}

#endif
