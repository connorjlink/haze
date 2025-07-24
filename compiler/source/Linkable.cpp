import std;

#include <toolchain/Linkable.h>
#include <command/IntermediateCommand.h>
#include <symbol/Symbol.h>

// Haze Linkable.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	void Linkable::optimize(void)
	{
		std::vector<IntermediateCommand*> out{};

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
			auto penultimate = *(ir.end() - 2);
			auto last = *(ir.end() - 1);

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


		if (ir.size() >= 2)
		{
			const auto size = ir.size();

			for (auto i = 0; i < size; i++)
			{
				auto c0 = ir[i + 0];

				// TODO: one byte optimizations

				if (i < size - 1)
				{
					auto c1 = ir[i + 1];

					// TODO: two byte optimizations

					if (i < size - 2)
					{
						auto c2 = ir[i + 2];

						// TODO: three byte optimizations
					}
				}
			}
		}

		for (auto& command : ir)
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
		for (auto& command : ir)
		{
			if (!command->marked_for_deletion)
			{
				out.emplace_back(command);
			}
		}

		// overrwrite the existing code with the newly optimized bytecode
		ir = out;
	}
}
