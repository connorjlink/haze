#ifndef HAZE_X86BUILDERVALIDATOR_H
#define HAZE_X86BUILDERVALIDATOR_H

#include "Validator.h"

// Haze X86BuilderValidator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ErrorFrame;

	class X86BuilderValidator : public Validator
	{
	private:
		static bool validate_modrm(ErrorFrame);
		static bool validate_modrm_rr(ErrorFrame);

	private:
		static bool validate_sib(ErrorFrame);

	private:
		static bool validate_push_r(ErrorFrame);
		static bool validate_push_i(ErrorFrame);
		static bool validate_push_m(ErrorFrame);

	private:
		static bool validate_pop_r(ErrorFrame);

	private:
		static bool validate_mov_rr(ErrorFrame);
		static bool validate_mov_ro(ErrorFrame);

	private:
		static bool validate_add_rr(ErrorFrame);
		static bool validate_add_ri(ErrorFrame);

	private:
		static bool validate_sub_rr(ErrorFrame);
		static bool validate_sub_ri(ErrorFrame);

	private:
		static bool validate_or_rr(ErrorFrame);
		static bool validate_or_ri(ErrorFrame);

	private:
		static bool validate_and_rr(ErrorFrame);
		static bool validate_and_ri(ErrorFrame);

	private:
		static bool validate_xor_rr(ErrorFrame);
		static bool validate_xor_ri(ErrorFrame);

	public:
		X86BuilderValidator();
	};
}

#endif
