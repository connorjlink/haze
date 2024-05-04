#include "Optimizer.h"
#include "Parser.h"
#include "Log.h"
#include "Allocator.h"
#include "BinaryExpression.h"
#include "IntegerLiteralExpression.h"
#include "IdentifierExpression.h"

namespace hz
{
    Optimizer::Optimizer(std::vector<Function*>& program, Parser* parser, std::size_t parameters)
        : program(program), parser(parser), parameters(parameters)
    {
    }

    Optimizer::AST Optimizer::optimize_ast(AST& functions)
    {
        for (auto function : functions)
        {
            if (function->body->optimize())
            {
                return optimize_ast(functions);
            }
        }

        return {};
    }

    void Optimizer::optimize()
    {
        if (parameters & OPTIMIZE_AST)
        {
            for (auto pass = 0; optimize_ast(program); pass++)
            {
                Log::info(fmt::format("AST optimization pass {} successful", pass));
            }
        }
    }
}