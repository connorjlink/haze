#ifndef HAZE_STATEMENT_H
#define HAZE_STATEMENT_H

#include <vector>
#include <string_view>

#include "Expression.h"
#include "Node.h"
#include "Segment.h"

#define AS_COMPOUND_STATEMENT(x) static_cast<CompoundStatement*>(x)
#define AS_VARIABLE_STATEMENT(x) static_cast<VariableStatement*>(x)
#define AS_RETURN_STATEMENT(x) static_cast<ReturnStatement*>(x)

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
		CompoundStatement(std::vector<Statement*> substatements)
			: substatements(substatements)
		{
		}

	public:
		virtual Statement::Type stype() const final override;
		virtual std::string string() const final override;
		virtual CompoundStatement* copy() const final override;
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
		VariableStatement(std::string_view name, Expression* value, Allocation* allocation)
			: name(name), value(value), allocation(allocation)
		{
		}

	public:
		virtual Statement::Type stype() const final override;
		virtual std::string string() const final override;
		virtual VariableStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
	};

	class ReturnStatement : public Statement
	{
	private:
		Expression* value;
		Allocation* allocation;

	public:
		ReturnStatement(Expression* value, Allocation* allocation)
			: value(value), allocation(allocation)
		{
		}

	public:
		virtual Statement::Type stype() const final override;
		virtual std::string string() const final override;
		virtual ReturnStatement* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Statement* optimize() final override;
	};

}

#endif //HAZE_STATEMENT_H
