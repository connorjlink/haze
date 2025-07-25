#ifndef HAZE_VALIDATOR_H
#define HAZE_VALIDATOR_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <validator/Test.h>

// Haze Validator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Validator
		: public InjectSingleton<ErrorReporter>
	{
	private:
		std::vector<Test> _tests;
		static inline std::size_t _num_tests = 0;

	protected:
		std::size_t test_count() const;
		void add_test(const Test&);

	public:
		void run_tests() const;
	};
}

#endif
