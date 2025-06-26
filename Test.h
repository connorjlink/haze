#ifndef HAZE_TEST_H
#define HAZE_TEST_H

// Haze Test.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ErrorContext;

	struct ErrorFrame
	{
		ErrorContext* context;
		std::string filepath;
	};

	class Test
	{
	private:
		std::string _name;
		std::function<bool(ErrorFrame)> _test;

	private:
		ErrorFrame _frame;

	public:
		void attach(const ErrorFrame&);
		bool run_test() const;

	public:
		Test(const std::string&, std::function<bool(ErrorFrame)>);
	};
}

#endif
