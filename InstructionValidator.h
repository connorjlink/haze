#ifndef HAZE_INSTRUCTIONVALIDATOR_H
#define HAZE_INSTRUCTIONVALIDATOR_H

#include "Instruction.h"
#include "Validator.h"

namespace hz
{
    class InstructionValidator : public Validator
    {
    private:
        static bool validate_constructor();
        static bool validate_bytes();

    public:
        InstructionValidator();
    };
}

#endif //HAZE_INSTRUCTIONVALIDATOR_H
