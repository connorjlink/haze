#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

#include "Node.h"
#include "ExpressionType.h"
#include "RuntimeAllocator.h"
#include "Generator.h"
#include "SymbolDatabase.h"
#include "ErrorReporter.h"
#include "DependencyInjector.h"

// Haze ExpressionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Expression 
		: public Node
		, public InjectService<RuntimeAllocator, Generator>
		, public InjectSingleton<ErrorReporter, SymbolDatabase>
	{
	public:
		Expression(const Token& token)
			: Node{ token }
		{
		}

	public:
		virtual NodeType ntype() const final override;
		virtual ExpressionType etype() const = 0;
	};
}

#endif
