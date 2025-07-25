#ifndef HAZE_EVALUATOR_H
#define HAZE_EVALUATOR_H

#include <ast/Node.h>
#include <runtime/models/Variable.h>

// Haze Evaluator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	Variable* node_to_variable(Node*);
	Node* variable_to_node(Variable*);
	std::string format(Variable*);
}

#endif
