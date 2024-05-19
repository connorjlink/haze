#include "Function.h"

#include <format>
#include <ranges>

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
		
	}


	Function* Function::optimize()
	{
		if (auto body_optimized = body->optimize(); body_optimized)
		{
			return new Function{ name, arguments, static_cast<Statement*>(body_optimized) };
		}

		return nullptr;
	}
}
