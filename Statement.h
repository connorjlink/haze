#ifndef HAZE_STATEMENT_H
#define HAZE_STATEMENT_H

#include "Node.h"
#include "StatementType.h"
#include "Generator.h"
#include "DependencyInjector.h"

// Haze Statement.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Statement 
		: public Node
		, public InjectService<Generator>
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
