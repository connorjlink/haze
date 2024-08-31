#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include "Linkable.h"
#include "ErrorReporter.h"

#include <string>
#include <vector>
#include <cstdint>

// Haze Generator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Parser;
	class InstructionCommand;

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
		void make_load(Register, std::uint32_t);
		void make_copy(Register, std::uint8_t);
		void make_save(std::uint32_t, Register);
		void make_iadd(Register, Register);
		void make_isub(Register, Register);
		void make_band(Register, Register);
		void make_bior(Register, Register);
		void make_bxor(Register, Register);
		void make_call(std::string);
		void make_call(std::uint32_t);
		void make_exit();
		void make_push(Register);
		void make_pull(Register);
		void make_brnz(std::string, Register);
		void make_brnz(std::uint32_t, Register);
		void make_bool(Register);
		void make_stop();

	public:
		void make_raw(InstructionCommand*);

	public:
		std::uint32_t write_pointer() const;

	public:
		void image(std::vector<InstructionCommand*>&&, std::uint32_t);

	public:
		Generator(std::vector<Node*>&&, const std::string&);
		~Generator();

	public:
		std::vector<Linkable> generate();
	};

	extern Generator* _generator;
}

#endif
