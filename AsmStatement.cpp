#include "AsmStatement.h"
#include "Generator.h"

#include <format>

namespace hz
{
	Statement::Type AsmStatement::stype() const
	{
		return Statement::Type::ASM;
	}

	std::string AsmStatement::string() const
	{
		std::string iis_string = "";

		for (auto ii : iis)
		{
			iis_string += std::format("\t({}) {}\n", ii.address, ii.instruction->string());
		}

		return std::format("asm statement \n [\n{}]\n", iis_string);
	}

	AsmStatement* AsmStatement::copy() const
	{
		return new AsmStatement{ *this };
	}

	void AsmStatement::generate(Allocation*)
	{
		for (auto ii : iis)
		{
			generator->raw(ii.instruction);
		}
	}
}