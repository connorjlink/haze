#include "HookStatement.h"
#include "Context.h"

#include <format>

namespace hz
{
	StatementType HookStatement::stype() const
	{
		return StatementType::HOOK;
	}

	std::string HookStatement::string() const
	{
		const auto prefix = hook ? "" : "un";
		return std::format("{}hook statement", prefix);
	}

	HookStatement* HookStatement::copy() const
	{
		return new HookStatement{ *this };
	}

	void HookStatement::generate(Allocation*)
	{
		Log::error("Hook statements cannot be generated in a compiled context");
	}

	Statement* HookStatement::optimize()
	{
		// No optimizations possible for a hook statement
		return nullptr;
	}

	Node* HookStatement::evaluate(Context* context) const
	{
		if (hook)
		{
			context->hook();
		}

		else
		{
			context->unhook();
		}

		return nullptr;
	}
}