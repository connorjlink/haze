import std;

#include "IntermediateCommand.h"
#include "BinaryUtilities.h"
#include "BinaryConstants.h"
#include "Generator.h"
#include "Parser.h"
#include "X86Builder.h"

// Haze IntermediateCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	IntermediateType BranchLabelCommand::itype() const
	{
		return IntermediateType::BRANCH_LABEL;
	}

	byterange BranchLabelCommand::emit() const
	{
		//label:

		_generator->resolve_branch_target_real(_label, _generator->resolve_origin());

		return {};
	}


	IntermediateType EnterScopeCommand::itype() const
	{
		return IntermediateType::ENTER_SCOPE;
	}

	byterange EnterScopeCommand::emit() const
	{
		// push ebp
		// mov ebp, esp
		// sub esp, N

		// NOTE: equivalent to -->
		// enter N, 0

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
		// mov esp, ebp
		// pop ebp
		
		// NOTE: equivalent to -->
		// leave

		byterange out{};

		PUT(X86Builder::mov_rr(ESP, EBP));
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
		// mov destination, [pointer]

		byterange out{};

		PUT(X86Builder::mov_rm(_destination, _pointer));

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

		PUT(X86Builder::mov_ri(_destination, _immediate));

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

		const auto target = _generator->query_branch_target(_function);
#pragma message("TODO: function call command")

		PUT(X86Builder::call(target));

		auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, _function, NULL_TOKEN);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		// only need to clean up the stack frame if one was created in the first place
		if (function_symbol->locals_count != 0)
		{
			PUT(X86Builder::add_ri(ESP, 0x1000));
		}

		return out;
	}


	IntermediateType VoidReturnCommand::itype() const
	{
		return IntermediateType::VOID_RETURN;
	}

	byterange VoidReturnCommand::emit() const
	{
		// ret

		byterange out{};

		PUT(X86Builder::ret());

		return out;
	}


	IntermediateType ValueReturnCommand::itype() const
	{
		return IntermediateType::VALUE_RETURN;
	}

	byterange ValueReturnCommand::emit() const
	{
		// mov eax, location
		// ret

		byterange out{};

		// NOTE: this will clobber anything currently in the EAX register
		// but this should be OK since they are locals anyway

#pragma message("TODO: clean this up so the intermediate command is not responsible for optimization")
		
		/*if (_location != EAX)
		{
			PUT(X86Builder::mov_rr(EAX, _location));
		}*/
		
		//PUT(X86Bioldd)
		PUT(X86Builder::ret());

		return out;
	}


	IntermediateType IfNotZeroCommand::itype() const
	{
		return IntermediateType::IF_NOT_ZERO;
	}

	byterange IfNotZeroCommand::emit() const
	{
		// test eax, eax
		// je index

		const auto origin = _generator->resolve_origin();
		const auto target = _index;

		byterange out{};

		PUT(X86Builder::test_rr(_value, _value));
		//PUT(X86Builder::je(_gener))
#pragma message("TODO: ifnz codegen")

		return out;
	}

	IntermediateType IfZeroCommand::itype() const
	{
		return IntermediateType::IF_ZERO;
	}

	byterange IfZeroCommand::emit() const
	{
		// test eax, eax
		// jne skip
		// { code... }
		//skip:

		byterange code{};


		byterange out{};

		PUT(X86Builder::test_rr(_value, _value));
#pragma message("TODO: ifz codegen")


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
		// WriteConsoleA(STDHANDLE, &string, strlen, NULL, NULL)
		// push NULL
		// push NULL
		// push strlen
		// push &string
		// push [0x004033F0] ; provided console stdout handle

		byterange out{};

		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(_length));
		PUT(X86Builder::push_i32(_pointer));
		PUT(X86Builder::push_m(0x004033F0));

#pragma message("TODO: finish printMessage codegen")

		return out;
	}


	IntermediateType PrintNumberCommand::itype() const
	{
		return IntermediateType::PRINT_NUMBER;
	}

	byterange PrintNumberCommand::emit() const
	{
#pragma message("TODO: print number codegen")
		return {};
	}


	IntermediateType ExitProgramCommand::itype() const
	{
		return IntermediateType::EXIT_PROGRAM;
	}

	byterange ExitProgramCommand::emit() const
	{
		// push code
		// call [0x402068] (ExitProcess)

		byterange out{};

		PUT(X86Builder::push_r(_code));
		PUT(X86Builder::call(PROCEDURE(exitprocess_iat_va)));

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
