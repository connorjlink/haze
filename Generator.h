#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include "Linkable.h"
#include "InstructionEncoding.h"
#include "Scope.h"
#include "IntermediateCommand.h"

// Haze Generator.h
// (c) Connor J. Link. All Rights Reserved.

#define UNSUPPORTED_OPERATION(x) _error_reporter->post_uncorrectable(std::format("unsupported operation `{}()`", x), NULL_TOKEN)

namespace hz
{
	class Node;
	class Parser;
	class InstructionCommand;
	class Allocation;

	using index_t = std::int32_t;

	class Generator
	{
	private:
		//Imported from the parser in the constructor
		std::vector<Node*> _program;

	private:
		std::vector<Linkable> _linkables;
		std::int32_t _current_function;

	private:
		std::unordered_map<std::uint32_t, std::uint32_t> _string_length_map;

	public:
		void begin_function(std::string);

	public:
		void label(const std::string&);

	public:
		// associate a command object with its branch target label
		void register_branch(IntermediateCommand*, const std::string&);
		// discover the IntermediateCommand index of a particular label
		index_t query_branch_target(const std::string&);
		// set the real binary code offset of a particular label
		void resolve_branch_target_real(const std::string&, std::uint32_t);

	public:
		// push a new empty scope with a unique local variable set
		// also used to declare a function prologue
		void begin_scope();
		// pop the most recent scope and clean up all local variables' storage
		// also used to declare a function epilogue
		void end_scope();

	public:
		// push a new variable private to the current scope
		void define_local(const std::string&, register_t);
		// push a new undefined variable private to the current scope
		void define_local(const std::string&);
		// delete a previously defined local variable from the current scope
		void destroy_local(const std::string&);
		// read a defined local variable into a target register
		void read_local(register_t, const std::string&);

	public:
		// push a new variable public to the entire program
		void make_global(register_t, variable_t);

	public:
		// destination = memory[pointer]
		void memory_read(register_t, std::uint32_t);
		// memory[pointer] = source
		void memory_write(std::uint32_t, register_t);

	public:
		// NOTE: all math operations are { LHS, RHS, destination }
		// destination = lhs + rhs
		void compute_add(register_t, register_t, register_t);
		// destination = lhs - rhs
		void compute_subtract(register_t, register_t, register_t);
		// destination = lhs | rhs
		void compute_bitor(register_t, register_t, register_t);
		// destination = lhs & rhs
		void compute_bitand(register_t, register_t, register_t);
		// destination = lhs ^ rhs
		void compute_bitxor(register_t, register_t, register_t);

	public:
		// destination = source + 1
		void compute_increment(register_t, register_t);
		// destination = source - 1
		void compute_decrement(register_t, register_t);

	public:
		// destination = source
		void make_copy(register_t, register_t);
		// destination = immediate
		void make_immediate(register_t, std::uint32_t);

	public:
		// position a new function argument for the next call
		void make_argument(const std::string&, register_t);
		// pull a function argument during call from those prepared
		void take_argument(const std::string&, register_t, std::int32_t);
		// link execution to a user-defined function
		void call_function(const std::string&);
		// link execution to a user-defined function
		void call_function(const std::string&, const arguments_t&, Allocation*);
		// return from a call to a `nvr` function
		void make_return();
		// return from a call to a value-typed function
		void make_return(register_t);

	public:
		// { condition, index to which to jump }
		void check_ifnz(register_t, std::int32_t);
		// { condition, label to which to jump }
		void check_ifnz(register_t, const std::string&);

	public:
		// { condition, index to which to jump }
		void check_ifz(register_t, std::int32_t);
		// { condition, label to which to jump }
		void check_ifz(register_t, const std::string&);

	public:
		// index to which to jump
		void goto_command(std::int32_t);
		// label to which to jump
		void goto_command(const std::string&);

	public:
		// memory[pointer] = message
		void make_message(std::uint32_t, std::string);
		// print(pointer)
		void print_message(std::uint32_t);

	public:
		// print(integer)
		void print_number(register_t);

	public:
		// exit(code)
		void exit_program(register_t);

	public:
		// { assembly code }
		void raw_binary(byterange&&);

	public:
		// { assembly code, bytecount }
		void inline_assembly(byterange&&, std::uint32_t);


	public:
		// get the current index for Command generation
		index_t resolve_origin_old() const;
		// get the current index for IntermediateCommand generation
		index_t resolve_origin() const;

	public:


	public:
		// ast, filepath
		Generator(std::vector<Node*>&&, const std::string&);
		~Generator();

	public:
		std::vector<Linkable> generate();
	};

	extern Generator* _generator;
}

#endif
