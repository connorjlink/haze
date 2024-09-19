import std;

#include "X86Builder.h"
#include "BinaryUtilities.h"

// Haze X86Builder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::uint8_t X86Builder::modrm(std::uint8_t mod, std::uint8_t reg, std::uint8_t rm)
	{
		return ((mod & 0b11) << 6) | ((reg & 0b111) << 3) | ((rm & 0b111) << 0);
	}

	std::uint8_t X86Builder::modrm_rr(std::uint8_t destination, std::uint8_t source)
	{
		const auto mod = 0b11;
		const auto reg = source;
		const auto rm = destination;

		return modrm(mod, reg, rm);
	}


	std::uint8_t X86Builder::sib(std::uint8_t scale, std::uint8_t index, std::uint8_t base)
	{
		return X86Builder::modrm(scale, index, base);
	}


	byterange X86Builder::push_r(std::uint8_t source)
	{
		const auto reg = source & 0b111;

		return { static_cast<std::uint8_t>(0x50 | reg) };
	}

	byterange X86Builder::push_i(std::uint8_t source)
	{
		byterange out{};

		PUT(BinaryUtilities::range8(0x6A));
		PUT(BinaryUtilities::range8(source));

		return out;
	}

	// non-dereferencing
	byterange hz::X86Builder::push_ea(std::uint32_t address)
	{
		byterange out{};

		PUT(BinaryUtilities::range8(0x68)); 
		PUT(BinaryUtilities::range32(address));

		return out;
	}


	byterange X86Builder::pop_r(std::uint8_t destination)
	{
		const auto reg = destination & 0b111;

		return { static_cast<std::uint8_t>(0x58 | reg) };
	}



	byterange X86Builder::mov_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		PUT(BinaryUtilities::range8(0x89));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::mov_ro(std::uint8_t destination, std::int32_t offset)
	{
		byterange out{};

		PUT(BinaryUtilities::range8(0x8B));

		if (offset >= 0 && offset <= 0xFF)
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b01, destination, EBP)));
			PUT(BinaryUtilities::range8(offset));
		}

		else
		{
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b10, destination, EBP)));
			PUT(BinaryUtilities::range32(offset));
		}

		return out;
	}


	byterange X86Builder::mov_rm(std::uint8_t destination, std::uint32_t pointer)
	{
		byterange out{};

		if (destination == EAX)
		{
			// A1 --> MOV EAX, moffs32*
			PUT(BinaryUtilities::range8(0xA1));
			PUT(BinaryUtilities::range32(pointer));
		}

		else
		{
			// 8B /r --> MOV r32, r/m32
			PUT(BinaryUtilities::range8(0x8B));
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b00, destination, 0b101)));
			PUT(BinaryUtilities::range32(pointer));
		}

		return out;
	}

	byterange X86Builder::mov_rm(std::uint32_t pointer, std::uint8_t source)
	{
		byterange out{};

		if (source == EAX)
		{
			// A3 --> MOV moffs32*, EAX
			PUT(BinaryUtilities::range8(0xA3));
			PUT(BinaryUtilities::range32(pointer));
		}
		
		else
		{
			// 89 /r --> MOV r/m32,r32
			PUT(BinaryUtilities::range8(0x89));
			PUT(BinaryUtilities::range8(X86Builder::modrm(0b00, source, 0b101)));
			PUT(BinaryUtilities::range32(pointer));
		}


		return out;
	}


	byterange X86Builder::add_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 01 /r --> ADD r/m32, r32
		PUT(BinaryUtilities::range8(0x01));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::add_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 04 ib --> ADD AL, imm8
				PUT(BinaryUtilities::range8(0x04));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 05 id --> ADD EAX, imm32
				PUT(BinaryUtilities::range8(0x05));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /0 ib --> ADD r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b000, destination)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /0 id --> ADD r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b000, destination)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::sub_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 29 /r --> SUB r/m32,r32
		PUT(BinaryUtilities::range8(0x29));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::sub_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 2C ib --> SUB AL, imm8
				PUT(BinaryUtilities::range8(0x2C));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 2D id --> SUB EAX, imm32
				PUT(BinaryUtilities::range8(0x2D));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /5 ib --> SUB r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b101, destination)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /5 id --> SUB r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b101, destination)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::or_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 09 /r --> OR r/m32, r32
		PUT(BinaryUtilities::range8(0x09));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::or_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 0C ib --> OR AL, imm8
				PUT(BinaryUtilities::range8(0x0C));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 0D id --> OR EAX, imm32
				PUT(BinaryUtilities::range8(0x0D));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /1 ib --> OR r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b001, destination)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /1 id --> OR r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b001, destination)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::and_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		// 09 /r --> OR r/m32, r32
		PUT(BinaryUtilities::range8(0x09));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::and_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 24 ib --> AND AL, imm8
				PUT(BinaryUtilities::range8(0x24));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 25 id --> AND EAX, imm32
				PUT(BinaryUtilities::range8(0x25));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /4 ib --> AND r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b100, destination)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /4 id --> AND r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b100, destination)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::xor_rr(std::uint8_t destination, std::uint8_t source)
	{
		byterange out{};

		//
		PUT(BinaryUtilities::range8(0x09));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(destination, source)));

		return out;
	}

	byterange X86Builder::xor_ri(std::uint8_t destination, std::int32_t value)
	{
		byterange out{};

		if (destination == EAX)
		{
			if (value >= 0 && value <= 0xFF)
			{
				//34 ib --> XOR AL, imm8
				PUT(BinaryUtilities::range8(0x34));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 35 id --> XOR EAX, imm32
				PUT(BinaryUtilities::range8(0x35));
				PUT(BinaryUtilities::range32(value));
			}
		}

		else
		{
			if (value >= 0 && value <= 0xFF)
			{
				// 80 /6 ib --> XOR r/m8, imm8
				PUT(BinaryUtilities::range8(0x80));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b110, destination)));
				PUT(BinaryUtilities::range8(value));
			}

			else
			{
				// 81 /6 id --> XOR r/m32, imm32
				PUT(BinaryUtilities::range8(0x81));
				PUT(BinaryUtilities::range8(X86Builder::modrm_rr(0b110, destination)));
				PUT(BinaryUtilities::range32(value));
			}
		}

		return out;
	}


	byterange X86Builder::inc_r(std::uint8_t source)
	{
		byterange out{};

		// 40+ rd --> INC r32
		PUT(BinaryUtilities::range8(0x40 | source));

		return out;
	}

	byterange X86Builder::dec_r(std::uint8_t source)
	{
		byterange out{};

		// 48+rd --> DEC r32
		PUT(BinaryUtilities::range8(0x48 | source));

		return out;
	}


	byterange X86Builder::test_rr(std::uint8_t lhs, std::uint8_t rhs)
	{
		byterange out{};

		// 85 /r --> TEST r/m32, r32
		PUT(BinaryUtilities::range8(0x85));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(lhs, rhs)));

		return out;
	}

	byterange X86Builder::cmp_rr(std::uint8_t lhs, std::uint8_t rhs)
	{
		byterange out{};

		// 39 /r --> CMP r/m32, r32
		PUT(BinaryUtilities::range8(0x39));
		PUT(BinaryUtilities::range8(X86Builder::modrm_rr(lhs, rhs)));

		return out;
	}


	byterange X86Builder::call(std::uint32_t address)
	{
		byterange out{};

		// FF /2 --> CALL r/m32
		PUT(BinaryUtilities::range8(0xFF));
		PUT(BinaryUtilities::range8(0x15));
		PUT(BinaryUtilities::range32(address));

		return out;
	}

	byterange X86Builder::ret()
	{
		byterange out{};

		// C3 --> RET
		PUT(BinaryUtilities::range8(0xC3));

		return out;
	}
}
