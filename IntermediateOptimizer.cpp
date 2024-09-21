import std;

#include "IntermediateOptimizer.h"
#include "Symbol.h"
#include "Parser.h"

// Haze IntermediateOptimizer.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	std::vector<IntermediateCommand*> IntermediateOptimizer::optimize()
	{
		std::vector<IntermediateCommand*> out{};


		auto symbol = _parser->reference_symbol(SymbolType::FUNCTION, _function, NULL_TOKEN);
		auto function_symbol = AS_FUNCTION_SYMBOL(symbol);

		// if there are no local variables, there is no need to push a stack frame
		if (function_symbol->locals_count == 0)
		{
			if (_ir.size() >= 2)
			{
				auto first = *_ir.begin();
				auto penultimate = *(_ir.end() - 2);
				auto last = *(_ir.end() - 1);

				if (first->itype() == IntermediateType::ENTER_SCOPE)
				{
					first->marked_for_deletion = true;

					if (last->itype() == IntermediateType::LEAVE_SCOPE)
					{
						last->marked_for_deletion = true;
					}

					else if (penultimate->itype() == IntermediateType::LEAVE_SCOPE)
					{
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
		}


		for (auto& command : _ir)
		{
			using enum IntermediateType;
			switch (command->itype())
			{
				case COPY:
				{
					const auto copy_command = static_cast<CopyCommand*>(command);

					// remove any copy commands wherefor in the source and destination match
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
