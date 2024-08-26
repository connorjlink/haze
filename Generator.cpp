#include "Generator.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Symbol.h"
#include "Utility.h"
#include "IdentifierExpression.h"
#include "Linkable.h"
#include "LabelCommand.h"

#include <format>
#include <algorithm>
#include <cstdlib>

namespace hz
{
	void Generator::fake_push(Register source)
	{
		make_save(fake_stack[source], source);
		fake_stack[source]++;
	}

	void Generator::fake_pull(Register destination)
	{
		fake_stack[destination]--;
		make_load(destination, fake_stack[destination]);
	}

	void Generator::begin_function(std::string name)
	{
		current_function++;

		const auto linkable = Linkable{ _parser->reference_symbol(SymbolType::FUNCTION, name, NULL_TOKEN), {}, 0 };
		linkables.emplace_back(linkable);
	}

	void Generator::label(const std::string& identifier)
	{
		linkables[current_function].object_code.emplace_back(new LabelCommand{ identifier });
	}

#define ENCODE(x) linkables[current_function].object_code.emplace_back(x)
	void Generator::make_move(Register destination, Register source)
	{
		//move destination, source
		auto instruction = new InstructionCommand{ MOVE, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_load(Register destination, std::uint16_t address)
	{
		//load destination, &address
		auto instruction = new InstructionCommand{ LOAD, destination, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_copy(Register destination, std::uint8_t immediate)
	{
		//copy destination, #immediate
		auto instruction = new InstructionCommand{ COPY, destination, DC, immediate, 0 };
		ENCODE(instruction);
	}

	void Generator::make_save(std::uint16_t address, Register source)
	{
		//save &address, source
		auto instruction = new InstructionCommand{ SAVE, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_iadd(Register destination, Register source)
	{
		//iadd destination, source
		auto instruction = new InstructionCommand{ IADD, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_isub(Register destination, Register source)
	{
		//isub destination, source
		auto instruction = new InstructionCommand{ ISUB, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_band(Register destination, Register source)
	{
		//band destination, source
		auto instruction = new InstructionCommand{ BAND, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_bior(Register destination, Register source)
	{
		//bior destination, source
		auto instruction = new InstructionCommand{ BIOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_bxor(Register destination, Register source)
	{
		//bxor destination, source
		auto instruction = new InstructionCommand{ BXOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::make_call(std::string name)
	{
		//call label
		auto instruction = new InstructionCommand{ CALL, DC, DC, 0, 0xCCCC, name };
		ENCODE(instruction);
	}

	void Generator::make_call(std::uint16_t address)
	{
		//call &address
		auto instruction = new InstructionCommand{ CALL, DC, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_exit()
	{
		//exit
		auto instruction = new InstructionCommand{ EXIT, DC, DC };
		ENCODE(instruction);
	}

	void Generator::make_push(Register source)
	{
		//push source
		auto instruction = new InstructionCommand{ PUSH, DC, source };
		ENCODE(instruction);
	}

	void Generator::make_pull(Register destination)
	{
		//pull destination
		auto instruction = new InstructionCommand{ PULL, destination, DC };
		ENCODE(instruction);
	}

	void Generator::make_brnz(std::string name, Register source)
	{
		// brez label, source
		auto instruction = new InstructionCommand{ BRNZ, DC, DC, 0, 0xCCCC, name };
		ENCODE(instruction);
	}

	void Generator::make_brnz(std::uint16_t address, Register source)
	{
		//brez &address, source
		auto instruction = new InstructionCommand{ BRNZ, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::make_bool(Register source)
	{
		//bool source
		auto instruction = new InstructionCommand{ BOOL, DC, source };
		ENCODE(instruction);
	}

	void Generator::make_raw(InstructionCommand* instruction)
	{
		ENCODE(instruction);
	}
#undef ENCODE

	std::uint16_t Generator::write_pointer() const
	{
		return static_cast<std::uint16_t>(linkables[current_function].object_code.size());
	}

	void Generator::image(std::vector<InstructionCommand*>&& object_code, std::uint16_t approximate_size)
	{
		auto dummy_command = new InstructionCommand{ 0 };
		dummy_command->embedded_object_code = std::move(object_code);
		dummy_command->approximate_embedded_size = approximate_size;

		linkables[current_function].object_code.emplace_back(dummy_command);
	}

	std::vector<Linkable> Generator::generate()
	{
		for (auto function : program)
		{
			//TODO: this allocation scheme does not work
			function->generate();
		}

		//Reorder the functions so `main` is first since it's our entrypoint.
		std::ranges::partition(linkables, [](const auto& linkable)
		{
			return linkable.symbol->name == "main";
		});

		return linkables;	
	}
}
