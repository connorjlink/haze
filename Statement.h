#ifndef HAZE_STATEMENT_H
#define HAZE_STATEMENT_H

#include <vector>
#include <string_view>

#include "Expression.h"
#include "Node.h"
#include "Segment.h"

namespace hz
{
    class Statement : public Node
    {
    public:
        enum class Type
        {
            COMPOUND,
            VARIABLE,
            RETURN,
        };

    public:
        virtual Statement::Type stype() const = 0;
        virtual Node::Type ntype() const final override;
    };

    class CompoundStatement : public Statement
    {
    private:
        std::vector<Statement*> substatements;

    public:
        virtual Statement::Type stype() const final override;
        virtual void generate(Allocation*) final override;
        virtual Statement* optimize() final override;
    };

    class VariableStatement : public Statement
    {
    private:
        std::string_view name;
        Expression* value;
        Allocation* allocation;

    public:
        virtual Statement::Type stype() const final override;
        virtual void generate(Allocation*) final override;
        virtual Statement* optimize() final override;
    };

    class ReturnStatement : public Statement
    {
    private:
        Expression* value;
        Allocation* allocation;

    public:
        virtual Statement::Type stype() const final override;
        virtual void generate(Allocation*) final override;
        virtual Statement* optimize() final override;
    };

}

#endif //HAZE_STATEMENT_H
