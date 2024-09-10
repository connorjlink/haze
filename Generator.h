#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include "Linkable.h"
#include "InstructionEncoding.h"
#include "Scope.h"
#include "IntermediateCommand.h"

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
		std::vector<Node*> _program;

	private:
		std::vector<Linkable> _linkables;
		std::int32_t _current_function;

	public:
		void begin_function(std::string);

	public:
		void label(const std::string&);

	public:
		// push a new empty scope with a unique local variable set
		void begin_scope();
		// pop the most recent scope and clean up all local variables' storage
		void end_scope();

	public:
		// push a new variable private to the current scope
		void make_local(register_t, variable_t);
		// push a new variable public to the entire program
		void make_global(register_t, variable_t);

	public:
		// NOTE: all math operations are { LHS, RHS, destination }
		// destination = lhs + rhs
		void make_add(register_t, register_t, register_t);
		// destination = lhs - rhs
		void make_subtract(register_t, register_t, register_t);
		// destination = lhs | rhs
		void make_bitor(register_t, register_t, register_t);
		// destination = lhs & rhs
		void make_bitand(register_t, register_t, register_t);
		// destination = lhs ^ rhs
		void make_bitxor(register_t, register_t, register_t);

	public:
		// destination = source
		void make_copy(register_t, register_t);

	public:
		// position a new function argument for the next call
		void make_argument(register_t, variable_t);
		// pull a function argument during call from those prepared
		void take_argument(register_t);
		// link execution to a user-defined function
		void call_function(std::string);
		// return from a call to a `nvr` function
		void make_return();
		// return from a call to a value-typed function
		void make_return(register_t);

	private:
		// declare a function prologue
		void journal_entry();
		// declare a function epilogue
		void journal_leave();

	public:
		// { condition, if body, else body }
		void make_ifnz(register_t, std::vector<IntermediateCommand*>, std::vector<IntermediateCommand*>);

		void make_move(register_t, register_t);
		void make_load(register_t, std::uint32_t);
		void make_copy(register_t, std::uint8_t);
		void make_save(std::uint32_t, register_t);
		void make_iadd(register_t, register_t);
		void make_isub(register_t, register_t);
		void make_band(register_t, register_t);
		void make_bior(register_t, register_t);
		void make_bxor(register_t, register_t);
		void make_call(std::string);
		void make_call(std::uint32_t);
		void make_exit(register_t);
		void make_push(register_t);
		void make_pull(register_t);
		void make_make(register_t);
		void make_take(register_t);
		void make_brnz(std::string, register_t);
		void make_brnz(std::uint32_t, register_t);
		void make_bool(register_t);
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
