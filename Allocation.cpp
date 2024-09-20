import std;

#include "Allocation.h"
#include "Allocator.h"
#include "Generator.h"
#include "IdentifierExpression.h"
#include "ErrorReporter.h"

// Haze Allocation.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	AllocationType ObserverAllocation::atype() const
	{
		return AllocationType::OBSERVER;
	}

	AllocationType StackAllocation::atype() const
	{
		return AllocationType::STACK;
	}

	AllocationType AutoStackAllocationImpl::atype() const
	{
		return AllocationType::STACK_AUTO;
	}

	AllocationType HeapAllocation::atype() const
	{
		return AllocationType::HEAP;
	}

	AllocationType AutoHeapAllocationImpl::atype() const
	{
		return AllocationType::HEAP_AUTO;
	}

	
	ObserverAllocation::ObserverAllocation(register_t source)
	{
		allocation = source;
	}

	AutoObserverAllocation::AutoObserverAllocation(register_t source)
	{
		_source = new ObserverAllocation{ source };
	}

	AutoObserverAllocation::~AutoObserverAllocation()
	{
		delete _source;
	}

	StackAllocation::StackAllocation()
	{
		allocation = _stack_allocator->allocate();
	}

	AutoStackAllocationImpl::~AutoStackAllocationImpl()
	{
		_stack_allocator->release(allocation);
	}

	AutoStackAllocation::AutoStackAllocation()
	{
		_source = new AutoStackAllocationImpl{};
	}

	AutoStackAllocation::~AutoStackAllocation()
	{
		delete _source;
	}

	HeapAllocation::HeapAllocation(std::uint32_t bytes)
		: bytes{ bytes }
	{
		address = _heap_allocator->allocate(bytes);
	}

	AutoHeapAllocationImpl::~AutoHeapAllocationImpl()
	{
		_heap_allocator->release(address, bytes);
	}

	AutoHeapAllocation::AutoHeapAllocation(std::uint32_t bytes)
	{
		_source = new AutoHeapAllocationImpl{ bytes };
	}

	AutoHeapAllocation::~AutoHeapAllocation()
	{
		delete _source;
	}


	register_t ObserverAllocation::read() const
	{
		return allocation;
	}

	void ObserverAllocation::write(variable_t value) const
	{
		UNSUPPORTED_OPERATION(__FUNCTION__);
	}

	void ObserverAllocation::copy_into(Allocation* allocation) const
	{
		UNSUPPORTED_OPERATION(__FUNCTION__);
	}


	register_t StackAllocation::read() const
	{
		return allocation;
	}

	void StackAllocation::write(variable_t value) const
	{
		switch (auto v = value.index())
		{
			// only integers are supported for now
			case 0:
			{
				const auto int_value = std::get<0>(value);
				_generator->make_immediate(allocation, int_value);
			} break;

			default:
			{
				_error_reporter->post_error(std::format("invalid stack allocation write type variant `{}`", v), NULL_TOKEN);
			} break;
		}
	}

	void StackAllocation::copy_into(Allocation* destination) const
	{
		using enum AllocationType;
		switch (destination->atype())
		{
			case OBSERVER:
			{
				_error_reporter->post_error("cannot modify constant allocation type `observer`", NULL_TOKEN);
			} break;

			// NOTE: StackAllocation and AutoStackAllocationImpl are transitive
			case STACK: [[fallthrough]];
			case STACK_AUTO:
			{
				auto stack_allocation = static_cast<StackAllocation*>(destination);
				_generator->make_copy(stack_allocation->allocation, allocation);
			} break;

			// NOTE: HeapAllocation and AutoHeapAllocationImpl are transitive
			case HEAP: [[fallthrough]];
			case HEAP_AUTO:
			{
				auto heap_allocation = static_cast<HeapAllocation*>(destination);
				AutoStackAllocation temp{};
				_generator->memory_write(heap_allocation->address, temp.source()->read());
			} break;
		}
	}


	register_t HeapAllocation::read() const
	{
		UNSUPPORTED_OPERATION(__FUNCTION__);
	}

	void HeapAllocation::write(variable_t value) const
	{
		UNSUPPORTED_OPERATION(__FUNCTION__);
	}

	void HeapAllocation::copy_into(Allocation* destination) const
	{
		UNSUPPORTED_OPERATION(__FUNCTION__);
	}


	/*StaticAllocation::~StaticAllocation()
	{
		if (was_forced)
		{
			_generator->make_pull(reg);
			return;
		}

		_allocator->register_ledger[reg] = Status::FREE;
	}

	DynamicAllocation::~DynamicAllocation()
	{
		if (need_free)
		{
			_allocator->heap_ledger[address] = Status::FREE;
		}
	}


	Register StaticAllocation::read()
	{
		return reg;
	}

	Register DynamicAllocation::read()
	{
		Log::error(std::format("dynamic allocation at address {} cannot be mapped as a register for reading", address));
	}


	void StaticAllocation::write(std::uint8_t value)
	{
		_generator->make_copy(reg, value);
	}

	void DynamicAllocation::write(std::uint8_t value)
	{
		auto do_write = [&](auto reg)
		{
			_generator->make_copy(reg, value);
			_generator->make_save(address, reg);
		};

		auto reg = _allocator->find_register();

		if (reg.has_value())
		{
			do_write(reg.value());
		}

		else
		{
			_generator->make_push(R3);
			do_write(R3);
			_generator->make_pull(R3);
		}
	}

	
	void StaticAllocation::copy(Allocation* allocation) const 
	{
		switch (allocation->atype())
		{
			case Type::STATIC: _generator->make_move(reg, AS_STATIC_ALLOCATION(allocation)->reg); break;
			case Type::DYNAMIC: _generator->make_load(reg, AS_DYNAMIC_ALLOCATION(allocation)->address); break;
		}
	}

	void DynamicAllocation::copy(Allocation* allocation) const
	{
		switch (allocation->atype())
		{
			case Type::STATIC: _generator->make_save(address, AS_STATIC_ALLOCATION(allocation)->reg); break;

			case Type::DYNAMIC: 
			{
				auto do_copy = [&](auto reg)
				{
					_generator->make_load(reg, AS_DYNAMIC_ALLOCATION(allocation)->address);
					_generator->make_save(address, reg);
				};

				auto reg = _allocator->find_register();

				if (reg.has_value())
				{
					do_copy(reg.value());
				}

				else
				{
					_generator->make_push(R3);
					do_copy(R3);
					_generator->make_pull(R3);
				}
			} break;
		}
	}



	ManagedStaticAllocation::ManagedStaticAllocation(Register reg, bool force)
	{
		allocation = _allocator->allocate_static(reg, force);
	}

	ManagedStaticAllocation::~ManagedStaticAllocation()
	{
		delete allocation;
	}


	ManagedDynamicAllocation::ManagedDynamicAllocation(std::uint32_t bytes, bool free)
	{
		allocation = _allocator->allocate_dynamic(bytes, free);
	}

	ManagedDynamicAllocation::~ManagedDynamicAllocation()
	{
		delete allocation;
	}*/
}
