#include "Instruction.h"

namespace hz
{
    std::uint32_t Instruction::build(Opcode opcode, Register op1, Register op2, std::uint8_t imm, std::uint16_t mem)
    {
        std::uint32_t instruction = 0u;

        instruction |= mem;
        instruction |= imm << 8;
        instruction |= op2 << 16;
        instruction |= op1 << 18;
        instruction |= opcode << 20;

        return instruction;
    }
}