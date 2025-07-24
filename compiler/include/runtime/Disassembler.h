#ifndef HAZE_DISASSEMBLER_H
#define HAZE_DISASSEMBLER_H

#include <utility/Constants.h>

// Haze Disassembler.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Disassembler
	{
	public:
		static std::string disassemble_instruction(std::uint32_t);
		static std::string disassemble_program(const std::vector<ubyte>&, std::size_t, std::size_t);
	};
}

#endif
