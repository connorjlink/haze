#ifndef HAZE_TYPE_H
#define HAZE_TYPE_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <toolchain/models/Token.h>
#include <type/defs/StorageClass.h>
#include <type/defs/TypeKind.h>
#include <type/defs/TypeSignedness.h>
#include <type/defs/TypeQualifier.h>
#include <utility/Constants.h>
#include <utility/Sum.h>

// Haze Type.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Type)

#define TYPE_METHODS(X, handlet) \
	X(size, Offset) \
	X(is_complete, bool) \
	X(type_kind, TypeKind)

	DEFINE_SUM(Type, TYPE_METHODS)


	struct TypeBase 
		: public TypeFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		template<typename T>
		using Dispatcher = TypeSumDispatcher<T>;
		using Storage = TypeSumStorage;

		template<typename Self>
		TypeKind type_kind2(this Self&&);

		TypeKind type_kind() const
		{
			return const_cast<TypeBase*>(this)->type_kind2();
		}
	};
}

// NOTE: this secondary include block is mandatory to allow the incomplete type facade
#include <type/VoidType.h>
#include <type/IntType.h>
#include <type/FloatType.h>
#include <type/PointerType.h>
#include <type/StructOrUnionType.h>
#include <type/EnumType.h>
#include <type/TypedefNameType.h>
#include <type/ArrayType.h>
#include <type/FunctionType.h>

namespace hz
{
	//////////////////////////////////////////////////////
	// All Types
	//////////////////////////////////////////////////////

	// not for public consumption
	template<typename SumMemberT, typename SumStorageT>
	concept IsType = SumTuple<SumMemberT, SumStorageT, TypeMethods<SumStorageT>>;

	using TypeKinds = SumTypeList
	<
#define X(enumerator, precedence, type, name) type,
		TYPE_KINDS(X)
#undef X
		void
	>;

	using TypeSumImplementation = MakeSum<TypeMethods, TypeKinds>::Type;

	struct TypeSumStorage : public TypeSumImplementation::Storage
	{
		using TypeSumImplementation::Storage::Storage;

		using Type = TypeSumImplementation::Type;
		using Anchor = TypeSumImplementation::Anchor;
	};


	template<typename Self>
	TypeKind TypeBase::type_kind2(this Self&& self)
	{
		switch (self.tag_type())
		{
#define X(enumerator, precedence, name, type) case TypeIndexV<TypeKind::enumerator, typename Storage::Type>: return TypeKind::enumerator;
			TYPE_KINDS(X)
#undef X
		}

		USE_SAFE(ErrorReporter)->post_error(std::format(
			"invalid type tag `{}`", self.tag_type()), NULL_TOKEN);

		return TypeKind::VOID;
	}
}

#endif
