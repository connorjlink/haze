#ifndef HAZE_STATEMENT_H
#define HAZE_STATEMENT_H

#include <allocator/RuntimeAllocator.h>
#include <ast/Node.h>
#include <ast/StatementType.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Context.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <toolchain/Parser.h>
#include <toolchain/models/Token.h>

// Haze Statement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Statement 
		: public Node
		, public InjectService<Generator, RuntimeAllocator, Parser>
		, public InjectSingleton<Context, ErrorReporter, SymbolDatabase>
	{
	public:
		Statement(Token token)
			: Node{ token }
		{
		}

	public:
		virtual NodeType ntype() const final override;
		virtual StatementType stype() const = 0;
	};
}

#endif
