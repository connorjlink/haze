#ifndef HAZE_COMMONTOOLCHAIN_H
#define HAZE_COMMONTOOLCHAIN_H

#include <utility/Constants.h>

// Haze CommonToolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class InstructionCommand;

	// shared between compiler and assembler
	std::vector<InstructionCommand*> common_link(Address, Address);
	void common_finalize(const ByteRange&, const std::string&);
}

#endif 
