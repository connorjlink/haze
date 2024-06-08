#include "InlineAsmStatement.h"
#include "Generator.h"

#include <format>

namespace hz
{
	Statement::Type InlineAsmStatement::stype() const
	{
		return Statement::Type::INLINEASM;
	}

	std::string InlineAsmStatement::string() const
	{
		std::string commands_string = "";

		for (auto command : commands)
		{
			commands_string += std::format("\t{}\n", command->string());
		}

		return std::format("inline asm statement \n [\n{}]\n", commands_string);
	}

	InlineAsmStatement* InlineAsmStatement::copy() const
	{
		return new InlineAsmStatement{ *this };
	}

	void InlineAsmStatement::generate(Allocation*)
	{
		for (auto command : commands)
		{
			command->generate();
		}
	}

	Statement* InlineAsmStatement::optimize()
	{
		return nullptr;
	}
}