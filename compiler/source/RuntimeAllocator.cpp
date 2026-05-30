import std;

#include <allocator/Allocator.h>
#include <error/ErrorReporter.h>

// Haze RuntimeAllocator.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace 
{
	using namespace hz;

	void already_defined_error(const std::string& name, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("variable `{}` was already defined in the current scope", name), token);
	}

	void not_defined_error(const std::string& name, const Token& token)
	{
		USE_UNSAFE(ErrorReporter)->post_error(std::format("variable `{}` was not defined in the current scope", name), token);
	}
}

namespace hz
{
	std::optional<Offset> Allocator::get_function_stack_size(const std::string& name)
	{
		if (!stack_size.contains(name))
		{
			return std::nullopt;
		}
		return { stack_size.at(name) };
	}

	bool Allocator::define_local(const std::string& name)
	{
		const auto& current_function = REQUIRE_SAFE(Generator)->current_function();
		
		if (locals_offsets[current_function].contains(name))
		{
			::already_defined_error(name, NULL_TOKEN);
			return false;
		}

		// NOTE: intentionally leaving `undefined` state to show that it is currently unmapped
		// but want to indicate that the variable exists at least
		locals_offsets[current_function][name] = -1;
		
		return true;
	}

	bool Allocator::define_local(const std::string& name, Register source)
	{
		if (!define_local(name))
		{
			return false;
		}

		const auto& current_function = REQUIRE_SAFE(Generator)->current_function();

		// figure out how much memory is required for the current local 
		const auto symbol = USE_SAFE(SymbolDatabase)->reference_variable(name, NULL_TOKEN);
		const auto size = symbol->type.size();

		stack_size[current_function] += size;

		const auto offset = -stack_size[current_function];
		REQUIRE_SAFE(Generator)->stack_write(offset, source);
		locals_offsets[current_function][name] = offset;

		return true;
	}

	void Allocator::attach_local(const std::string& name, Offset offset)
	{
		const auto& current_function = REQUIRE_SAFE(Generator)->current_function();

		if (locals_offsets[current_function].contains(name))
		{
			::already_defined_error(name, NULL_TOKEN);
			return;
		}

		locals_offsets[current_function][name] = offset;
	}

	void Allocator::destroy_local(const std::string& name)
	{
		locals_offsets.erase(name);
	}

	void Allocator::read_local(Register destination, const std::string& name)
	{
		const auto& current_function = REQUIRE_SAFE(Generator)->current_function();

		if (!locals_offsets[current_function].contains(name))
		{
			::not_defined_error(name, NULL_TOKEN);
			return;
		}

		const auto offset = locals_offsets[current_function].at(name);
		REQUIRE_SAFE(Generator)->stack_read(destination, offset);
	}

	void Allocator::write_local(const std::string& name, Register source)
	{
		const auto& current_function = REQUIRE_SAFE(Generator)->current_function();

		if (!locals_offsets[current_function].contains(name))
		{
			::not_defined_error(name, NULL_TOKEN);
			return;
		}

		const auto offset = locals_offsets[current_function].at(name);
		REQUIRE_SAFE(Generator)->stack_write(offset, source);
	}
}
