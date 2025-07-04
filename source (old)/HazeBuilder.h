#ifndef HAZE_HAZEBUILDER_H
#define HAZE_HAZEBUILDER_H

#include "Constants.h"
#include "InstructionEncoding.h"

// Haze HazeBuilder.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class HazeBuilder
	{
	private:
		// convert Haze Register to integer
		static register_t convert(Register);

	public:
		// Haze opcode-destination-source byte
		static std::uint8_t opcode(HazeOpcode, Register, Register);
		// Haze immediate-memory bytes
		static byterange imm_mem(std::uint8_t, std::uint16_t);

	public:
		// move destination, source
		static byterange move(Register, Register);
		// load destination, &address
		static byterange load(Register, std::uint16_t);
		// copy destination, #immediate
		static byterange copy(Register, std::uint8_t);
		// iadd destination, source
		static byterange iadd(Register, Register);
		// isub destination, source
		static byterange isub(Register, Register);
		// band destination, source
		static byterange band(Register, Register);
		// bior destination, source
		static byterange bior(Register, Register);
		// bxor destination, source
		static byterange bxor(Register, Register);
		// call &address
		static byterange call(std::uint16_t);
		// exit
		static byterange exit();
		// push source
		static byterange push(Register);
		// pull destination
		static byterange pull(Register);
		// brnz &address, source
		static byterange brnz(std::uint16_t, Register);
		// lneg source
		static byterange lneg(Register);
		// stop
		static byterange stop();
	};
}

#endif
