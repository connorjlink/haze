cmake_minimum_required(VERSION 3.26)
project(haze)

set(CMAKE_CXX_STANDARD 23)

add_executable(haze Main.cpp
        Parser.cpp
        Parser.h
        Token.h
        Lexer.cpp
        Lexer.h
        Symbol.h
        Generator.cpp
        Generator.h
        Expression.h
        Statement.h
        Function.h
        Allocator.cpp
        Allocator.h
        Preprocessor.cpp
        Preprocessor.h
        Log.h
        Simulator.cpp
        Simulator.h
        Optimizer.cpp
        Optimizer.h
        Driver.cpp
        Driver.h
        Reducer.cpp
        Reducer.h
        Instruction.cpp
        Instruction.h
)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(haze PRIVATE fmt::fmt-header-only)