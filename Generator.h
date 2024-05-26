#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include <vector>
#include <string>

#include "Expression.h"
#include "Statement.h"
#include "Function.h"
#include "Symbol.h"
#include "Instruction.h"

namespace hz
{
	class Parser;

	class Generator
	{
	private:
		//Imported from the parser in the constructor
		std::vector<Node*> program;

	private:
		std::vector<std::uint8_t> bytes;

	public:
		void move(Register, Register);
		void load(Register, std::uint16_t);
		void copy(Register, std::uint8_t);
		void save(std::uint16_t, Register);
		void iadd(Register, Register);
		void isub(Register, Register);
		void band(Register, Register);
		void bior(Register, Register);
		void bxor(Register, Register);
		void call(std::uint16_t);
		void exit();
		void push(Register);
		void pull(Register);
		void brez(std::uint16_t, Register);

	public:
		explicit Generator(std::vector<Node*>&& program)
			: program(std::move(program))
		{
		}

	public:
		std::vector<std::uint8_t> generate();
	};

	extern Generator* generator;
}

#endif //HAZE_GENERATOR_H
