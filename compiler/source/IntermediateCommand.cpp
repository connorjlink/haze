import std;

#include <ir/IntermediateCommand.h>
#include <symbol/Symbol.h>
#include <type/Type.h>
#include <utility/BinaryUtilities.h>
#include <utility/BinaryConstants.h>
#include <utility/Constants.h>
#include <x86/X86Builder.h>
#include <x86/defs/X86Register.h>
#include <x86/X86Instruction.h>

// Haze IntermediateCommand.cpp
// (c) Connor J. Link. All Rights Reserved.

#define EMIT(x) PUT(x.emit())

namespace
{
	using namespace hz;

	constexpr void assert(bool condition, const char* message = nullptr)
	{
#ifndef NDEBUG
		if (!condition)
		{
			const auto real_message = message != nullptr 
				? message 
				: "Debug assertion failed";

			USE_UNSAFE(ErrorReporter)->post_uncorrectable(real_message, NULL_TOKEN);
		}
#endif
	}

	struct SumFunctor
	{
		template<std::ranges::input_range R>
		constexpr auto operator()(R&& r) const
		{
			using T = std::ranges::range_value_t<R>;
			return std::ranges::fold_left(std::forward<R>(r), T{}, std::plus<>{});
		}

		template<std::ranges::input_range R>
		friend constexpr auto operator|(R&& r, const SumFunctor& fn)
		{
			return fn(std::forward<R>(r));
		}
	};

	inline constexpr SumFunctor sum{};
}

namespace hz
{
	using namespace x86;

	IntermediateTypeKind BranchLabelCommand::instruction_kind() const
	{
		return IntermediateTypeKind::BRANCH_LABEL;
	}

	ByteRange BranchLabelCommand::emit() const
	{
		// No binary code is generated for a label
		return {};
	}


	IntermediateType EnterScopeCommand::instruction_kind() const
	{
		return IntermediateType::ENTER_SCOPE;
	}

	constexpr ByteRange EnterScopeCommand::_emit(std::uint32_t locals_count, std::int32_t bytes) const
	{
		// push ebp
		// mov ebp, esp
		// sub esp, N

		// NOTE: equivalent to -->
		// enter 0, N (size of local variables)

		ByteRange out{};

		EMIT(push(std::make_unique<RegisterOperand>(EBP)));
		EMIT(push(std::make_unique<RegisterOperand>(EBP), std::make_unique<RegisterOperand>(ESP)));

		if (locals_count > 0)
		{
			// stack frame corresponds to total size of guaranteed local variables
			EMIT(sub(std::make_unique<RegisterOperand>(ESP), std::make_unique<ImmediateOperand>(bytes)));
		}

		return out;
	}

	ByteRange EnterScopeCommand::emit() const
	{
		const auto& current_function = REQUIRE_SAFE(Generator)->current_function();

		auto symbol = USE_SAFE(SymbolDatabase)->reference_symbol(SymbolKind::FUNCTION, current_function, NULL_TOKEN);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		const auto locals_count = function_symbol->locals_count;

		return _emit(locals_count, _bytes);
	}

	/*constexpr std::int32_t EnterScopeCommand::bytes() const
	{
		return _emit(1, 0).size();
	}*/


	IntermediateType LeaveScopeCommand::instruction_kind() const
	{
		return IntermediateType::LEAVE_SCOPE;
	}

	ByteRange LeaveScopeCommand::emit() const
	{
		// mov esp, ebp
		// pop ebp
		
		// NOTE: equivalent to -->
		// leave

		ByteRange out{};

		EMIT(leave());
		
		const auto& current_function = REQUIRE_SAFE(Generator)->current_function();

		const auto symbol = USE_SAFE(SymbolDatabase)->reference_symbol(SymbolKind::FUNCTION, current_function, NULL_TOKEN);
		const auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		const auto arity = function_symbol->arity();
		if (arity == 0)
		{
			EMIT(ret());
		}
		else
		{
			// pop all arguments pushed by the caller
			const auto size = function_symbol->arguments
				| std::ranges::views::transform([](auto argument) { return AS_ARGUMENT_EXPRESSION(argument)->type->size(); })
				| ::sum;

			EMIT(ret(size));
		}

		return out;
	}


	IntermediateType LocalVariableCommand::instruction_kind() const
	{
		return IntermediateType::LOCAL_VARIABLE;
	}

	ByteRange LocalVariableCommand::emit() const
	{
		ByteRange out{};

#pragma message("TODO: local variable code generation")

		return out;
	}


	IntermediateType GlobalVariableCommand::instruction_kind() const
	{
		return IntermediateType::GLOBAL_VARIABLE;
	}

	ByteRange GlobalVariableCommand::emit() const
	{
		ByteRange out{};

#pragma message("TODO: GLOBAL VARIABLE CODE GENERATION")

		return out;
	}


	IntermediateType HeapReadCommand::instruction_kind() const
	{
		return IntermediateType::HEAP_READ;
	}

	ByteRange HeapReadCommand::emit() const
	{
		// mov destination, [pointer]

		ByteRange out{};

		EMIT(mov(std::make_unique<RegisterOperand>(destination), std::make_unique<IndirectOperand>(pointer)));

		return out;
	}


	IntermediateType HeapWriteCommand::instruction_kind() const
	{
		return IntermediateType::HEAP_WRITE;
	}

	ByteRange HeapWriteCommand::emit() const
	{
		// mov [pointer], destination

		ByteRange out{};

		EMIT(mov(std::make_unique<IndirectOperand>(pointer), std::make_unique<RegisterOperand>(source)));

		return out;
	}


	IntermediateType StackReadCommand::instruction_kind() const
	{
		return IntermediateType::STACK_READ;
	}

	ByteRange StackReadCommand::emit() const
	{
		// mov destination, [ebp + offset]

		ByteRange out{};

		EMIT(mov(std::make_unique<RegisterOperand>(destination), std::make_unique<RegisterDisplacedOperand>(EBP, offset)));

		return out;
	}


	IntermediateType StackWriteCommand::instruction_kind() const
	{
		return IntermediateType::STACK_WRITE;
	}

	ByteRange StackWriteCommand::emit() const
	{
		// mov [ebp + offset], destination
		
		ByteRange out{};

		PUT(X86Builder::mov_obr(offset, source));
		EMIT(mov(std::make_unique<RegisterDisplacedOperand>(EBP, offset), std::make_unique<RegisterOperand>(destination)));

		return out;
	}


	IntermediateType BoolCommand::instruction_kind() const
	{
		return IntermediateType::BOOL;
	}

	ByteRange BoolCommand::emit() const
	{
		// test source, source
		// sete destination
		// movzx destination, destination

		ByteRange out{};

		PUT(X86Builder::test_rr(source, source));
		PUT(X86Builder::sete_r(destination));
		PUT(X86Builder::movzx_rr(destination, source));

		return out;
	}


	IntermediateType BinaryCommand::instruction_kind() const
	{
		return IntermediateType::BINARY;
	}


	BinaryCommandType AddCommand::btype() const
	{
		return BinaryCommandType::ADD;
	}

	ByteRange AddCommand::emit() const
	{
		ByteRange out{};

		if (destination == _lhs)
		{
			// NOTE: the left is already in the destination register, so add right directly
			// add destination, right
			PUT(X86Builder::add_rr(destination, _rhs));
		}

		else if (destination == _rhs)
		{
			// NOTE: the right is already in the destination register, so add left directly
			PUT(X86Builder::add_rr(destination, _lhs));
		}

		else
		{
			// mov destination, left
			// add destination, right
			PUT(X86Builder::mov_rr(destination, _lhs));
			PUT(X86Builder::add_rr(destination, _rhs));
		}

		return out;
	}


	BinaryCommandType SubtractCommand::btype() const
	{
		return BinaryCommandType::SUBTRACT;
	}

	ByteRange SubtractCommand::emit() const
	{
		ByteRange out{};

		if (destination == _lhs)
		{
			// NOTE: the left is already in the destination register, so sub right directly
			// sub destination, right
			PUT(X86Builder::sub_rr(destination, _rhs));
		}
		
		// NOTE: since subtraction is anti-commutative swapping operand order here is less efficient than just moving
		/*else if (destination == _rhs)
		{
		}*/

		else
		{
			// mov destination, left
			// sub destination, right
			PUT(X86Builder::mov_rr(destination, _lhs));
			PUT(X86Builder::sub_rr(destination, _rhs));
		}

		return out;
	}


	BinaryCommandType MultiplyCommand::btype() const
	{
		return BinaryCommandType::MULTIPLY;
	}

	ByteRange MultiplyCommand::emit() const
	{
		ByteRange out{};

#pragma message("TODO: imul instruction")

		return out;
	}


	BinaryCommandType BitorCommand::btype() const
	{
		return BinaryCommandType::BITOR;
	}

	ByteRange BitorCommand::emit() const
	{
		ByteRange out{};

		if (destination == _lhs)
		{
			// NOTE: the left is already in the destination register, so or right directly
			// or destination, right
			PUT(X86Builder::or_rr(destination, _rhs));
		}

		else if (destination == _rhs)
		{
			// NOTE: the right is already in the destination register, so add left directly
			PUT(X86Builder::or_rr(destination, _lhs));
		}

		else
		{
			// mov destination, left
			// or destination, right
			PUT(X86Builder::mov_rr(destination, _lhs));
			PUT(X86Builder::or_rr(destination, _rhs));
		}

		return out;
	}


	BinaryCommandType BitandCommand::btype() const
	{
		return BinaryCommandType::BITAND;
	}

	ByteRange BitandCommand::emit() const
	{
		ByteRange out{};

		if (destination == _lhs)
		{
			// NOTE: the left is already in the destination register, so and right directly
			// and destination, right
			PUT(X86Builder::and_rr(destination, _rhs));
		}

		else if (destination == _rhs)
		{
			// NOTE: the right is already in the destination register, so and left directly
			PUT(X86Builder::and_rr(destination, _lhs));
		}

		else
		{
			// mov destination, left
			// and destination, right
			PUT(X86Builder::mov_rr(destination, _lhs));
			PUT(X86Builder::and_rr(destination, _rhs));
		}

		return out;
	}


	BinaryCommandType BitxorCommand::btype() const
	{
		return BinaryCommandType::BITXOR;
	}

	ByteRange BitxorCommand::emit() const
	{
		ByteRange out{};

		if (destination == _lhs)
		{
			// NOTE: the left is already in the destination register, so xor right directly
			// xor destination, right
			PUT(X86Builder::xor_rr(destination, _rhs));
		}

		else if (destination == _rhs)
		{
			// NOTE: the right is already in the destination register, so xor left directly
			PUT(X86Builder::xor_rr(destination, _lhs));
		}

		else
		{
			// mov destination, left
			// xor destination, right
			PUT(X86Builder::mov_rr(destination, _lhs));
			PUT(X86Builder::xor_rr(destination, _rhs));
		}

		return out;
	}


	BinaryCommandType BitlshiftCommand::btype() const
	{
		return BinaryCommandType::BITLSHIFT;
	}

	ByteRange BitlshiftCommand::emit() const
	{
		ByteRange out{};

		PUT(X86Builder::mov_rr(destination, _lhs));
		PUT(X86Builder::sal_imm(destination, _rhs));

		return out;
	}


	BinaryCommandType BitrshiftCommand::btype() const
	{
		return BinaryCommandType::BITRSHIFT;
	}

	ByteRange BitrshiftCommand::emit() const
	{
		ByteRange out{};

		PUT(X86Builder::mov_rr(destination, _lhs));
		PUT(X86Builder::sar_imm(destination, _rhs));
		
		return out;
	}


	BinaryCommandType EqualityCommand::btype() const
	{
		return BinaryCommandType::EQUALITY;
	}

	ByteRange EqualityCommand::emit() const
	{
		// cmp left, right
		// sete destination
		// movzx destination, destination

		ByteRange out{};

		PUT(X86Builder::cmp_rr(_lhs, _rhs));
		PUT(X86Builder::sete_r(destination));
		PUT(X86Builder::movzx_rr(destination, destination));

		return out;
	}


	BinaryCommandType InequalityCommand::btype() const
	{
		return BinaryCommandType::INEQUALITY;
	}

	ByteRange InequalityCommand::emit() const
	{
		// cmp left, right
		// setne destination
		// movzx destination, destination

		ByteRange out{};

		PUT(X86Builder::cmp_rr(_lhs, _rhs));
		PUT(X86Builder::setne_r(destination));
		PUT(X86Builder::movzx_rr(destination, destination));

		return out;
	}


	BinaryCommandType LessCommand::btype() const
	{
		return BinaryCommandType::LESS;
	}

	ByteRange LessCommand::emit() const
	{
		// NOTE: comparison is for two SIGNED integers
		// use set(above|below) for unsigned comparisons

		// cmp left, right
		// setl destination
		// movzx destination, destination

		ByteRange out{};

		PUT(X86Builder::cmp_rr(_lhs, _rhs));
		PUT(X86Builder::setl_r(destination));
		PUT(X86Builder::movzx_rr(destination, destination));

		return out;
	}


	BinaryCommandType GreaterCommand::btype() const
	{
		return BinaryCommandType::GREATER;
	}

	ByteRange GreaterCommand::emit() const
	{
		// NOTE: comparison is for two SIGNED integers
		// use set(above|below) for unsigned comparisons

		// cmp left, right
		// setg destination
		// movzx destination, destination

		ByteRange out{};

		PUT(X86Builder::cmp_rr(_lhs, _rhs));
		PUT(X86Builder::setg_r(destination));
		PUT(X86Builder::movzx_rr(destination, destination));

		return out;
	}


	IntermediateType IncrementCommand::instruction_kind() const
	{
		return IntermediateType::INCREMENT;
	}

	ByteRange IncrementCommand::emit() const
	{
		ByteRange out{};

		if (destination != source)
		{
			// mov destination, source
			PUT(X86Builder::mov_rr(destination, source));
		}

		// inc destination
		PUT(X86Builder::inc_r(destination));

		return out;
	}


	IntermediateType DecrementCommand::instruction_kind() const
	{
		return IntermediateType::DECREMENT;
	}

	ByteRange DecrementCommand::emit() const
	{
		ByteRange out{};

		if (destination != source)
		{
			// mov destination, source
			PUT(X86Builder::mov_rr(destination, source));
		}

		// dec destination
		PUT(X86Builder::dec_r(destination));

		return out;
	}


	IntermediateType CopyCommand::instruction_kind() const
	{
		return IntermediateType::COPY;
	}

	ByteRange CopyCommand::emit() const
	{

		ByteRange out{};

		if (destination != source)
		{
			// mov destination, source
			PUT(X86Builder::mov_rr(destination, source));
		}

		return out;
	}


	IntermediateType MakeImmediateCommand::instruction_kind() const
	{
		return IntermediateType::MAKE_IMMEDIATE;
	}

	ByteRange MakeImmediateCommand::emit() const
	{
		ByteRange out{};

		PUT(X86Builder::mov_ri(destination, immediate));

		return out;
	}


	IntermediateType MakeArgumentCommand::instruction_kind() const
	{
		return IntermediateType::MAKE_ARGUMENT;
	}

	ByteRange MakeArgumentCommand::emit() const
	{
		// push source

		ByteRange out{};

		PUT(X86Builder::push_r(source));

		return out;
	}


	IntermediateType TakeArgumentCommand::instruction_kind() const
	{
		return IntermediateType::TAKE_ARGUMENT;
	}

	ByteRange TakeArgumentCommand::emit() const
	{
		// mov destination, [ebp + offset]

		ByteRange out{};

		PUT(X86Builder::mov_rbo(destination, offset));

		return out;
	}


	IntermediateType BranchCommand::instruction_kind() const
	{
		return IntermediateType::BRANCH;
	}


	BranchCommandType CallFunctionCommand::btype() const
	{
		return BranchCommandType::CALL_FUNCTION;
	}

	ByteRange CallFunctionCommand::emit() const
	{
		ByteRange out{};

		// NOTE: old method
		//const auto target = _generator->query_branch_target(function);

		// NOTE: old method
		// now, the callee is responsible for popping function parameters
		// which is done by `ret imm16` instead of manually `sub`-bing here
		//PUT(X86Builder::add_ri(ESP, 0x1000));

		::assert(target_offset.has_value(), "Function call relative jump target offset was not defined");
		PUT(X86Builder::call_relative(*target_offset));

		return out;
	}


	BranchCommandType VoidReturnCommand::btype() const
	{
		return BranchCommandType::VOID_RETURN;
	}

	ByteRange VoidReturnCommand::emit() const
	{
		// NOTE: not directly `ret` to ensure avoid duplicating epilogue code
		// - jmp end_function_label

		ByteRange out{};

		::assert(target_offset.has_value(), "Return statement relative jump target offset was not defined");
		PUT(X86Builder::jmp_relative(*target_offset));

		return out;
	}


	BranchCommandType ValueReturnCommand::btype() const
	{
		return BranchCommandType::VALUE_RETURN;
	}

	ByteRange ValueReturnCommand::emit() const
	{
		// NOTE: not directly `ret` to ensure avoid duplicating epilogue code
		// - jmp end_function_label

		ByteRange out{};

		::assert(target_offset.has_value(), "Return statement relative jump target offset was not defined");
		PUT(X86Builder::jmp_relative(*target_offset));

		return out;
	}


	BranchCommandType IfNotZeroCommand::btype() const
	{
		return BranchCommandType::IF_NOT_ZERO;
	}

	ByteRange IfNotZeroCommand::emit() const
	{
		// - test value, value
		// - jne target_offset

		ByteRange out{};

		PUT(X86Builder::test_rr(_value, _value));
		::assert(target_offset.has_value(), "Ifnz conditional statement relative jump target offset was not defined");
		PUT(X86Builder::jne_relative(*target_offset));

		return out;
	}


	BranchCommandType IfZeroCommand::btype() const
	{
		return BranchCommandType::IF_ZERO;
	}

	ByteRange IfZeroCommand::emit() const
	{
		// - test value, value
		// - je target_offset

		ByteRange out{};

		PUT(X86Builder::test_rr(_value, _value));
		::assert(target_offset.has_value(), "Ifz conditional statement relative jump target offset was not defined");
		PUT(X86Builder::je_relative(*target_offset));

		return out;
	}


	BranchCommandType GotoCommand::btype() const
	{
		return BranchCommandType::GOTO;
	}

	ByteRange GotoCommand::emit() const
	{
		// - jmp target_offset

		ByteRange out{};

		::assert(target_offset.has_value(), "Goto statement relative jump target offset was not defined");
		PUT(X86Builder::jmp_relative(*target_offset));

		return out;
	}


	IntermediateType MakeMessageCommand::instruction_kind() const
	{
		return IntermediateType::MAKE_MESSAGE;
	}

	ByteRange MakeMessageCommand::emit() const
	{
		ByteRange out{};

#pragma message("TODO: make message code generation")

		return out;
	}

	
	IntermediateType PrintMessageCommand::instruction_kind() const
	{
		return IntermediateType::PRINT_MESSAGE;
	}

	ByteRange PrintMessageCommand::emit() const
	{
		// WriteConsole(STDHANDLE, &string, strlen, NULL, NULL):
		// - push NULL
		// - push NULL
		// - push strlen
		// - push &string
		// - push [0x004032F0] ; provided console stdout handle
		// - call [WriteConsole]

		ByteRange out{};

		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(0x00));
		PUT(X86Builder::push_i8(length));
		PUT(X86Builder::push_i32(pointer));
		PUT(X86Builder::push_m(STDOUT_HANDLE));
		PUT(X86Builder::call_absolute(PROCEDURE(writeconsole_iat_va)));

		return out;
	}


	IntermediateType PrintNumberCommand::instruction_kind() const
	{
		return IntermediateType::PRINT_NUMBER;
	}

	ByteRange PrintNumberCommand::emit() const
	{
		// wnsprintfA(buffer, 0xF, "%d\n", _value)"
		// - push value
		// - push format_string
		// - push buffer_size
		// - push buffer
		// - call [wnsprintfA]

		// WriteConsole(STDHANDLE, &string, strlen, NULL, NULL):
		// - push NULL
		// - push NULL
		// - push strlen
		// - push &string
		// - push [0x4032F0] ; provided console stdout handle
		// - call [WriteConsole]

		ByteRange out{};

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


	IntermediateType ExitProgramCommand::instruction_kind() const
	{
		return IntermediateType::EXIT_PROGRAM;
	}

	ByteRange ExitProgramCommand::emit() const
	{
		// - push code
		// - call [ExitProcess]

		ByteRange out{};

		PUT(X86Builder::push_r(_code));
		PUT(X86Builder::call_absolute(PROCEDURE(exitprocess_iat_va)));

		return out;
	}


	IntermediateType InlineAssemblyCommand::instruction_kind() const
	{
		return IntermediateType::INLINE_ASSEMBLY;
	}

	ByteRange InlineAssemblyCommand::emit() const
	{
		return code;
	}
}
