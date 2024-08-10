#ifndef HAZE_COLORINTRINSIC_H
#define HAZE_COLORINTRINSIC_H

#include "Intrinsic.h"

namespace hz
{
	class ColorIntrinsic : public Intrinsic
	{
	public:
		float r, g, b;

	public:
		ColorIntrinsic(std::string&& name, float r, float g, float b)
			: Intrinsic{ std::move(name) }, r{ r }, g{ g }, b{ b }
		{
		}

	public:
		virtual IntrinsicType itype() const final override;
		virtual std::string string() const final override;
		virtual ColorIntrinsic* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Intrinsic* optimize() final override;
		virtual Node* evaluate(Context*) const final override;

	public:
		using Intrinsic::Intrinsic;
	};
}

#endif
