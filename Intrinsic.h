#ifndef HAZE_INTRINSIC_H
#define HAZE_INTRINSIC_H

#include "Node.h"

namespace hz
{
	class Intrinsic : public Node
	{
	public:
		enum class Type
		{
			COLOR,
		};

	public:
		explicit Intrinsic()
		{
		}

	public:
		virtual Node::Type ntype() const final override;
		virtual Intrinsic::Type itype() const = 0;
	};

	class ColorIntrinsic : public Intrinsic
	{
	public:
		float r, g, b;

	public:
		ColorIntrinsic(float r, float g, float b)
			: r{ r }, g{ g }, b{ b }
		{
		}

	public:
		virtual Intrinsic::Type itype() const final override;
		virtual std::string string() const final override;
		virtual ColorIntrinsic* copy() const final override;
		virtual void generate(Allocation*) final override;
		virtual Intrinsic* optimize() final override;
	};

}

#endif 
