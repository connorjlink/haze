#ifndef HAZE_ALLOCATION_H
#define HAZE_ALLOCATION_H

#include "InstructionCommand.h"
#include "AllocationType.h"
#include "StackAllocator.h"
#include "HeapAllocator.h"
#include "Context.h"

// Haze Allocation.h
// (c) Connor J. Link. All Rights Reserved.

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
		virtual void write(variable_t) const = 0;
		virtual void copy_into(Allocation*) const = 0;
	};

	class ObserverAllocation : public Allocation
	{
	public:
		register_t allocation;

	public:
		virtual AllocationType atype() const final override;
		virtual register_t read() const final override;
		virtual void write(variable_t) const final override;
		virtual void copy_into(Allocation*) const final override;

	public:
		ObserverAllocation(register_t);
	};

	class AutoObserverAllocation
	{
	private:
		ObserverAllocation* _source;

	public:
		decltype (_source) source() const
		{
			return _source;
		}

	public:
		AutoObserverAllocation(register_t);
		~AutoObserverAllocation();
	};

	class StackAllocation : public Allocation
	{
	public:
		register_t allocation;

	public:
		virtual AllocationType atype() const override;
		virtual register_t read() const final override;
		virtual void write(variable_t) const final override;
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
		decltype(_source) source() const
		{
			return _source;
		}

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
		virtual void write(variable_t) const final override;
		virtual void copy_into(Allocation*) const final override;

	public:
		HeapAllocation(std::uint32_t);
	};

	class AutoHeapAllocationImpl : public HeapAllocation
	{
	public:
		virtual AllocationType atype() const final override;

	public:
		using HeapAllocation::HeapAllocation;
		~AutoHeapAllocationImpl();
	};

	class AutoHeapAllocation
	{
	public:
		AutoHeapAllocationImpl* _source;

	public:
		decltype(_source) source() const
		{
			return _source;
		}

	public:
		AutoHeapAllocation(std::uint32_t);
		~AutoHeapAllocation();
	};
}

#endif //HAZE_ALLOCATION_H
