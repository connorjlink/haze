#ifndef HAZE_ARCHITECTURETYPE_H
#define HAZE_ARCHITECTURETYPE_H

#include <error/ErrorReporter.h>
#include <riscv/defs/RISCVRegister.h>
#include <utility/AutoEnum.h>
#include <utility/Constants.h>
#include <x86/defs/X86Register.h>

// Haze ArchitectureKind.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#define ARCHITECTURE_KINDS(X) \
	X(X86, x86) \
	X(RISCV, risc-v)


#define AUTOENUM_ROUTER(X, enumerator, name) X(enumerator, FakeType, name, ArchitectureKind)

	DEFINE_ENUM(ARCHITECTURE_KINDS, ArchitectureKind, architecture kind)

#undef AUTOENUM_ROUTER


	// returns the pair <minimum index, maximum index>
	constexpr std::pair<std::size_t, std::size_t> get_register_extrema(ArchitectureKind kind)
	{
		using enum ArchitectureKind;
		switch (kind)
		{
			case X86: return std::make_pair(0, 7); // eax, ..., edi
			case RISCV: return std::make_pair(1, 31); // x1, ..., x31
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"invalid architecture kind `{}`", kind), NULL_TOKEN);
	}

	constexpr Register get_abi_return_register(ArchitectureKind kind)
	{
		using enum ArchitectureKind;
		switch (kind)
		{
			case X86: return to_register(X86Register::EAX);
			case RISCV: return to_register(RISCVRegister::A0);
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"invalid architecture kind `{}`", kind), NULL_TOKEN);
	}

#define NAMEOF(x) #x

	constexpr std::string_view get_stack_frame_pointer(ArchitectureKind kind)
	{
		using enum ArchitectureKind;
		switch (kind)
		{
			case X86: return NAMEOF(EBP);
			case RISCV: return NAMEOF(FP);
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"invalid architecture kind `{}`", kind), NULL_TOKEN);
	}

	constexpr Address get_linker_origin(ArchitectureKind kind)
	{
		using enum ArchitectureKind;
		switch (kind)
		{
			case X86: return 0x401000; // default image base for PE files
			case RISCV: return 0x10000; // arbitrary choice aligned with RARS
		}

		USE_UNSAFE(ErrorReporter)->post_uncorrectable(std::format(
			"invalid architecture kind `{}`", kind), NULL_TOKEN);
	}

#undef NAMEOF
}

#endif 
