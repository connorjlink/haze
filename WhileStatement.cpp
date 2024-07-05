#include "WhileStatement.h"

#include "Allocation.h"
#include "Allocator.h"

#include "Utility.h"

#include <format>

namespace hz
{
	Statement::Type WhileStatement::stype() const
	{
		return Statement::Type::WHILE;
	}

	std::string WhileStatement::string() const
	{
		return std::format("while statement ({}) \n[\n{}\n]\n", 
			condition->string(), body->string());
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
		const auto loop_uuid = hz::generate(3);

		const auto start_label = std::format("start_while_{:02d}", loop_uuid);
		const auto end_label = std::format("end_while_{:02d}", loop_uuid);

		generator->label(start_label);

		body->generate();

		condition->generate(condition_allocation);
		
		// unfortunately our processor's limitation also mean we need to force this mess, too :(
		auto temp_allocation = allocator->allocate_static(condition_allocation->reg, true);

		// TODO: finish while statement codegen here!
		generator->make_brnz(start_label, condition_allocation->reg);
	}

	Statement* WhileStatement::optimize()
	{
		auto condition_optimized = AS_EXPRESSION(condition->optimize());
		auto body_optimized = AS_STATEMENT(body->optimize());

		if (condition_optimized == nullptr &&
			body_optimized == nullptr)
		{
			return nullptr;
		}

		if (condition_optimized == nullptr)
		{
			condition_optimized = condition;
		}

		if (body_optimized == nullptr)
		{
			body_optimized = body;
		}

		return new WhileStatement{ condition_optimized, body_optimized };

	}
}