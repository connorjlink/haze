#ifndef HAZE_TYPEVALIDATOR_H
#define HAZE_TYPEVALIDATOR_H

#include <validator/Validator.h>

// Haze TypeValidator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class TypeValidator : public Validator
	{
	private:
		struct Parameters
		{
			ErrorFrame error_frame;
			TypeSumStorage sum;
		};

	private:
		using Test = Test<Parameters>;

	private:
		static bool validate_primitive(Parameters&);
		static bool validate_pointer(Parameters&);
		static bool validate_enum(Parameters&);
		static bool validate_struct(Parameters&);
		static bool validate_union(Parameters&);
		static bool validate_function(Parameters&);
		static bool validate_array(Parameters&);

	public:
		static bool validate_parse_primitive(Parameters&);
		static bool validate_parse_pointer(Parameters&);
		static bool validate_parse_enum(Parameters&);
		static bool validate_parse_struct(Parameters&);
		static bool validate_parse_union(Parameters&);
		static bool validate_parse_function(Parameters&);
		static bool validate_parse_array(Parameters&);

	public:
		TypeValidator();
	};
}

#endif
