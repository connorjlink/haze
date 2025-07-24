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
#include <toolchain/models/InstructionEncoding.h>

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
		virtual byterange emit() const = 0;
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
		virtual byterange emit() const final override;
	};

	class EnterScopeCommand : public IntermediateCommand
	{
	private:
		std::int32_t _bytes;
		// TODO: support dynamically resizable stack frame sizes
		// for now it is fixed 16k per frame (will overflow quickly)

	public:
		EnterScopeCommand(std::int32_t bytes = 0x1000)
			: _bytes{ bytes }
		{
		}

	private:
		constexpr byterange _emit(std::uint32_t, std::int32_t) const;

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
		//virtual constexpr std::int32_t bytes() const final override;
	};

	class LeaveScopeCommand : public IntermediateCommand
	{
	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class LocalVariableCommand : public IntermediateCommand
	{
	private:
		register_t _location;
		Variable* _value;

	public:
		LocalVariableCommand(register_t location, Variable* value)
			: _location{ location }, _value{ value }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class GlobalVariableCommand : public IntermediateCommand
	{
	private:
		register_t _location;
		Variable* _value;

	public:
		GlobalVariableCommand(register_t location, Variable* value)
			: _location{ location }, _value{ value }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class HeapReadCommand : public IntermediateCommand
	{
	private:
		register_t _destination;
		std::uint32_t _pointer;

	public:
		HeapReadCommand(register_t destination, std::uint32_t pointer)
			: _destination{ destination }, _pointer{ pointer }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class HeapWriteCommand : public IntermediateCommand
	{
	private:
		std::uint32_t _pointer;
		register_t _source;

	public:
		HeapWriteCommand(std::uint32_t pointer, register_t source)
			: _pointer{ pointer }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class StackReadCommand : public IntermediateCommand
	{
	private:
		register_t _destination;
		std::int32_t _offset;

	public:
		StackReadCommand(register_t destination, std::int32_t offset)
			: _destination{ destination }, _offset{ offset }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class StackWriteCommand : public IntermediateCommand
	{
	private:
		std::int32_t _offset;
		register_t _source;

	public:
		StackWriteCommand(std::int32_t offset, register_t source)
			: _offset{ offset }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class BoolCommand : public IntermediateCommand
	{
	private:
		register_t _destination, _source;

	public:
		BoolCommand(register_t destination, register_t source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class BinaryCommand : public IntermediateCommand
	{
	protected:
		register_t _lhs, _rhs, _destination;

	public:
		BinaryCommand(register_t lhs, register_t rhs, register_t destination)
			: _lhs{ lhs }, _rhs{ rhs }, _destination{ destination }
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
		virtual byterange emit() const final override;
	};

	class SubtractCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class MultiplyCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class BitorCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class BitandCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class BitxorCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class BitlshiftCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class BitrshiftCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class EqualityCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class InequalityCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class LessCommand : public BinaryCommand
	{ 
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class GreaterCommand : public BinaryCommand
	{
	public:
		using BinaryCommand::BinaryCommand;

	public:
		virtual BinaryCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class IncrementCommand : public IntermediateCommand
	{
	private:
		register_t _destination, _source;

	public:
		IncrementCommand(register_t destination, register_t source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class DecrementCommand : public IntermediateCommand
	{
	private:
		register_t _destination, _source;

	public:
		DecrementCommand(register_t destination, register_t source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class CopyCommand : public IntermediateCommand
	{
	public:
		register_t _destination, _source;

	public:
		CopyCommand(register_t destination, register_t source)
			: _destination{ destination }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class MakeImmediateCommand : public IntermediateCommand
	{
	private:
		register_t _destination;
		std::int32_t _immediate;

	public:
		MakeImmediateCommand(register_t destination, IntegerLiteral* immediate)
			: _destination{ destination }
		{
			using enum IntegerLiteralType;
			switch (immediate->itype())
			{
				case UBYTE: _immediate = AS_UNSIGNED_BYTE_INTEGER_LITERAL(immediate)->value; break;
				case SBYTE: _immediate = AS_SIGNED_BYTE_INTEGER_LITERAL(immediate)->value; break;

				case UWORD: _immediate = AS_UNSIGNED_WORD_INTEGER_LITERAL(immediate)->value; break;
				case SWORD: _immediate = AS_SIGNED_WORD_INTEGER_LITERAL(immediate)->value; break;

				case UDWORD: _immediate = AS_UNSIGNED_DOUBLE_WORD_INTEGER_LITERAL(immediate)->value; break;
				case SDWORD: _immediate = AS_SIGNED_DOUBLE_WORD_INTEGER_LITERAL(immediate)->value; break;

				// since x86 is building 32-bit, 64-bit immediates are not supported for now
				default:
				{
					CommonErrors::invalid_integer_literal_type(immediate->itype(), NULL_TOKEN);
				} break;
			}
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class MakeArgumentCommand : public IntermediateCommand
	{
	private:
		register_t _location;
		// TODO: use this later to support custom-size objects
		// in that case, each argument will need to know its own offset in the stack frame
		//std::int32_t _offset;

	public:
		MakeArgumentCommand(register_t location)
			: _location{ location }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class TakeArgumentCommand : public IntermediateCommand
	{
	private:
		register_t _location;
		std::int32_t _offset;

	public:
		TakeArgumentCommand(register_t location, std::int32_t offset)
			: _location{ location }, _offset{ offset }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class BranchCommand : public IntermediateCommand
	{
	public:
		// jump destination
		std::string label;
		std::optional<std::int32_t> target_offset;

	public:
		BranchCommand(const std::string& label)
			: label{ label }, target_offset{ std::nullopt }
		{
		}

		BranchCommand(std::int32_t target_offset)
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
		virtual byterange emit() const final override;
	};

	class VoidReturnCommand : public BranchCommand
	{
	public:
		using BranchCommand::BranchCommand;

	public:
		virtual BranchCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class ValueReturnCommand : public BranchCommand
	{
	private:
		register_t _location;

	public:
		ValueReturnCommand(const std::string& label, register_t location)
			: BranchCommand{ label }, _location { location }
		{
		}

		ValueReturnCommand(std::int32_t target_offset, register_t location)
			: BranchCommand{ target_offset }, _location{ location }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class IfNotZeroCommand : public BranchCommand
	{
	private:
		// location storing the value to compare
		register_t _value;

	public:
		IfNotZeroCommand(const std::string& label, register_t value)
			: BranchCommand{ label }, _value{ value }
		{
		}

		IfNotZeroCommand(std::int32_t target_offset, register_t value)
			: BranchCommand{ target_offset }, _value{ value }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class IfZeroCommand : public BranchCommand
	{
	private:
		// location storing the value to compare
		register_t _value;

	public:
		IfZeroCommand(const std::string& label, register_t value)
			: BranchCommand{ label }, _value{ value }
		{
		}

		IfZeroCommand(std::int32_t target_offset, register_t value)
			: BranchCommand{ target_offset }, _value{ value }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class GotoCommand : public BranchCommand
	{
	public:
		GotoCommand(const std::string& label)
			: BranchCommand{ label }
		{
		}

		GotoCommand(std::int32_t target_offset)
			: BranchCommand{ target_offset }
		{
		}

	public:
		virtual BranchCommandType btype() const final override;
		virtual byterange emit() const final override;
	};

	class MakeMessageCommand : public IntermediateCommand
	{
	private:
		std::uint32_t _pointer;
		std::string _message;
		
	public:
		MakeMessageCommand(std::uint32_t pointer, const std::string& message)
			: _pointer{ pointer }, _message{ message }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class PrintMessageCommand : public IntermediateCommand
	{
	private:
		std::uint32_t _pointer;
		std::uint32_t _length;

	public:
		PrintMessageCommand(std::uint32_t pointer, std::uint32_t length)
			: _pointer{ pointer }, _length{ length }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class PrintNumberCommand : public IntermediateCommand
	{
	private:
		register_t _value;

	public:
		PrintNumberCommand(register_t value)
			: _value{ value }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class ExitProgramCommand : public IntermediateCommand
	{
	private:
		register_t _code;

	public:
		ExitProgramCommand(register_t code)
			: _code{ code }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class InlineAssemblyCommand : public IntermediateCommand
	{
	private:
		byterange _code;
		std::uint32_t _approximate_size;

	public:
		InlineAssemblyCommand(byterange&& code, std::uint32_t approximate_size)
			: _code{ std::move(code) }, _approximate_size{ approximate_size }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};
}

#endif
