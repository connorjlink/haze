import std;

#include "Symbol.h"
#include "IntermediateCommand.h"
#include "Linkable.h"

// Haze Linkable.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::vector<IntermediateCommand*> IntermediateOptimizer::optimize()
	{
		std::vector<IntermediateCommand*> out{};

		auto symbol = USE_SAFE(SymbolDatabase)->reference_symbol(SymbolType::FUNCTION, _function, NULL_TOKEN);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		// NOTE: old method
		// the main function does not require a stack frame
		/*if (function_symbol->name == "main")
		{
			auto first = *_ir.begin();

			if (first->itype() == IntermediateType::ENTER_SCOPE)
			{
				first->marked_for_deletion = true;
			}
		}*/

		if (function_symbol->name == "main")
		{
			auto penultimate = *(_ir.end() - 2);
			auto last = *(_ir.end() - 1);

			penultimate->marked_for_deletion = true;
			last->marked_for_deletion = true;
		}


		// NOTE: old method
		// if there are no local variables, there is no need to push a stack frame
		/*if (function_symbol->locals_count == 0)
		{
			if (_ir.size() >= 2)
			{
				auto first = *_ir.begin();
				auto penultimate = *(_ir.end() - 2);
				auto last = *(_ir.end() - 1);

				if (first->itype() == IntermediateType::ENTER_SCOPE)
				{
					if (last->itype() == IntermediateType::LEAVE_SCOPE)
					{
						first->marked_for_deletion = true;
						last->marked_for_deletion = true;
					}

					else if (penultimate->itype() == IntermediateType::LEAVE_SCOPE)
					{
						first->marked_for_deletion = true;
						penultimate->marked_for_deletion = true;
					}
				}

				for (auto i = 0; i < _ir.size() - 1; i++)
				{
					if (_ir[i + 0]->itype() == IntermediateType::ENTER_SCOPE &&
						_ir[i + 1]->itype() == IntermediateType::LEAVE_SCOPE)
					{
						_ir[i + 0]->marked_for_deletion = true;
						_ir[i + 1]->marked_for_deletion = true;
					}
				}
			}
		}*/


		if (_ir.size() >= 2)
		{
			const auto size = _ir.size();

			for (auto i = 0; i < size; i++)
			{
				auto c0 = _ir[i + 0];

				// TODO: one byte optimizations

				if (i < size - 1)
				{
					auto c1 = _ir[i + 1];

					// TODO: two byte optimizations

					if (i < size - 2)
					{
						auto c2 = _ir[i + 2];

						// TODO: three byte optimizations
					}
				}
			}
		}

		for (auto& command : _ir)
		{
			using enum IntermediateType;
			switch (command->itype())
			{
			case COPY:
			{
				const auto copy_command = static_cast<CopyCommand*>(command);

				// remove any copy commands wherefor the source and destination match
				if (copy_command->_destination == copy_command->_source)
				{
					copy_command->marked_for_deletion = true;
				}
			} break;

			default:
			{
				// command->marked_for_deletion = false;
			} break;
			}
		}

		// gather all undeleted instructions
		for (auto& command : _ir)
		{
			if (!command->marked_for_deletion)
			{
				out.emplace_back(command);
			}
		}

		return out;
	}
}
