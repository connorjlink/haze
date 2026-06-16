#ifndef HAZE_VARIABLE_H
#define HAZE_VARIABLE_H

#include <ast/expression/defs/ExpressionKind.h>
#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>
#include <runtime/defs/VariableKind.h>
#include <type/Type.h>
#include <utility/ExtendedInteger.h>
#include <utility/Sum.h>

// Haze Variable.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Variable)
	FORWARD_DECLARE_SUM(Expression)

#define VARIABLE_METHODS(X, handlet) \
	X(variable_kind, VariableKind) \
	X(format, std::string) \
	X(value, ExpressionHandle) \
	X(is_truthy, bool) \
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
		using Storage = VariableStorage;

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
		VariableKind variable_kind() const;
		std::string format() const;
		ExpressionHandle value(const VariableStorage&) const;
		bool is_truthy() const;
		TypeHandle get_type(const TypeStorage&) const;

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
		VariableKind variable_kind() const;
		std::string format() const;
		ExpressionHandle value(const VariableStorage&) const;
		bool is_truthy() const;
		TypeHandle get_type(const TypeStorage&) const;

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
		VariableKind variable_kind() const;
		std::string format() const;
		ExpressionHandle value(const VariableStorage&) const;
		bool is_truthy() const;
		TypeHandle get_type(const TypeStorage&) const;

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
		std::unordered_map<std::string, ExpressionHandle> members;

	public:
		VariableKind variable_kind() const;
		std::string format() const;
		ExpressionHandle value(const VariableStorage&) const;
		bool is_truthy() const;
		TypeHandle get_type(const TypeStorage&) const;

	public:
		StructOrUnionVariable(TypeHandle, std::unordered_map<std::string, ExpressionHandle>);
	};
#define MAKE_STRUCT_OR_UNION_VARIABLE(type, members) StructOrUnionVariable{ type, members }

	struct EnumVariable : public VariableBase
	{
	private:
		std::string_view enumerator;

	public:
		VariableKind variable_kind() const;
		std::string format() const;
		ExpressionHandle value(const VariableStorage&) const;
		bool is_truthy() const;
		TypeHandle get_type(const TypeStorage&) const;

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

	template<typename SumMemberT, typename StorageT>
	concept IsVariable = SumTuple<SumMemberT, StorageT, VariableMethods<typename StorageT::Anchor>>;

	using VariableKinds = SumTypeList
	<
#define X(enumerator, type, name) type,
		VARIABLE_KINDS(X)
#undef X
		void
	>;


	using VariableSumImplementation = MakeSum<VariableMethods, VariableKinds>::Type;

	struct VariableStorage : public VariableSumImplementation::Storage
	{
		using VariableSumImplementation::Storage::Storage;

		using Type = VariableSumImplementation::Type;
		using Anchor = VariableSumImplementation::Anchor;
	};
}

#endif
