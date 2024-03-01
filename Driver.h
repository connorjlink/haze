#ifndef HAZE_DRIVER_H
#define HAZE_DRIVER_H

#include "Preprocessor.h"
#include "Lexer.h"
#include "Parser.h"
#include "Optimizer.h"
#include "Generator.h"
#include "Reducer.h"
#include "Simulator.h"

#include <string>
#include <string_view>

namespace hz
{
    class Driver
    {
    private:
        Preprocessor* preprocessor;
        Lexer* lexer;
        Parser* parser;
        Optimizer* optimizer;
        Generator* generator;
        Reducer* reducer;
        Simulator* simulator;

    public:
        Driver(std::string, std::string_view);
    };
}

#endif //HAZE_DRIVER_H
