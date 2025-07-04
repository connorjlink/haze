#ifndef HAZE_NODE_H
#define HAZE_NODE_H

#include <ast/NodeType.h>
#include <toolchain/models/Token.h>

// Haze Node.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// Required to avoid dependency between Instruction and Allocation
	class Allocation;

	// Required to avoid dependency between Node and Context
	class Context;

	class Node
	{
	public:
		Token _token;

	public:
		Node(const Token& token)
			: _token{ token }
		{
		}
		virtual ~Node() = default;

	public:
		virtual NodeType ntype(void) const = 0;
		virtual Node* copy(void) const = 0;
		virtual void generate(Allocation* = nullptr) = 0;
		virtual Node* optimize(void) = 0;

		// use this to store execution information in the interpreter
		// this could be extended later to do compile time stuff in the compiler
		virtual Node* evaluate(Context*) const = 0;
	};
}

#endif
