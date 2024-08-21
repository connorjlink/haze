#ifndef HAZE_IDENTIFIEREXPRESSION_H
#define HAZE_IDENTIFIEREXPRESSION_H

#include "Expression.h"

#include <string>

namespace hz
{
	class IdentifierExpression : public Expression
	{
	public:
		std::string name;

	public:
		IdentifierExpression(std::string name)
			: name(std::move(name))
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual std::string string() const final override;
		virtual IdentifierExpression* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context*) const final override;
	};
}

#endif //HAZE_IDENTIFIEREXPRESSION_H
