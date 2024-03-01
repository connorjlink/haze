#ifndef HAZE_STATEMENT_H
#define HAZE_STATEMENT_H

#include <vector>
#include <string_view>

#include "Expression.h"

namespace hz
{
    struct Compound;
    struct VariableDeclaration;
    struct Return;

    enum class StatementType
    {
        COMPOUND,
        VARIABLE_DECLARATION,
        RETURN,
    };
    struct Statement
    {
        StatementType type;

        union
        {
            Compound* compound_statement;
            VariableDeclaration* variable_declaration;
            Return* return_statement;
        } as;
    };

    struct Compound
    {
        std::vector<Statement*> statements;
    };

    struct VariableDeclaration
    {
        std::string_view name;
        Expression* expression;
    };

    struct Return
    {
        Expression* expression;
    };
}

#endif //HAZE_STATEMENT_H
