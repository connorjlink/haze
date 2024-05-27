#ifndef HAZE_FUNCTIONCALLEXPRESSION_H
#define HAZE_FUNCTIONCALLEXPRESSION_H

#include "Expression.h"

#include <string>
#include <vector>

namespace hz
{
	class FunctionCallExpression : public Expression
	{
	public:
		std::string name;
		std::vector<Expression*> arguments;

	public:
		FunctionCallExpression(std::string name, std::vector<Expression*>&& arguments)
			: name(std::move(name)), arguments(std::move(arguments))
		{
		}

	public:
		virtual Expression::Type etype() const final override;
		virtual std::string string() const final override;
		virtual FunctionCallExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
	};
}

#endif //HAZE_FUNCTIONCALLEXPRESSION_H
