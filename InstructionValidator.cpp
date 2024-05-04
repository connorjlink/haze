#include "InstructionValidator.h"
#include "Log.h"

namespace hz
{
    bool InstructionValidator::validate_constructor()
    {

    }

    bool InstructionValidator::validate_bytes()
    {

    }

    InstructionValidator::InstructionValidator()
    {
        add_test(Test{ "Instruction()", std::bind(&validate_constructor) });
        add_test(Test{ ".bytes()", std::bind(&validate_bytes) });

        auto tests = test_count();


        hz::Log



        un_rests();

    }
}