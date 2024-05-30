#include "Linker.h"
#include "Utility.h"
#include "Symbol.h"

namespace
{
	std::uint8_t make_opcode(hz::Opcode opcode, hz::Register operand1, hz::Register operand2)
	{
		return ((opcode & 0b1111) << 4) | ((operand1 & 0b11) << 2) | ((operand2 & 0b11) << 0);
	}
}

namespace hz
{
	void Linker::optimize()
	{
		for (auto&[symbol, function] : linkables)
		{
			for (auto i = 0; i < function.size(); i++)
			{
				for (auto r = R0; r <= R3; r = static_cast<Register>(r + 1))
				{
					//TODO: ensure none of our bytes are branch targets

					auto o0 = function[i + 0];

					if (i + 1 < function.size())
					{
						auto o1 = function[i + 1];

						if ((o0->opcode == PUSH && o0->op2 == r &&  //push r
							 o1->opcode == PULL && o1->op1 == r) || //pull r
							
							(o0->opcode == PULL && o0->op1 == r &&  //pull r
							 o1->opcode == PUSH && o1->op2 == r))   //push r
						{
							//These instructions are entirely redundant
							o0->marked_for_deletion = true;
							o1->marked_for_deletion = true;
							i += 1;
							break;
						}


						if (i + 2 < function.size())
						{
							auto o2 = function[i + 2];

							if (o0->opcode == COPY && o0->op1 == r && //copy r, #x
								o1->opcode == SAVE && o1->op2 == r && //save &x, r
								o2->opcode == LOAD && o2->op1 == r)   //load r, &y
							{
								if (o1->mem == o2->mem)
								{
									o1->marked_for_deletion = true;
									o2->marked_for_deletion = true;
									i += 2;
									break;
								}
							}
						}
					}

					//No optimizations could be applied here
					break;
				}
			}
		}
	}

	std::vector<std::uint8_t> Linker::link()
	{
		std::vector<std::uint8_t> executable;

		for (auto [symbol, function] : linkables)
		{
			if (symbol->was_referenced)
			{
				if constexpr (OPTIMIZE_LTO)
				{
					optimize();
				}

				for (const auto& instruction : function)
				{
					if (!instruction->marked_for_deletion)
					{
						executable.append_range(extract(instruction->bytes()));
					}
				}	
			}
		}

		return executable;
	}
}
