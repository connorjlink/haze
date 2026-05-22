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
		ParametersT parameters;

	public:
		void attach(ParametersT& parameters)
		{
			this->parameters = parameters;
		}

		bool run_test() const
		{
			return test(frame);
		}

	public:
		Test(const std::string& name, std::function<bool(ParametersT&)> test)
			: name{ name }, test{ test }, parameters{}
		{
			// explicit default on parameters
		}
	};
}

#endif
