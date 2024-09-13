#include "X86BuilderValidator.h"
#include "X86Builder.h"
#include "ErrorReporter.h"

#include <format>

// Haze X86BuilderValidator.cpp
// (c) Connor J. Link. All Rights Reserved.

// `x` is the error context
// `v` is the expected value
// `e` is the variable against which to compare
#define VALIDATE(x, v, e) if (e != v) \
{ \
	_error_reporter->post_error(std::format("{} != 0x{:02X} (actually 0x{:02X})", x, v, e), NULL_TOKEN); \
	had_error = true; \
}

namespace
{
	constexpr auto _FILENAME = "X86Builder.cpp";
}

namespace hz
{
	X86BuilderValidator::X86BuilderValidator()
	{
		//generic mod/rm byte encoding synthesis
		auto context0 = _error_reporter->open_context(_FILENAME, "validating modrm()");
		auto test0 = Test{ "modrm()", validate_modrm };
		test0.attach(context0);
		add_test(test0);

		//reg/reg-specific mod/rm byte encoding synthesis
		auto context1 = _error_reporter->open_context(_FILENAME, "validating modrm_rr()");
		auto test1 = Test{ "modrm_rr()", validate_modrm_rr };
		test1.attach(context1);
		add_test(test1);

		//generic sib byte encoding synthesis
		auto parameters2 = _error_reporter->open_context(_FILENAME, "validating sib()");
		auto test2 = Test{ "sib()", validate_sib };
		test2.attach(parameters2);
		add_test(test2);
	}


	bool X86BuilderValidator::validate_modrm(TestParameters parameters)
	{
		auto had_error = false;

		// NOTE: the operand order is reversed in the Mod R/M encoding compared to the assembly syntax
		// Intel ASM: [opcode] [destination], [source]
		// Mod R/M: [mod] [reg (source)] [r/m (destination)]

		// 0x00 - n/a
		auto case0 = X86Builder::modrm(0b00, 0b000, 0b000);
		VALIDATE("modrm(0b00, 0b000, 0b000)", 0x00, case0);

		// 0xFF - n/a
		auto case1 = X86Builder::modrm(0b11, 0b111, 0b111);
		VALIDATE("modrm(0b11, 0b111, 0b111)", 0xFF, case1);

		// 0xC0 - eax, eax
		auto case2 = X86Builder::modrm(0b11, EAX, EAX);
		VALIDATE("modrm(0b11, EAX, EAX)", 0xC0, case2);

		// 0xDB - ebx, ebx
		auto case3 = X86Builder::modrm(0b11, EBX, EBX);
		VALIDATE("modrm(0b11, EBX, EBX)", 0xDB, case3);

		// 0xD7 - edi, edx
		auto case4 = X86Builder::modrm(0b11, EDX, EDI);
		VALIDATE("modrm(0b11, EDX, EDI)", 0xD7, case4);

		// 0x74 - esp, esi
		auto case5 = X86Builder::modrm(0b01, ESI, ESP);
		VALIDATE("modrm(0b01, ESI, ESP)", 0x74, case5);

		return had_error;
	}

	bool X86BuilderValidator::validate_modrm_rr(TestParameters parameters)
	{
		auto had_error = false;

		// NOTE: See X86BuilderValidator::validate_modrm() for more information
		// on operand ordering -- reversed compared to Intel ASM syntax

		// 0xC8 - eax, ecx
		auto case0 = X86Builder::modrm_rr(EAX, ECX);
		VALIDATE("modrm_rr(EAX, ECX)", 0xC8, case0);

		// 0xDA - edx, ebx
		auto case1 = X86Builder::modrm_rr(EDX, EBX);
		VALIDATE("modrm_rr(EDX, EBX)", 0xDA, case1);

		// 0xC6 - esi, eax
		auto case2 = X86Builder::modrm_rr(ESI, EAX);
		VALIDATE("modrm_rr(ESI, EAX)", 0xC6, case2);

		// 0xFD - ebp, edi
		auto case3 = X86Builder::modrm_rr(EBP, EDI);
		VALIDATE("modrm_rr(EBP, EDI)", 0xFD, case3);

		// 0xE1 - ecx, esp
		auto case4 = X86Builder::modrm_rr(ECX, ESP);
		VALIDATE("modrm_rr(ECX, ESP)", 0xE1, case4);

		// 0xF7 - edi, esi
		auto case5 = X86Builder::modrm_rr(EDI, ESI);
		VALIDATE("modrm_rr(EDI, ESI)", 0xF7, case5);

		return had_error;
	}

	bool X86BuilderValidator::validate_sib(TestParameters parameters)
	{
		auto had_error = false;

		// 0x00 - n/a
		auto case0 = X86Builder::sib(0b00, 0b000, 0b000);
		VALIDATE("sib(0b00, 0b000, 0b000)", 0x00, case0);

		// 0xFF - n/a
		auto case1 = X86Builder::sib(0b11, 0b111, 0b111);
		VALIDATE("sib(0b11, 0b111, 0b111)", 0xFF, case1);

		// 0xC0 - eax, eax
		auto case2 = X86Builder::sib(0b11, EAX, EAX);
		VALIDATE("sib(0b11, EAX, EAX)", 0xC0, case2);

		// 0xDB - ebx, ebx
		auto case3 = X86Builder::sib(0b11, EBX, EBX);
		VALIDATE("sib(0b11, EBX, EBX)", 0xDB, case3);

		// 0xD7 - edi, edx
		auto case4 = X86Builder::sib(0b11, EDX, EDI);
		VALIDATE("sib(0b11, EDX, EDI)", 0xD7, case4);

		// 0x74 - esp, esi
		auto case5 = X86Builder::sib(0b01, ESI, ESP);
		VALIDATE("sib(0b01, ESI, ESP)", 0x74, case5);

		return had_error;
	}
}
