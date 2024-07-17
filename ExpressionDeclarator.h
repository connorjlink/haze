#ifndef HAZE_EXPRESSIONDECLARATOR_H
#define HAZE_EXPRESSIONDECLARATOR_H

#include "Declarator.h"
#include "Expression.h"

namespace hz
{
	class ExpressionDeclarator : public Declarator
	{
	public:
		virtual ~ExpressionDeclarator() = default;

	public:
		virtual Declarator::Type dtype() const final override;
		virtual ExpressionType etype() const = 0;
	};
}

#endif
