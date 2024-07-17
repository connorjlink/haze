#ifndef HAZE_DECLARATOR_H
#define HAZE_DECLARATOR_H

#include "Node.h"

#include "DeclaratorType.h"

namespace hz
{
	class Declarator : public Node
	{
	public:
		virtual ~Declarator() = default;

	public:
		virtual Node::Type ntype() const final override;
		virtual Declarator::Type dtype() const = 0;
	};
}

#endif 
