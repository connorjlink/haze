import std;

#include <ast/InlineAsmStatement.h>
#include <error/CommonErrors.h>
#include <toolchain/AssemblerLinker.h>
#include <toolchain/Emitter.h>
#include <toolchain/Generator.h>
#include <utility/Constants.h>

// Haze InlineAsmStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType InlineAsmStatement::stype() const
	{
		return StatementType::INLINEASM;
	}

	InlineAsmStatement* InlineAsmStatement::copy() const
	{
		return new InlineAsmStatement{ *this };
	}

	void InlineAsmStatement::generate(Allocation*)
	{
		auto linker = new AssemblerLinker{ std::move(_commands), _assembler_parser, _enclosing_file };
		auto commands = linker->link(REQUIRE_SAFE(Generator)->resolve_origin(), UWORD_MAX); 
		
		auto emitter = Emitter::from_architecture(std::move(commands), _enclosing_file);
		auto object_code = emitter->emit();

		REQUIRE_SAFE(Generator)->inline_assembly(std::move(object_code), linker->approximate_size());
	}

	Statement* InlineAsmStatement::optimize()
	{
		// No optimizations performed for inline assembly
		return nullptr;
	}

	Node* InlineAsmStatement::evaluate(Context* context) const
	{
		CommonErrors::unsupported_statement("interpreter", "asm", _token);
		return nullptr;
	}
}