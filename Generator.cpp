import std;

#include "Generator.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Symbol.h"
#include "Linkable.h"
#include "LabelCommand.h"
#include "IntermediateCommand.h"
#include "RuntimeAllocator.h"
#include "ErrorReporter.h"

// Haze Generator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// GLOBALS
	Generator* _generator;
	// GLOBALS


	Generator::Generator(std::vector<Node*>&& program, const std::string& filepath)
		: _program{ std::move(program) }, _current_function{ -1 }
	{
		_string_length_map = {};
		_error_reporter->open_context(filepath, "generating");
	}

	Generator::~Generator()
	{
		_error_reporter->close_context();
	}


	void Generator::begin_function(std::string name)
	{
		_current_function++;

		const auto linkable = Linkable{ _parser->reference_symbol(SymbolType::FUNCTION, name, NULL_TOKEN), {}, {}, 0 };
		_linkables.emplace_back(linkable);
	}

	void Generator::label(const std::string& identifier)
	{
		_linkables[_current_function].commands.emplace_back(new LabelCommand{ identifier, NULL_TOKEN });
	}

	void Generator::register_branch(IntermediateCommand* command, const std::string& label)
	{
#pragma message("TODO branch target registration")
	}

#define ENCODE(x) _linkables[_current_function].commands.emplace_back(x)
#define COMPOSE(x) _linkables[_current_function].ir.emplace_back(x)

	void Generator::begin_scope()
	{
		auto command = new EnterScopeCommand{};
		COMPOSE(command);
	}

	void Generator::end_scope()
	{
		auto command = new LeaveScopeCommand{};
		COMPOSE(command);
	}

	void hz::Generator::write_local(const std::string&name, register_t source)
	{
		_runtime_allocator->define_local(name, source);
	}

	void hz::Generator::write_local(const std::string&name)
	{
		_runtime_allocator->define_local(name);
	}

	void Generator::read_local(register_t destination, const std::string& name)
	{
		_runtime_allocator->read_local(destination, name);
	}

	void Generator::make_global(register_t location, variable_t value)
	{
		auto command = new GlobalVariableCommand{ location, value };
		COMPOSE(command);
	}

	void Generator::memory_read(register_t destination, std::uint32_t pointer)
	{
		auto command = new MemoryReadCommand{ destination, pointer };
		COMPOSE(command);
	}

	void Generator::memory_write(std::uint32_t pointer, register_t source)
	{
		auto command = new MemoryWriteCommand{ pointer, source };
		COMPOSE(command);
	}

	void Generator::compute_add(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new AddCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_subtract(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new SubtractCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitor(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitorCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitand(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitandCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitxor(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitxorCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_increment(register_t destination, register_t source)
	{
		auto command = new IncrementCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::compute_decrement(register_t destination, register_t source)
	{
		auto command = new DecrementCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::make_copy(register_t destination, register_t source)
	{
		auto command = new CopyCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::make_immediate(register_t destination, std::uint32_t source)
	{
		auto command = new MakeImmediateCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::make_argument(register_t location)
	{
		auto command = new MakeArgumentCommand{ location };
		COMPOSE(command);
	}

	void Generator::take_argument(register_t location, std::int32_t offset)
	{
		auto command = new TakeArgumentCommand{ location, offset };
		COMPOSE(command);
	}

	void Generator::call_function(const std::string& function)
	{
		auto command = new CallFunctionCommand{ function };
		COMPOSE(command);
	}

	void Generator::make_return()
	{
		auto command = new VoidReturnCommand{};
		COMPOSE(command);
	}

	void Generator::make_return(register_t value)
	{
		auto command = new ValueReturnCommand{ value };
		COMPOSE(command);
	}

	void Generator::check_ifnz(register_t value, std::int32_t index)
	{
		auto command = new IfNotZeroCommand{ value, index };
		COMPOSE(command);
	}

	void Generator::check_ifnz(register_t value, const std::string& label)
	{
		auto command = new IfNotZeroCommand{ value, TEMP_ADDRESS };
		register_branch(command, label);
		COMPOSE(command);
	}

	void Generator::check_ifz(register_t value, std::int32_t index)
	{
		auto command = new IfZeroCommand{ value, index };
		COMPOSE(command);
	}

	void Generator::check_ifz(register_t value, const std::string& label)
	{
		auto command = new IfZeroCommand{ value, TEMP_ADDRESS };
		register_branch(command, label);
		COMPOSE(command);
	}

	void Generator::goto_command(std::int32_t index)
	{
		auto command = new GotoCommand{ index };
		COMPOSE(command);
	}

	void Generator::goto_command(const std::string& label)
	{
		auto command = new GotoCommand{ TEMP_ADDRESS };
		register_branch(command, label);
		COMPOSE(command);
	}

	void Generator::make_message(std::uint32_t pointer, std::string message)
	{
		// including an extra byte for the implicit NULL terminator
		const auto length = message.length() + 1;

		_string_length_map[pointer] = static_cast<std::uint32_t>(length);

		auto command = new MakeMessageCommand{ pointer, message };
		COMPOSE(command);
	}

	void Generator::print_message(std::uint32_t pointer)
	{
		if (_string_length_map.contains(pointer))
		{
			auto command = new PrintMessageCommand{ pointer, _string_length_map.at(pointer) };
			COMPOSE(command);
			return;
		}

		_error_reporter->post_error(std::format("undefined string pointer `{:08X}`", pointer), NULL_TOKEN);
	}

	void Generator::print_number(register_t value)
	{
		auto command = new PrintNumberCommand{ value };
		COMPOSE(command);
	}

	void Generator::exit_program(register_t code)
	{
		auto command = new ExitProgramCommand{ code };
		COMPOSE(command);
	}


	std::uint32_t Generator::write_pointer() const
	{
		return static_cast<std::uint32_t>(_linkables[_current_function].commands.size());
	}

	void Generator::raw_binary(byterange&& object_code)
	{
		const auto approximate_length = static_cast<std::uint32_t>(object_code.size());

		// emplacing a bitstream is basically the same as compiled inline assembly
		inline_assembly(std::move(object_code), approximate_length);
	}

	void Generator::inline_assembly(byterange&& object_code, std::uint32_t approximate_size)
	{
		auto command = new InlineAssemblyCommand{ std::move(object_code), approximate_size };
		COMPOSE(command);
	}


	std::vector<Linkable> Generator::generate()
	{
		for (auto function : _program)
		{
			function->generate();
		}

		// Reorder defined functions so `main` is first since it's the entrypoint
		std::ranges::partition(_linkables, [](auto& linkable)
		{
			if (linkable.symbol->name == "main")
			{
				// put a stop instruction at the end of main
				*(linkable.commands.end() - 1) = (new InstructionCommand{ NULL_TOKEN, Opcode::STOP, DC, DC });
				return true;
			}

			return false;
		});

		return _linkables;	
	}
}
