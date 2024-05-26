#include "Function.h"

#include <format>

namespace hz
{
	Node::Type Function::ntype() const
	{
		return Node::Type::FUNCTION;
	}


	std::string Function::string() const
	{
		return std::format("function ({}())", name);
	}


	Function* Function::copy() const
	{
		return new Function{ *this };
	}


	void Function::generate(Allocation* allocation)
	{
		//TODO: determine the best way of netting the proper return value
		
		//Populate the allocation with a new static allocation and return
	}


	Function* Function::optimize()
	{
		if (auto body_optimized = body->optimize(); body_optimized)
		{
			return new Function{ name, std::move(arguments), static_cast<Statement*>(body_optimized) };
		}

		return nullptr;
	}
}
