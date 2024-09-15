#ifndef HAZE_COLORINTRINSIC_H
#define HAZE_COLORINTRINSIC_H

#include "Intrinsic.h"

// Haze ColorIntrinsic.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ColorIntrinsic : public Intrinsic
	{
	public:
		float r, g, b;

	public:
		ColorIntrinsic(std::string&& name, float r, float g, float b, Token token)
			: Intrinsic{ std::move(name), token }, r{ r }, g{ g }, b{ b }
		{
		}

	public:
		virtual IntrinsicType itype() const final override;
		virtual ColorIntrinsic* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Intrinsic* optimize() final override;
		virtual Node* evaluate(Context*) const final override;

	public:
		using Intrinsic::Intrinsic;
	};
}

#endif
