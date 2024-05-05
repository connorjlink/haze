#include "Simulator.h"
#include "Instruction.h"
#include "Disassembler.h"
#include "Log.h"

#include <iostream>

namespace hz
{
    bool Simulator::step()
    {
        instruction_pointer &= 0xBFFF;

        const auto fetch1 = rom[instruction_pointer - HALF_DWORD_MAX + 0];
        const auto fetch2 = rom[instruction_pointer - HALF_DWORD_MAX + 1];
        const auto fetch3 = rom[instruction_pointer - HALF_DWORD_MAX + 2];

        opcode = (fetch1 & 0xF0) >> 4;
        operand1 = (fetch1 & 0x0F) >> 2;
        operand2 = (fetch1 & 0x0F) >> 0;

        immediate = fetch2;
        memory = (fetch2 << 8) | fetch3;


        switch (opcode & 0x0F)
        {
            case MOVE:
            {
                //move reg, reg
                register_file[operand1] = register_file[operand2];
            } break;

            case LOAD:
            {
                //load reg, mem
                register_file[operand1] = ram[memory];
            } break;

            case COPY:
            {
                //copy reg, imm
                register_file[operand1] = immediate;
            } break;

            case SAVE:
            {
                //save mem, reg
                ram[memory] = register_file[operand2];
            } break;

            case IADD:
            {
                //iadd reg, reg
                register_file[operand1] += register_file[operand2];
            } break;

            case ISUB:
            {
                //isub reg, reg
                register_file[operand1] -= register_file[operand2];
            } break;

            case BAND:
            {
                //band reg, reg
                register_file[operand1] &= register_file[operand2];
            } break;

            case BIOR:
            {
                //bior reg, reg
                register_file[operand1] |= register_file[operand2];
            } break;

            case BXOR:
            {
                //bxor reg, reg
                register_file[operand1] ^= register_file[operand2];
            } break;

            case BNOT:
            {
                //bnot reg
                register_file[operand1] = ~register_file[operand1];
            } break;

            case CALL:
            {
                //call mem
                ram[STACK_TOP - stack_pointer] = (instruction_pointer & 0xFF00) >> 8;
                stack_pointer++;
                ram[STACK_TOP - stack_pointer] = (instruction_pointer & 0x00FF) >> 0;
                stack_pointer++;
                instruction_pointer = memory;
            } break;

            case EXIT:
            {
                //exit
                stack_pointer--;
                const auto lower = ram[STACK_TOP - stack_pointer];
                stack_pointer--;
                const auto upper = ram[STACK_TOP - stack_pointer];
                instruction_pointer = (upper << 8) | lower;
            } break;

            case PUSH:
            {
                //push reg
                ram[STACK_TOP - stack_pointer] = register_file[operand2];
                stack_pointer++;
            } break;

            case PULL:
            {
                //pull reg
                stack_pointer--;
                register_file[operand1] = ram[STACK_TOP - stack_pointer];
            } break;

            case BREZ:
            {
                //brez mem, reg
                if (register_file[operand2] == 0)
                {
                    instruction_pointer = memory;
                }
            } break;

            case RSVD:
            {
                //XXXX
                //TODO: use as instruction set extension prefix?
                return false;
            } break;
        }

        instruction_pointer += 3;
        return true;
    }

    void Simulator::reset()
    {
        register_file = { 0 };
        instruction_pointer = HALF_DWORD_MAX;
        stack_pointer = 0;

        ram = { 0 };
        dsp = { 0 };
    }

    void Simulator::run()
    {
        Log::info("Simulation loading...");
        Log::info("Simulation ready!");

        Log::info("Press 'return' to start the simulation:");

        std::cin.get();

        Log::info("Simulation starting...");
        Log::info("Input 's' to step forward one instruction or 'd' to display the disassembly");

        for (char option = '\0'; ; std::cin >> option)
        {
            switch (option)
            {
                case 'd':
                {
                    Log::info("Instruction listing:");
                    const auto offset = instruction_pointer - HALF_DWORD_MAX;
                    Log::raw(Disassembler::disassemble_program(rom, offset - std::min(3, offset), rom.size()));
                } break;

                case 's':
                {
                    step();

                    Log::info(fmt::format("CPU State: registers=[{}] ip=${:X} sp={:0X} ({}) committed={{opcode:{:04B} op1:{:02B} op2:{:02B} imm:(${:02X}) mem:${:04X}}}",
                        fmt::join(register_file, ", "), instruction_pointer, stack_pointer, STACK_TOP - stack_pointer, opcode, operand1, operand2, immediate, memory));
                } break;

                case 'q':
                {
                    Log::info("Simulation ending...");
                    std::exit(EXIT_SUCCESS);
                } break;

                default:
                {
                    Log::info("Unrecognized simulation command");
                } break;
            }
        }
    }
}
