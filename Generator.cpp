#include "Generator.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Symbol.h"
#include "IdentifierExpression.h"
#include "Linkable.h"
#include "LabelCommand.h"
#include "ErrorReporter.h"

#include <format>
#include <algorithm>
#include <cstdlib>

// Haze Generator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Generator::Generator(std::vector<Node*>&& program, const std::string& filepath)
		: program{ std::move(program) }, linkables{}, current_function{ -1 }
	{
		_error_reporter->open_context(filepath, "generating");
	}

	Generator::~Generator()
	{
		_error_reporter->close_context();
	}


	void Generator::begin_function(std::string name)
	{
		current_function++;

		const auto linkable = Linkable{ _parser->reference_symbol(SymbolType::FUNCTION, name, NULL_TOKEN), {}, 0 };
		linkables.emplace_back(linkable);
	}

	void Generator::label(const std::string& identifier)
	{
		linkables[current_function].commands.emplace_back(new LabelCommand{ identifier, NULL_TOKEN });
	}

#define ENCODE(x) linkables[current_function].commands.emplace_back(x)
	void Generator::make_move(Register destination, Register source)
	{
		//move destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, MOVE, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_load(Register destination, std::uint32_t address)
	{
		//load destination, &address
		auto instruction = new InstructionCommand{ NULL_TOKEN, LOAD, destination, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_copy(Register destination, std::uint8_t immediate)
	{
		//copy destination, #immediate
		auto instruction = new InstructionCommand{ NULL_TOKEN, COPY, destination, DC, immediate, 0 };
		ENCODE(instruction);
	}

	void Generator::make_save(std::uint32_t address, Register source)
	{
		//save &address, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, SAVE, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_iadd(Register destination, Register source)
	{
		//iadd destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, IADD, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_isub(Register destination, Register source)
	{
		//isub destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, ISUB, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_band(Register destination, Register source)
	{
		//band destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, BAND, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_bior(Register destination, Register source)
	{
		//bior destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, BIOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_bxor(Register destination, Register source)
	{
		//bxor destination, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, BXOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_call(std::string name)
	{
		//call label
		auto instruction = new InstructionCommand{ NULL_TOKEN, CALL, DC, DC, 0, 0xCCCCCCCC, name };
		ENCODE(instruction);
	}

	void Generator::make_call(std::uint32_t address)
	{
		//call &address
		auto instruction = new InstructionCommand{ NULL_TOKEN, CALL, DC, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_exit()
	{
		//exit
		auto instruction = new InstructionCommand{ NULL_TOKEN, EXIT, DC, DC };
		ENCODE(instruction);
	}

	void Generator::make_push(Register source)
	{
		//push source
		auto instruction = new InstructionCommand{ NULL_TOKEN, PUSH, DC, source };
		ENCODE(instruction);
	}

	void Generator::make_pull(Register destination)
	{
		//pull destination
		auto instruction = new InstructionCommand{ NULL_TOKEN, PULL, destination, DC };
		ENCODE(instruction);
	}

	void Generator::make_brnz(std::string name, Register source)
	{
		// brez label, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, BRNZ, DC, source, 0, 0xCCCCCCCC, name };
		ENCODE(instruction);
	}

	void Generator::make_brnz(std::uint32_t address, Register source)
	{
		//brez &address, source
		auto instruction = new InstructionCommand{ NULL_TOKEN, BRNZ, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_bool(Register source)
	{
		//bool source
		auto instruction = new InstructionCommand{ NULL_TOKEN, BOOL, DC, source };
		ENCODE(instruction);
	}
	
	void Generator::make_stop()
	{
		auto instruction = new InstructionCommand{ NULL_TOKEN, STOP, DC, DC };
		ENCODE(instruction);
	}


	void Generator::make_raw(InstructionCommand* instruction)
	{
		ENCODE(instruction);
	}
#undef ENCODE

	std::uint32_t Generator::write_pointer() const
	{
		return static_cast<std::uint32_t>(linkables[current_function].commands.size());
	}

	void Generator::image(std::vector<InstructionCommand*>&& object_code, std::uint32_t approximate_size)
	{
		auto dummy_command = new InstructionCommand{ 0, NULL_TOKEN };
		dummy_command->embedded_object_code = std::move(object_code);
		dummy_command->approximate_embedded_size = approximate_size;

		linkables[current_function].commands.emplace_back(dummy_command);
	}

	std::vector<Linkable> Generator::generate()
	{
		for (auto function : program)
		{
			//TODO: this allocation scheme does not work
			function->generate();
		}

		//Reorder the functions so `main` is first since it's our entrypoint.
		std::ranges::partition(linkables, [](auto& linkable)
		{
			if (linkable.symbol->name == "main")
			{
				// put a stop instruction at the end of main
				*(linkable.commands.end() - 1) = (new InstructionCommand{ NULL_TOKEN, STOP, DC, DC });
				return true;
			}

			return false;
		});

		return linkables;	
	}
}
