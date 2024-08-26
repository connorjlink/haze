#include "InlineAsmStatement.h"
#include "Generator.h"
#include "AssemblerLinker.h"
#include "FileManager.h"

#include <format>

namespace hz
{
	StatementType InlineAsmStatement::stype() const
	{
		return StatementType::INLINEASM;
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
		auto linker = new AssemblerLinker{ std::move(commands), assembler_parser, _current_file };
		auto object_code = linker->link(_generator->write_pointer());

		_generator->image(std::move(object_code), AS_ASSEMBLER_LINKER(linker)->approximate_size());
	}

	Statement* InlineAsmStatement::optimize()
	{
		return nullptr;
	}

	Node* InlineAsmStatement::evaluate(Context* context) const
	{
		Log::error("Inline assembly statements cannot be evaluated in an interpreted context");
	}
}