#include <fmt/format.h>
#include "Generator.h"
#include "Allocator.h"
#include "Log.h"
#include "Parser.h"
#include "Symbol.h"

#include <unordered_map>
#include <iostream>
#include <cstdlib>

namespace hz
{
    void Generator::include(std::string_view path)
    {
        code += fmt::format("@include \"{}\"\n", path);
    }

    void Generator::label(std::string_view name)
    {
        code += fmt::format("@{}:\n", name);
    }


    void Generator::move(Register destination, Register source)
    {
        code += fmt::format("\tmove {}, {}\n", unmap(destination), unmap(source));
        auto instruction = Instruction{ MOVE, destination, source };
        bytes.emplace_back(Instruction::compose(MOVE, destination, source));
    }

    void Generator::load(Register destination, int address)
    {
        code += fmt::format("\tload {}, &{}\n", unmap(destination), address);
        bytes.emplace_back(Instruction::compose(LOAD, destination, DC, 0, address));
    }

    void Generator::copy(Register destination, int immediate)
    {
        code += fmt::format("\tcopy {}, #{}\n", unmap(destination), immediate);
        bytes.emplace_back(Instruction::compose(COPY, destination, DC, immediate, 0));
    }

    void Generator::save(int address, Register source)
    {
        code += fmt::format("\tsave &{}, {}\n", address, unmap(source));
        bytes.emplace_back(Instruction::compose(SAVE, DC, source, 0, address));
    }

    void Generator::iadd(Register destination, Register source)
    {
        code += fmt::format("\tiadd {}, {}\n", unmap(destination), unmap(source));
        bytes.emplace_back(Instruction::compose(IADD, destination, source));
    }

    void Generator::isub(Register destination, Register source)
    {
        code += fmt::format("\tisub {}, {}\n", unmap(destination), unmap(source));
        bytes.emplace_back(Instruction::compose(ISUB, destination, source));
    }

    void Generator::band(Register destination, Register source)
    {
        code += fmt::format("\tband {}, {}\n", unmap(destination), unmap(source));
        bytes.emplace_back(Instruction::compose(BAND, destination, source));
    }

    void Generator::bior(Register destination, Register source)
    {
        code += fmt::format("\tbior {}, {}\n", unmap(destination), unmap(source));
        bytes.emplace_back(Instruction::compose(BIOR, destination, source));
    }

    void Generator::bxor(Register destination, Register source)
    {
        code += fmt::format("\tbxor {}, {}\n", unmap(destination), unmap(source));
        bytes.emplace_back(Instruction::compose(BXOR, destination, source));
    }

    void Generator::bnot(Register destination)
    {
        code += fmt::format("\tbnot {}\n", unmap(destination));
        bytes.emplace_back(Instruction::compose(BNOT, destination, DC));
    }

    void Generator::call(int address)
    {
        code += fmt::format("\tcall &{}\n", address);
        bytes.emplace_back(Instruction::compose(CALL, DC, DC, 0, address));
    }

    void Generator::exit()
    {
        code += fmt::format("\texit\n");
        bytes.emplace_back(Instruction::compose(EXIT, DC, DC));
    }

    void Generator::push(Register source)
    {
        code += fmt::format("\tpush {}\n", unmap(source));
        bytes.emplace_back(Instruction::compose(PUSH, DC, source));
    }

    void Generator::pull(Register destination)
    {
        code += fmt::format("\tpull {}\n", unmap(destination));
        bytes.emplace_back(Instruction::compose(PULL, destination, DC));
    }

    void Generator::brez(int address, Register source)
    {
        code += fmt::format("\tbrez &{}, {}\n", address, unmap(source));
        bytes.emplace_back(Instruction::compose(BREZ, DC, source, 0, address));
    }

    void Generator::rsvd(Register destination, Register source)
    {
        Log::error("This opcode is currently reserved for future use as an instruction set extension prefix");
    }


    void Generator::generate_expression(Expression* expression, Allocation& allocation)
    {
        if (expression == nullptr)
        {
            Allocator::write(allocation, 0);
            return;
        }

        using enum ExpressionType;
        switch (expression->type)
        {
            case INTEGER_LITERAL:
            {
                auto integer_literal_expression = expression->as.integer_literal;
                Allocator::write(allocation, integer_literal_expression->value);
            } break;

            case IDENTIFIER:
            {
                auto identifier_expression = expression->as.identifier;
                auto symbol = parser->reference_symbol(SymbolType::VARIABLE, identifier_expression->name);
                Allocator::write(allocation, Allocator::read(*symbol->as.variable->allocation));
            } break;

            case FUNCTION_CALL:
            {
                auto function_call = expression->as.function_call;

                auto symbol = parser->reference_symbol(SymbolType::FUNCTION, function_call->name);

                auto return_allocation = Allocator::allocate_dynamic(true);
                symbol->as.function->allocation = &return_allocation;

                for (auto argument : function_call->arguments)
                {
                    auto argument_allocation = Allocator::allocate_dynamic();
                    generate_expression(argument, argument_allocation);
                }

                //TODO: finish generating the function code
            } break;

            case BINARY_EXPRESSION:
            {
                auto binary = expression->as.binary_expression;

                auto left = binary->left;
                auto right = binary->right;

                //TODO: what a fucking hack—needed to get exclude new alloc from being target register
                auto allocation2 = Allocator::allocate_static();

                generate_expression(left, allocation);
                generate_expression(right, allocation2);

                using enum BinaryExpressionType;
                switch (binary->type)
                {
                    case PLUS:
                    {
                        iadd(Allocator::read(allocation), Allocator::read(allocation2));
                    } break;

                    case MINUS:
                    {
                        isub(Allocator::read(allocation), Allocator::read(allocation2));
                    } break;

                    //implement other compiletime operators for ease of use
                    case MULTIPLY: //[[fallthrough]];
                    {
                        Log::error("operator '*' is prohibited at runtime");
                    } break;
                }
            } break;
        }
        DONE: //quick escape
    }

    void Generator::generate_statement(Statement* statement, Statement* parent)
    {
        using enum StatementType;
        switch (statement->type)
        {
            case COMPOUND:
            {
                auto compound_statement = statement->as.compound_statement;
                for (auto substatement : compound_statement->statements)
                {
                    generate_statement(substatement, statement);
                }
            } break;

            case VARIABLE_DECLARATION:
            {
                auto variable_declaration_statement = statement->as.variable_declaration;
                auto allocation2 = Allocator::allocate_dynamic(false);

                generate_expression(variable_declaration_statement->expression, allocation2);
            } break;

            case RETURN:
            {
                auto return_statement = statement->as.return_statement;
                auto allocation2 = Allocator::allocate_static();

                generate_expression(return_statement->expression, allocation2);
                push(Allocator::read(allocation2));
            } break;
        }
    }

    void Generator::generate_function(Function* function)
    {
        label(fmt::format("function_{}", function->name));

        generate_statement(function->body, nullptr);

        exit();
    }

    void Generator::generate_program()
    {
        for (auto function: program)
        {
            generate_function(function);
        }
    }

    void Generator::generate()
    {
        code.reserve(1 << 12);
        generate_program();
    }
}