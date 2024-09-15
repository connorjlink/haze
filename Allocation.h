#ifndef HAZE_ALLOCATION_H
#define HAZE_ALLOCATION_H

#include "InstructionCommand.h"
#include "AllocationType.h"
#include "StackAllocator.h"
#include "HeapAllocator.h"

#include <cstdint>

namespace hz
{
	class Allocation
	{
	public:
		virtual ~Allocation()
		{
		}

	public:
		virtual AllocationType atype() const = 0;
		virtual register_t read() const = 0;
		virtual void write(std::uint8_t) const = 0;
		virtual void copy_into(Allocation*) const = 0;
	};

	class StackAllocation : public Allocation
	{
	public:
		register_t allocation;

	public:
		virtual AllocationType atype() const override;
		virtual register_t read() const final override;
		virtual void write(std::uint8_t) const final override;
		virtual void copy_into(Allocation*) const final override;

	public:
		StackAllocation();
	};

	class AutoStackAllocationImpl : public StackAllocation
	{
	public:
		using StackAllocation::StackAllocation;

	public:
		virtual AllocationType atype() const final override;

	public:
		~AutoStackAllocationImpl();
	};

	class AutoStackAllocation
	{
	private:
		AutoStackAllocationImpl* _source;

	public:
		AllocationType atype() const;
		register_t read() const;
		void write(std::uint8_t) const;
		void copy_into(Allocation*) const;

	public:
		AutoStackAllocation();
		~AutoStackAllocation();
	};

	class HeapAllocation : public Allocation
	{
	public:
		std::uint32_t address;
		std::uint32_t bytes;

	public:
		virtual AllocationType atype() const override;
		virtual register_t read() const final override;
		virtual void write(std::uint8_t) const final override;
		virtual void copy_into(Allocation*) const final override;

	public:
		HeapAllocation(std::uint32_t bytes)
			: bytes{ bytes }
		{
			address = _heap_allocator->allocate(bytes);
		}
	};

	class AutoHeapAllocationImpl : public HeapAllocation
	{
	public:
		virtual AllocationType atype() const final override;

	public:
		~AutoHeapAllocationImpl()
		{
			_heap_allocator->release(address, bytes);
		}
	};

	class AutoHeapAllocation
	{
	public:
		AutoHeapAllocationImpl* _source;

	public:
		AllocationType atype() const;
		register_t read() const;
		void write(std::uint8_t) const;
		void copy_into(Allocation*) const;

	public:
		AutoHeapAllocation()
		{
			_source = new AutoHeapAllocationImpl{};
		}
		
		~AutoHeapAllocation()
		{
			delete _source;
		}
	};
}

#endif //HAZE_ALLOCATION_H
