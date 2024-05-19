#ifndef HAZE_ALLOCATION_H
#define HAZE_ALLOCATION_H

#include "Instruction.h"

#include <cstdint>

#define AS_STATIC(x) static_cast<StaticAllocation*>(x)
#define AS_DYNAMIC(x) static_cast<DynamicAllocation*>(x)

namespace hz
{
	class Allocation
	{
	protected:
		friend class Allocator;
		enum class Type
		{
			STATIC,
			DYNAMIC,
		};

	public:
		virtual Allocation::Type atype() const = 0;

		virtual void deallocate() = 0;

		virtual Register& read() = 0;
		virtual void write(std::uint8_t) = 0;
		virtual void copy(Allocation*) const = 0;
	};

	class StaticAllocation : public Allocation
	{
	public:
		Register reg;

	public:
		virtual Allocation::Type atype() const final override;

		virtual void deallocate() final override;

		virtual Register& read() final override;
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
			: address(address), need_free(need_free)
		{
		}

	public:
		virtual Allocation::Type atype() const final override;

		virtual void deallocate() final override;

		virtual Register& read() final override;
		virtual void write(std::uint8_t) final override;
		virtual void copy(Allocation*) const final override;
	};

}

#endif //HAZE_ALLOCATION_H
