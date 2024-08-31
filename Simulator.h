#ifndef HAZE_SIMULATOR_H
#define HAZE_SIMULATOR_H

#include "Constants.h"
#include "ErrorReporter.h"

#include <array>
#include <vector>
#include <format>
#include <cstdint>

// Haze Simulator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Simulator
	{
	private:
		static constexpr auto DATA_STACK = HALF_DWORD_MAX - 1;
		static constexpr auto CODE_STACK = QUARTER_DWORD_MAX - 1;

	private:
		std::array<byte, 4> register_file;
		word instruction_pointer;
		word data_stack_pointer;
		word code_stack_pointer;

	private:
		std::array<byte, HALF_DWORD_MAX> ram;
		std::array<byte, HALF_DWORD_MAX> rom;

	private:
		int opcode, operand1, operand2, immediate, memory;

	private:
		bool running = true;

	public:
		Simulator(const std::vector<std::uint8_t>& bytes)
		{
			if (bytes.size() <= HALF_DWORD_MAX)
			{
				//populate rom
				rom = { 0 };
				std::copy(bytes.begin(), bytes.end(), rom.begin());

				reset();
				return;
			}

			_error_reporter->post_uncorrectable(std::format("object code length {} bytes exceeds the ROM capacity of {} bytes", bytes.size(), rom.size()), NULL_TOKEN);
		}

	private:
		bool step();

	public:
		void reset();
		void run();
	};
}

#endif
