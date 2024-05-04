#ifndef HAZE_DISASSEMBLER_H
#define HAZE_DISASSEMBLER_H

#include <cstdint>
#include <string>
#include <vector>

namespace hz
{
    class Disassembler
    {
    public:
        static std::string disassemble_instruction(std::uint32_t);
        static std::string disassemble_program(const std::vector<std::uint8_t>&, std::size_t, std::size_t);
    };
}

#endif //HAZE_DISASSEMBLER_H
