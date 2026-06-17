import std;

#include <allocator/Allocator.h>
#include <ast/expression/Expression.h>
#include <ir/IntermediateCommand.h>
#include <command/Command.h>
#include <symbol/models/Symbol.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <toolchain/models/Linkable.h>

// Haze Generator.cpp
// (c) Connor J. Link. All Rights Reserved.

#define ENCODE(x) linkables[linkables.size() - 1].commands.emplace_back(x)
#define COMPOSE(x) linkables[linkables.size() - 1].ir.emplace_back(x)

namespace hz
{
	std::string_view Generator::current_function() const
	{
		const auto& linkable = linkables[linkables.size() - 1];
		const auto function = linkable.function;

		return function.get<Function>()->name;
	}

	void Generator::begin_function(std::string_view name)
	{
		linkables.emplace_back(USE_SAFE(SymbolDatabase)->reference_symbol(SymbolKind::FUNCTION, name, NULL_TOKEN), {}, {}, 0);
	}

	void Generator::label_command(const std::string& identifier)
	{
		linkables[linkables.size() - 1].commands.emplace_back(new LabelCommand{identifier, NULL_TOKEN});
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

	void Generator::make_global(Register location, Variable* value)
	{
		auto command = new GlobalVariableCommand{ location, value };
		COMPOSE(command);
	}

	void Generator::heap_read(Register destination, Address pointer)
	{
		auto command = new HeapReadCommand{ destination, pointer };
		COMPOSE(command);
	}

	void Generator::heap_write(Address pointer, Register source)
	{
		auto command = new HeapWriteCommand{ pointer, source };
		COMPOSE(command);
	}

	void Generator::stack_read(Register destination, Offset offset)
	{
		auto command = new StackReadCommand{ destination, offset };
		COMPOSE(command);
	}

	void Generator::stack_write(Offset offset, Register source)
	{
		auto command = new StackWriteCommand{ offset, source };
		COMPOSE(command);
	}

	void Generator::compute_add(Register left, Register right, Register destination)
	{
		auto command = new AddCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_subtract(Register left, Register right, Register destination)
	{
		auto command = new SubtractCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_multiplication(Register left, Register right, Register destination)
	{
		auto command = new MultiplyCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitor(Register left, Register right, Register destination)
	{
		auto command = new BitorCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitand(Register left, Register right, Register destination)
	{
		auto command = new BitandCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitlshift(Register left, Register right, Register destination)
	{
		auto command = new BitlshiftCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitrshift(Register left, Register right, Register destination)
	{
		auto command = new BitrshiftCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_bitxor(Register left, Register right, Register destination)
	{
		auto command = new BitxorCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_equality(Register left, Register right, Register destination)
	{
		auto command = new EqualityCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_inequality(Register left, Register right, Register destination)
	{
		auto command = new InequalityCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_less(Register left, Register right, Register destination)
	{
		auto command = new LessCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_greater(Register left, Register right, Register destination)
	{
		auto command = new GreaterCommand{ left, right, destination };
		COMPOSE(command);
	}

	void Generator::compute_bool(Register destination, Register source)
	{
		auto command = new BoolCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::compute_increment(Register destination, Register source)
	{
		auto command = new IncrementCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::compute_decrement(Register destination, Register source)
	{
		auto command = new DecrementCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::make_copy(Register destination, Register source)
	{
		auto command = new CopyCommand{ destination, source };
		COMPOSE(command);
	}

	void Generator::make_immediate(BigInteger value)
	{
		auto command = new MakeImmediateCommand{ value };
		COMPOSE(command);
	}

	void Generator::make_argument(const std::string& name, Register location)
	{
		auto command = new MakeArgumentCommand{ location };
		COMPOSE(command);
	}

	void Generator::take_argument(const std::string& name, Register location, Offset offset)
	{
		auto command = new TakeArgumentCommand{ location, offset };
		COMPOSE(command);
	}

	void Generator::call_function(const std::string& function)
	{
		auto command = new CallFunctionCommand{ function };
		COMPOSE(command);
	}

	void Generator::call_function(const std::string& function, const std::vector<ExpressionReference<ArgumentExpression>>& arguments, ValueHandle allocation)
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

	void Generator::make_return(const std::string& label, std::int8_t value)
	{
		auto command = new ValueReturnCommand{ label, value };
		COMPOSE(command);
	}

	void Generator::make_return(std::int32_t target_offset, std::int8_t value)
	{
		auto command = new ValueReturnCommand{ target_offset, value };
		COMPOSE(command);
	}

	void Generator::check_ifnz(std::int32_t target_offset, std::int8_t value)
	{
		auto command = new IfNotZeroCommand{ target_offset, value };
		COMPOSE(command);
	}

	void Generator::check_ifnz(const std::string& label, std::int8_t value)
	{
		auto command = new IfNotZeroCommand{ label, value };
		// NOTE: old method
		//register_branch(command, label);
		COMPOSE(command);
	}

	void Generator::check_ifz(std::int32_t target_offset, std::int8_t value)
	{
		auto command = new IfZeroCommand{ target_offset, value };
		COMPOSE(command);
	}

	void Generator::check_ifz(const std::string& label, std::int8_t value)
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

	void Generator::make_message(std::uint32_t pointer, const std::string& message)
	{
		// including an extra byte for the implicit NULL terminator
		const auto length = message.length() + 1;

		string_length_map[pointer] = static_cast<std::uint32_t>(length);

		auto command = new MakeMessageCommand{ pointer, message };
		COMPOSE(command);
	}

	void Generator::print_message(std::uint32_t pointer)
	{
		if (string_length_map.contains(pointer))
		{
			auto command = new PrintMessageCommand{ pointer, string_length_map.at(pointer) };
			COMPOSE(command);
			return;
		}

		USE_SAFE(ErrorReporter)->post_error(std::format("undefined string pointer `{:08X}`", pointer), NULL_TOKEN);
	}

	void Generator::print_number(std::int8_t value)
	{
		auto command = new PrintNumberCommand{ value };
		COMPOSE(command);
	}

	void Generator::exit_program(std::int8_t code)
	{
		auto command = new ExitProgramCommand{ code };
		COMPOSE(command);
	}


	Address Generator::resolve_origin() const
	{
		return static_cast<Address>(linkables[linkables.size() - 1].ir.size());
	}


	void Generator::raw_binary(const ByteRange& object_code)
	{
		const auto approximate_length = static_cast<std::uint32_t>(object_code.size());

		// emplacing a bitstream is basically the same as compiled inline assembly
		inline_assembly(object_code, approximate_length);
	}

	void Generator::inline_assembly(const ByteRange& object_code, std::uint32_t approximate_size)
	{
		auto command = new InlineAssemblyCommand{ object_code, approximate_size };
		COMPOSE(command);
	}


	std::vector<Linkable> Generator::generate()
	{
		for (auto function : program)
		{
			function.generate();
		}

		// reorder defined functions so `main` is first since it's the entrypoint
		// IntelliSense error only
		std::ranges::partition(linkables, [](auto& linkable)
		{
			if (linkable.symbol->name == "main")
			{
				return true;
			}

			return false;
		});

		return linkables;
	}


	Generator::Generator(std::vector<DeclarationHandle> program, const std::filesystem::path& filepath)
		: program{ std::move(program) }
	{
		USE_SAFE(ErrorReporter)->open_context(filepath, "generating");
	}

	Generator::~Generator()
	{
		USE_SAFE(ErrorReporter)->close_context();
	}

}
