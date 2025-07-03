#ifndef HAZE_SCOPE_H
#define HAZE_SCOPE_H

#include "Context.h"

// Haze Scope.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Scope
	{
	public:
		Scope* _parent;
		Context* _locals;

	public:
		Scope();
	};
}

#endif
