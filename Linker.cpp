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
				//optimize `copy-save-load` into `copy` 
				if (i + 2 < object_code.size())
				{
					const auto o0 = object_code[i + 0],
							   o1 = object_code[i + 1],
							   o2 = object_code[i + 2];

					if (o0->opcode == COPY && o0->op1 == r && //copy r, #x
						o1->opcode == SAVE && o1->op2 == r && //save &x, r
						o2->opcode == LOAD && o2->op1 == r)   //load r, &y
					{
						if (o1->mem == o2->mem)
						{
							optimized_object_code.emplace_back(o0);
							//TODO: should two or three be added here?
							i += 2;
							break;
						}

						
					}
				}

				else if (false)
				{
					//TODO: other LTCG peephole optimizations go here!
				}

				else
				{
					//No optimizations could be applied here, so insert the old code
					optimized_object_code.emplace_back(object_code[i]);
					break;
				}
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
				if constexpr (OPTIMIZE)
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
