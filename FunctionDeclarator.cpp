#include "FunctionDeclarator.h"

#include <format>

namespace hz
{
	Declarator::Type FunctionDeclarator::dtype() const
	{
		return Declarator::Type::FUNCTION;
	}

	std::string FunctionDeclarator::string() const
	{
		return std::format("function declarator ({})", function->string());
	}

	FunctionDeclarator* FunctionDeclarator::copy() const
	{
		return new FunctionDeclarator{ *this };
	}

	void FunctionDeclarator::generate(Allocation*)
	{
		// TODO: nothing to do here??

		// A possible option is to re-utilize generate() as 
		// an equivalent to evaluate() for the interpreter context!
	}

	Declarator* FunctionDeclarator::optimize()
	{
		auto function_optimized = AS_FUNCTION(function->optimize());

		if (function_optimized != nullptr)
		{
			return new FunctionDeclarator{ function_optimized };
		}

		return nullptr;
	}
}