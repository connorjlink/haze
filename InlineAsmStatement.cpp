import std;

#include "InlineAsmStatement.h"
#include "AssemblerLinker.h"
#include "FileManager.h"
#include "Generator.h"
#include "Emitter.h"

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
		auto linker = new AssemblerLinker{ std::move(commands), assembler_parser, _file_manager->_current_file };
		auto commands = linker->link(_generator->write_pointer());
		
		auto emitter = Emitter::from_architecture(std::move(commands), _file_manager->_current_file);
		auto object_code = emitter->emit();

		_generator->inline_assembly(std::move(object_code), linker->approximate_size());
	}

	Statement* InlineAsmStatement::optimize()
	{
		// No optimizations performed for inline assembly
		return nullptr;
	}

	Node* InlineAsmStatement::evaluate(Context* context) const
	{
		_error_reporter->post_error("unsupported interpreter statement type `asm`", _token);
		return nullptr;
	}
}