#ifndef HAZE_VALUESUM_H
#define HAZE_VALUESUM_H

#include <allocator/defs/ValueKind.h>
#include <cli/CommandLineOptions.h>
#include <cli/defs/ArchitectureKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/Generator.h>
#include <utility/Constants.h>
#include <utility/Sum.h>
#include <utility/Typing.h>

// Haze Value.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

#define VALUE_METHODS(X, handlet) \
	X(tag_type,   ValueKind) \
	X(format,     std::string) \
	X(load_into,  void) \
	X(store_from, void)

	FORWARD_DECLARE_SUM(Value)

	// expose a strict polymorphic interface for values
	template<typename AnchorT>
	using ValueMethods = AllButLastT
	<
#define X(name, handlet) METHOD_TUPLE_ENTRY(name, handlet)
		VALUE_METHODS(X, ValueHandle)
#undef X
		void
	>;

	DEFINE_SUM(Value, VALUE_METHODS)
	

	class ValueBase : public ValueFacade
	{
	public:
		using Storage = ValueSumStorage;

		template<typename Self>
		ValueKind value_kind(this Self&&);
	};
}

namespace hz
{
	class RegisterValue : public ValueBase
	{
	public:
		Register index;

	public:
		ValueKind tag_type() const
		{
			return ValueKind::REGISTER;
		}

		std::string format() const
		{
			return std::format("r{}", index);
		}

		void load_into(Generator& generator, Register destination) const
		{
			if (destination != index)
			{
				generator.make_copy(destination, index);
			}
		}

		void store_from(Generator& generator, Register source) const
		{
			if (source != index)
			{
				generator.make_copy(index, source);
			}
		}
	};

	class StackValue
		: public ValueBase
		, public InjectSingleton<CommandLineOptions>
	{
	public:
		// relative offset from the base pointer, negative is below the current stack frame
		Offset index;

	public:
		ValueKind tag_type() const
		{
			return ValueKind::STACK;
		}

		std::string format() const
		{
			const auto architecture_type = USE_SAFE(CommandLineOptions)->architecture;
			const auto format = get_stack_frame_pointer(architecture_type);

			return std::format("[{} + {}]", format, index);
		}

		void load_into(Generator& generator, Register destination) const
		{
			generator.stack_read(destination, index);
		}

		void store_from(Generator& generator, Register source) const
		{
			generator.stack_write(index, source);
		}
	};

	class StaticValue
		: public ValueBase
		, public InjectSingleton<ErrorReporter>
	{
	public:
		Address index;

	public:
		ValueKind tag_type() const
		{
			return ValueKind::STATIC;
		}

		std::string format() const
		{
			static constexpr auto bits_per_hex_digit = std::countr_zero(16u);
			static constexpr auto hex_digits = (sizeof(Address) * std::numeric_limits<unsigned char>::digits + bits_per_hex_digit - 1) / bits_per_hex_digit;

			// NOTE: +2 required to account for the `0x` prefix
			return std::format("[{:#0{}x}]", index, hex_digits + 2);
		}

		void load_into(Generator& generator, Register destination) const
		{
			generator.heap_read(destination, index);
		}

		void store_from(Generator& generator, Register source) const
		{
			USE_SAFE(ErrorReporter)->post_uncorrectable("cannot write to static (read-only) data region", NULL_TOKEN);
		}
	};

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept Value = SumTuple<SumMemberT, SumStorageT, ValueMethods<SumStorageT>>;
	
	// nonfacade types for public consumption
	using ValueKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		VALUE_KINDS(X)
#undef X
		void
	>;

	using ValueSumImplementation = MakeSum<ValueMethods, ValueKinds>::Type;

	struct ValueSumStorage : public ValueSumImplementation::Storage
	{
		using ValueSumImplementation::Storage::Storage;

		using Type = ValueSumImplementation::Type;
		using Anchor = ValueSumImplementation::Anchor;
	};


	template<typename Self>
	ValueKind ValueBase::value_kind(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, associativity, precedence, type, name) case TypeIndexV<type, typename Storage::Type>: return ValueKind::enumerator;
			PRIMARY_EXPRESSION_KINDS(X)
			POSTFIX_EXPRESSION_KINDS(X)
			UNARY_EXPRESSION_KINDS(X)
			BINARY_EXPRESSION_KINDS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid value tag `{}`", self.tag_type()), self.token);

		// error recovery does not care about value kind
		return ValueKind::STACK;
	}
}

#endif
