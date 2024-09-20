#ifndef HAZE_INTERMEDIATECOMMAND_H
#define HAZE_INTERMEDIATECOMMAND_H

#include "Constants.h"
#include "Context.h"
#include "InstructionEncoding.h"
#include "IntermediateType.h"

// Haze IntermediateCommand.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class IntermediateCommand
	{
	private:
		std::int32_t _self_address;

	public:
		virtual ~IntermediateCommand() = default;

	public:
		virtual IntermediateType itype() const = 0;
		virtual byterange emit() const = 0;
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

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
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
		variable_t _value;

	public:
		LocalVariableCommand(register_t location, variable_t value)
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
		variable_t _value;

	public:
		GlobalVariableCommand(register_t location, variable_t value)
			: _location{ location }, _value{ value }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class MemoryReadCommand : public IntermediateCommand
	{
	private:
		register_t _destination;
		std::uint32_t _pointer;

	public:
		MemoryReadCommand(register_t destination, std::uint32_t pointer)
			: _destination{ destination }, _pointer{ pointer }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class MemoryWriteCommand : public IntermediateCommand
	{
	private:
		std::uint32_t _pointer;
		register_t _source;

	public:
		MemoryWriteCommand(std::uint32_t pointer, register_t source)
			: _pointer{ pointer }, _source{ source }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class AddCommand : public IntermediateCommand
	{
	private:
		register_t _lhs, _rhs, _destination;

	public:
		AddCommand(register_t lhs, register_t rhs, register_t destination)
			: _lhs{ lhs }, _rhs{ rhs }, _destination{ destination }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class SubtractCommand : public IntermediateCommand
	{
	private:
		register_t _lhs, _rhs, _destination;

	public:
		SubtractCommand(register_t lhs, register_t rhs, register_t destination)
			: _lhs{ lhs }, _rhs{ rhs }, _destination{ destination }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class BitorCommand : public IntermediateCommand
	{
	private:
		register_t _lhs, _rhs, _destination;

	public:
		BitorCommand(register_t lhs, register_t rhs, register_t destination)
			: _lhs{ lhs }, _rhs{ rhs }, _destination{ destination }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class BitandCommand : public IntermediateCommand
	{
	private:
		register_t _lhs, _rhs, _destination;

	public:
		BitandCommand(register_t lhs, register_t rhs, register_t destination)
			: _lhs{ lhs }, _rhs{ rhs }, _destination{ destination }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class BitxorCommand : public IntermediateCommand
	{
	private:
		register_t _lhs, _rhs, _destination;

	public:
		BitxorCommand(register_t lhs, register_t rhs, register_t destination)
			: _lhs{ lhs }, _rhs{ rhs }, _destination{ destination }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
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
	private:
		register_t _source, _destination;

	public:
		CopyCommand(register_t source, register_t destination)
			: _source{ source }, _destination{ destination }
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
		std::uint32_t _source;

	public:
		MakeImmediateCommand(register_t destination, std::uint32_t source)
			: _destination{ destination }, _source{ source }
		{
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

	class CallFunctionCommand : public IntermediateCommand
	{
	private:
		std::string _function;

	public:
		CallFunctionCommand(const std::string& function)
			: _function{ function }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class VoidReturnCommand : public IntermediateCommand
	{
	public:
		VoidReturnCommand() = default;

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class ValueReturnCommand : public IntermediateCommand
	{
	private:
		register_t _location;

	public:
		ValueReturnCommand(register_t location)
			: _location{ location }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class IfNotZeroCommand : public IntermediateCommand
	{
	private:
		// location storing the value to compare
		register_t _value;
		// conditional code
		std::int32_t _index;

	public:
		IfNotZeroCommand(register_t value, std::int32_t index)
			: _value{ value }, _index{ index }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class IfZeroCommand : public IntermediateCommand
	{
	private:
		// location storing the value to compare
		register_t _value;
		// conditional code
		std::int32_t _index;

	public:
		IfZeroCommand(register_t value, std::int32_t index)
			: _value{ value }, _index{ index }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
		virtual byterange emit() const final override;
	};

	class GotoCommand : public IntermediateCommand
	{
	private:
		std::int32_t _target;

	public:
		GotoCommand(std::int32_t target)
			: _target{ target }
		{
		}

	public:
		virtual IntermediateType itype() const final override;
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
