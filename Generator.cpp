import std;

#include "Generator.h"
#include "Allocation.h"
#include "Parser.h"
#include "Symbol.h"
#include "Expression.h"
#include "ArgumentExpression.h"
#include "Linkable.h"
#include "LabelCommand.h"
#include "IntermediateCommand.h"
#include "IntegerLiteral.h"
#include "RuntimeAllocator.h"
#include "X86Builder.h"
#include "ErrorReporter.h"

// Haze Generator.cpp
// (c) Connor J. Link. All Rights Reserved.

#define ENCODE(x) _linkables[_linkables.size() - 1].commands.emplace_back(x)
#define COMPOSE(x) _linkables[_linkables.size() - 1].ir.emplace_back(x)

namespace hz
{
	// GLOBALS
	Generator* _generator;
	// GLOBALS


	Generator::Generator(const std::vector<Node*>& program, const std::string& filepath)
		: _program{ program }
	{
		_string_length_map = {};
		_error_reporter->open_context(filepath, "generating");
	}

	Generator::~Generator()
	{
		_error_reporter->close_context();
	}


	const std::string& Generator::current_function() const
	{
		const auto& linkable = _linkables[_linkables.size() - 1];
		const auto symbol = linkable.symbol;
		const auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		return function_symbol->name;
	}

	void Generator::begin_function(const std::string& name)
	{
		_linkables.emplace_back(Linkable{ _parser->reference_symbol(SymbolType::FUNCTION, name, NULL_TOKEN), {}, {}, 0 });
	}

	void Generator::label_command(const std::string& identifier)
	{
		_linkables[_linkables.size() - 1].commands.emplace_back(new LabelCommand{identifier, NULL_TOKEN});
	}

	void Generator::branch_label(const std::string& label)
	{
		auto command = new BranchLabelCommand{ label };
		COMPOSE(command);
	}

	void Generator::begin_scope()
	{
		auto command = new EnterScopeCommand{};
		COMPOSE(command);
	}

	void Generator::end_scope()
	{
		auto command = new LeaveScopeCommand{};
		COMPOSE(command);
	}

	void Generator::define_local(const std::string& name, register_t source)
	{
		_runtime_allocator->define_local(name, source);
	}

	void Generator::define_local(const std::string& name)
	{
		_runtime_allocator->define_local(name);
	}

	void Generator::attach_local(const std::string& name, std::int32_t offset)
	{
		_runtime_allocator->attach_local(name, offset);
	}

	void Generator::destroy_local(const std::string& name)
	{
		_runtime_allocator->destroy_local(name);
	}

	void Generator::read_local(register_t destination, const std::string& name)
	{
		_runtime_allocator->read_local(destination, name);
	}

	void Generator::update_local(const std::string& name, register_t source)
	{
		_runtime_allocator->update_local(name, source);
	}

	void Generator::make_global(register_t location, Variable* value)
	{
		auto command = new GlobalVariableCommand{ location, value };
		COMPOSE(command);
	}

	void Generator::heap_read(register_t destination, std::uint32_t pointer)
	{
		auto command = new HeapReadCommand{ destination, pointer };
		COMPOSE(command);
	}

	void Generator::heap_write(std::uint32_t pointer, register_t source)
	{
		auto command = new HeapWriteCommand{ pointer, source };
		COMPOSE(command);
	}

	void Generator::stack_read(register_t destination, std::int32_t offset)
	{
		auto command = new StackReadCommand{ destination, offset };
		COMPOSE(command);
	}

	void Generator::stack_write(std::int32_t offset, register_t source)
	{
		auto command = new StackWriteCommand{ offset, source };
		COMPOSE(command);
	}

	void Generator::compute_add(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new AddCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_subtract(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new SubtractCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitor(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitorCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitand(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitandCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitxor(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new BitxorCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_equality(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new EqualityCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_inequality(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new InequalityCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_less(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new LessCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_greater(register_t lhs, register_t rhs, register_t destination)
	{
		auto command = new GreaterCommand{ lhs, rhs, destination };
		COMPOSE(command);
	}

	void Generator::compute_bool(register_t destination, register_t source)
	{
		auto command = new BoolCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::compute_increment(register_t destination, register_t source)
	{
		auto command = new IncrementCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::compute_decrement(register_t destination, register_t source)
	{
		auto command = new DecrementCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::make_copy(register_t destination, register_t source)
	{
		auto command = new CopyCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::make_immediate(register_t destination, IntegerLiteral immediate)
	{
		auto command = new MakeImmediateCommand{ destination, immediate };
		COMPOSE(command);
	}

	void Generator::make_argument(const std::string& name, register_t location)
	{
		// NOTE: old method
		//define_local(name, location);
		
		auto command = new MakeArgumentCommand{ location };
		COMPOSE(command);
	}

	void Generator::take_argument(const std::string& name, register_t location, std::int32_t offset)
	{
		// NOTE: old method
		//define_local(name, location);

		auto command = new TakeArgumentCommand{ location, offset };
		COMPOSE(command);
	}

	void Generator::call_function(const std::string& function)
	{
		auto command = new CallFunctionCommand{ function };
		COMPOSE(command);
	}

	void Generator::call_function(const std::string& function, const std::vector<Expression*>& arguments, Allocation* allocation)
	{
		for (auto expression : arguments)
		{
			auto argument_expression = AS_ARGUMENT_EXPRESSION(expression);
			argument_expression->generate(allocation);

			make_argument(argument_expression->identifier->name, allocation->read());
		}

		auto command = new CallFunctionCommand{ function };
		COMPOSE(command);
	}

	void Generator::make_return(const std::string& label)
	{
		auto command = new VoidReturnCommand{ label };
		COMPOSE(command);
	}

	void Generator::make_return(std::int32_t target_offset)
	{
		auto command = new VoidReturnCommand{ target_offset };
		COMPOSE(command);
	}

	void Generator::make_return(const std::string& label, register_t value)
	{
		auto command = new ValueReturnCommand{ label, value };
		COMPOSE(command);
	}

	void Generator::make_return(std::int32_t target_offset, register_t value)
	{
		auto command = new ValueReturnCommand{ target_offset, value };
		COMPOSE(command);
	}

	void Generator::check_ifnz(std::int32_t target_offset, register_t value)
	{
		auto command = new IfNotZeroCommand{ target_offset, value };
		COMPOSE(command);
	}

	void Generator::check_ifnz(const std::string& label, register_t value)
	{
		auto command = new IfNotZeroCommand{ label, value };
		// NOTE: old method
		//register_branch(command, label);
		COMPOSE(command);
	}

	void Generator::check_ifz(std::int32_t target_offset, register_t value)
	{
		auto command = new IfZeroCommand{ target_offset, value };
		COMPOSE(command);
	}

	void Generator::check_ifz(const std::string& label, register_t value)
	{
		auto command = new IfZeroCommand{ label, value };
		// NOTE: old method
		//register_branch(command, label);
		COMPOSE(command);
	}

	void Generator::goto_command(std::int32_t target_offset)
	{
		auto command = new GotoCommand{ target_offset };
		COMPOSE(command);
	}

	void Generator::goto_command(const std::string& label)
	{
		auto command = new GotoCommand{ label };
		COMPOSE(command);
	}

	void Generator::make_message(std::uint32_t pointer, std::string message)
	{
		// including an extra byte for the implicit NULL terminator
		const auto length = message.length() + 1;

		_string_length_map[pointer] = static_cast<std::uint32_t>(length);

		auto command = new MakeMessageCommand{ pointer, message };
		COMPOSE(command);
	}

	void Generator::print_message(std::uint32_t pointer)
	{
		if (_string_length_map.contains(pointer))
		{
			auto command = new PrintMessageCommand{ pointer, _string_length_map.at(pointer) };
			COMPOSE(command);
			return;
		}

		_error_reporter->post_error(std::format("undefined string pointer `{:08X}`", pointer), NULL_TOKEN);
	}

	void Generator::print_number(register_t value)
	{
		auto command = new PrintNumberCommand{ value };
		COMPOSE(command);
	}

	void Generator::exit_program(register_t code)
	{
		auto command = new ExitProgramCommand{ code };
		COMPOSE(command);
	}


	index_t Generator::resolve_origin_old() const
	{
		UNSUPPORTED_OPERATION(__FUNCTION__);
		//return static_cast<index_t>(_linkables[_linkables.size() - 1].commands.size());
	}

	index_t Generator::resolve_origin() const
	{
		return static_cast<index_t>(_linkables[_linkables.size() - 1].ir.size());
	}


	void Generator::raw_binary(byterange&& object_code)
	{
		const auto approximate_length = static_cast<std::uint32_t>(object_code.size());

		// emplacing a bitstream is basically the same as compiled inline assembly
		inline_assembly(std::move(object_code), approximate_length);
	}

	void Generator::inline_assembly(byterange&& object_code, std::uint32_t approximate_size)
	{
		auto command = new InlineAssemblyCommand{ std::move(object_code), approximate_size };
		COMPOSE(command);
	}


	std::vector<Linkable> Generator::generate()
	{
		for (auto function : _program)
		{
			function->generate();
		}

		// Reorder defined functions so `main` is first since it's the entrypoint
		std::ranges::partition(_linkables, [](auto& linkable)
		{
			if (linkable.symbol->name == "main")
			{
				// NOTE: old method
				// put a stop instruction at the end of main
				//*(linkable.commands.end() - 1) = (new InstructionCommand{ NULL_TOKEN, Opcode::STOP, DC, DC });
				return true;
			}

			return false;
		});

		return _linkables;	
	}
}
