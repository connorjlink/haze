#ifndef HAZE_SYMBOL_H
#define HAZE_SYMBOL_H

#include <utility/Sum.h>

// Haze Symbol.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Declaration)
	FORWARD_DECLARE_SUM(Function)
	FORWARD_DECLARE_SUM(Statement)
	FORWARD_DECLARE_SUM(Expression)
	FORWARD_DECLARE_SUM(Type)

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
