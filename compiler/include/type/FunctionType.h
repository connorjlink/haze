#ifndef HAZE_FUNCTIONTYPE_H
#define HAZE_FUNCTIONTYPE_H

#include <type/PointerType.h>

// Haze FunctionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class FunctionType : public TypeBase
	{
	public:
		std::vector<TypeHandle> parameters;
		TypeHandle return_type;
		bool is_variadic;

	public:
		TypeKind type_kind() const
		{
			return TypeKind::FUNCTION;
		}

		Offset size() const
		{
			return PointerType::size();
		}

		bool is_complete() const
		{
			// this is basically impossible for functions, but it is closer to complete since sizeof() works
			return true;
		}
	};
}

#endif
