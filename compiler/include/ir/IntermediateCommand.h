#ifndef HAZE_INTERMEDIATECOMMAND_H
#define HAZE_INTERMEDIATECOMMAND_H

#include <allocator/Allocator.h>
#include <data/DependencyInjector.h>
#include <error/CommonErrors.h>
#include <ir/defs/IntermediateKind.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <utility/Constants.h>

// Haze IntermediateCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
#pragma message("TODO: implement remaining intermediate commands")

	struct IntermediateCommand 
		: public InjectService<Generator, Allocator>
		, public InjectSingleton<SymbolDatabase>
	{
	public:
		Offset offset;
		std::uint8_t size;
		bool marked_for_deletion = false;

	public:
		virtual ~IntermediateCommand() = default;

	public:
		virtual IntermediateKind intermediate_kind() const = 0;
		virtual ByteRange emit() const = 0;
		virtual constexpr std::int32_t bytes() const
		{
			return -1;
		}
	};


	struct BranchLabelCommand : public IntermediateCommand
	{
	public:
		std::string label;

	public:
		BranchLabelCommand(const std::string& label)
			: label{ label }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct EnterScopeCommand : public IntermediateCommand
	{
	private:
		Offset bytes;

	private:
		inline static constexpr Offset MINIMUM_STACK_SIZE = 0x1000;

	public:
		EnterScopeCommand()
			: bytes{ MINIMUM_STACK_SIZE }
		{
		}

		EnterScopeCommand(const std::string& function_name)
		{
			bytes = REQUIRE_SAFE(Allocator)->get_function_stack_size(function_name)
				.value_or(MINIMUM_STACK_SIZE);
		}

	private:
		constexpr ByteRange _emit(Address, Offset) const;

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
		//virtual constexpr std::int32_t bytes() const final override;
	};

	struct LeaveScopeCommand : public IntermediateCommand
	{
	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct LocalVariableCommand : public IntermediateCommand
	{
	private:
		Register location;
		Variable* value;

	public:
		LocalVariableCommand(Register location, Variable* value)
			: location{ location }, value{ value }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct GlobalVariableCommand : public IntermediateCommand
	{
	private:
		Register location;
		Variable* value;

	public:
		GlobalVariableCommand(Register location, Variable* value)
			: location{ location }, value{ value }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct HeapReadCommand : public IntermediateCommand
	{
	private:
		Address pointer;
		Register destination;

	public:
		HeapReadCommand(Register destination, Address pointer)
			: destination{ destination }, pointer{ pointer }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct HeapWriteCommand : public IntermediateCommand
	{
	private:
		Address pointer;
		Register source;

	public:
		HeapWriteCommand(Address pointer, Register source)
			: pointer{ pointer }, source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct StackReadCommand : public IntermediateCommand
	{
	private:
		Offset offset;
		Register destination;

	public:
		StackReadCommand(Register destination, Offset offset)
			: destination{ destination }, offset{ offset }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct StackWriteCommand : public IntermediateCommand
	{
	private:
		Offset offset;
		Register source;

	public:
		StackWriteCommand(Offset offset, Register source)
			: offset{ offset }, source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BoolCommand : public IntermediateCommand
	{
	private:
		std::int8_t destination, source;

	public:
		BoolCommand(std::int8_t destination, std::int8_t source)
			: destination{ destination }, source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BinaryCommand : public IntermediateCommand
	{
	protected:
		std::int8_t left, right, destination;

	public:
		BinaryCommand(std::int8_t left, std::int8_t right, std::int8_t destination)
			: left{ left }, right{ right }, destination{ destination }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual BinaryCommandType btype() const = 0;
	};

	struct AddCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct SubtractCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct MultiplyCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BitorCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BitandCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BitxorCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BitlshiftCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BitrshiftCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct EqualityCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct InequalityCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct LessCommand : public BinaryCommand
	{ 
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct GreaterCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct IncrementCommand : public IntermediateCommand
	{
	private:
		Register destination, source;

	public:
		IncrementCommand(Register destination, Register source)
			: destination{ destination }, source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct DecrementCommand : public IntermediateCommand
	{
	private:
		Register destination, source;

	public:
		DecrementCommand(Register destination, Register source)
			: destination{ destination }, source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct CopyCommand : public IntermediateCommand
	{
	public:
		Register destination, source;

	public:
		CopyCommand(Register destination, Register source)
			: destination{ destination }, source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct MakeImmediateCommand : public IntermediateCommand
	{
	private:
		BigInteger immediate;

	public:
		MakeImmediateCommand(BigInteger immediate)
			: immediate{ immediate }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct MakeArgumentCommand : public IntermediateCommand
	{
	private:
		Register source;
		#pragma message("TODO: implement custom-size objects on the stack using the offset field")
		// in that case, each argument will need to know its own offset in the stack frame
		//std::int32_t _offset;

	public:
		MakeArgumentCommand(Register source)
			: source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct TakeArgumentCommand : public IntermediateCommand
	{
	private:
		Register destination;
		Offset offset;

	public:
		TakeArgumentCommand(Register destination, Offset offset)
			: destination{ destination }, offset{ offset }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct BranchCommand : public IntermediateCommand
	{
	public:
		// jmp destination
		std::string label;
		std::optional<Offset> target_offset;

	public:
		BranchCommand(const std::string& label)
			: label{ label }, target_offset{ std::nullopt }
		{
		}

		BranchCommand(Offset target_offset)
			: target_offset{ target_offset }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual BranchCommandType btype() const = 0;
	};

	struct CallFunctionCommand : public BranchCommand
	{
	public:
		CallFunctionCommand(const std::string& label)
			: BranchCommand{ label }
		{
		}

		CallFunctionCommand(std::int32_t target_offset)
			: BranchCommand{ target_offset }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct VoidReturnCommand : public BranchCommand
	{
	public:
		using BranchCommand::BranchCommand;

	public:
		virtual BranchCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct ValueReturnCommand : public BranchCommand
	{
	public:
		ValueReturnCommand(const std::string& label)
			: BranchCommand{ label }
		{
		}

		ValueReturnCommand(Offset target_offset)
			: BranchCommand{ target_offset }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct IfNotZeroCommand : public BranchCommand
	{
	private:
		// location storing the value to compare
		Register source;

	public:
		IfNotZeroCommand(const std::string& label, Register source)
			: BranchCommand{ label }, source{ source }
		{
		}

		IfNotZeroCommand(Offset target_offset, Register source)
			: BranchCommand{ target_offset }, source{ source }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct IfZeroCommand : public BranchCommand
	{
	private:
		// location storing the value to compare
		Register source;

	public:
		IfZeroCommand(const std::string& label, Register source)
			: BranchCommand{ label }, source{ source }
		{
		}

		IfZeroCommand(Offset target_offset, Register source)
			: BranchCommand{ target_offset }, source{ source }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct GotoCommand : public BranchCommand
	{
	public:
		GotoCommand(const std::string& label)
			: BranchCommand{ label }
		{
		}

		GotoCommand(Offset target_offset)
			: BranchCommand{ target_offset }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	struct MakeMessageCommand : public IntermediateCommand
	{
	private:
		Address pointer;
		std::string message;
		
	public:
		MakeMessageCommand(Address pointer, const std::string& message)
			: pointer{ pointer }, message{ message }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct PrintMessageCommand : public IntermediateCommand
	{
	private:
		Address pointer;
		Address length;

	public:
		PrintMessageCommand(Address pointer, Address length)
			: pointer{ pointer }, length{ length }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct PrintNumberCommand : public IntermediateCommand
	{
	private:
		Register source;

	public:
		PrintNumberCommand(Register value)
			: source{ value }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct ExitProgramCommand : public IntermediateCommand
	{
	private:
		Register source;

	public:
		ExitProgramCommand(Register source)
			: source{ source }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};

	struct InlineAssemblyCommand : public IntermediateCommand
	{
	private:
		ByteRange code;

	public:
		InlineAssemblyCommand(const ByteRange& code)
			: code{ code }
		{
		}

	public:
		virtual IntermediateKind intermediate_kind() const final override;
		virtual ByteRange emit() const final override;
	};
}

#endif
