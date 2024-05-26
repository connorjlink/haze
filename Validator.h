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
        static inline std::size_t num_tests = 0;

    protected:
        std::size_t test_count() const
        {
            return num_tests;
        }

        void add_test(const Test& test)
        {
            tests.emplace_back(test);
            num_tests++;
        }


        //Method to get the tests variable
		
    };
}

#endif //HAZE_VALIDATOR_H
