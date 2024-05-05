#include "Disassembler.h"
#include "Instruction.h"

#include <fmt/format.h>

#include "Constants.h"

namespace hz
{
    std::string Disassembler::disassemble_instruction(std::uint32_t raw)
    {
        const auto instruction = Instruction{ raw };

        const auto op1 = unmap(instruction.op1);
        const auto op2 = unmap(instruction.op2);

        const auto& imm = instruction.imm;
        const auto& mem = instruction.mem;

        switch (instruction.opcode)
        {
            case MOVE: return fmt::format("move {}, {} ", op1, op2);
            case LOAD: return fmt::format("load {}, &{}", op1, mem);
            case COPY: return fmt::format("copy {}, #{}", op1, imm);
            case SAVE: return fmt::format("save &{}, {}", mem, op2);
            case IADD: return fmt::format("iadd {}, {} ", op1, op2);
            case ISUB: return fmt::format("isub {}, {} ", op1, op2);
            case BAND: return fmt::format("band {}, {} ", op1, op2);
            case BIOR: return fmt::format("bior {}, {} ", op1, op2);
            case BXOR: return fmt::format("bxor {}, {} ", op1, op2);
            case BNOT: return fmt::format("bnot {}     ", op1);
            case CALL: return fmt::format("call &{}    ", mem);
            case EXIT: return fmt::format("exit        ");
            case PUSH: return fmt::format("push {}     ", op2);
            case PULL: return fmt::format("pull {}     ", op1);
            case BREZ: return fmt::format("brez &{}, {}", mem, op2);
            default:   return                  "ILLEGAL OPCODE";
        }
    }

    std::string Disassembler::disassemble_program(const std::array<std::uint8_t, QUARTER_DWORD_MAX>& program, std::size_t first, std::size_t last)
    {
        std::string result{};

        for (auto i = first; i < last - 2; i+= 3)
        {
            const auto byte1 = program[i + 0];
            const auto byte2 = program[i + 1];
            const auto byte3 = program[i + 2];

            const auto instruction = (program[i + 0] << 16) | (program[i + 1] << 8) | (program[i + 2] << 0);

            result.append(fmt::format("{} ; #{:02X} {:02X} {:02X}", disassemble_instruction(instruction), byte1, byte2, byte3));
        }

        return result;
    }
}
