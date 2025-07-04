#ifndef HAZE_COMMONTOOLCHAIN_H
#define HAZE_COMMONTOOLCHAIN_H

#include <utility/Constants.h>
#include <utility/PlatformVariables.h>

// Haze CommonToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class InstructionCommand;

	// shared between compiler and assembler
	std::vector<InstructionCommand*> common_link(native_uint, native_uint);
	byterange common_emit(std::vector<InstructionCommand*>&&, const std::string& filepath);
	void common_finalize(const byterange&, const std::string&);
}

#endif 
