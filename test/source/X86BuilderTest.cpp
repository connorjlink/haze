#include <cstddef>

#include <vector>
#include <string>
#include <string_view>
#include <format>
#include <utility>

#include <x86/X86Builder.h>
#include <x86/defs/X86Register.h>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace hz
{
	using enum X86Register;

	TEST_CLASS(X86BuilderTest)
	{
	public:
		TEST_METHOD(ValidateModrm)
		{
			// NOTE: the operand order is reversed in the Mod R/M encoding compared to the assembly syntax
			// Intel ASM: [opcode] [destination], [source]
			// Mod R/M: [mod] [reg (source)] [r/m (destination)]

			Assert::AreEqual(0x00, static_cast<int>(X86Builder::modrm(0b00, 0b000, 0b000)));
			Assert::AreEqual(0xFF, static_cast<int>(X86Builder::modrm(0b11, 0b111, 0b111)));

			// eax, eax
			Assert::AreEqual(0xC0, static_cast<int>(X86Builder::modrm(0b11, to_register(EAX), to_register(EAX))));
			// ebx, ebx
			Assert::AreEqual(0xDB, static_cast<int>(X86Builder::modrm(0b11, to_register(EBX), to_register(EBX))));
			// edi, edx
			Assert::AreEqual(0xD7, static_cast<int>(X86Builder::modrm(0b11, to_register(EDX), to_register(EDI))));
			// esp, esi
			Assert::AreEqual(0x74, static_cast<int>(X86Builder::modrm(0b01, to_register(ESI), to_register(ESP))));
		}

		TEST_METHOD(ValidateModrmRr)
		{
			// eax, ecx
			Assert::AreEqual(0xC8, static_cast<int>(X86Builder::modrm_rr(to_register(EAX), to_register(ECX))));
			// edx, ebx
			Assert::AreEqual(0xDA, static_cast<int>(X86Builder::modrm_rr(to_register(EDX), to_register(EBX))));
			// esi, eax
			Assert::AreEqual(0xC6, static_cast<int>(X86Builder::modrm_rr(to_register(ESI), to_register(EAX))));
			// ebp, edi
			Assert::AreEqual(0xFD, static_cast<int>(X86Builder::modrm_rr(to_register(EBP), to_register(EDI))));
			// ecx, esp
			Assert::AreEqual(0xE1, static_cast<int>(X86Builder::modrm_rr(to_register(ECX), to_register(ESP))));
			// edi, esi
			Assert::AreEqual(0xF7, static_cast<int>(X86Builder::modrm_rr(to_register(EDI), to_register(ESI))));
		}

		TEST_METHOD(ValidateSib)
		{
			Assert::AreEqual(0x00, static_cast<int>(X86Builder::sib(0b00, 0b000, 0b000)));
			Assert::AreEqual(0xFF, static_cast<int>(X86Builder::sib(0b11, 0b111, 0b111)));

			// eax, eax
			Assert::AreEqual(0xC0, static_cast<int>(X86Builder::sib(0b11, to_register(EAX), to_register(EAX))));
			// ebx, ebx
			Assert::AreEqual(0xDB, static_cast<int>(X86Builder::sib(0b11, to_register(EBX), to_register(EBX))));
			// edi, edx
			Assert::AreEqual(0xD7, static_cast<int>(X86Builder::sib(0b11, to_register(EDX), to_register(EDI))));
			// esp, esi
			Assert::AreEqual(0x74, static_cast<int>(X86Builder::sib(0b01, to_register(ESI), to_register(ESP))));
		}
	};
}
