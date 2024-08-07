#include "FunctionCallExpression.h"
#include "Parser.h"
#include "Allocator.h"
#include "Allocation.h"
#include "Log.h"

#include <format>

namespace hz
{
	ExpressionType FunctionCallExpression::etype() const
	{
		return ExpressionType::FUNCTION_CALL;
	}

	std::string FunctionCallExpression::string() const
	{
		return std::format("function call ({})", name);
	}

	FunctionCallExpression* FunctionCallExpression::copy() const
	{
		return new FunctionCallExpression{ *this };
	}

	void FunctionCallExpression::generate(Allocation* allocation)
	{
		auto symbol = _parser->reference_symbol(Symbol::Type::FUNCTION, name, true);

		const auto expected_arity = AS_FUNCTION_SYMBOL(symbol)->arity;
		const auto got_arity = arguments.size();

		if (expected_arity != got_arity)
		{
			Log::error(std::format("function {} expects {} arguments but was called with {}", name, expected_arity, got_arity));
		}

		if (!arguments.empty())
		{
			ManagedStaticAllocation temp{ allocation->read() };

			for (auto argument : arguments)
			{
				argument->generate(temp.allocation);
				_generator->make_push(temp.allocation->read());
			}
		}

		//Placeholder call address before we hot-patch in the correct target after linking
		_generator->make_call(name);

		//return value comes off stack
		_generator->make_pull(allocation->read());
	}

	Expression* FunctionCallExpression::optimize()
	{
		//TODO: implement determination if a function is constexpr
		//in that case, we can fold it down to a single integer literal node
		//for now, we don't have that functionality so just terminate further optimization of this node
		return nullptr;
	}

	Node* FunctionCallExpression::evaluate(Context* context) const
	{
		return nullptr;
	}
}