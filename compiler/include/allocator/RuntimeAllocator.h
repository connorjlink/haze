#ifndef HAZE_RUNTIMEALLOCATOR_H
#define HAZE_RUNTIMEALLOCATOR_H

#include <data/DependencyInjector.h>
#include <toolchain/Generator.h>
#include <utility/Constants.h>

// Haze RuntimeAllocator.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class RuntimeAllocator 
		: public ServiceTag<RuntimeAllocator>
		, public InjectService<Generator>
	{
	private:
		// map of (function -> (map of variables -> offset))
		std::unordered_map<std::string, std::unordered_map<std::string, Offset>> locals_offsets;
		// map of (function -> current stack size)
		std::unordered_map<std::string, Offset> stack_size;

	public:
		void define_local(const std::string&);
		void define_local(const std::string&, Register);
		void attach_local(const std::string&, Offset);

	public:
		void destroy_local(const std::string&);

	public:
		void read_local(Register, const std::string&);
		void write_local(const std::string&, Register);

	public:
		RuntimeAllocator()
			: locals_offsets{}
		{
		}
	};
}

#endif 
