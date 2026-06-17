#ifndef HAZE_FUNCTIONTYPE_H
#define HAZE_FUNCTIONTYPE_H

#include <type/PointerType.h>
#include <utility/Constants.h>

// Haze FunctionType.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct FunctionType : public TypeBase
	{
	public:
		std::vector<TypeHandle> parameters;
		TypeHandle return_type;
		bool is_variadic;

	public:
		TypeKind type_kind() const;
		// NOTE: this is static to allow other classes a convenient way to get platform variables
		static Offset size();
		bool is_complete() const;

	public:
		FunctionType(std::vector<TypeHandle> parameters, TypeHandle return_type, bool is_variadic)
			: parameters{ std::move(parameters) }, return_type{ return_type }, is_variadic{ is_variadic }
		{
		}
	};
}

#endif
