#ifndef HAZE_OPTIMIZER_H
#define HAZE_OPTIMIZER_H

#include <cstdlib>
#include <vector>

#include "Function.h"
#include "Expression.h"
#include "Statement.h"
#include "Parser.h"

namespace hz
{
#ifndef BIT
#define BIT(x) (1 << x)
    enum Parameters : std::size_t
    {
        OPTIMIZE_AST = BIT(0),
    };
#undef BIT
#endif

    class Optimizer
    {
    private:
        std::vector<Function*> program;
        Parser* parser;
        std::size_t parameters;

    public:
        std::vector<Function*>& get_ast()
        {
            return program;
        }

    public:
        Optimizer(std::vector<Function*>&, Parser*, std::size_t);

    private:
        bool optimize_expression(Expression*);
        bool optimize_statement(Statement*);
        bool optimize_ast(std::vector<Function*>&);

    public:
        void optimize();
    };
}

#endif //HAZE_OPTIMIZER_H
