#ifndef HAZE_FUNCTIONCALLEXPRESSION_H
#define HAZE_FUNCTIONCALLEXPRESSION_H

#include <ast/Expression.h>

// Haze FunctionCallExpression.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class FunctionCallExpression : public Expression
	{
	public:
		std::string name;
		std::vector<Expression*> arguments;

	public:
		FunctionCallExpression(const std::string& name, std::vector<Expression*>&& arguments, Token token)
			: Expression{ token }, name{ name }, arguments{ std::move(arguments) }
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual FunctionCallExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
