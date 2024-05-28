#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include <vector>
#include <string>
#include <utility>

#include "Instruction.h"
#include "Utility.h"

namespace hz
{
	class Parser;

	class Generator
	{
	private:
		//Imported from the parser in the constructor
		std::vector<Node*> program;

	private:
		std::vector<Linkable> object_code;
		std::int32_t current_function;

	public:
		void begin_function(std::string);

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
			: program(std::move(program)), object_code(), current_function(-1)
		{
		}

	public:
		std::vector<Linkable> generate();
	};

	extern Generator* generator;
}

#endif //HAZE_GENERATOR_H
