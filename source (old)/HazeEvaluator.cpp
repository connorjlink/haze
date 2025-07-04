import std;

#include "HazeEvaluator.h"
#include "Node.h"

// Haze HazeEvaluator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	HazeEvaluator::HazeEvaluator(std::vector<Node*>&& declarators, const std::string& filepath)
		: declarators{ std::move(declarators) }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "evaluating");
	}

	HazeEvaluator::~HazeEvaluator()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}

	void HazeEvaluator::evaluate()
	{
		for (auto& declarator : declarators)
		{
#pragma message("TODO: figure out if there are ever return values from Declarator->Evaluate() that we actually need to hold onto")
			declarator->evaluate(USE_SAFE(Context).get());
		}
	}
}