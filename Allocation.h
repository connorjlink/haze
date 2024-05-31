#ifndef HAZE_ALLOCATION_H
#define HAZE_ALLOCATION_H

#include "Instruction.h"

#include <cstdint>

#define AS_STATIC_ALLOCATION(x) static_cast<StaticAllocation*>(x)
#define AS_DYNAMIC_ALLOCATION(x) static_cast<DynamicAllocation*>(x)

namespace hz
{
	class Allocation
	{
	protected:
		enum class Type
		{
			STATIC,
			DYNAMIC,
		};

	public:
		virtual Allocation::Type atype() const = 0;

		virtual ~Allocation()
		{
		}

		virtual Register read() = 0;
		virtual void write(std::uint8_t) = 0;
		virtual void copy(Allocation*) const = 0;
	};

	class StaticAllocation : public Allocation
	{
	public:
		Register reg;
		bool was_forced;

	public:
		StaticAllocation(Register reg, bool was_forced)
			: reg{ reg }, was_forced{ was_forced }
		{
		}

	public:
		virtual Allocation::Type atype() const final override;

		virtual ~StaticAllocation() final override;

		virtual Register read() final override;
		virtual void write(std::uint8_t) final override;
		virtual void copy(Allocation*) const final override;
	};

	class DynamicAllocation : public Allocation
	{
	public:
		std::uint16_t address;
		bool need_free;

	public:
		DynamicAllocation(std::uint16_t address, bool need_free = false)
			: address{ address }, need_free{ need_free }
		{
		}

	public:
		virtual Allocation::Type atype() const final override;

		virtual ~DynamicAllocation() final override;

		virtual Register read() final override;
		virtual void write(std::uint8_t) final override;
		virtual void copy(Allocation*) const final override;
	};


	class ManagedStaticAllocation
	{
	public:
		StaticAllocation* allocation;

	public:
		ManagedStaticAllocation(Register = DC, bool = false);
		~ManagedStaticAllocation();
	};

	class ManagedDynamicAllocation
	{
	public:
		DynamicAllocation* allocation;

	public:
		ManagedDynamicAllocation(std::uint16_t = 1, bool = true);
		~ManagedDynamicAllocation();
	};
}

#endif //HAZE_ALLOCATION_H
