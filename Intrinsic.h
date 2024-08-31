#ifndef HAZE_INTRINSIC_H
#define HAZE_INTRINSIC_H

#include "Node.h"
#include "IntrinsicType.h"

// Haze Intrinsic.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Intrinsic : public Node
	{
	public:
		std::string name;

	public:
		Intrinsic(std::string&& name, Token token)
			: Node{ token }, name{ std::move(name) }
		{
		}

	public:
		virtual NodeType ntype() const final override;
		virtual IntrinsicType itype() const = 0;
	};
}

#endif 
