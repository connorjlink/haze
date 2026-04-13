#ifndef HAZE_X86BUILDERVALIDATOR_H
#define HAZE_X86BUILDERVALIDATOR_H

#include <validator/Validator.h>

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

	public:
		X86BuilderValidator();
	};
}

#endif
