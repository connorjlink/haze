import std;

#include <ast/InlineAsmStatement.h>
#include <error/CommonErrors.h>
#include <toolchain/Generator.h>
#include <toolchain/Linker.h>
#include <utility/Constants.h>

// Haze InlineAsmStatement.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	StatementType InlineAsmStatement::stype() const
	{
		return StatementType::INLINEASM;
	}

	void InlineAsmStatement::generate(Allocation*)
	{
		auto linker = new Linker{ commands, enclosing_file };
		const auto commands = linker->link(REQUIRE_SAFE(Generator)->resolve_origin(), UWORD_MAX); 
		
		const auto object_code = commands
			| std::ranges::views::transform([](auto command) 
			{ 
				return command->object_code; 
			})
			| std::ranges::views::join
			| std::ranges::to<ByteRange>();

		REQUIRE_SAFE(Generator)->inline_assembly(object_code, object_code.size());
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