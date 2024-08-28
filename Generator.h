#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include "InstructionCommand.h"
#include "Linkable.h"
#include "ErrorReporter.h"

#include <string>
#include <array>
#include <vector>
#include <cstdint>

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


		// NOTE: fake stacks grow upwards
		// pointers always point to the about to be written by the next PUSH
	private:
		// for function calls
		// each register has its own custom "stack"
		// function return values also have their own stack
		std::array<std::uint16_t, 7> fake_stack = { 0x0000, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000 };

	public:
		// for function calls
		void fake_push(Register, Register);
		std::uint16_t fake_pull(Register, Register);

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
		void make_stop();

	public:
		void make_raw(InstructionCommand*);

	public:
		std::uint16_t write_pointer() const;

	public:
		void image(std::vector<InstructionCommand*>&&, std::uint16_t);

	public:
		Generator(std::vector<Node*>&& program, const std::string& filepath)
			: program{ std::move(program) }, linkables{}, current_function{ -1 }
		{
			_error_reporter->open_context(filepath, "generating");
		}

		~Generator()
		{
			_error_reporter->close_context();
		}

	public:
		std::vector<Linkable> generate();
	};

	extern Generator* _generator;
}

#endif //HAZE_GENERATOR_H
