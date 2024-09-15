#include "X86BuilderValidator.h"
#include "X86Builder.h"
#include "ErrorReporter.h"

import std;

// Haze X86BuilderValidator.cpp
// (c) Connor J. Link. All Rights Reserved.

// `x` is the error context
// `v` is the expected value
// `e` is the variable against which to compare
#define VALIDATE_IMPL(x, v, e, a, b, p) if (e != v) \
{ \
	_error_reporter->post_error(parameters.context, parameters.file, \
		std::format(x " != {} (actually " p, a, b), NULL_TOKEN); \
	had_error = true; \
}

#define VALIDATE(x, v, e) VALIDATE_IMPL(x, v, e, v, e, "0x{:02X})")
#define VALIDATE_CONVERT(x, v, e) VALIDATE_IMPL(x, v, e, ::bytestring(v), ::bytestring(e), "{})")

namespace
{
	constexpr auto _FILENAME = "X86Builder.cpp";

	using namespace hz;

	std::string bytestring(byterange bytes)
	{
		std::string result = "{ ";

		for (auto byte : bytes)
		{
			result += std::format("0x{:02X}, ", byte);
		}

		result = result.substr(0, result.length() - 2);

		return result + " }";
	}
}

namespace hz
{
	X86BuilderValidator::X86BuilderValidator()
	{
		//generic mod/rm byte encoding synthesis
		auto parameters0 = _error_reporter->open_context(_FILENAME, "validating modrm()");
		auto test0 = Test{ "modrm()", validate_modrm };
		test0.attach(parameters0);
		add_test(test0);

		//reg/reg-specific mod/rm byte encoding synthesis
		auto parameters1 = _error_reporter->open_context(_FILENAME, "validating modrm_rr()");
		auto test1 = Test{ "modrm_rr()", validate_modrm_rr };
		test1.attach(parameters1);
		add_test(test1);

		//generic sib byte encoding synthesis
		auto parameters2 = _error_reporter->open_context(_FILENAME, "validating sib()");
		auto test2 = Test{ "sib()", validate_sib };
		test2.attach(parameters2);
		add_test(test2);

		//push 32-bit register encoding synthesis
		auto parameters3 = _error_reporter->open_context(_FILENAME, "validating push_r()");
		auto test3 = Test{ "push_r()", validate_push_r };
		test3.attach(parameters3);
		add_test(test3);

		//push 8-bit immediate encoding synthesis
		auto parameters4 = _error_reporter->open_context(_FILENAME, "validating push_i()");
		auto test4 = Test{ "push_i()", validate_push_i };
		test4.attach(parameters4);
		add_test(test4);

		//push 32-bit immediate (memory effective address) encoding synthesis
		auto parameters5 = _error_reporter->open_context(_FILENAME, "validating push_m()");
		auto test5 = Test{ "push_m()", validate_push_m };
		test5.attach(parameters5);
		add_test(test5);
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

	bool X86BuilderValidator::validate_push_r(TestParameters parameters)
	{
		auto had_error = false;

		// 0x50 - eax
		auto case0 = X86Builder::push_r(EAX);
		byterange expected0 = { 0x50 };
		VALIDATE_CONVERT("push_r(EAX)", expected0, case0);

		// 0x51 - ecx
		auto case1 = X86Builder::push_r(ECX);
		byterange expected1 = { 0x51 };
		VALIDATE_CONVERT("push_r(ECX)", expected1, case1);

		// 0x53 - ebx
		auto case2 = X86Builder::push_r(EBX);
		byterange expected2 = { 0x53 };
		VALIDATE_CONVERT("push_r(EBX)", expected2, case2);

		// 0x54 - esp
		auto case3 = X86Builder::push_r(ESP);
		byterange expected3 = { 0x54 };
		VALIDATE_CONVERT("push_r(ESP)", expected3, case3);

		// 0x56 - esi
		auto case4 = X86Builder::push_r(ESI);
		byterange expected4 = { 0x56 };
		VALIDATE_CONVERT("push_r(ESI)", expected4, case4);

		// 0x57 - edi
		auto case5 = X86Builder::push_r(EDI);
		byterange expected5 = { 0x57 };
		VALIDATE_CONVERT("push_r(EDI)", expected5, case5);

		return had_error;
	}

	bool X86BuilderValidator::validate_push_i(TestParameters parameters)
	{
		auto had_error = false;

		// 0x00 - 0
		auto case0 = X86Builder::push_i(0x00);
		byterange expected0 = { 0x6A, 0x00 };
		VALIDATE_CONVERT("push_i(0x00)", expected0, case0);

		// 0x11 - +17
		auto case1 = X86Builder::push_i(0x11);
		byterange expected1 = { 0x6A, 0x11 };
		VALIDATE_CONVERT("push_i(0x11)", expected1, case1);

		// 0x7F - +127
		auto case2 = X86Builder::push_i(0x7F);
		byterange expected2 = { 0x6A, 0x7F };
		VALIDATE_CONVERT("push_i(0x7F)", expected2, case2);

		// 0x80 - -128
		auto case3 = X86Builder::push_i(0x80);
		byterange expected3 = { 0x6A, 0x80 };
		VALIDATE_CONVERT("push_i(0x80)", expected3, case3);

		// 0xBB - -69
		auto case4 = X86Builder::push_i(0xBB);
		byterange expected4 = { 0x6A, 0xBB };
		VALIDATE_CONVERT("push_i(0xBB)", expected4, case4);

		// 0xFF - -1
		auto case5 = X86Builder::push_i(0xFF);
		byterange expected5 = { 0x6A, 0xFF };
		VALIDATE_CONVERT("push_i(0xFF)", expected5, case5);

		return had_error;
	}

	bool X86BuilderValidator::validate_push_m(TestParameters parameters)
	{
		auto had_error = false;

		// 0x00000000 - 0
		auto case0 = X86Builder::push_m(0x00000000);
		byterange expected0 = { 0x68, 0x00, 0x00, 0x00, 0x00 };
		VALIDATE_CONVERT("push_m(0x00000000)", expected0, case0);

		// 0x000000FF - 255
		auto case1 = X86Builder::push_m(0x000000FF);
		byterange expected1 = { 0x68, 0xFF, 0x00, 0x00, 0x00 };
		VALIDATE_CONVERT("push_m(0x000000FF)", expected1, case1);

		// 0x0000FF00 - 65280
		auto case2 = X86Builder::push_m(0x0000FF00);
		byterange expected2 = { 0x68, 0x00, 0xFF, 0x00, 0x00 };
		VALIDATE_CONVERT("push_m(0x0000FF00)", expected2, case2);

		// 0x00FF0000 - 16711680
		auto case3 = X86Builder::push_m(0x00FF0000);
		byterange expected3 = { 0x68, 0x00, 0x00, 0xFF, 0x00 };
		VALIDATE_CONVERT("push_m(0x00FF0000)", expected3, case3);

		// 0xFF000000 - -16777216
		auto case4 = X86Builder::push_m(0xFF000000);
		byterange expected4 = { 0x68, 0x00, 0x00, 0x00, 0xFF };
		VALIDATE_CONVERT("push_m(0xFF000000)", expected4, case4);

		// 0x12345678 - 305419896
		auto case5 = X86Builder::push_m(0x12345678);
		byterange expected5 = { 0x68, 0x78, 0x56, 0x34, 0x12 };
		VALIDATE_CONVERT("push_m(0x12345678)", expected5, case5);

		return had_error;
	}

}
