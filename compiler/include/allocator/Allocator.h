#ifndef HAZE_ALLOCATOR_H
#define HAZE_ALLOCATOR_H

#include <data/DependencyInjector.h>
#include <toolchain/Generator.h>
#include <utility/Constants.h>

// Haze Allocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Allocator 
		: public ServiceTag<Allocator>
		, public InjectService<Generator>
	{
	private:
		// map of (function -> (map of variables -> offset))
		std::unordered_map<std::string, std::unordered_map<std::string, Offset>> locals_offsets;
		// map of (function -> current stack size)
		std::unordered_map<std::string, Offset> stack_size;

	public:
		std::optional<Offset> get_function_stack_size(const std::string&);

	public:
		void define_local(const std::string&);
		void define_local(const std::string&, Register);
		// view but do not allocate space for a new local
		void attach_local(const std::string&, Offset);

	public:
		void destroy_local(const std::string&);

	public:
		void read_local(Register, const std::string&);
		void write_local(const std::string&, Register);

	public:
		Allocator()
			: locals_offsets{}
		{
		}
	};
}

#endif 
