#ifndef HAZE_FUNCTIONDECLARATOR_H
#define HAZE_FUNCTIONDECLARATOR_H

#include "Declarator.h"
#include "Function.h"

namespace hz
{
	class FunctionDeclarator : public Declarator
	{
	public:
		Function* function;

	public:
		FunctionDeclarator(Function* function)
			: function{ function }
		{
		}

	public:
		virtual Declarator::Type dtype() const final override;
		virtual std::string string() const final override;
		virtual FunctionDeclarator* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Declarator* optimize() final override;
	};
}

#endif
