#include "Validator.h"
#include "ErrorReporter.h"
#include "Log.h"

import std;

// Haze Validator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    std::size_t Validator::test_count() const
    {
        return _num_tests;
    }

    void Validator::add_test(const Test& test)
    {
        _tests.emplace_back(test);
        _num_tests++;
    }

    void Validator::run_tests() const
    {
        auto had_error = false;

	    for (auto& test : _tests)
	    {
            if (test.run_test())
            {
                had_error = true;
            }
	    }

        if (had_error)
        {
            _error_reporter->close_all_contexts();
            Log::raw(_error_reporter->generate_report());
        }
    }
}
