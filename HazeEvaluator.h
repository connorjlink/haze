#ifndef HAZE_HAZEEVALUATOR_H
#define HAZE_HAZEEVALUATOR_H

// Haze HazeEvaluator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Node;

	class HazeEvaluator
	{
	private:
		std::vector<Node*> declarators;

	public:
		HazeEvaluator(std::vector<Node*>&&, const std::string&);
		~HazeEvaluator();

	public:
		void evaluate();
	};
}

#endif 
