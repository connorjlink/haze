#ifndef HAZE_AST_H
#define HAZE_AST_H

// Haze AST.h
// (c) Connor J. Link. All Rights Reserved.

// TODO REMOVE
#include <tuple>
#include <vector>
#include <cstdint>
#include <type_traits>
#include <iostream>

namespace hz
{
    // maximum 255 AST node types (0 reserved for invalid)
    using ASTTag = std::uint8_t;
    using ASTIndexType = std::uint32_t;

    class AdjustExpression;
    class ArgumentExpression;
    class FunctionArgumentExpression;
    class BinaryExpression;
    class FunctionCallExpression;
    class CastExpression;
    class ConditionalExpression;
    class IdentifierExpression;
    class IntegerLiteralExpression;
    class StringLiteralExpression;
    class UnaryExpression;
    class BinaryExpression;
    class MemberAccessExpression;
    class ArrayAccessExpression;
    class StructLiteralExpression;

    template<template<typename...> class T>
    using ExpressionTypes = T
    <
        AdjustExpression,
        ArgumentExpression,
        FunctionArgumentExpression,
        BinaryExpression,
        FunctionCallExpression,
        CastExpression,
        ConditionalExpression,
        IdentifierExpression,
        IntegerLiteralExpression,
        StringLiteralExpression,
        StructLiteralExpression,
        UnaryExpression,
        BinaryExpression,
        MemberAccessExpression,
        ArrayAccessExpression
    >;

    using ExpressionType = ExpressionTypes<std::tuple>;
    using ExpressionAnchor = std::tuple_element_t<0, ExpressionType>;

    // expression mapper
    template<std::size_t I>
    using ExpressionTypeAt = std::tuple_element_t<I, ExpressionType>;


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

    using StatementType = StatementTypes<std::tuple>;
    using StatementAnchor = std::tuple_element_t<0, StatementType>;

    // statement mapper
    template<std::size_t I>
    using StatementTypeAt = std::tuple_element_t<I, StatementType>;


    // map AST node template types to AST tag
    template<typename T, typename Tuple>
    struct TypeIndex;

    template<typename T, typename... Types>
    struct TypeIndex<T, std::tuple<T, Types...>> : 
        public std::integral_constant<std::size_t, 0> 
    {
    };

    template<typename T, typename U, typename... Types>
    struct TypeIndex<T, std::tuple<U, Types...>> : 
        public std::integral_constant<std::size_t, 1 + TypeIndex<T, std::tuple<Types...>>::value> 
    {
    };

    template<typename T, typename Tuple>
    using TypeIndexV = TypeIndex<T, Tuple>::value;

    template<typename T>
    using ExpressionIndex = TypeIndex<T, ExpressionType>;

    template<typename T>
    using StatementIndex = TypeIndex<T, StatementType>;



    // expose a strict polymorphic interface for AST nodes
    template<auto MethodPointer, typename Signature>
    struct ASTMethod;

    template<auto MethodPointer, typename R, typename... Args>
    struct ASTMethod<MethodPointer, R(Args...)>
    {
        static constexpr auto pointer = MethodPointer;
        using return_type = R;
    };

    template<typename Anchor>
    using ASTMethods = std::tuple<
        ASTMethod<&Anchor::print, void(const ExpressionAST&)>,
        ASTMethod<&Anchor::evaluate, Anchor(const ExpressionAST&)>,
        ASTMethod<&Anchor::optimize, Anchor(const ExpressionAST&)>,
        ASTMethod<&Anchor::check_types, bool(const ExpressionAST&)>
    >;

    using ExpressionMethods = ASTMethods<ExpressionAnchor>;
    using StatementMethods = ASTMethods<StatementAnchor>;


    template<typename T, typename Ast, typename Method>
    concept ImplementsMethod = requires(const T& node, const Ast& ast)
    {
        { (node.*Method::pointer)(ast) } -> std::same_as<typename Method::return_type>;
    };

    template<typename T, typename Ast, typename Tuple, std::size_t... Is>
    concept ASTNodeImplTupleHelper =
        (ImplementsMethod<T, Ast, std::tuple_element_t<Is, Tuple>> && ...);

    template<typename T, typename Ast, typename Tuple>
    concept ASTNodeImplTuple =
        ASTNodeImplTupleHelper<T, Ast, Tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>{}>;

    template<typename T, typename Ast, typename Anchor>
    concept ASTNode = ASTNodeImplTuple<T, Ast, ASTMethods<Anchor>>;


    template<typename... Ts>
    struct ASTStorage 
    {
    public:
        static constexpr std::size_t size = sizeof...(Ts);

    public:
        std::tuple<std::vector<Ts>...> storage;

    public:
        template<typename T>
        std::vector<T>& get()
        {
            constexpr auto I = TypeIndex<T, std::tuple<Ts...>>::value;
            return std::get<I>(storage);
        }

        template<typename T>
        const std::vector<T>& get() const
        {
            constexpr auto I = TypeIndex<T, std::tuple<Ts...>>::value;
            return std::get<I>(storage);
        }

    public:
        template<typename T, typename... Args>
        T& emplace_back(Args&&... args)
        {
            return get<T>().emplace_back(std::forward<Args>(args)...);
        }

        template<typename T>
        void push_back(T&& value)
        {
            get<T>().push_back(std::forward<T>(value));
        }
    };

    struct ExpressionAST
    {   
    public:
        using Type = ExpressionType;

    public:
        ExpressionTypes<ASTStorage> storage;
    };



    template<typename Method, typename Ast, typename Tuple, std::size_t... Is>
    consteval auto make_dispatch_table_impl(std::index_sequence<Is...>)
    {
        return std::array{
            [](const Ast& ast, ASTIndexType index) -> typename Method::return_type
            {
                using T = std::tuple_element_t<Is, Tuple>;
                const auto& vector = ast.storage.template get<T>();
                return (vector[index].*Method::pointer)(ast);
            }...
        };
    }

    template<typename Method, typename Ast, typename Tuple>
    consteval auto make_dispatch_table()
    {
        return make_dispatch_table_impl<Method, Ast, Tuple>(
            std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }



    template<typename Ast, typename Tuple>
    struct ASTHandle
    {
    public:
        using Anchor = std::tuple_element_t<0, Tuple>;

    public:
        const Ast& ast;
        ASTIndexType index;
        std::size_t tag;

    public:
        template<typename Method>
        decltype(auto) call() const
        {
            static constinit auto table =
                make_dispatch_table<Method, Ast, Tuple>();

            return table[tag](ast, index);
        }

    public:
        #define AST_HANDLE_METHODS(X) \
            X(print) \
            X(evaluate) \
            X(optimize) \
            X(check_types)

        #define AST_HANDLE_METHOD(name) \
            decltype(auto) name() const \
            { \
                return call<ASTMethod<&Anchor::name, decltype(&Anchor::name)>>(); \
            }

        AST_HANDLE_METHODS(AST_HANDLE_METHOD)

        #undef AST_HANDLE_METHOD
        #undef AST_HANDLE_METHODS
    };


    // entry point into AST node dispatch
    template<typename Node, typename Ast, typename Tuple>
    ASTHandle<Ast, Tuple> make_handle(const Ast& ast, ASTIndexType index)
    {
        return ASTHandle<Ast, Tuple>{ast, index, TypeIndex<Node, Tuple>::value};
    }



    struct BinaryExpression
    {
    private:
        static constexpr auto tag = ExpressionIndex<BinaryExpression>::value;

    public:
        ASTIndexType left_index;   // index of left expression in the ExpressionAST storage
        ASTIndexType right_index;  // index of right expression
        char op;                   // '+', '-', '*', '/' etc.

        void print(const ExpressionAST& ast) const {
            ASTHandle<ExpressionAST, ExpressionType> left(ast, left_index, tag);
            ASTHandle<ExpressionAST, ExpressionType> right(ast, right_index, tag);
            left.print();
            std::cout << " " << op << " ";
            right.print();
        }

        BinaryExpression evaluate(const ExpressionAST& ast) const {
            ASTHandle<ExpressionAST, ExpressionType> left_handle(ast, left_index, tag);
            ASTHandle<ExpressionAST, ExpressionType> right_handle(ast, right_index, tag);
            auto left_val = left_handle.evaluate();
            auto right_val = right_handle.evaluate();
            return *this;
        }

        BinaryExpression optimize(const ExpressionAST& ast) const {
            ASTHandle<ExpressionAST, ExpressionType> left_handle(ast, left_index, tag);
            ASTHandle<ExpressionAST, ExpressionType> right_handle(ast, right_index, tag);
            auto left_opt = left_handle.optimize();
            auto right_opt = right_handle.optimize();
            return *this;
        }

        bool check_types(const ExpressionAST& ast) const {
            ASTHandle<ExpressionAST, ExpressionType> left_handle(ast, left_index, tag);
            ASTHandle<ExpressionAST, ExpressionType> right_handle(ast, right_index, tag);
            return left_handle.check_types() && right_handle.check_types();
        }
    };
}

#endif
