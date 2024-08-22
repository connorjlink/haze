#ifndef HAZE_STRINGEXPRESSION_H
#define HAZE_STRINGEXPRESSION_H

#include "Expression.h"

namespace hz
{
	class StringExpression : public Expression
	{
	public:
		std::string message;

	public:
		StringExpression(std::string message)
			: message{ std::move(message) }
		{
		}
		
	public:
		virtual ExpressionType etype() const final override;
		virtual std::string string() const final override;
		virtual StringExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif
