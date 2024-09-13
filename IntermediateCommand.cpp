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


		return out;
	}


	IntermediateType GlobalVariableCommand::itype() const
	{
		return IntermediateType::GLOBAL_VARIABLE;
	}

	byterange GlobalVariableCommand::emit() const
	{
		byterange out{};


		return out;
	}


	IntermediateType AddCommand::itype() const
	{
		return IntermediateType::BINARY_ADD;
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
		return IntermediateType::BINARY_SUBTRACT;
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
		return IntermediateType::BINARY_BITOR;
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
		return IntermediateType::BINARY_BITAND;
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
		return IntermediateType::BINARY_BITXOR;
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


	IntermediateType CopyCommand::itype() const
	{
		return IntermediateType::UNARY_COPY;
	}

	byterange CopyCommand::emit() const
	{
		// mov destination, src

		byterange out{};

		PUT(X86Builder::mov_rr(_destination, _source));

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


		return out;
	}


	IntermediateType GotoCommand::itype() const
	{
		return IntermediateType::GOTO;
	}

	byterange GotoCommand::emit() const
	{
		byterange out{};


		return out;
	}
}
