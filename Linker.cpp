#include "Linker.h"
#include "Instruction.h"
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
	std::vector<Instruction*> Linker::optimize(const std::vector<Instruction*>& object_code) const
	{
		std::vector<Instruction*> optimized_object_code{};

		for (auto i = 0; i < object_code.size(); i++)
		{
			for (auto r = R0; r <= R3; r = static_cast<Register>(r + 1))
			{
				//TODO: ensure none of our bytes are branch targets

				const auto o0 = object_code[i + 0];

				if (i + 1 < object_code.size())
				{
					const auto o1 = object_code[i + 1];

					if ((o0->opcode == PUSH && o0->op2 == r &&  //push r
						 o1->opcode == PULL && o1->op1 == r) || //pull r
						
						(o0->opcode == PULL && o0->op1 == r &&  //pull r
						 o1->opcode == PUSH && o1->op2 == r))   //push r
					{
						//These instructions are entirely redundant
						i += 1;
						break;
					}


					if (i + 2 < object_code.size())
					{
						const auto o2 = object_code[i + 2];

						if (o0->opcode == COPY && o0->op1 == r && //copy r, #x
							o1->opcode == SAVE && o1->op2 == r && //save &x, r
							o2->opcode == LOAD && o2->op1 == r)   //load r, &y
						{
							if (o1->mem == o2->mem)
							{
								optimized_object_code.emplace_back(o0);
								i += 2;
								break;
							}
						}
					}
				}

				//No optimizations could be applied here, so insert the old code
				optimized_object_code.emplace_back(object_code[i]);
				break;
			}
		}

		return optimized_object_code;
	}

	std::vector<std::uint8_t> Linker::link() const
	{
		std::vector<std::uint8_t> executable;

		for (auto [symbol, function] : object_code)
		{
			//Optimize away functions that are not even called
			if (symbol->was_referenced)
			{
				//This is probably not the best way of doing this since we copy
				//But, at least it looks clean
				if constexpr (OPTIMIZE_LTO)
				{
					function = optimize(function);
				}

				for (const auto& instruction : function)
				{
					executable.append_range(extract(instruction->bytes()));
				}	
			}
		}

		return executable;
	}
}
