#ifndef HAZE_ARGUMENTEXPRESSION_H
#define HAZE_ARGUMENTEXPRESSION_H

#include "Expression.h"
#include "IdentifierExpression.h"

namespace hz
{
	enum class ReturnType;

	class ArgumentExpression : public Expression
	{
	public:
		ReturnType type_specifier;
		IdentifierExpression* identifier;

	public:
		ArgumentExpression(ReturnType type_specifier, IdentifierExpression* identifier)
			: type_specifier{ type_specifier }, identifier{ identifier }
		{
		}

	public:
		virtual ExpressionType etype() const final override;
		virtual ArgumentExpression* copy() const final override;
		virtual std::string string() const final override;
		virtual void generate(Allocation*) final override;
		virtual Expression* optimize() final override;
		virtual Node* evaluate(Context* context) const final override;
	}; 
}

#endif 
