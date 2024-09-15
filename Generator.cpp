#include "Generator.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Symbol.h"
#include "Linkable.h"
#include "LabelCommand.h"
#include "ErrorReporter.h"
#include "IntermediateCommand.h"

import std;

// Haze Generator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Generator::Generator(std::vector<Node*>&& program, const std::string& filepath)
		: _program{ std::move(program) }, _current_function{ -1 }
	{
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

	void Generator::make_local(register_t location, variable_t value)
	{
		auto command = new LocalVariableCommand{ location, value };
		COMPOSE(command);
	}

	void Generator::make_global(register_t location, variable_t value)
	{
		auto command = new GlobalVariableCommand{ location, value };
		COMPOSE(command);
	}

	void Generator::make_add(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new AddCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::make_subtract(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new SubtractCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::make_bitor(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitorCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::make_bitand(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitandCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::make_bitxor(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitxorCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::make_copy(register_t destination, register_t source)
	{
		auto command = new CopyCommand{ destination, source };
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

	void Generator::journal_entry()
	{
		auto command = new JournalEntryCommand{};
		COMPOSE(command);
	}

	void Generator::journal_leave()
	{
		auto command = new JournalLeaveCommand{};
		COMPOSE(command);
	}

	void Generator::make_ifnz(register_t value, const std::vector<IntermediateCommand*>& code)
	{
		auto command = new IfNotZeroCommand{ value, code };
		COMPOSE(command);
	}



	void Generator::make_move(register_t destination, register_t source)
	{
		//move destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::MOVE, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_load(register_t destination, std::uint32_t address)
	{
		//load destination, &address
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::LOAD, destination, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_copy(register_t destination, std::uint8_t immediate)
	{
		//copy destination, #immediate
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::COPY, destination, DC, immediate, 0 };
		ENCODE(instruction);
	}

	void Generator::make_save(std::uint32_t address, register_t source)
	{
		//save &address, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::SAVE, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_iadd(register_t destination, register_t source)
	{
		//iadd destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::IADD, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_isub(register_t destination, register_t source)
	{
		//isub destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::ISUB, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_band(register_t destination, register_t source)
	{
		//band destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::BAND, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_bior(register_t destination, register_t source)
	{
		//bior destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::BIOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_bxor(register_t destination, register_t source)
	{
		//bxor destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::BXOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_call(std::string name)
	{
		//call label
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::CALL, DC, DC, 0, 0xCCCCCCCC, name };
		ENCODE(instruction);
	}

	void Generator::make_call(std::uint32_t address)
	{
		//call &address
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::CALL, DC, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_exit(register_t destination)
	{
		//exit
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::EXIT, destination, DC };
		ENCODE(instruction);
	}

	void Generator::make_push(register_t source)
	{
		//push source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::PUSH, DC, source };
		ENCODE(instruction);
	}

	void Generator::make_pull(register_t destination)
	{
		// pull destination
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::PULL, destination, DC };
		ENCODE(instruction);
	}

	void Generator::make_make(register_t source)
	{
		// make source 
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::MAKE, DC, source };
		ENCODE(instruction);
	}

	void Generator::make_take(register_t destination)
	{
		// take destination
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::TAKE, destination, DC };
		ENCODE(instruction);
	}

	void Generator::make_brnz(std::string name, register_t source)
	{
		// brez label, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::BRNZ, DC, source, 0, 0xCCCCCCCC, name };
		ENCODE(instruction);
	}

	void Generator::make_brnz(std::uint32_t address, register_t source)
	{
		//brez &address, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::BRNZ, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_bool(register_t source)
	{
		//bool source
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::BOOL, DC, source };
		ENCODE(instruction);
	}
	
	void Generator::make_stop()
	{
		auto instruction = new InstructionCommand{ NULL_TOKEN, Opcode::STOP, DC, DC };
		ENCODE(instruction);
	}


	void Generator::make_raw(InstructionCommand* instruction)
	{
		ENCODE(instruction);
	}
#undef ENCODE

	std::uint32_t Generator::write_pointer() const
	{
		return static_cast<std::uint32_t>(_linkables[_current_function].commands.size());
	}

	void Generator::image(std::vector<InstructionCommand*>&& object_code, std::uint32_t approximate_size)
	{
		// this instruction should not matter since it is being used purely to emplace the imaged blob
		auto dummy_command = new InstructionCommand{ NULL_TOKEN, Opcode::MOVE, DC, DC };
		dummy_command->embedded_object_code = std::move(object_code);
		dummy_command->approximate_embedded_size = approximate_size;

		_linkables[_current_function].commands.emplace_back(dummy_command);
	}

	std::vector<Linkable> Generator::generate()
	{
		for (auto function : _program)
		{
			function->generate();
		}

		// Reorders the functions so `main` is first since it's the entrypoint
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
