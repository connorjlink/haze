#ifndef HAZE_TEST_H
#define HAZE_TEST_H

#include <error/ErrorContext.h>

// Haze Test.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename ParametersT>
	class Test
	{
	private:
		std::string name;
		std::function<bool(ParametersT&)> test;

	private:
		ParametersT& parameters;

	public:
		bool run_test() const
		{
			return test(parameters);
		}

	public:
		Test(const std::string& name, std::function<bool(ParametersT&)> test, Parameters& parameters)
			: name{ name }, test{ test }, parameters{ parameters }
		{
			// explicit default on parameters
		}
	};
}

#endif
