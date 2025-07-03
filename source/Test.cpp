import std;

#include "Test.h"

// Haze Test.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Test::attach(const ErrorFrame& parameters)
	{
		_frame =
		{
			parameters.context,
			parameters.filepath,
		};
	}

	bool Test::run_test() const
	{
		return _test(_frame);
	}

	Test::Test(const std::string& name, std::function<bool(ErrorFrame)> test)
		: _name{ name }, _test{ test }
	{
		// explicit default to nullptr
		_frame =
		{
			nullptr,
			"",
		};
	}
}
