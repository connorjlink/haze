#include "ColorIntrinsic.h"

#include <format>

namespace hz
{
	IntrinsicType ColorIntrinsic::itype() const
	{
		return IntrinsicType::COLOR;
	}

	ColorIntrinsic* ColorIntrinsic::copy() const
	{
		return new ColorIntrinsic{ *this };
	}

	void ColorIntrinsic::generate(Allocation*)
	{
		// TODO: nothing to do here??
	}

	Intrinsic* ColorIntrinsic::optimize()
	{
		return nullptr;
	}

	Node* ColorIntrinsic::evaluate(Context*) const
	{
#pragma message("TODO: implement intrinsic interpreter evaluation here")
		return nullptr;
	}
}