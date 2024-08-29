#ifndef HAZE_EVALUATOR_H
#define HAZE_EVALUATOR_H

#include "Context.h"
#include "Node.h"

// Haze Evaluator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	variable_t harvest(Node*);
	Node* unharvest(variable_t);
	std::string format(variable_t);
}

#endif
