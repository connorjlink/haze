#ifndef HAZE_INTRINSICDECLARATOR_H
#define HAZE_INTRINSICDECLARATOR_H

#include "Declarator.h"
#include "Intrinsic.h"

namespace hz
{
	class IntrinsicDeclarator : public Declarator
	{
	public:
		std::string name;
		Intrinsic* intrinsic;

	public:
		IntrinsicDeclarator(std::string name, Intrinsic* intrinsic)
			: name{ name }, intrinsic{ intrinsic }
		{
		}

	public:
		virtual Declarator::Type dtype() const final override;
		virtual std::string string() const final override;
		virtual IntrinsicDeclarator* copy() const final override;
		virtual void generator(Allocation*) final override;
		virtual Declarator* optimize() final override;
	};
}

#endif
