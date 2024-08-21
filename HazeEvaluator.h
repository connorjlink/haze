#ifndef HAZE_HAZEEVALUATOR_H
#define HAZE_HAZEEVALUATOR_H

#include "Node.h"

#include <vector>

namespace hz
{
	class HazeEvaluator
	{
	private:
		std::vector<Node*> declarators;

	private:
		Context& context;

	public:
		HazeEvaluator(std::vector<Node*>& declarators, Context& context)
			: declarators{ std::move(declarators) }, context{ context }
		{
		}

	public:
		void evaluate();
	};
}

#endif 
