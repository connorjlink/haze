#ifndef HAZE_INTERMEDIATEOPTIMIZER_H
#define HAZE_INTERMEDIATEOPTIMIZER_H

#include "IntermediateCommand.h"

// Haze IntermediateOptimizer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IntermediateOptimizer
	{
	private:
		std::string _function;
		std::vector<IntermediateCommand*> _ir;

	public:
		IntermediateOptimizer(const std::string& function, std::vector<IntermediateCommand*>&& ir)
			: _function{ function }, _ir { std::move(ir) }
		{
		}

	public:
		std::vector<IntermediateCommand*> optimize();
	};
}

#endif
