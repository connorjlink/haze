#ifndef HAZE_ASSEMBLERPARSER_H
#define HAZE_ASSEMBLERPARSER_H

#include "Parser.h"

namespace hz
{
	class AssemblerParser : public Parser
	{
	private:
		Register parse_register();

	private:
		Expression* parse_address();
		Expression* parse_immediate();

	private:
		Node* parse_dotorg_command();
		Node* parse_label_command();
		Node* parse_instruction_command();

	private:
		Node* parse_command();
		std::vector<Node*> parse_commands();

	public:
		virtual std::vector<Node*> parse() final override;

	public:
		using Parser::Parser;
	};
}

#endif
