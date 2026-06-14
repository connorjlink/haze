#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <ast/declaration/Declaration.h>
#include <ast/expression/Expression.h>
#include <ast/function/Function.h>
#include <ast/statement/Statement.h>
#include <type/Type.h>

// Haze Symbol.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	using Symbol = std::variant
	<
		DeclarationHandle,
		ExpressionHandle,
		FunctionHandle,
		StatementHandle,
		TypeHandle
	>;

}

#endif
