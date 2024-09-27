import std;

#include "IntermediateCommand.h"
#include "BinaryUtilities.h"
#include "BinaryConstants.h"
#include "Generator.h"
#include "Parser.h"
#include "CompilerParser.h"
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
		// No binary code is generated for a label
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
		// enter 0, N (size of local variables)

		byterange out{};

		PUT(X86Builder::push_r(EBP));
		PUT(X86Builder::mov_rr(EBP, ESP));

		const auto& current_function = _generator->current_function();

		auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, current_function, NULL_TOKEN);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		if (function_symbol->locals_count > 0)
		{
			// set up a fixed-size stack frame of 4096 bytes
			PUT(X86Builder::sub_ri(ESP, _bytes));
		}

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

		// NOTE: old method
		/*PUT(X86Builder::mov_rr(ESP, EBP));
		PUT(X86Builder::pop_r(EBP));*/

		PUT(X86Builder::leave());
		
		const auto& current_function = _generator->current_function();

		const auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, current_function, NULL_TOKEN);
		const auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		const auto arity = function_symbol->arity();

		if (arity == 0)
		{
			PUT(X86Builder::ret());
		}

		else
		{
#pragma message("TODO: compute the correct number of bytes to pop based on argument sizes")
			// pop all arguments pushed by the caller
			const auto bytes = static_cast<std::uint16_t>(arity * 4);
			PUT(X86Builder::ret(bytes));
		}


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


	IntermediateType HeapReadCommand::itype() const
	{
		return IntermediateType::HEAP_READ;
	}

	byterange HeapReadCommand::emit() const
	{
		// mov destination, [pointer]

		byterange out{};

		PUT(X86Builder::mov_rm(_destination, _pointer));

		return out;
	}


	IntermediateType HeapWriteCommand::itype() const
	{
		return IntermediateType::HEAP_WRITE;
	}

	byterange HeapWriteCommand::emit() const
	{
		// mov [pointer], destination

		byterange out{};

		PUT(X86Builder::mov_mr(_pointer, _source));

		return out;
	}


	IntermediateType StackReadCommand::itype() const
	{
		return IntermediateType::STACK_READ;
	}

	byterange StackReadCommand::emit() const
	{
		byterange out{};

		// mov destination, [ebp + offset]
		PUT(X86Builder::mov_ro(_destination, _offset));

		return out;
	}


	IntermediateType StackWriteCommand::itype() const
	{
		return IntermediateType::STACK_WRITE;
	}

	byterange StackWriteCommand::emit() const
	{
		byterange out{};

		// mov [ebp + offset], destination
		PUT(X86Builder::mov_or(_offset, _source));

		return out;
	}


	IntermediateType BoolCommand::itype() const
	{
		return IntermediateType::BOOL;
	}

	byterange BoolCommand::emit() const
	{
		// test source, source
		// sete destination
		// movzx destination, destination

		byterange out{};

		PUT(X86Builder::test_rr(_source, _source));
		PUT(X86Builder::sete(_destination));
		PUT(X86Builder::movzx(_destination, _source));

		return out;
	}


	IntermediateType BinaryCommand::itype() const
	{
		return IntermediateType::BINARY;
	}


	BinaryCommandType AddCommand::btype() const
	{
		return BinaryCommandType::ADD;
	}

	byterange AddCommand::emit() const
	{
		byterange out{};

		if (_destination == _lhs)
		{
			// NOTE: the lhs is already in the destination register, so add rhs directly
			// add destination, rhs
			PUT(X86Builder::add_rr(_destination, _rhs));
		}

		else if (_destination == _rhs)
		{
			// NOTE: the rhs is already in the destination register, so add lhs directly
			PUT(X86Builder::add_rr(_destination, _lhs));
		}

		else
		{
			// mov destination, lhs
			// add destination, rhs
			PUT(X86Builder::mov_rr(_destination, _lhs));
			PUT(X86Builder::add_rr(_destination, _rhs));
		}

		return out;
	}


	BinaryCommandType SubtractCommand::btype() const
	{
		return BinaryCommandType::SUBTRACT;
	}

	byterange SubtractCommand::emit() const
	{
		byterange out{};

		if (_destination == _lhs)
		{
			// NOTE: the lhs is already in the destination register, so sub rhs directly
			// sub destination, rhs
			PUT(X86Builder::sub_rr(_destination, _rhs));
		}
		
		// NOTE: since subtraction is anti-commutative swapping operand order here is less efficient than just moving
		/*else if (_destination == _rhs)
		{
		}*/

		else
		{
			// mov destination, lhs
			// sub destination, rhs
			PUT(X86Builder::mov_rr(_destination, _lhs));
			PUT(X86Builder::sub_rr(_destination, _rhs));
		}

		return out;
	}


	BinaryCommandType BitorCommand::btype() const
	{
		return BinaryCommandType::BITOR;
	}

	byterange BitorCommand::emit() const
	{
		byterange out{};

		if (_destination == _lhs)
		{
			// NOTE: the lhs is already in the destination register, so or rhs directly
			// or destination, rhs
			PUT(X86Builder::or_rr(_destination, _rhs));
		}

		else if (_destination == _rhs)
		{
			// NOTE: the rhs is already in the destination register, so add lhs directly
			PUT(X86Builder::or_rr(_destination, _lhs));
		}

		else
		{
			// mov destination, lhs
			// or destination, rhs
			PUT(X86Builder::mov_rr(_destination, _lhs));
			PUT(X86Builder::or_rr(_destination, _rhs));
		}

		return out;
	}


	BinaryCommandType BitandCommand::btype() const
	{
		return BinaryCommandType::BITAND;
	}

	byterange BitandCommand::emit() const
	{
		byterange out{};

		if (_destination == _lhs)
		{
			// NOTE: the lhs is already in the destination register, so and rhs directly
			// and destination, rhs
			PUT(X86Builder::and_rr(_destination, _rhs));
		}

		else if (_destination == _rhs)
		{
			// NOTE: the rhs is already in the destination register, so and lhs directly
			PUT(X86Builder::and_rr(_destination, _lhs));
		}

		else
		{
			// mov destination, lhs
			// and destination, rhs
			PUT(X86Builder::mov_rr(_destination, _lhs));
			PUT(X86Builder::and_rr(_destination, _rhs));
		}

		return out;
	}


	BinaryCommandType BitxorCommand::btype() const
	{
		return BinaryCommandType::BITXOR;
	}

	byterange BitxorCommand::emit() const
	{
		byterange out{};

		if (_destination == _lhs)
		{
			// NOTE: the lhs is already in the destination register, so xor rhs directly
			// xor destination, rhs
			PUT(X86Builder::xor_rr(_destination, _rhs));
		}

		else if (_destination == _rhs)
		{
			// NOTE: the rhs is already in the destination register, so xor lhs directly
			PUT(X86Builder::xor_rr(_destination, _lhs));
		}

		else
		{
			// mov destination, lhs
			// xor destination, rhs
			PUT(X86Builder::mov_rr(_destination, _lhs));
			PUT(X86Builder::xor_rr(_destination, _rhs));
		}

		return out;
	}


	BinaryCommandType EqualityCommand::btype() const
	{
		return BinaryCommandType::EQUALITY;
	}

	byterange EqualityCommand::emit() const
	{
		// cmp lhs, rhs
		// sete destination
		// movzx destination, destination

		byterange out{};

		PUT(X86Builder::cmp_rr(_lhs, _rhs));
		PUT(X86Builder::sete(_destination));
		PUT(X86Builder::movzx(_destination, _destination));

		return out;
	}


	BinaryCommandType LessCommand::btype() const
	{
		return BinaryCommandType::LESS;
	}

	byterange LessCommand::emit() const
	{
		// NOTE: comparison is for two SIGNED integers
		// use set(above|below) for unsigned comparisons

		// cmp lhs, rhs
		// setl destination
		// movzx destination, destination

		byterange out{};

		PUT(X86Builder::cmp_rr(_lhs, _rhs));
		PUT(X86Builder::setl(_destination));
		PUT(X86Builder::movzx(_destination, _destination));

		return out;
	}


	BinaryCommandType GreaterCommand::btype() const
	{
		return BinaryCommandType::GREATER;
	}

	byterange GreaterCommand::emit() const
	{
		// NOTE: comparison is for two SIGNED integers
		// use set(above|below) for unsigned comparisons

		// cmp lhs, rhs
		// setg destination
		// movzx destination, destination

		byterange out{};

		PUT(X86Builder::cmp_rr(_lhs, _rhs));
		PUT(X86Builder::setg(_destination));
		PUT(X86Builder::movzx(_destination, _destination));

		return out;
	}


	IntermediateType IncrementCommand::itype() const
	{
		return IntermediateType::INCREMENT;
	}

	byterange IncrementCommand::emit() const
	{
		byterange out{};

		if (_destination != _source)
		{
			// mov destination, source
			PUT(X86Builder::mov_rr(_destination, _source));
		}

		// inc destination
		PUT(X86Builder::inc_r(_destination));

		return out;
	}


	IntermediateType DecrementCommand::itype() const
	{
		return IntermediateType::DECREMENT;
	}

	byterange DecrementCommand::emit() const
	{
		byterange out{};

		if (_destination != _source)
		{
			// mov destination, source
			PUT(X86Builder::mov_rr(_destination, _source));
		}

		// dec destination
		PUT(X86Builder::dec_r(_destination));

		return out;
	}


	IntermediateType CopyCommand::itype() const
	{
		return IntermediateType::COPY;
	}

	byterange CopyCommand::emit() const
	{

		byterange out{};

		if (_destination != _source)
		{
			// mov destination, src
			PUT(X86Builder::mov_rr(_destination, _source));
		}

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


	IntermediateType BranchCommand::itype() const
	{
		return IntermediateType::BRANCH;
	}


	BranchCommandType CallFunctionCommand::btype() const
	{
		return BranchCommandType::CALL_FUNCTION;
	}

	byterange CallFunctionCommand::emit() const
	{
		byterange out{};

		// NOTE: old method
		//const auto target = _generator->query_branch_target(function);

		// NOTE: old method
		// now, the callee is responsible for popping function parameters
		// which is done by `ret imm16` instead of manually `sub`-bing here
		//PUT(X86Builder::add_ri(ESP, 0x1000));

		PUT(X86Builder::call_relative(target_offset));

		return out;
	}


	BranchCommandType VoidReturnCommand::btype() const
	{
		return BranchCommandType::VOID_RETURN;
	}

	byterange VoidReturnCommand::emit() const
	{
		// NOTE: old method
		// ret

		// jmp end_function_label

		byterange out{};

		// NOTE: old methood
		// tearing down the stack frame is now done in `LeaveScopeCommand`
		/*const auto& current_function = _generator->current_function();

		const auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, current_function, NULL_TOKEN);
		const auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		const auto arity = function_symbol->arity();

		if (arity == 0)
		{
			PUT(X86Builder::ret());
		}

		else
		{
#pragma message("TODO: compute the correct number of bytes to pop based on argument sizes")
			// pop all arguments pushed by the caller
			const auto bytes = arity * 4;
			PUT(X86Builder::ret(bytes));
		}*/

		PUT(X86Builder::jmp_relative(target_offset));

		return out;
	}


	BranchCommandType ValueReturnCommand::btype() const
	{
		return BranchCommandType::VALUE_RETURN;
	}

	byterange ValueReturnCommand::emit() const
	{
		// NOTE: old method
		// mov eax, location
		// ret

		// jmp end_function_label

		byterange out{};

		// NOTE: old method
		// NOTE: this will clobber anything currently in the EAX register
		// but this should be OK since they are locals anyway

		/*if (_location != EAX)
		{
			PUT(X86Builder::mov_rr(EAX, _location));
		}*/
		
		// NOTE: old method
		// now this is done when tearing down the stack frame in `LeaveScopeCommand`
		/*const auto& current_function = _generator->current_function();

		const auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, current_function, NULL_TOKEN);
		const auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		const auto arity = function_symbol->arity();

		if (arity == 0)
		{
			PUT(X86Builder::ret());
		}

		else
		{
#pragma message("TODO: compute the correct number of bytes to pop based on argument sizes")
			// pop all arguments pushed by the caller
			const auto bytes = arity * 4;
			PUT(X86Builder::ret(bytes));
		}*/

		PUT(X86Builder::jmp_relative(target_offset));

		return out;
	}


	BranchCommandType IfNotZeroCommand::btype() const
	{
		return BranchCommandType::IF_NOT_ZERO;
	}

	byterange IfNotZeroCommand::emit() const
	{
		// test value, value
		// jne target_offset

		byterange out{};

		PUT(X86Builder::test_rr(_value, _value));
		PUT(X86Builder::jne_relative(target_offset));

		return out;
	}


	BranchCommandType IfZeroCommand::btype() const
	{
		return BranchCommandType::IF_ZERO;
	}

	byterange IfZeroCommand::emit() const
	{
		// test value, value
		// je target_offset

		byterange out{};

		PUT(X86Builder::test_rr(_value, _value));
		PUT(X86Builder::je_relative(target_offset));

		return out;
	}


	BranchCommandType GotoCommand::btype() const
	{
		return BranchCommandType::GOTO;
	}

	byterange GotoCommand::emit() const
	{
		// jmp target_offset

		byterange out{};

		PUT(X86Builder::jmp_relative(target_offset));

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
		// WriteConsole(STDHANDLE, &string, strlen, NULL, NULL)
		// push NULL
		// push NULL
		// push strlen
		// push &string
		// push [0x004032F0] ; provided console stdout handle
		// call [WriteConsole]

		byterange out{};

		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(_length));
		PUT(X86Builder::push_i32(_pointer));
		PUT(X86Builder::push_m(STDOUT_HANDLE));
		PUT(X86Builder::call_absolute(PROCEDURE(writeconsole_iat_va)));

		return out;
	}


	IntermediateType PrintNumberCommand::itype() const
	{
		return IntermediateType::PRINT_NUMBER;
	}

	byterange PrintNumberCommand::emit() const
	{
		// wnsprintfA(buffer, 0xF, "%d\n", _value)
		// push value
		// push format_string
		// push buffer_size
		// push buffer
		// call [wnsprintfA]

		// WriteConsole(STDHANDLE, &string, strlen, NULL, NULL)
		// push NULL
		// push NULL
		// push strlen
		// push &string
		// push [0x4032F0] ; provided console stdout handle
		// call [WriteConsole]

		byterange out{};

		PUT(X86Builder::push_r(EAX));

		// print --> "Program output: "
		// HACK: TEMP
		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(16));
		PUT(X86Builder::push_i32(PROGRAM_OUTPUT_STRING));
		PUT(X86Builder::push_m(STDOUT_HANDLE));
		PUT(X86Builder::call_absolute(PROCEDURE(writeconsole_iat_va)));
		// HACK: TEMP

		PUT(X86Builder::pop_r(EAX));

		PUT(X86Builder::push_r(_value));
		PUT(X86Builder::push_i32(NUMBER_FORMAT_STRING));
		PUT(X86Builder::push_i8(PRINT_BUFFER_SIZE));
		PUT(X86Builder::push_i32(PRINT_BUFFER));
		PUT(X86Builder::call_absolute(PROCEDURE(wnsprintfa_iat_va)));

		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(PRINT_BUFFER_SIZE));
		PUT(X86Builder::push_i32(PRINT_BUFFER));
		PUT(X86Builder::push_m(STDOUT_HANDLE));
		PUT(X86Builder::call_absolute(PROCEDURE(writeconsole_iat_va)));

		return out;
	}


	IntermediateType ExitProgramCommand::itype() const
	{
		return IntermediateType::EXIT_PROGRAM;
	}

	byterange ExitProgramCommand::emit() const
	{
		// push code
		// call [ExitProcess]

		byterange out{};

		PUT(X86Builder::push_r(_code));
		PUT(X86Builder::call_absolute(PROCEDURE(exitprocess_iat_va)));

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
