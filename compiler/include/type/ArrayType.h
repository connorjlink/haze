#ifndef HAZE_ARRAYTYPE_H
#define HAZE_ARRAYTYPE_H

#include <type/Type.h>

// Haze ArrayType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class ArrayType : public TypeBase
	{
	public:
		TypeHandle element_type;
		std::optional<Address> length;

	public:
		TypeKind ttype() const
		{
			return TypeKind::ARRAY;
		}

		Offset size() const
		{
			if (!length.has_value())
			{
				return PointerType::size();
			}

			return element_type.get().size() * length.value();
		}

		std::string string() const
		{
			const auto length_string = length.has_value() ? std::to_string(length.value()) : "";

			return std::format("{}[{}]",
				element_type.get().string(),
				length_string);
		}

		bool is_complete() const
		{
			// an array is incomplete if it doesn't have a length yet
			// this does not work for functions since they decay to pointers as parameters
			if (!length.has_value())
			{
				return false;
			}

			return element_type.get().is_complete();
		}
	};
}

#endif
