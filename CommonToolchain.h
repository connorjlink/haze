#ifndef HAZE_COMMONTOOLCHAIN_H
#define HAZE_COMMONTOOLCHAIN_H

#include "Constants.h"

// Haze CommonToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class InstructionCommand;

	// shared between compiler and assembler
	std::vector<InstructionCommand*> common_link(std::uint32_t = HALF_DWORD_MAX);
	byterange common_emit(std::vector<InstructionCommand*>&&, const std::string& filepath);
	void common_finalize(const byterange&, const std::string&);
}

#endif 
