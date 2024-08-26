#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include <cstdint>
#include <unordered_map>

#include "Command.h"

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
        BRNZ = 0xD,
        BOOL = 0xE,
        //0xF reserved for future ISA extensions
    };

    enum Register
    {
        R0 = 0b000,
        R1 = 0b001,
        R2 = 0b010,
        R3 = 0b011,
        DC = 0b100,
        RV = DC, // alias for return values "fake stack"
    };

    static const std::unordered_map<Register, std::string_view> register_map =
    {
        { R0, "r0" },
        { R1, "r1" },
        { R2, "r2" },
        { R3, "r3" },
        { DC, "ud" },
    };

    static const auto unmap = [](Register r)
    {
        return register_map.at(r);
    };

    class InstructionCommand : public Command
    {
    public:
        Opcode opcode;
        Register dst, src;
        std::uint8_t imm;
        std::uint16_t mem;
        bool marked_for_deletion;
        std::string branch_target;

    public:
        std::vector<InstructionCommand*> embedded_object_code;
        std::uint16_t approximate_embedded_size;

    public:
        InstructionCommand() = delete;
        explicit InstructionCommand(std::uint32_t);
        InstructionCommand(Opcode, Register, Register, std::uint8_t = 0, std::uint16_t = 0, std::string = "");
        std::uint32_t bytes() const;

    public:
        virtual CommandType ctype() const final override;
        virtual std::string string() const final override;
        virtual InstructionCommand* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual InstructionCommand* optimize() final override;
        virtual Node* evaluate(Context*) const final override;
    };
}

#endif //HAZE_INSTRUCTION_H
