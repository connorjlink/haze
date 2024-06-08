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
        Node* parse_instruction();
        std::vector<Node*> parse_instructions();

    public:
        virtual std::vector<Node*> parse() final override;

    public:
        using Parser::Parser;
    };
}

#endif
