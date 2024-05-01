#include "Instruction.h"

namespace hz
{
    Instruction::Instruction(std::uint32_t bytes)
    {
        mem = bytes & 0xFFFF;
        imm = (bytes & 0xFF00) >> 8;
        op2 = static_cast<Register>((bytes & 0x030000) >> 16);
        op1 = static_cast<Register>((bytes & 0x0C0000) >> 18);
        opcode = static_cast<Opcode>((bytes & 0xF00000) >> 20);
    }

    std::uint32_t Instruction::bytes() const
    {
        std::uint32_t instruction{};

        instruction |= mem;
        instruction |= imm << 8;
        instruction |= op2 << 16;
        instruction |= op1 << 18;
        instruction |= opcode << 20;

        return instruction;
    }
}