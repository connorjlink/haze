#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include <ast/declaration/Declaration.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/models/Linkable.h>
#include <utility/Constants.h>

// Haze Generator.h
// (c) Connor J. Link. All Rights Reserved.

#define UNSUPPORTED_OPERATION(x) USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format("unsupported operation `{}()`", x), NULL_TOKEN)

namespace hz
{
	class ArgumentExpression;

	FORWARD_DECLARE_SUM(Expression);
	FORWARD_DECLARE_SUM(Value);


	class Generator
		: public ServiceTag<Generator>
		, public InjectSingleton<ErrorReporter, SymbolDatabase>
	{
	private:
		// imported upon reload()
		std::vector<DeclarationHandle> program;

	private:
		std::vector<Linkable> linkables;

	private:
		std::unordered_map<Address, Offset> string_length_map;

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
		// push a new variable public to the entire program
		void make_global(Register, Variable*);

	public:
		// destination = memory[pointer]
		void heap_read(Register, Address);
		// memory[pointer] = source
		void heap_write(Address, Register);

	public:
		// destination = stack[ebp + offset]
		void stack_read(Register, Offset);
		// stack[ebp + offset] = source
		void stack_write(Offset, Register);

	public:
		// NOTE: all math operations are { left, right, destination }
		// destination = left + right
		void compute_add(Register, Register, Register);
		// destination = left - right
		void compute_subtract(Register, Register, Register);
		// destination = left * right
		void compute_multiplication(Register, Register, Register);
		// destination = left | right
		void compute_bitor(Register, Register, Register);
		// destination = left ^ right
		void compute_bitxor(Register, Register, Register);
		// destination = left & right
		void compute_bitand(Register, Register, Register);
		// destination = left << right
		void compute_bitlshift(Register, Register, Register);
		// destination = left >> right
		void compute_bitrshift(Register, Register, Register);

	public:
		// destination = left == right
		void compute_equality(Register, Register, Register);
		// destination = left != right
		void compute_inequality(Register, Register, Register);
		// destination = left < right
		void compute_less(Register, Register, Register);
		// destination = left > right
		void compute_greater(Register, Register, Register);

	private:
		// destination = (bool)source
		void compute_bool(Register, Register);

	public:
		// destination = source + 1
		void compute_increment(Register, Register);
		// destination = source - 1
		void compute_decrement(Register, Register);

	public:
		// destination = source
		void make_copy(Register, Register);
		// destination = immediate
		void make_immediate(BigInteger);

	public:
		// position a new function argument for the next call
		void make_argument(const std::string&, Register);
		// pull a function argument during call from those prepared
		void take_argument(const std::string&, Register, Offset);
		// link execution to a user-defined function
		void call_function(const std::string&);
		// link execution to a user-defined function
		void call_function(const std::string&, const std::vector<ExpressionReference<ArgumentExpression>>&, ValueHandle);
		// return from a call to a `nvr` function
		void make_return(const std::string&);
		// return from a call to a `nvr` function
		void make_return(Offset);
		// return from a call to a value-typed function
		void make_return(const std::string&, Register);
		// return from a call to a value-typed function
		void make_return(Offset, Register);

	public:
		// { target_offset, condition }
		void check_ifnz(Offset, Register);
		// { label, condition }
		void check_ifnz(const std::string&, Register);

	public:
		// { target_offset, condition }
		void check_ifz(Offset, Register);
		// { label, condition }
		void check_ifz(const std::string&, Register);

	public:
		// target_offset
		void goto_command(Offset);
		// label to which to jump
		void goto_command(const std::string&);

	public:
		// memory[pointer] = message
		void make_message(Address, const std::string&);
		// print(pointer)
		void print_message(Address);

	public:
		// print(integer)
		void print_number(Register);

	public:
		// exit(code)
		void exit_program(Register);

	public:
		// { assembly code }
		void raw_binary(const ByteRange&);

	public:
		// { assembly code, bytecount }
		void inline_assembly(const ByteRange&, Address);

	public:
		// get the current index for IntermediateCommand generation
		Address resolve_origin() const;

	public:
		// ast, filepath
		void reload(std::vector<DeclarationHandle>, const std::string&);

	public:
		Generator(const std::string&);
		~Generator();

	public:
		std::vector<Linkable> generate();
	};
}

#endif
