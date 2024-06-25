#include "Intrinsic.h"

#include <format>

namespace hz
{
	Node::Type Intrinsic::ntype() const
	{
		return Node::Type::INTRINSIC;
	}

	Intrinsic::Type ColorIntrinsic::itype() const
	{
		return Intrinsic::Type::COLOR;
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
}