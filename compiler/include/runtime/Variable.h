#ifndef HAZE_VARIABLE_H
#define HAZE_VARIABLE_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Context.h>
#include <runtime/defs/VariableKind.h>
#include <type/Type.h>
#include <utility/Sum.h>

// Haze Variable.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Variable);

#define VARIABLE_METHODS(X, handlet) \
	X(value, ExpressionHandle) \
	X(get_type, TypeHandle)

	DEFINE_SUM(Variable, VARIABLE_METHODS)


	struct VariableBase
		: public VariableFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		template<typename T>
		using Dispatcher = VariableSumDispatcher<T>;
		using Storage = VariableSumStorage;

	public:
		template<typename Self>
		VariableKind variable_kind(this Self&&);
	};
}

namespace hz
{
	//////////////////////////////////////////////////////
	// All Variables
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept IsVariable = SumTuple<SumMemberT, SumStorageT, TypeMethods<SumStorageT>>;

	using VariableKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		VARIABLE_KINDS(X)
#undef X
		void
	>;

	using VariableSumImplementation = MakeSum<VariableMethods, VariableKinds>::Type;

	struct VariableSumStorage : public VariableSumImplementation::Storage
	{
		using VariableSumImplementation::Storage::Storage;

		using Type = VariableSumImplementation::Type;
		using Anchor = VariableSumImplementation::Anchor;
	};


	template<typename Self>
	VariableKind VariableBase::variable_kind(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, type, name) case TypeIndexV<type, VariableSumStorage::Type>: return VariableKind::enumerator;
			VARIABLE_KINDS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid type tag `{}`", self.tag_type()), NULL_TOKEN);

		return VariableKind::VOID;
	}
}

#endif
