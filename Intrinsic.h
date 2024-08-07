#ifndef HAZE_INTRINSIC_H
#define HAZE_INTRINSIC_H

#include "Node.h"

#include "IntrinsicType.h"

namespace hz
{
	class Intrinsic : public Node
	{
	public:
		std::string name;

	public:
		Intrinsic(std::string&& name)
			: name{ std::move(name) }
		{
		}

	public:
		virtual NodeType ntype() const final override;
		virtual IntrinsicType itype() const = 0;
	};
}

#endif 
