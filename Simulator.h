#ifndef HAZE_SIMULATOR_H
#define HAZE_SIMULATOR_H

#include <cstdint>
#include <array>
#include <limits>
#include <vector>

#include "Log.h"

namespace hz
{
    class Simulator
    {
    private:
        using word = std::uint8_t;
        using dword = std::uint16_t;

    private:
        static constexpr auto WORD_MAX = std::numeric_limits<word>::max() + 1;
        static constexpr auto DWORD_MAX = std::numeric_limits<dword>::max() + 1;
        static constexpr auto HALF_WORD_MAX = WORD_MAX / 2;
        static constexpr auto HALF_DWORD_MAX = DWORD_MAX / 2;
        static constexpr auto QUARTER_WORD_MAX = HALF_WORD_MAX / 2;
        static constexpr auto QUARTER_DWORD_MAX = HALF_DWORD_MAX / 2;

        static constexpr auto STACK_TOP = HALF_DWORD_MAX - 1;

    private:
        std::array<word, 4> register_file;
        dword instruction_pointer;
        dword stack_pointer;

    private:
        std::array<word, HALF_DWORD_MAX> ram;
        std::array<word, QUARTER_DWORD_MAX> rom;
        std::array<word, QUARTER_DWORD_MAX> dsp;
    private:
        int opcode, operand1, operand2, immediate, memory;

    public:
        explicit Simulator(std::vector<std::uint8_t>& bytes)
        {
            if (bytes.size() <= QUARTER_DWORD_MAX)
            {
                //populate rom
                rom = { 0 };
                std::copy(bytes.begin(), bytes.end(), rom.begin());

                reset();
                return;
            }

            Log::error(fmt::format("Compiled code of length {} bytes exceeds the maximum ROM size of {} bytes", bytes.size(), QUARTER_DWORD_MAX));
        }

    private:
        bool step();

    public:
        void reset();
        void run();
    };
}

#endif //HAZE_SIMULATOR_H
