#ifndef HAZE_SIMULATOR_H
#define HAZE_SIMULATOR_H

#include <cstdint>
#include <array>
#include <vector>

#include "Log.h"
#include "Constants.h"

namespace hz
{
	class Simulator
	{
	private:
		static constexpr auto STACK_TOP = HALF_DWORD_MAX - 1;

	private:
		std::array<word, 4> register_file;
		dword instruction_pointer;
		dword stack_pointer;

	private:
		std::array<word, HALF_DWORD_MAX> ram;
		std::array<word, HALF_DWORD_MAX> rom;
	private:
		int opcode, operand1, operand2, immediate, memory;

	public:
		explicit Simulator(std::vector<std::uint8_t>&& bytes)
		{
			if (bytes.size() <= HALF_DWORD_MAX)
			{
				//populate rom
				rom = { 0 };
				std::copy(bytes.begin(), bytes.end(), rom.begin());

				reset();
				return;
			}

			Log::error(std::format("Object code of length {} bytes exceeds the maximum ROM size of {} bytes", bytes.size(), rom.size()));
		}

	private:
		bool step();

	public:
		void reset();
		void run();
	};
}

#endif //HAZE_SIMULATOR_H
