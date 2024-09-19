#ifndef HAZE_TEST_H
#define HAZE_TEST_H

// Haze Test.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class ErrorContext;

    struct TestParameters
    {
        ErrorContext* context;
        std::string file;
    };

    class Test
    {
    private:
        std::string _name;
        std::function<bool(TestParameters)> _test;

    private:
        TestParameters _test_parameters;

    public:
        void attach(const TestParameters& parameters)
        {
            _test_parameters =
            {
                parameters.context,
                parameters.file,
            };
        }

    	bool run_test() const
        {
            return _test(_test_parameters);
        }

    public:
        Test(const std::string& name, std::function<bool(TestParameters)> test)
            : _name{ name }, _test{ test }
        {
            // explicit default to nullptr
            _test_parameters =
            {
                nullptr,
                "",
            };
        }
    };

}

#endif
