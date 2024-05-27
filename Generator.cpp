#include "Generator.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Log.h"
#include "Parser.h"
#include "Symbol.h"
#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "FunctionCallExpression.h"
#include "BinaryExpression.h"

#include <unordered_map>
#include <format>
#include <iostream>
#include <cstdlib>

#include "Utility.h"

namespace
{
	void encode(std::vector<std::uint8_t>& vec, std::uint32_t bytes)
	{
		for (auto byte : hz::extract(bytes))
		{
			vec.emplace_back(byte);
		}
	}
}

namespace hz
{
	void Generator::begin_function()
	{
		current_function++;
		bytes.emplace_back();
	}

	void Generator::move(Register destination, Register source)
	{
		//move destination, source
		::encode(bytes[current_function], Instruction{ MOVE, destination, source }.bytes());
	}

	void Generator::load(Register destination, std::uint16_t address)
	{
		//load destination, &address
		::encode(bytes[current_function], Instruction{ LOAD, destination, DC, 0, address }.bytes());
	}

	void Generator::copy(Register destination, std::uint8_t immediate)
	{
		//copy destination, #immediate
		::encode(bytes[current_function], Instruction{ COPY, destination, DC, immediate, 0 }.bytes());
	}

	void Generator::save(std::uint16_t address, Register source)
	{
		//save &address, source
		::encode(bytes[current_function], Instruction{ SAVE, DC, source, 0, address }.bytes());
	}

	void Generator::iadd(Register destination, Register source)
	{
		//iadd destination, source
		::encode(bytes[current_function], Instruction{ IADD, destination, source }.bytes());
	}

	void Generator::isub(Register destination, Register source)
	{
		//isub destination, source
		::encode(bytes[current_function], Instruction{ ISUB, destination, source }.bytes());
	}

	void Generator::band(Register destination, Register source)
	{
		//band destination, source
		::encode(bytes[current_function], Instruction{ BAND, destination, source }.bytes());
	}

	void Generator::bior(Register destination, Register source)
	{
		//bior destination, source
		::encode(bytes[current_function], Instruction{ BIOR, destination, source }.bytes());
	}

	void Generator::bxor(Register destination, Register source)
	{
		//bxor destination, source
		::encode(bytes[current_function], Instruction{ BXOR, destination, source }.bytes());
	}

	void Generator::call(std::uint16_t address)
	{
		//call &address
		::encode(bytes[current_function], Instruction{ CALL, DC, DC, 0, address }.bytes());
	}

	void Generator::exit()
	{
		//exit
		::encode(bytes[current_function], Instruction{ EXIT, DC, DC }.bytes());
	}

	void Generator::push(Register source)
	{
		//push source
		::encode(bytes[current_function], Instruction{ PUSH, DC, source }.bytes());
	}

	void Generator::pull(Register destination)
	{
		//pull destination
		::encode(bytes[current_function], Instruction{ PULL, destination, DC }.bytes());
	}

	void Generator::brez(std::uint16_t address, Register source)
	{
		//brez &address, source
		::encode(bytes[current_function], Instruction{ BREZ, DC, source, 0, address }.bytes());
	}

	std::vector<std::vector<std::uint8_t>> Generator::generate()
	{
		for (auto function : program)
		{
			//TODO: this allocation scheme does not work
			auto allocation = allocator->allocate_static();
			function->generate(allocation);
		}

		return bytes;
	}
}
