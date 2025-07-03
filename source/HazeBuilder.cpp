import std;

#include "HazeBuilder.h"
#include "BinaryUtilities.h"
#include "ErrorReporter.h"

// Haze HazeBuilder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	register_t HazeBuilder::convert(Register source)
	{
		switch (source)
		{
			case R0: return 0;
			case R1: return 1;
			case R2: return 2;
			case R3: return 3;
			
			// since DC is used as a special value to indicate unused,
			// just return 0 for the instruction encoding
			case DC: return 0;

			default:
			{
				USE_UNSAFE(ErrorReporter)->post_error(std::format("invalid source register `{}`", static_cast<int>(source)), NULL_TOKEN);
				return -1;
			}
		}
	}

	std::uint8_t HazeBuilder::opcode(HazeOpcode opcode, Register destination, Register source)
	{
		const auto o = static_cast<int>(opcode);
		const auto d = convert(destination);
		const auto s = convert(source);

		return static_cast<std::uint8_t>((o << 4) | (d << 2) | s);
	}

	byterange HazeBuilder::imm_mem(std::uint8_t immediate, std::uint16_t address)
	{
		// imm-mem composition
		const auto value = (immediate << 8) | address;

		byterange out{};

		PUT(bin::range16(value));

		return out;
	}


	byterange HazeBuilder::move(Register destination, Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(MOVE, destination, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::load(Register destination, std::uint16_t address)
	{
		byterange out{};

		PUT(bin::range8(opcode(LOAD, destination, DC)));
		PUT(bin::range16(address));

		return out;
	}

	byterange HazeBuilder::copy(Register destination, std::uint8_t immediate)
	{
		byterange out{};

		PUT(bin::range8(opcode(COPY, destination, DC)));
		PUT(imm_mem(immediate, 0x0000));

		return out;
	}

	byterange HazeBuilder::iadd(Register destination, Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(IADD, destination, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::isub(Register destination, Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(ISUB, destination, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::band(Register destination, Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(BAND, destination, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::bior(Register destination, Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(BIOR, destination, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::bxor(Register destination, Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(BXOR, destination, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::call(std::uint16_t address)
	{
		byterange out{};

		PUT(bin::range8(opcode(CALL, DC, DC)));
		PUT(bin::range16(address));

		return out;
	}

	byterange HazeBuilder::exit()
	{
		byterange out{};

		PUT(bin::range8(opcode(EXIT, DC, DC)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::push(Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(PUSH, source, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::pull(Register destination)
	{
		byterange out{};

		PUT(bin::range8(opcode(PULL, destination, destination)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::brnz(std::uint16_t address, Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(BRNZ, source, source)));
		PUT(bin::range16(address));

		return out;
	}

	byterange HazeBuilder::lneg(Register source)
	{
		byterange out{};

		PUT(bin::range8(opcode(LNEG, source, source)));
		PUT(bin::range16(0x0000));

		return out;
	}

	byterange HazeBuilder::stop()
	{
		byterange out{};

		PUT(bin::range8(opcode(STOP, DC, DC)));
		PUT(bin::range16(0x0000));

		return out;
	}
}
