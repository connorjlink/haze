#ifndef HAZE_DISASSEMBLER_H
#define HAZE_DISASSEMBLER_H

#include <utility/Constants.h>

// Haze Disassembler.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class DisassemblerType
	{
		RISCV,
	};

	class Disassembler
	{
	public:
		virtual DissassemblerType dtype() const = 0;
		// returns the next instruction
		virtual std::string disassemble_next(const std::vector<ubyte>&, std::size_t&) const = 0;
	};
}

#endif
