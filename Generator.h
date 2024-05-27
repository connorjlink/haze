#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include <array>
#include <vector>
#include <string>
#include <utility>

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
		std::vector<std::pair<std::string, std::vector<std::array<std::uint8_t, 3>>>> bytes;
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
			: current_function(-1), bytes({}), program(std::move(program))
		{
		}

	public:
		std::vector<std::pair<std::string, std::vector<std::array<std::uint8_t, 3>>>> generate();
	};

	extern Generator* generator;
}

#endif //HAZE_GENERATOR_H
