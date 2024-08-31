#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include "Command.h"
#include "InstructionEncoding.h"

#include <cstdint>

// Haze InstructionCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class InstructionCommand : public Command
    {
    public:
        Opcode opcode;
        Register dst, src;
        std::uint8_t imm;
        std::uint32_t mem;
        bool marked_for_deletion;
        std::string branch_target;

    public:
        std::vector<InstructionCommand*> embedded_object_code;
        std::uint32_t approximate_embedded_size;

    public:
        InstructionCommand() = delete;
        explicit InstructionCommand(std::uint32_t, Token);
        InstructionCommand(Token, Opcode, Register, Register, std::uint8_t = 0, std::uint32_t = 0, std::string = "");
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
