import std;

#include "HazeEvaluator.h"
#include "Node.h"
#include "Context.h"
#include "ErrorReporter.h"

// Haze HazeEvaluator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	HazeEvaluator::HazeEvaluator(std::vector<Node*>&& declarators, const std::string& filepath)
		: declarators{ std::move(declarators) }
	{
		_error_reporter->open_context(filepath, "evaluating");
	}

	HazeEvaluator::~HazeEvaluator()
	{
		_error_reporter->close_context();
	}

	void HazeEvaluator::evaluate()
	{
		for (auto& declarator : declarators)
		{
#pragma message("TODO: figure out if there are ever return values from Declarator->Evaluate() that we actually need to hold onto")
			declarator->evaluate(_context);
		}
	}
}