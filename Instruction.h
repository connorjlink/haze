#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <cstdint>

namespace hz
{
    enum Opcode
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
        BNOT = 0x9,
        CALL = 0xA,
        EXIT = 0xB,
        PUSH = 0xC,
        PULL = 0xD,
        BREZ = 0xE,
        RSVD = 0xF,
    };

    enum Register
    {
        DC = 0b00,
        R0 = 0b00,
        R1 = 0b01,
        R2 = 0b10,
        R3 = 0b11,
    };

    class Instruction
    {
    public:
        static std::uint32_t build(Opcode, Register, Register, std::uint8_t = 0, std::uint16_t = 0);
    };
}

#endif //HAZE_INSTRUCTION_H
