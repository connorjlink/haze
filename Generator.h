#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include "Linkable.h"
#include "InstructionEncoding.h"
#include "Scope.h"
#include "IntermediateCommand.h"
#include "ErrorReporter.h"
#include "DependencyInjector.h"

// Haze Generator.h
// (c) Connor J. Link. All Rights Reserved.

#define UNSUPPORTED_OPERATION(x) USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format("unsupported operation `{}()`", x), NULL_TOKEN)

namespace hz
{
	class Node;
	class Parser;
	class InstructionCommand;
	class Allocation;
	class Variable;

	class Generator
		: public ServiceTag<Generator>
		, public InjectSingleton<ErrorReporter>
	{
	private:
		// Imported from the parser in the constructor
		const std::vector<Node*>& _program;

	private:
		std::vector<Linkable> _linkables;

	private:
		std::unordered_map<std::uint32_t, std::uint32_t> _string_length_map;

	public:
		const std::string& current_function() const;

	public:
		void begin_function(const std::string&);

	public:
		// NOTE: old method
		void label_command(const std::string&);

	public:
		void branch_label(const std::string&);

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
		// resolve a local by attaching a name to an existing stack allocation
		void attach_local(const std::string&, std::int32_t);
		// delete a previously defined local variable from the current scope
		void destroy_local(const std::string&);
		// read a defined local variable into a target register
		void read_local(register_t, const std::string&);
		// update a local variable with the contents of another register (re-assignment)
		void write_local(const std::string&, register_t);


	public:
		// push a new variable public to the entire program
		void make_global(register_t, Variable*);

	public:
		// destination = memory[pointer]
		void heap_read(register_t, std::uint32_t);
		// memory[pointer] = source
		void heap_write(std::uint32_t, register_t);

	public:
		// destination = stack[ebp + offset]
		void stack_read(register_t, std::int32_t);
		// stack[ebp + offset] = source
		void stack_write(std::int32_t, register_t);

	public:
		// NOTE: all math operations are { LHS, RHS, destination }
		// destination = lhs + rhs
		void compute_add(register_t, register_t, register_t);
		// destination = lhs - rhs
		void compute_subtract(register_t, register_t, register_t);
		// destination = lhs * rhs
		void compute_multiplication(register_t, register_t, register_t);
		// destination = lhs | rhs
		void compute_bitor(register_t, register_t, register_t);
		// destination = lhs ^ rhs
		void compute_bitxor(register_t, register_t, register_t);
		// destination = lhs & rhs
		void compute_bitand(register_t, register_t, register_t);
		// destination = lhs << rhs
		void compute_bitlshift(register_t, register_t, register_t);
		// destination = lhs >> rhs
		void compute_bitrshift(register_t, register_t, register_t);

	public:
		// destination = lhs == rhs
		void compute_equality(register_t, register_t, register_t);
		// destination = lhs != rhs
		void compute_inequality(register_t, register_t, register_t);
		// destination = lhs < rhs
		void compute_less(register_t, register_t, register_t);
		// destination = lhs > rhs
		void compute_greater(register_t, register_t, register_t);

	private:
		// destination = (bool)source
		void compute_bool(register_t, register_t);

	public:
		// destination = source + 1
		void compute_increment(register_t, register_t);
		// destination = source - 1
		void compute_decrement(register_t, register_t);

	public:
		// destination = source
		void make_copy(register_t, register_t);
		// destination = immediate
		void make_immediate(register_t, IntegerLiteral*);

	public:
		// position a new function argument for the next call
		void make_argument(const std::string&, register_t);
		// pull a function argument during call from those prepared
		void take_argument(const std::string&, register_t, std::int32_t);
		// link execution to a user-defined function
		void call_function(const std::string&);
		// link execution to a user-defined function
		void call_function(const std::string&, const std::vector<Expression*>&, Allocation*);
		// return from a call to a `nvr` function
		void make_return(const std::string&);
		// return from a call to a `nvr` function
		void make_return(std::int32_t);
		// return from a call to a value-typed function
		void make_return(const std::string&, register_t);
		// return from a call to a value-typed function
		void make_return(std::int32_t, register_t);

	public:
		// { target_offset, condition }
		void check_ifnz(std::int32_t, register_t);
		// { label, condition }
		void check_ifnz(const std::string&, register_t);

	public:
		// { target_offset, condition }
		void check_ifz(std::int32_t, register_t);
		// { label, condition }
		void check_ifz(const std::string&, register_t);

	public:
		// target_offset
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
		std::int32_t resolve_origin_old() const;
		// get the current index for IntermediateCommand generation
		std::int32_t resolve_origin() const;

	public:
		// ast, filepath
		Generator(const std::vector<Node*>&, const std::string&);
		~Generator();

	public:
		std::vector<Linkable> generate();
	};
}

#endif
