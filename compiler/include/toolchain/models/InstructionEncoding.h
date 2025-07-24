#ifndef HAZE_INSTRUCTIONENCODING_H
#define HAZE_INSTRUCTIONENCODING_H

// Haze InstructionEncoding.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class Opcode
	{
		MOVE,
		LOAD,
		COPY,
		SAVE,
		IADD,
		ISUB,
		BAND,
		BIOR,
		BXOR,
		CALL,
		EXIT,
		PUSH, // data
		PULL, // data
		MAKE, // args
		TAKE, // args
		BRNZ,
		BOOL,
		STOP,
	};

	enum HazeOpcode
	{
		MOVE = 0x0,
		LOAD = 0x1,
		COPY = 0x2,
		SAVE = 0x3,
		IADD = 0x4,
		ISUB = 0x5,
		BAND = 0x6,
		BIOR = 0x7,
		BXOR = 0x8,
		CALL = 0x9,
		EXIT = 0xA,
		PUSH = 0xB, 
		PULL = 0xC,
		BRNZ = 0xD,
		LNEG = 0xE,
		STOP = 0xF,
	};

	enum Register
	{
		R0 = 0b000,
		R1 = 0b001,
		R2 = 0b010,
		R3 = 0b011,
		DC = 0b100,
	};

	using register_t = std::int32_t;

	static const std::unordered_map<Register, std::string_view> _register_map =
	{
		{ R0, "r0" },
		{ R1, "r1" },
		{ R2, "r2" },
		{ R3, "r3" },
		{ DC, "ud" },
	};
}

#endif 
