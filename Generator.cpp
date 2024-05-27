#include "Generator.h"

#include "Allocator.h"
#include "Allocation.h"
#include "Log.h"
#include "Parser.h"
#include "Symbol.h"
#include "Utility.h"
#include "IdentifierExpression.h"
#include "IntegerLiteralExpression.h"
#include "FunctionCallExpression.h"
#include "BinaryExpression.h"

#include <unordered_map>
#include <format>
#include <iostream>
#include <cstdlib>
#include <algorithm>

namespace
{
	void encode(std::pair<std::string, std::vector<std::array<std::uint8_t, 3>>>& vec, std::uint32_t bytes)
	{
		vec.second.emplace_back(hz::extract(bytes));
	}
}

namespace hz
{
	void Generator::begin_function(std::string name)
	{
		current_function++;
		//TODO: is there a more efficient way to do this (without copying?)
		bytes.push_back({ name, {} });
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

	std::vector<std::pair<std::string, std::vector<std::array<std::uint8_t, 3>>>> Generator::generate()
	{
		for (auto function : program)
		{
			//TODO: this allocation scheme does not work
			auto allocation = allocator->allocate_static();
			function->generate(allocation);
		}

		//Reorder the functions so `main` is first since it's our entrypoint.
		std::ranges::partition(bytes, [](const auto& function)
		{
			return function.first == "main";
		});

		return bytes;
	}
}
