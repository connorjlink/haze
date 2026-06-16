#ifndef HAZE_VALUESUM_H
#define HAZE_VALUESUM_H

#include <allocator/defs/ValueKind.h>
#include <cli/CommandLineOptions.h>
#include <cli/defs/ArchitectureKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <utility/Constants.h>
#include <utility/Sum.h>
#include <utility/Typing.h>

// Haze Value.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Generator;

	FORWARD_DECLARE_SUM(Value)

#define VALUE_METHODS(X, handlet) \
	X(value_kind, ValueKind) \
	X(format,     std::string) \
	X(load_into,  void) \
	X(store_from, void)

	DEFINE_SUM(Value, VALUE_METHODS)
	

	struct ValueBase 
		: public ValueFacade
		, public InjectSingleton<CommandLineOptions, ErrorReporter>
	{
	public:
		using Storage = ValueStorage;
	};
}

namespace hz
{
	//////////////////////////////////////////////////////
	// Register Value
	//////////////////////////////////////////////////////

	struct RegisterValue : public ValueBase
	{
	public:
		Register index;

	public:
		ValueKind value_kind() const;
		std::string format() const;
		void load_into(Generator&, Register) const;
		void store_from(Generator&, Register) const;
	};


	//////////////////////////////////////////////////////
	// Stack Value
	//////////////////////////////////////////////////////

	struct StackValue : public ValueBase
	{
	public:
		// relative offset from the base pointer, negative is below the current stack frame
		Offset index;

	public:
		ValueKind value_kind() const;
		std::string format() const;
		void load_into(Generator&, Register) const;
		void store_from(Generator&, Register) const;
	};


	//////////////////////////////////////////////////////
	// Static Value
	//////////////////////////////////////////////////////

	struct StaticValue : public ValueBase
	{
	public:
		Address index;

	public:
		ValueKind value_kind() const;
		std::string format() const;
		void load_into(Generator&, Register) const;
		void store_from(Generator&, Register) const;
	};


	//////////////////////////////////////////////////////
	// All Values
	//////////////////////////////////////////////////////

	template<typename SumMemberT, typename StorageT>
	concept IsValue = SumTuple<SumMemberT, StorageT, ValueMethods<typename StorageT::Anchor>>;
	
	using ValueKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		VALUE_KINDS(X)
#undef X
		void
	>;


	using ValueSumImplementation = MakeSum<ValueMethods, ValueKinds>::Type;

	struct ValueStorage : public ValueSumImplementation::Storage
	{
		using ValueSumImplementation::Storage::Storage;

		using Type = ValueSumImplementation::Type;
		using Anchor = ValueSumImplementation::Anchor;
	};
}

#endif
