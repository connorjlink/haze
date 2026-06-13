#ifndef HAZE_VARIABLE_H
#define HAZE_VARIABLE_H

#include <ast/expression/Expression.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/Context.h>
#include <runtime/defs/VariableKind.h>
#include <type/Type.h>
#include <utility/ExtendedInteger.h>
#include <utility/Sum.h>

// Haze Variable.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// forward declare sum storage and self-referential types for facade

	FORWARD_DECLARE_SUM(Variable)

#define VARIABLE_METHODS(X, handlet) \
	X(value, ExpressionHandle) \
	X(get_type, TypeHandle)

	DEFINE_SUM(Variable, VARIABLE_METHODS)


	struct VariableBase
		: public VariableFacade
		, public InjectSingleton<ErrorReporter>
	{
	protected:
		TypeHandle type;

	public:
		template<typename T>
		using Dispatcher = VariableSumDispatcher<T>;
		using Storage = VariableSumStorage;

	public:
		template<typename Self>
		VariableKind variable_kind(this Self&&);

	public:
		VariableBase(TypeHandle type)
			: type{ type }
		{
		}
	};
}

namespace hz
{
	//////////////////////////////////////////////////////
	// All Variables
	//////////////////////////////////////////////////////

	struct VoidVariable : public VariableBase
	{
	public:
		ExpressionHandle value(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		VoidVariable(TypeHandle type)
			: VariableBase{ type }
		{
		}
	};
#define MAKE_VOID_VARIABLE(type) VoidVariable{ type }

	struct IntegerVariable : public VariableBase
	{
	private:
		ExtendedInteger integer;

	public:
		ExpressionHandle value(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		IntegerVariable(TypeHandle type, ExtendedInteger integer)
			: VariableBase{ type }, integer{ integer }
		{
		}
	};
#define MAKE_INTEGER_VARIABLE(type, integer) IntegerVariable{ type, integer }

	struct FloatVariable : public VariableBase
	{
	private:
		double floating_point;

	public:
		ExpressionHandle value(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		FloatVariable(TypeHandle type, double floating_point)
			: VariableBase{ type }, floating_point{ floating_point }
		{
		}
	};
#define MAKE_FLOAT_VARIABLE(type, floating_point) FloatVariable{ type, floating_point }

	struct StructOrUnionVariable : public VariableBase
	{
	private:
		std::unordered_map<std::string, ExpressionHandle> member_values;

	public:
		ExpressionHandle value(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		StructOrUnionVariable(TypeHandle type, std::unordered_map<std::string, ExpressionHandle> member_values)
			: VariableBase{ type }, member_values{ std::move(member_values) }
		{
		}
	};
#define MAKE_STRUCT_OR_UNION_VARIABLE(type, member_values) StructOrUnionVariable{ type, member_values }

	struct EnumVariable : public VariableBase
	{
	private:
		std::string_view enumerator;

	public:
		ExpressionHandle value(const Storage&) const;
		TypeHandle get_type(const Storage&) const;

	public:
		EnumVariable(TypeHandle type, std::string_view enumerator)
			: VariableBase{ type }, enumerator{ enumerator }
		{
		}
	};
#define MAKE_ENUM_VARIABLE(type, enumerator) EnumVariable{ type, enumerator }



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
