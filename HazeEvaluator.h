#ifndef HAZE_HAZEEVALUATOR_H
#define HAZE_HAZEEVALUATOR_H

#include "Node.h"
#include "ErrorReporter.h"

#include <string>
#include <vector>

namespace hz
{
	class HazeEvaluator
	{
	private:
		std::vector<Node*> declarators;

	public:
		HazeEvaluator(std::vector<Node*>&& declarators, const std::string& filepath)
			: declarators{ std::move(declarators) }
		{
			_error_reporter->open_context(filepath, "evaluating");
		}

		~HazeEvaluator()
		{
			_error_reporter->close_context();
		}

	public:
		void evaluate();
	};
}

#endif 
