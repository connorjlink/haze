#ifndef HAZE_INTERMEDIATECOMMAND_H
#define HAZE_INTERMEDIATECOMMAND_H

#include <allocator/Allocator.h>
#include <data/DependencyInjector.h>
#include <error/CommonErrors.h>
#include <ir/defs/IntermediateCommandKind.h>
#include <ir/defs/IntermediateTypeKind.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <utility/Constants.h>
#include <utility/Sum.h>

// Haze IntermediateCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	FORWARD_DECLARE_SUM(Command)
	FORWARD_DECLARE_SUM(IntermediateCommand)

#define INTERMEDIATE_COMMAND_METHODS(X, handlet) \
	X(intermediate_kind, IntermediateCommandKind) \
	X(emit, std::vector<CommandHandle>) \
	X(bytes, Address)

	DEFINE_SUM(IntermediateCommand, INTERMEDIATE_COMMAND_METHODS)


	struct IntermediateCommandBase
		: public IntermediateCommandFacade
		, public InjectSingleton<ErrorReporter>
	{
	public:
		template<typename T>
		using Dispatcher = IntermediateCommandSumDispatcher<T>;
		using Storage = IntermediateCommandStorage;
	};
}


namespace hz
{
	struct AllocateIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		Register destination;
		Address count;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		AllocateIntermediateCommand(IntermediateTypeKind kind, Register destination, Address count)
			: kind{ kind }, destination{ destination }, count{ count }
		{
		}
	};
#define MAKE_ALLOCATE_COMMAND(type, destination, count) AllocateIntermediateCommand{ IntermediateTypeKind::type, destination, count }

	struct DereferenceIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		Register destination;
		Register pointer;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		DereferenceIntermediateCommand(IntermediateTypeKind kind, Register destination, Register pointer)
			: kind{ kind }, destination{ destination }, pointer{ pointer }
		{
		}
	};
#define MAKE_DEREFERENCE_COMMAND(type, destination, pointer) DereferenceIntermediateCommand{ IntermediateTypeKind::type, destination, pointer }

	struct ReferenceIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		Register destination;
		Register value;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		ReferenceIntermediateCommand(IntermediateTypeKind kind, Register destination, Register value)
			: kind{ kind }, destination{ destination }, value{ value }
		{
		}
	};
#define MAKE_REFERENCE_COMMAND(type, destination, value) ReferenceIntermediateCommand{ IntermediateTypeKind::type, destination, value }

	struct ImmediateIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		Register destination;
		BigInteger value;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		ImmediateIntermediateCommand(IntermediateTypeKind kind, Register destination, BigInteger value)
			: kind{ kind }, destination{ destination }, value{ value }
		{
		}
	};
#define MAKE_IMMEDIATE_COMMAND(type, destination, value) ImmediateIntermediateCommand{ IntermediateTypeKind::type, destination, value }

	struct CopyIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		Register destination;
		Register source;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		CopyIntermediateCommand(IntermediateTypeKind kind, Register destination, Register source)
			: kind{ kind }, destination{ destination }, source{ source }
		{
		}
	};
#define MAKE_COPY_COMMAND(type, destination, source) CopyIntermediateCommand{ IntermediateTypeKind::type, destination, source }

	struct BinaryIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		BinaryIntermediateCommandKind op;
		Register destination;
		Register left;
		Register right;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		BinaryIntermediateCommand(IntermediateTypeKind kind, BinaryIntermediateCommandKind op, Register destination, Register left, Register right)
			: kind{ kind }, op{ op }, destination{ destination }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_BINARY_COMMAND(type, op, destination, left, right) BinaryIntermediateCommand{ IntermediateTypeKind::type, BinaryIntermediateCommandKind::op, destination, left, right }

	struct UnaryIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		UnaryIntermediateCommandKind op;
		Register destination;
		Register operand;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		UnaryIntermediateCommand(IntermediateTypeKind kind, UnaryIntermediateCommandKind op, Register destination, Register operand)
			: kind{ kind }, op{ op }, destination{ destination }, operand{ operand }
		{
		}
	};
#define MAKE_UNARY_COMMAND(type, op, destination, operand) UnaryIntermediateCommand{ IntermediateTypeKind::type, UnaryIntermediateCommandKind::op, destination, operand }

	struct CompareIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		CompareIntermediateCommandKind op;
		Register destination;
		Register left;
		Register right;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		CompareIntermediateCommand(IntermediateTypeKind kind, CompareIntermediateCommandKind op, Register destination, Register left, Register right)
			: kind{ kind }, op{ op }, destination{ destination }, left{ left }, right{ right }
		{
		}
	};
#define MAKE_COMPARE_COMMAND(type, op, destination, left, right) CompareIntermediateCommand{ IntermediateTypeKind::type, CompareIntermediateCommandKind::op, destination, left, right }

	struct BranchIntermediateCommand : public IntermediateCommandBase
	{
	public:
		Register condition;
		std::string_view label_true;
		std::string_view label_false;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		// unconditional branch
		BranchIntermediateCommand(std::string_view label)
			: condition{ std::numeric_limits<Register>::max() }, label_true{ label }, label_false{ "" }
		{
		}

		// conditional branch
		BranchIntermediateCommand(Register condition, std::string_view label_true, std::string_view label_false)
			: condition{ condition }, label_true{ label_true }, label_false{ label_false }
		{
		}
	};
#define MAKE_BRANCH_COMMAND(condition, label_true, label_false) BranchIntermediateCommand{ condition, label_true, label_false }

	struct CallIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind return_type;
		Register destination;
		std::string_view target_function;
		std::vector<Register> arguments;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		CallIntermediateCommand(Register destination, IntermediateTypeKind return_type, std::string_view target_function, std::vector<Register> arguments)
			: destination{ destination }, return_type{ return_type }, target_function{ target_function }, arguments{ std::move(arguments) }
		{
		}
	};
#define MAKE_CALL_COMMAND(destination, return_type, target_function, arguments) CallIntermediateCommand{ destination, IntermediateTypeKind::return_type, target_function, arguments }

	struct ReturnIntermediateCommand : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind kind;
		Register value;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		ReturnIntermediateCommand(IntermediateTypeKind kind, Register value = std::numeric_limits<Register>::max())
			: kind{ kind }, value{ value }
		{
		}
	};
#define MAKE_RETURN_COMMAND(kind, value) ReturnIntermediateCommand{ IntermediateTypeKind::kind, value }

	struct LabelIntermediateCommand : public IntermediateCommandBase
	{
	public:
		std::string_view label;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		LabelIntermediateCommand(std::string_view label)
			: label{ label }
		{
		}
	};
#define MAKE_LABEL_COMMAND(label) LabelIntermediateCommand{ label }

	template<IntermediateCommandKind KindT>
	struct CastIntermediateCommandBase : public IntermediateCommandBase
	{
	public:
		IntermediateTypeKind extant;
		IntermediateTypeKind target;
		Register destination;
		Register operand;

	public:
		IntermediateCommandKind intermediate_kind() const 
		{
			return KindT;
		}
		ByteRange emit() const;
		Address bytes() const;

	public:
		CastIntermediateCommandBase(IntermediateTypeKind extant, IntermediateTypeKind target, Register destination, Register operand)
			: extant{ extant }, target{ target }, destination{ destination }, operand{ operand }
		{
		}
	};

	using SignExtendIntermediateCommand = CastIntermediateCommandBase<IntermediateCommandKind::SIGN_EXTEND>;
#define MAKE_SIGN_EXTEND_COMMAND(extant, target, destination, operand) SignExtendIntermediateCommand{ IntermediateTypeKind::extant, IntermediateTypeKind::target, destination, operand }

	using ZeroExtendIntermediateCommand = CastIntermediateCommandBase<IntermediateCommandKind::ZERO_EXTEND>;
#define MAKE_ZERO_EXTEND_COMMAND(extant, target, destination, operand) ZeroExtendIntermediateCommand{ IntermediateTypeKind::extant, IntermediateTypeKind::target, destination, operand }
	
	using TruncateIntermediateCommand = CastIntermediateCommandBase<IntermediateCommandKind::TRUNCATE>;
#define MAKE_TRUNCATE_COMMAND(extant, target, destination, operand) TruncateIntermediateCommand{ IntermediateTypeKind::extant, IntermediateTypeKind::target, destination, operand }

	using ReinterpretIntermediateCommand = CastIntermediateCommandBase<IntermediateCommandKind::REINTERPRET>;
#define MAKE_REINTERPRET_COMMAND(extant, target, destination, operand) ReinterpretIntermediateCommand{ IntermediateTypeKind::extant, IntermediateTypeKind::target, destination, operand }

	using CastIntermediateCommand = CastIntermediateCommandBase<IntermediateCommandKind::CAST>;
#define MAKE_CAST_COMMAND(extant, target, destination, operand) CastIntermediateCommand{ IntermediateTypeKind::extant, IntermediateTypeKind::target, destination, operand }
	

	struct OffsetOfIntermediateCommand : public IntermediateCommandBase
	{
	public:
		TypeReference<StructOrUnionType> kind;
		Register destination;
		Register base_pointer;
		std::string_view member;

	public:
		IntermediateCommandKind intermediate_kind() const;
		ByteRange emit() const;
		Address bytes() const;

	public:
		OffsetOfIntermediateCommand(TypeReference<StructOrUnionType> kind, Register destination, Register base_pointer, std::string_view member)
			: kind{ kind }, destination{ destination }, base_pointer{ base_pointer }, member{ member }
		{
		}
	};
#define MAKE_OFFSET_OF_COMMAND(destination, kind, base_pointer, member) OffsetOfIntermediateCommand{ destination, kind, base_pointer, member }


	//////////////////////////////////////////////////////
	// All Intermediate Commands
	//////////////////////////////////////////////////////

	template<typename SumMemberT, typename StorageT>
	concept IsIntermediateCommand = SumTuple<SumMemberT, StorageT, TypeMethods<typename StorageT::Anchor>>;

	using IntermediateCommandsKinds = SumTypeList
	<
#define X(enumerator, kind, names) kind,
		INTERMEDIATE_COMMAND_KINDS(X)
#undef X
		void
	>;

	using IntermediateCommandSumImplementation = MakeSum<TypeMethods, TypeKinds>::Type;

	struct IntermediateCommandStorage : public IntermediateCommandSumImplementation::Storage
	{
		using IntermediateCommandSumImplementation::Storage::Storage;

		using Type = IntermediateCommandSumImplementation::Type;
		using Anchor = IntermediateCommandSumImplementation::Anchor;
	};

}

#endif
