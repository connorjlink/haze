#ifndef HAZE_ALLOCATOR_H
#define HAZE_ALLOCATOR_H

#include <data/DependencyInjector.h>
#include <symbol/SymbolDatabase.h>
#include <toolchain/Generator.h>
#include <utility/Constants.h>

// Haze Allocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct Allocator 
		: public ServiceTag<Allocator>
		, public InjectService<Generator>
		, public InjectSingleton<SymbolDatabase>
	{
	private:
		// map of (function -> (map of variables -> offset))
		std::unordered_map<std::string_view, std::unordered_map<std::string_view, Offset>> locals_offsets;
		// map of (function -> current stack size)
		std::unordered_map<std::string_view, Offset> stack_size;

	public:
		std::optional<Offset> get_function_stack_size(std::string_view);

	public:
		bool define_local(std::string_view);
		bool define_local(std::string_view, Register);
		// view but do not allocate space for a new local
		void attach_local(std::string_view, Offset);

	public:
		void destroy_local(std::string_view);

	public:
		void read_local(Register, std::string_view);
		void write_local(std::string_view, Register);

	public:
		Allocator()
			: locals_offsets{}
		{
		}
	};
}

#endif 
