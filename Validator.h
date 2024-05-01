#ifndef HAZE_VALIDATOR_H
#define HAZE_VALIDATOR_H

#include <string_view>
#include <vector>
#include <functional>

namespace hz
{
    struct Test
    {
        std::string_view name;
        std::function<bool()> test;
    };

    class Validator
    {
    private:
        std::vector<Test> tests;
        static inline std::size_t test_counter = 0;

    protected:
        std::size_t test_count() const
        {
            return test_counter;
        }

        void add_test(const Test& test)
        {
            tests.emplace_back(test);
            test_counter++;
        }
    };
}

#endif //HAZE_VALIDATOR_H
