#ifndef HAZE_VALIDATOR_H
#define HAZE_VALIDATOR_H

#include "Test.h"
#include "ErrorContext.h"

#include <string>
#include <vector>
#include <functional>

// Haze Validator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class Validator
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

#endif //HAZE_VALIDATOR_H
