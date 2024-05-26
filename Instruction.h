#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <cstdint>
#include <unordered_map>

#include "Node.h"

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
        CALL = 0x9,
        EXIT = 0xA,
        PUSH = 0xB,
        PULL = 0xC,
        BREZ = 0xD,
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
        { hz::R0, "r0" },
        { hz::R1, "r1" },
        { hz::R2, "r2" },
        { hz::R3, "r3" },
        { hz::DC, "ud" },
    };

    static const auto unmap = [](hz::Register r)
    {
        return register_map.at(r);
    };

    class Instruction : public Node
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

    public:
        virtual Node::Type ntype() const final override;
        virtual std::string string() const final override;
        virtual Instruction* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual Instruction* optimize() final override;
    };
}

#endif //HAZE_INSTRUCTION_H
