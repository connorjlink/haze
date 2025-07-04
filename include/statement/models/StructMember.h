#ifndef HAZE_STRUCTMEMBER_H
#define HAZE_STRUCTMEMBER_H

// Haze StructMember.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Type;

	class StructMember
	{
	public:
		Type* type;
		std::uint32_t offset;

	public:
		StructMember(Type* type, std::uint32_t offset)
			: type{ type }, offset{ offset }
		{
		}
	};
}

#endif
