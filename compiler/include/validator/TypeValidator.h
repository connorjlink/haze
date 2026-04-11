#ifndef HAZE_TYPEVALIDATOR_H
#define HAZE_TYPEVALIDATOR_H

#include "Validator.h"

// Haze TypeValidator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ErrorFrame;

	class TypeValidator : public Validator
	{
	private:
		static bool validate_primitive(ErrorFrame);
		static bool validate_pointer(ErrorFrame);
        static bool validate_enum(ErrorFrame);
        static bool validate_struct(ErrorFrame);
        static bool validate_union(ErrorFrame);
        static bool validate_function(ErrorFrame);
        static bool validate_array(ErrorFrame);

    private:
        TypeSum sum;
        SumHandle<TypeSum::Storage> _int;
        SumHandle<TypeSum::Storage> _static_const_unsigned_short;
        SumHandle<TypeSum::Storage> _extern_const_volatile_long_long;
        SumHandle<TypeSum::Storage> _const_char;
        SumHandle<TypeSum::Storage> _const_enum;
        SumHandle<TypeSum::Storage> _struct_no_members;
        SumHandle<TypeSum::Storage> _extern_const_volatile_struct_2_members;
        SumHandle<TypeSum::Storage> _union_no_members;
        SumHandle<TypeSum::Storage> _extern_union_2_members;
        SumHandle<TypeSum::Storage> _int_pointer_function_long_double;
        SumHandle<TypeSum::Storage> _enum_function_pointer_const_char_pointer_variadic;
        SumHandle<TypeSum::Storage> _unsigned_short_array_10;
        SumHandle<TypeSum::Storage> _volatile_int_pointer_array_5;
        SumHandle<TypeSum::Storage> _volatile_int_array_5_pointer;
        SumHandle<TypeSum::Storage> _pointer_to_array_5_enum_function_pointer_const_char_pointer_variadic;

	public:
		TypeValidator();
	};
}

#endif
