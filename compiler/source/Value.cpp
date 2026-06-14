import std;

#include <allocator/Value.h>
#include <toolchain/Generator.h>

// Haze Value.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	//////////////////////////////////////////////////////
	// Register Value
	//////////////////////////////////////////////////////

	ValueKind RegisterValue::value_kind() const
	{
		return ValueKind::REGISTER;
	}

	std::string RegisterValue::format() const
	{
		return std::format("r{}", index);
	}

	void RegisterValue::load_into(Generator& generator, Register destination) const
	{
		if (destination != index)
		{
			generator.make_copy(destination, index);
		}
	}

	void RegisterValue::store_from(Generator& generator, Register source) const
	{
		if (source != index)
		{
			generator.make_copy(index, source);
		}
	}


	//////////////////////////////////////////////////////
	// Stack Value
	//////////////////////////////////////////////////////

	ValueKind StackValue::value_kind() const
	{
		return ValueKind::STACK;
	}

	std::string StackValue::format() const
	{
		const auto architecture_type = USE_UNSAFE(CommandLineOptions)->architecture;
		const auto format = get_stack_frame_pointer(architecture_type);

		return std::format("[{} + {}]", format, index);
	}

	void StackValue::load_into(Generator& generator, Register destination) const
	{
		generator.stack_read(destination, index);
	}

	void StackValue::store_from(Generator& generator, Register source) const
	{
		generator.stack_write(index, source);
	}


	//////////////////////////////////////////////////////
	// Static Value
	//////////////////////////////////////////////////////

	ValueKind StaticValue::value_kind() const
	{
		return ValueKind::STATIC;
	}

	std::string StaticValue::format() const
	{
		static constexpr auto bits_per_hex_digit = std::countr_zero(16u);
		static constexpr auto hex_digits = (sizeof(Address) * std::numeric_limits<unsigned char>::digits + bits_per_hex_digit - 1) / bits_per_hex_digit;

		// NOTE: +2 required to account for the `0x` prefix
		return std::format("[{:#0{}x}]", index, hex_digits + 2);
	}

	void StaticValue::load_into(Generator& generator, Register destination) const
	{
		generator.heap_read(destination, index);
	}

	void StaticValue::store_from(Generator& generator, Register source) const
	{
		USE_SAFE(ErrorReporter)->post_uncorrectable("cannot write to static (read-only) data region", NULL_TOKEN);
	}
}
