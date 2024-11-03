#ifndef HAZE_INSTRUCTION_H
#define HAZE_INSTRUCTION_H

#include "Command.h"
#include "PlatformVariables.h"
#include "InstructionEncoding.h"

// Haze InstructionCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class InstructionCommand : public Command
    {
    public:
        Opcode opcode;
        register_t destination, source;
        std::uint8_t immediate;
        std::uint32_t address;
        bool marked_for_deletion;
        std::string branch_target;

    public:
        std::vector<InstructionCommand*> embedded_object_code;
        std::uint32_t approximate_embedded_size;

    public:
        InstructionCommand() = delete;
        InstructionCommand(Token, std::uint32_t);
        InstructionCommand(Token, Opcode, register_t, register_t, platform_address_size = 0, platform_address_size = 0, const std::string& = "");
        //std::uint32_t bytes() const;

    public:
        virtual CommandType ctype() const final override;
        virtual InstructionCommand* copy() const final override;
        virtual void generate(Allocation*) final override;
        virtual InstructionCommand* optimize() final override;
        virtual Node* evaluate(Context*) const final override;
    };
}

#endif //HAZE_INSTRUCTION_H
