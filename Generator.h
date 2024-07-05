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
		void make_move(Register, Register);
		void make_load(Register, std::uint16_t);
		void make_copy(Register, std::uint8_t);
		void make_save(std::uint16_t, Register);
		void make_iadd(Register, Register);
		void make_isub(Register, Register);
		void make_band(Register, Register);
		void make_bior(Register, Register);
		void make_bxor(Register, Register);
		void make_call(std::string);
		void make_call(std::uint16_t);
		void make_exit();
		void make_push(Register);
		void make_pull(Register);
		void make_brnz(std::string, Register);
		void make_brnz(std::uint16_t, Register);
		void make_bool(Register);

	public:
		void make_raw(InstructionCommand*);

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
