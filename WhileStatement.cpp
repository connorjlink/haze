#include "WhileStatement.h"

#include "Allocation.h"
#include "Allocator.h"

#include <format>
#include <random>

namespace
{
	static std::random_device _device;
	static std::mt19937 _engine(_device);
	
	int generate(int num_digits = 3)
	{
		auto distribution = std::uniform_int_distribution(0, (num_digits * 10) - 1);
		return distribution(_engine);
	}
}

namespace hz
{
	Statement::Type WhileStatement::stype() const
	{
		return Statement::Type::WHILE;
	}

	std::string WhileStatement::string() const
	{
		return std::format("while statement ({}) \n[\n{}\n]\n", condition->string(), body->string());
	}

	WhileStatement* WhileStatement::copy() const
	{
		return new WhileStatement{ *this };
	}

	void WhileStatement::generate(Allocation*)
	{
		// we need to force allocate this one :(
		auto condition_allocation = allocator->allocate_static(DC, true);

		// 3 digits of randomness for now
		const auto loop_uuid = ::generate(3);

		const auto start_label = std::format("start_while_{:02d}", loop_uuid);
		const auto end_label = std::format("end_while_{:02d}", loop_uuid);

		generator->label(start_label);

		body->generate();

		condition->generate(condition_allocation);
		
		// unfortunately our processor's limitation also mean we need to force this mess, too :(
		auto temp_allocation = allocator->allocate_static(condition_allocation->reg, true);

		// negate our boolean expression since we only have `brez`
		generator->bxor(condition_allocation->reg, condition_allocation->reg);
		generator->copy(temp_allocation->reg, 1);
		generator->band(condition_allocation->reg, temp_allocation->reg);
		
		generator->brez(start_label, condition_allocation->reg);
	}
}