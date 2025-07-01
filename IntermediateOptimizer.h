#ifndef HAZE_INTERMEDIATEOPTIMIZER_H
#define HAZE_INTERMEDIATEOPTIMIZER_H

#include "IntermediateCommand.h"

// Haze IntermediateOptimizer.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IntermediateOptimizer 
		: public InjectService<Generator>
		, public InjectSingleton<SymbolDatabase>
	{
	private:
		std::string _function;

	private:
		const std::vector<IntermediateCommand*>& _ir;

	public:
		IntermediateOptimizer(const std::string& function, const std::vector<IntermediateCommand*>& ir)
			: _function{ function }, _ir{ ir }
		{
		}

	public:
		std::vector<IntermediateCommand*> optimize();
	};
}

#endif
