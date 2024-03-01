#include "Simulator.h"
#include "Log.h"

#include <chrono>
#include <thread>
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
            case 0b0000:
            {
                //move reg, reg
                register_file[operand1] = register_file[operand2];
            } break;

            case 0b0001:
            {
                //copy reg, imm
                register_file[operand1] = immediate;
            } break;

            case 0b0010:
            {
                //load reg, mem
                register_file[operand1] = ram[memory];
            } break;

            case 0b0011:
            {
                //save mem, reg
                ram[memory] = register_file[operand2];
            } break;

            case 0b0100:
            {
                //iadd reg, reg
                register_file[operand1] += register_file[operand2];
            } break;

            case 0b0101:
            {
                //isub reg, reg
                register_file[operand1] -= register_file[operand2];
            } break;

            case 0b0110:
            {
                //band reg, reg
                register_file[operand1] &= register_file[operand2];
            } break;

            case 0b0111:
            {
                //bior reg, reg
                register_file[operand1] |= register_file[operand2];
            } break;

            case 0b1000:
            {
                //bxor reg, reg
                register_file[operand1] ^= register_file[operand2];
            } break;

            case 0b1001:
            {
                //bnot reg
                register_file[operand1] = ~register_file[operand1];
            } break;

            case 0b1010:
            {
                //call mem
                ram[STACK_TOP - stack_pointer] = (instruction_pointer & 0xFF00) >> 8;
                stack_pointer++;
                ram[STACK_TOP - stack_pointer] = (instruction_pointer & 0x00FF) >> 0;
                stack_pointer++;
                instruction_pointer = memory;
            } break;

            case 0b1011:
            {
                //exit
                stack_pointer--;
                const auto lower = ram[STACK_TOP - stack_pointer];
                stack_pointer--;
                const auto upper = ram[STACK_TOP - stack_pointer];
                instruction_pointer = (upper << 8) | lower;
            } break;

            case 0b1100:
            {
                //push reg
                ram[STACK_TOP - stack_pointer] = register_file[operand2];
                stack_pointer++;
            } break;

            case 0b1101:
            {
                //pull reg
                stack_pointer--;
                register_file[operand1] = ram[STACK_TOP - stack_pointer];
            } break;

            case 0b1110:
            {
                //brez mem, reg
                if (register_file[operand2] == 0)
                {
                    instruction_pointer = memory;
                }
            } break;

            case 0b1111:
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
        Log::info("Simulation starting...");
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(850ms);

        Log::info("Press enter to step forward by 1 instruction:");

        do
        {
            Log::info(fmt::format("registers=[{}] ip=${:X} sp={:0X} ({}) committed={{opcode:{:04B} op1:{:02B} op2:{:02B} imm:(${:02X}) mem:${:04X}}}", fmt::join(register_file, ", "), instruction_pointer, stack_pointer, STACK_TOP - stack_pointer, opcode, operand1, operand2, immediate, memory));
            std::cin.get();
        } while (step());
    }
}