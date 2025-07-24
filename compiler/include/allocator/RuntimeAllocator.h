#ifndef HAZE_RUNTIMEALLOCATOR_H
#define HAZE_RUNTIMEALLOCATOR_H

#include <data/DependencyInjector.h>
#include <toolchain/models/InstructionEncoding.h>
#include <toolchain/Generator.h>

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
		std::unordered_map<std::string, std::unordered_map<std::string, std::int32_t>> _locals_offsets;

	public:
		std::uint32_t allocate(std::uint32_t bytes);

	public:
		void define_local(const std::string&);
		void define_local(const std::string&, register_t);
		void attach_local(const std::string&, std::int32_t);

	public:
		void destroy_local(const std::string&);

	public:
		void read_local(register_t, const std::string&);
		void write_local(const std::string&, register_t);

	public:
		RuntimeAllocator()
			: _locals_offsets{}
		{
		}
	};
}

#endif 
