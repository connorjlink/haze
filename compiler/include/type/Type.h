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
	FORWARD_DECLARE_SUM(Type)

#define TYPE_METHODS(X, handlet) \
	X(type_kind, TypeKind) \
	X(size, Offset) \
	X(is_complete, bool)

	DEFINE_SUM(Type, TYPE_METHODS)


	struct TypeBase 
		: public TypeFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		template<typename T>
		using Dispatcher = TypeSumDispatcher<T>;
		using Storage = TypeStorage;
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

	template<typename SumMemberT, typename StorageT>
	concept IsType = SumTuple<SumMemberT, StorageT, TypeMethods<typename StorageT::Anchor>>;

	using TypeKinds = SumTypeList
	<
#define X(enumerator, precedence, type, name) type,
		TYPE_KINDS(X)
#undef X
		void
	>;

	using TypeSumImplementation = MakeSum<TypeMethods, TypeKinds>::Type;

	struct TypeStorage : public TypeSumImplementation::Storage
	{
		using TypeSumImplementation::Storage::Storage;

		using Type = TypeSumImplementation::Type;
		using Anchor = TypeSumImplementation::Anchor;
	};
}

#endif
