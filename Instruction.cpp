#include "Instruction.h"
#include "Disassembler.h"
#include "Generator.h"
#include "Utility.h"

#include <format>

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

	Instruction::Instruction(Opcode opcode, Register op1, Register op2, std::uint8_t imm, std::uint16_t mem)
		: opcode(opcode), op1(op1), op2(op2), imm(imm), mem(mem)
	{
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


	Node::Type Instruction::ntype() const
	{
		return Node::Type::INSTRUCTION;
	}

	std::string Instruction::string() const
	{
		const auto data = bytes();
		const auto bytes = extract(data);

		return std::format("{} ; #{:02X} {:02X} {:02X}", Disassembler::disassemble_instruction(data), bytes[0], bytes[1], bytes[2]);
	}

	Instruction* Instruction::copy() const
	{
		return new Instruction{ *this };
	}

	void Instruction::generate(Allocation*)
	{
		switch (opcode)
		{
			case MOVE: generator->move(op1, op2); break;
			case LOAD: generator->load(op1, mem); break;
			case COPY: generator->copy(op1, imm); break;
			case SAVE: generator->save(mem, op2); break;
			case IADD: generator->iadd(op1, op2); break;
			case ISUB: generator->isub(op1, op2); break;
			case BAND: generator->band(op1, op2); break;
			case BIOR: generator->bior(op1, op2); break;
			case BXOR: generator->bxor(op1, op2); break;
			case CALL: generator->call(mem);      break;
			case EXIT: generator->exit();         break;
			case PUSH: generator->push(op2);      break;
			case PULL: generator->pull(op1);      break;
			case BREZ: generator->brez(mem, op2); break;
		}
	}

	Instruction* Instruction::optimize()
	{
		return nullptr;
	}

}