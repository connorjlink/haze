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
		linkables.emplace_back(parser->reference_symbol(Symbol::Type::FUNCTION, name), std::vector<Instruction*>{});
	}

#define ENCODE linkables[current_function].object_code.emplace_back
	void Generator::move(Register destination, Register source)
	{
		//move destination, source
		ENCODE(new Instruction{ MOVE, destination, source });
	}

	void Generator::load(Register destination, std::uint16_t address)
	{
		//load destination, &address
		ENCODE(new Instruction{ LOAD, destination, DC, 0, address });
	}

	void Generator::copy(Register destination, std::uint8_t immediate)
	{
		//copy destination, #immediate
		ENCODE(new Instruction{ COPY, destination, DC, immediate, 0 });
	}

	void Generator::save(std::uint16_t address, Register source)
	{
		//save &address, source
		ENCODE(new Instruction{ SAVE, DC, source, 0, address });
	}

	void Generator::iadd(Register destination, Register source)
	{
		//iadd destination, source
		ENCODE(new Instruction{ IADD, destination, source });
	}

	void Generator::isub(Register destination, Register source)
	{
		//isub destination, source
		ENCODE(new Instruction{ ISUB, destination, source });
	}

	void Generator::band(Register destination, Register source)
	{
		//band destination, source
		ENCODE(new Instruction{ BAND, destination, source });
	}

	void Generator::bior(Register destination, Register source)
	{
		//bior destination, source
		ENCODE(new Instruction{ BIOR, destination, source });
	}

	void Generator::bxor(Register destination, Register source)
	{
		//bxor destination, source
		ENCODE(new Instruction{ BXOR, destination, source });
	}

	void Generator::call(std::uint16_t address)
	{
		//call &address
		ENCODE(new Instruction{ CALL, DC, DC, 0, address });
	}

	void Generator::exit()
	{
		//exit
		ENCODE(new Instruction{ EXIT, DC, DC });
	}

	void Generator::push(Register source)
	{
		//push source
		ENCODE(new Instruction{ PUSH, DC, source });
	}

	void Generator::pull(Register destination)
	{
		//pull destination
		ENCODE(new Instruction{ PULL, destination, DC });
	}

	void Generator::brez(std::uint16_t address, Register source)
	{
		//brez &address, source
		ENCODE(new Instruction{ BREZ, DC, source, 0, address });
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
