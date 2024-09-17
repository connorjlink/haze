#include "IntermediateCommand.h"
#include "BinaryUtilities.h"
#include "X86Builder.h"

namespace hz
{
	IntermediateType EnterScopeCommand::itype() const
	{
		return IntermediateType::ENTER_SCOPE;
	}

	byterange EnterScopeCommand::emit() const
	{
		// push ebp
		// mov ebp, esp
		// sub esp, (# of bytes)

		byterange out{};

		PUT(X86Builder::push_r(EBP));
		PUT(X86Builder::mov_rr(EBP, ESP));
		PUT(X86Builder::sub_ri(ESP, _bytes));

		return out;
	}


	IntermediateType LeaveScopeCommand::itype() const
	{
		return IntermediateType::LEAVE_SCOPE;
	}

	byterange LeaveScopeCommand::emit() const
	{
		// add esp, (# of bytes)
		// pop ebp

		byterange out{};

		PUT(X86Builder::add_ri(ESP, _bytes));
		PUT(X86Builder::pop_r(EBP));

		return out;
	}


	IntermediateType LocalVariableCommand::itype() const
	{
		return IntermediateType::LOCAL_VARIABLE;
	}

	byterange LocalVariableCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: local variable code generation")
		return out;
	}


	IntermediateType GlobalVariableCommand::itype() const
	{
		return IntermediateType::GLOBAL_VARIABLE;
	}

	byterange GlobalVariableCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: GLOBAL VARIABLE CODE GENERATION")
		return out;
	}


	IntermediateType MemoryReadCommand::itype() const
	{
		return IntermediateType::MEMORY_READ;
	}

	byterange MemoryReadCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: memory read command code generation")

		return out;
	}


	IntermediateType MemoryWriteCommand::itype() const
	{
		return IntermediateType::MEMORY_WRITE;
	}

	byterange MemoryWriteCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: memory write command code generation")

		return out;
	}


	IntermediateType AddCommand::itype() const
	{
		return IntermediateType::ADD;
	}

	byterange AddCommand::emit() const
	{
		// mov destination, lhs
		// add destination, rhs

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _lhs));
		PUT(X86Builder::add_rr(_destination, _rhs));

		return out;
	}


	IntermediateType SubtractCommand::itype() const
	{
		return IntermediateType::SUBTRACT;
	}

	byterange SubtractCommand::emit() const
	{
		// mov destination, lhs
		// sub destination, rhs

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _lhs));
		PUT(X86Builder::sub_rr(_destination, _rhs));

		return out;
	}


	IntermediateType BitorCommand::itype() const
	{
		return IntermediateType::BITOR;
	}

	byterange BitorCommand::emit() const
	{
		// mov destination, lhs
		// or destination, rhs

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _lhs));
		PUT(X86Builder::or_rr(_destination, _rhs));

		return out;
	}


	IntermediateType BitandCommand::itype() const
	{
		return IntermediateType::BITAND;
	}

	byterange BitandCommand::emit() const
	{
		// mov destination, lhs
		// and destination, rhs

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _lhs));
		PUT(X86Builder::and_rr(_destination, _rhs));

		return out;
	}


	IntermediateType BitxorCommand::itype() const
	{
		return IntermediateType::BITXOR;
	}

	byterange BitxorCommand::emit() const
	{
		// mov destination, lhs
		// xor destination, rhs

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _lhs));
		PUT(X86Builder::xor_rr(_destination, _rhs));

		return out;
	}


	IntermediateType IncrementCommand::itype() const
	{
		return IntermediateType::INCREMENT;
	}

	byterange IncrementCommand::emit() const
	{
		// mov destination, source
		// inc destination

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _source));
		PUT(X86Builder::inc_r(_destination));

		return out;
	}


	IntermediateType DecrementCommand::itype() const
	{
		return IntermediateType::DECREMENT;
	}

	byterange DecrementCommand::emit() const
	{
		// mov destination, source
		// dec destination

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _source));
		PUT(X86Builder::dec_r(_destination));

		return out;
	}


	IntermediateType CopyCommand::itype() const
	{
		return IntermediateType::COPY;
	}

	byterange CopyCommand::emit() const
	{
		// mov destination, src

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _source));

		return out;
	}


	IntermediateType MakeImmediateCommand::itype() const
	{
		return IntermediateType::MAKE_IMMEDIATE;
	}

	byterange MakeImmediateCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: code generation for immediate command!")

		return out;
	}


	IntermediateType MakeArgumentCommand::itype() const
	{
		return IntermediateType::MAKE_ARGUMENT;
	}

	byterange MakeArgumentCommand::emit() const
	{
		// push location

		byterange out{};

		PUT(X86Builder::push_r(_location));

		return out;
	}


	IntermediateType TakeArgumentCommand::itype() const
	{
		return IntermediateType::TAKE_ARGUMENT;
	}

	byterange TakeArgumentCommand::emit() const
	{
		// mov _location, [ebp + offset]

		byterange out{};

		PUT(X86Builder::mov_ro(_location, _offset));

		return out;
	}


	IntermediateType CallFunctionCommand::itype() const
	{
		return IntermediateType::CALL_FUNCTION;
	}

	byterange CallFunctionCommand::emit() const
	{
		byterange out{};


		return out;
	}


	IntermediateType VoidReturnCommand::itype() const
	{
		return IntermediateType::VOID_RETURN;
	}

	byterange VoidReturnCommand::emit() const
	{
		byterange out{};

		return out;
	}


	IntermediateType ValueReturnCommand::itype() const
	{
		return IntermediateType::VALUE_RETURN;
	}

	byterange ValueReturnCommand::emit() const
	{
		byterange out{};

		return out;
	}


	IntermediateType JournalEntryCommand::itype() const
	{
		return IntermediateType::JOURNAL_ENTRY;
	}

	byterange JournalEntryCommand::emit() const
	{
		byterange out{};


		return out;
	}


	IntermediateType JournalLeaveCommand::itype() const
	{
		return IntermediateType::JOURNAL_LEAVE;
	}

	byterange JournalLeaveCommand::emit() const
	{
		byterange out{};


		return out;
	}


	IntermediateType IfNotZeroCommand::itype() const
	{
		return IntermediateType::IF_NOT_ZERO;
	}

	byterange IfNotZeroCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: code generation for ifnz!")
		return out;
	}


	IntermediateType GotoCommand::itype() const
	{
		return IntermediateType::GOTO;
	}

	byterange GotoCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: code generation for goto!")

		return out;
	}


	IntermediateType MakeMessageCommand::itype() const
	{
		return IntermediateType::MAKE_MESSAGE;
	}

	byterange MakeMessageCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: make message code generation")

		return out;
	}

	
	IntermediateType PrintMessageCommand::itype() const
	{
		return IntermediateType::PRINT_MESSAGE;
	}

	byterange PrintMessageCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: make print message code generation!")

		return out;
	}


	IntermediateType ExitProgramCommand::itype() const
	{
		return IntermediateType::EXIT_PROGRAM;
	}

	byterange ExitProgramCommand::emit() const
	{
		byterange out{};

#pragma message("TODO: exit program command code generation")

		return out;
	}


	IntermediateType InlineAssemblyCommand::itype() const
	{
		return IntermediateType::INLINE_ASSEMBLY;
	}

	byterange InlineAssemblyCommand::emit() const
	{
		return _code;
	}
}
