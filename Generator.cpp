#include "Generator.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Symbol.h"
#include "Utility.h"
#include "IdentifierExpression.h"
#include "Linkable.h"

#include <format>
#include <algorithm>
#include <cstdlib>

namespace hz
{
	void Generator::begin_function(std::string name)
	{
		current_function++;
		linkables.emplace_back(parser->reference_symbol(Symbol::Type::FUNCTION, name), std::vector<IndexedInstruction>{}, 0);
	}

#define ENCODE(x) linkables[current_function].object_code.emplace_back(linkables[current_function].object_code.size(), x)
	void Generator::move(Register destination, Register source)
	{
		//move destination, source
		auto instruction = new Instruction{ MOVE, destination, source };
		ENCODE(instruction);
	}

	void Generator::load(Register destination, std::uint16_t address)
	{
		//load destination, &address
		auto instruction = new Instruction{ LOAD, destination, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::copy(Register destination, std::uint8_t immediate)
	{
		//copy destination, #immediate
		auto instruction = new Instruction{ COPY, destination, DC, immediate, 0 };
		ENCODE(instruction);
	}

	void Generator::save(std::uint16_t address, Register source)
	{
		//save &address, source
		auto instruction = new Instruction{ SAVE, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::iadd(Register destination, Register source)
	{
		//iadd destination, source
		auto instruction = new Instruction{ IADD, destination, source };
		ENCODE(instruction);
	}

	void Generator::isub(Register destination, Register source)
	{
		//isub destination, source
		auto instruction = new Instruction{ ISUB, destination, source };
		ENCODE(instruction);
	}

	void Generator::band(Register destination, Register source)
	{
		//band destination, source
		auto instruction = new Instruction{ BAND, destination, source };
		ENCODE(instruction);
	}

	void Generator::bior(Register destination, Register source)
	{
		//bior destination, source
		auto instruction = new Instruction{ BIOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::bxor(Register destination, Register source)
	{
		//bxor destination, source
		auto instruction = new Instruction{ BXOR, destination, source };
		ENCODE(instruction);
	}

	void Generator::call(std::string name)
	{
		//call label
		auto instruction = new Instruction{ CALL, DC, DC, 0, 0xCCCC, name };
		ENCODE(instruction);
	}

	void Generator::call(std::uint16_t address)
	{
		//call &address
		auto instruction = new Instruction{ CALL, DC, DC, 0, address };
		ENCODE(instruction);
	}

	void Generator::exit()
	{
		//exit
		auto instruction = new Instruction{ EXIT, DC, DC };
		ENCODE(instruction);
	}

	void Generator::push(Register source)
	{
		//push source
		auto instruction = new Instruction{ PUSH, DC, source };
		ENCODE(instruction);
	}

	void Generator::pull(Register destination)
	{
		//pull destination
		auto instruction = new Instruction{ PULL, destination, DC };
		ENCODE(instruction);
	}

	void Generator::brez(std::uint16_t address, Register source)
	{
		//brez &address, source
		auto instruction = new Instruction{ BREZ, DC, source, 0, address };
		ENCODE(instruction);
	}

	void Generator::raw(Instruction* instruction)
	{
		ENCODE(instruction);
	}
#undef ENCODE

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
