#ifndef HAZE_INTERMEDIATECOMMAND_H
#define HAZE_INTERMEDIATECOMMAND_H

#include <command/BranchCommandType.h>
#include <command/BinaryCommandType.h>
#include <command/IntermediateType.h>
#include <command/models/IntegerLiteral.h>
#include <data/DependencyInjector.h>
#include <error/CommonErrors.h>
#include <symbol/SymbolDatabase.h>
#include <utility/Constants.h>
#include <toolchain/Generator.h>

// Haze IntermediateCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IntermediateCommand 
		: public InjectService<Generator>
		, public InjectSingleton<SymbolDatabase>
	{
	public:
		std::int32_t offset;
		std::uint8_t size;
		bool marked_for_deletion = false;

	public:
		virtual ~IntermediateCommand() = default;

	public:
		virtual IntermediateType itype() const = 0;
		virtual ByteRange emit() const = 0;
		virtual constexpr std::int32_t bytes() const
		{
			return -1;
		}
	};


	class BranchLabelCommand : public IntermediateCommand
	{
	public:
		std::string label;

	public:
		BranchLabelCommand(const std::string& label)
			: label{ label }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class EnterScopeCommand : public IntermediateCommand
	{
	private:
		std::int32_t _bytes;
#pragma message("TODO: implement dynamic stack frame resizing. for now it is fixed 16k per frame (will overflow quickly)")

	public:
		EnterScopeCommand(std::int32_t bytes = 0x1000)
			: _bytes{ bytes }
		{
		}

	private:
		constexpr ByteRange _emit(std::uint32_t, std::int32_t) const;

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
		//virtual constexpr std::int32_t bytes() const final override;
	};

	class LeaveScopeCommand : public IntermediateCommand
	{
	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class LocalVariableCommand : public IntermediateCommand
	{
	private:
		std::int8_t location;
		Variable* _value;

	public:
		LocalVariableCommand(std::int8_t location, Variable* value)
			: location{ location }, _value{ value }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class GlobalVariableCommand : public IntermediateCommand
	{
	private:
		std::int8_t location;
		Variable* _value;

	public:
		GlobalVariableCommand(std::int8_t location, Variable* value)
			: location{ location }, _value{ value }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class HeapReadCommand : public IntermediateCommand
	{
	private:
		std::int8_t destination;
		std::uint32_t pointer;

	public:
		HeapReadCommand(std::int8_t destination, std::uint32_t pointer)
			: destination{ destination }, pointer{ pointer }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class HeapWriteCommand : public IntermediateCommand
	{
	private:
		std::uint32_t pointer;
		std::int8_t _source;

	public:
		HeapWriteCommand(std::uint32_t pointer, std::int8_t source)
			: pointer{ pointer }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class StackReadCommand : public IntermediateCommand
	{
	private:
		std::int8_t destination;
		std::int32_t _offset;

	public:
		StackReadCommand(std::int8_t destination, std::int32_t offset)
			: destination{ destination }, _offset{ offset }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class StackWriteCommand : public IntermediateCommand
	{
	private:
		std::int32_t _offset;
		std::int8_t _source;

	public:
		StackWriteCommand(std::int32_t offset, std::int8_t source)
			: _offset{ offset }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BoolCommand : public IntermediateCommand
	{
	private:
		std::int8_t destination, _source;

	public:
		BoolCommand(std::int8_t destination, std::int8_t source)
			: destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BinaryCommand : public IntermediateCommand
	{
	protected:
		std::int8_t _lhs, _rhs, destination;

	public:
		BinaryCommand(std::int8_t lhs, std::int8_t rhs, std::int8_t destination)
			: _lhs{ lhs }, _rhs{ rhs }, destination{ destination }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual BinaryCommandType btype() const = 0;
	};

	class AddCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class SubtractCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class MultiplyCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BitorCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BitandCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BitxorCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BitlshiftCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BitrshiftCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class EqualityCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class InequalityCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class LessCommand : public BinaryCommand
	{ 
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class GreaterCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class IncrementCommand : public IntermediateCommand
	{
	private:
		std::int8_t destination, _source;

	public:
		IncrementCommand(std::int8_t destination, std::int8_t source)
			: destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class DecrementCommand : public IntermediateCommand
	{
	private:
		std::int8_t destination, _source;

	public:
		DecrementCommand(std::int8_t destination, std::int8_t source)
			: destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class CopyCommand : public IntermediateCommand
	{
	public:
		std::int8_t destination, _source;

	public:
		CopyCommand(std::int8_t destination, std::int8_t source)
			: destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class MakeImmediateCommand : public IntermediateCommand
	{
	private:
		Register destination;
		BigInteger immediate;

	public:
		MakeImmediateCommand(Register destination, BigInteger immediate)
			: destination{ destination }, immediate{ immediate }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class MakeArgumentCommand : public IntermediateCommand
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
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class TakeArgumentCommand : public IntermediateCommand
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
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class BranchCommand : public IntermediateCommand
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
		virtual IntermediateType itype() const final override;
		virtual BranchCommandType btype() const = 0;
	};

	class CallFunctionCommand : public BranchCommand
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

	class VoidReturnCommand : public BranchCommand
	{
	public:
		using BranchCommand::BranchCommand;

	public:
		virtual BranchCommandType btype() const final override;
		virtual ByteRange emit() const final override;
	};

	class ValueReturnCommand : public BranchCommand
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

	class IfNotZeroCommand : public BranchCommand
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

	class IfZeroCommand : public BranchCommand
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

	class GotoCommand : public BranchCommand
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

	class MakeMessageCommand : public IntermediateCommand
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
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class PrintMessageCommand : public IntermediateCommand
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
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class PrintNumberCommand : public IntermediateCommand
	{
	private:
		Register source;

	public:
		PrintNumberCommand(Register value)
			: source{ value }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class ExitProgramCommand : public IntermediateCommand
	{
	private:
		Register source;

	public:
		ExitProgramCommand(Register source)
			: source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};

	class InlineAssemblyCommand : public IntermediateCommand
	{
	private:
		ByteRange code;

	public:
		InlineAssemblyCommand(ByteRange&& code)
			: code{ std::move(code) }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual ByteRange emit() const final override;
	};
}

#endif
