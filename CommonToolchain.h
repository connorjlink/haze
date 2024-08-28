#ifndef HAZE_COMMONTOOLCHAIN_H
#define HAZE_COMMONTOOLCHAIN_H

#include "Linkable.h"
#include "Constants.h"

#include <string>
#include <vector>
#include <cstdint>

namespace hz
{
	// shared between compiler and assembler
	std::vector<InstructionCommand*> common_link(std::uint16_t = HALF_DWORD_MAX);
	std::vector<std::uint8_t> common_emit(std::vector<InstructionCommand*>&&, const std::string& filepath);
	void common_finalize(std::vector<std::uint8_t>&&, const std::string&);
}

#endif 
