#ifndef HAZE_EXPRESSION_H
#define HAZE_EXPRESSION_H

#include <allocator/RuntimeAllocator.h>
#include <ast/Node.h>
#include <ast/ExpressionType.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>

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
