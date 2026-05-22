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
		static bool validate_primitive(const Parameters&);
		static bool validate_pointer(const Parameters&);
		static bool validate_enum(const Parameters&);
		static bool validate_struct(const Parameters&);
		static bool validate_union(const Parameters&);
		static bool validate_function(const Parameters&);
		static bool validate_array(const Parameters&);

	public:
		static bool validate_parse_primitive(const Parameters&);
		static bool validate_parse_pointer(const Parameters&);
		static bool validate_parse_enum(const Parameters&);
		static bool validate_parse_struct(const Parameters&);
		static bool validate_parse_union(const Parameters&);
		static bool validate_parse_function(const Parameters&);
		static bool validate_parse_array(const Parameters&);

	public:
		TypeValidator();
	};
}

#endif
