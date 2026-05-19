import std;

#include <validator/Test.h>

// Haze Test.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Test::attach(const ErrorFrame& parameters)
	{
		frame =
		{
			parameters.context,
			parameters.filepath,
		};
	}

	bool Test::run_test() const
	{
		return _test(frame);
	}

	Test::Test(const std::string& name, std::function<bool(ErrorFrame)> test)
		: _name{ name }, _test{ test }
	{
		// explicit default to nullptr
		frame =
		{
			nullptr,
			"",
		};
	}
}
