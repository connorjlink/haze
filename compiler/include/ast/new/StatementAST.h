#ifndef HAZE_STATEMENT_AST_H
#define HAZE_STATEMENT_AST_H

// Haze StatementAST.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class NullStatement;
    class ExpressionStatement;
    class ReturnStatement;
    class IfStatement;
    class WhileStatement;
    class ForStatement;
    class CompoundStatement;
    class FunctionDeclarationStatement;
    class StructDeclarationStatement;
    class VariableDeclarationStatement;
    class InlineAssemblyStatement;

    template<template<typename...> class T>
    using StatementTypes = T
    <
        NullStatement,
        ExpressionStatement,
        ReturnStatement,
        IfStatement,
        WhileStatement,
        ForStatement,
        CompoundStatement,
        FunctionDeclarationStatement,
        StructDeclarationStatement,
        VariableDeclarationStatement,
        InlineAssemblyStatement
    >;
}

#endif
