#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include "InstructionCommand.h"
#include "Linkable.h"

#include <vector>
#include <string>

namespace hz
{
	class Parser;

	class Generator
	{
	private:
		//Imported from the parser in the constructor
		std::vector<Node*> program;

	private:
		std::vector<Linkable> linkables;
		std::int32_t current_function;

	public:
		void begin_function(std::string);

	public:
		void label(const std::string&);

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
		void call(std::string);
		void call(std::uint16_t);
		void exit();
		void push(Register);
		void pull(Register);
		void brez(std::string, Register);
		void brez(std::uint16_t, Register);

	public:
		void raw(InstructionCommand*);

	public:
		std::uint16_t write_pointer() const;

	public:
		void image(std::vector<std::uint8_t>&&, std::uint16_t);

	public:
		explicit Generator(std::vector<Node*>&& program)
			: program{ std::move(program) }, linkables{}, current_function{ -1 }
		{
		}

	public:
		std::vector<Linkable> generate();
	};

	extern Generator* generator;
}

#endif //HAZE_GENERATOR_H
