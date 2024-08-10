#include "ColorIntrinsic.h"

#include <format>

namespace hz
{
	IntrinsicType ColorIntrinsic::itype() const
	{
		return IntrinsicType::COLOR;
	}

	std::string ColorIntrinsic::string() const
	{
		return std::format("color intrinsic ({}, {}, {})",
			static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255));
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