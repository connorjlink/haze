#include "HazeEvaluator.h"

namespace hz
{
	void HazeEvaluator::evaluate()
	{
		for (auto& declarator : declarators)
		{
#pragma message("TODO: figure out if there are ever return values from Declarator->Evaluate() that we actually need to hold onto")
			DISCARD declarator->evaluate(context);
		}
	}
}