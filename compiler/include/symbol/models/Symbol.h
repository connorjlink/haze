#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <ast/declaration/Declaration.h>
#include <type/Type.h>

// Haze Symbol.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Symbol
	{
		TypeHandle type;
		DeclarationHandle declaration;
#pragma message("TODO: incorporate other information like alignment and size constraints")
		std::size_t stack_offset;
	};
}

#endif
