#ifndef HAZE_EVALUATOR_H
#define HAZE_EVALUATOR_H

#include "Node.h"
#include "Context.h"

// Haze Evaluator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Variable* harvest(Node*);
	Node* unharvest(Variable*);
	std::string format(Variable*);
}

#endif
