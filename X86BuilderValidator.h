#ifndef HAZE_X86BUILDERVALIDATOR_H
#define HAZE_X86BUILDERVALIDATOR_H

#include "Validator.h"

// Haze X86BuilderValidator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class X86BuilderValidator : public Validator
	{
	private:
		static bool validate_modrm(TestParameters);
		static bool validate_modrm_rr(TestParameters);

	private:
		static bool validate_sib(TestParameters);

	private:
		static bool validate_push_r(TestParameters);
		static bool validate_push_i(TestParameters);
		static bool validate_push_m(TestParameters);

	private:
		static bool validate_pop_r(TestParameters);

	private:
		static bool validate_mov_rr(TestParameters);
		static bool validate_mov_ro(TestParameters);

	private:
		static bool validate_add_rr(TestParameters);
		static bool validate_add_ri(TestParameters);

	private:
		static bool validate_sub_rr(TestParameters);
		static bool validate_sub_ri(TestParameters);

	private:
		static bool validate_or_rr(TestParameters);
		static bool validate_or_ri(TestParameters);

	private:
		static bool validate_and_rr(TestParameters);
		static bool validate_and_ri(TestParameters);

	private:
		static bool validate_xor_rr(TestParameters);
		static bool validate_xor_ri(TestParameters);

	public:
		X86BuilderValidator();
	};
}

#endif
