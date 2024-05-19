#ifndef HAZE_DISASSEMBLER_H
#define HAZE_DISASSEMBLER_H

#include <cstdint>
#include <string>
#include <array>

#include "Constants.h"

namespace hz
{
    class Disassembler
    {
    public:
        static std::string disassemble_instruction(std::uint32_t);
        static std::string disassemble_program(const std::array<std::uint8_t, QUARTER_DWORD_MAX>&, std::size_t, std::size_t);
    };
}

#endif //HAZE_DISASSEMBLER_H
