#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <cstdint>
#include <unordered_map>
#include <string_view>

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

    static const std::unordered_map<hz::Register, std::string_view> register_map =
    {
        { hz::R0, "x" },
        { hz::R1, "y" },
        { hz::R2, "z" },
        { hz::R3, "w" },
        { hz::DC, "D" },
    };

    static const auto unmap = [](hz::Register r)
    {
        return register_map.at(r);
    };

    struct Instruction
    {
    public:
        Opcode opcode;
        Register op1, op2;
        std::uint8_t imm;
        std::uint16_t mem;

    public:
        Instruction() = delete;
        explicit Instruction(std::uint32_t);
        Instruction(Opcode, Register, Register, std::uint8_t = 0, std::uint16_t = 0);
        std::uint32_t bytes() const;

        //static std::uint32_t compose(Opcode, Register, Register, std::uint8_t = 0, std::uint16_t = 0);
        //static Instruction decompose(std::uint32_t);
    };
}

#endif //HAZE_INSTRUCTION_H
