#ifndef HAZE_GENERATOR_H
#define HAZE_GENERATOR_H

#include <vector>
#include <string>

#include "Expression.h"
#include "Statement.h"
#include "Function.h"
#include "Symbol.h"
#include "Instruction.h"

namespace hz
{
    class Parser;

    class Generator
    {
    private:
        const std::vector<Function*>& program;
        std::string code;
        std::vector<std::uint8_t> bytes;

    private:
        inline static Parser* parser = nullptr;

    public:
        const std::string& get_code() const
        {
            return code;
        }

        std::vector<std::uint8_t>& get_bytes()
        {
            return bytes;
        }

    public:
        void include(std::string_view);
        void label(std::string_view);

    public:
        void move(Register, Register);
        void load(Register, int);
        void copy(Register, int);
        void save(int, Register);
        void iadd(Register, Register);
        void isub(Register, Register);
        void band(Register, Register);
        void bior(Register, Register);
        void bxor(Register, Register);
        void bnot(Register);
        void call(int);
        void exit();
        void push(Register);
        void pull(Register);
        void brez(int, Register);
        void rsvd(Register, Register);

    private:
        void generate_expression(Expression*, Allocation&);
        void generate_statement(Statement*, Statement*);
        void generate_function(Function*);
        void generate_program();

    public:
        static void set_parser(Parser* p)
        {
            parser = p;
        }

    public:
        explicit Generator(const std::vector<Function*>& program)
            : program(program)
        {
        }

    public:
        void generate();
    };
}

#endif //HAZE_GENERATOR_H
