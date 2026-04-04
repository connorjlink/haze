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
#include <format>

namespace hz
{
    // meta type traits
    template<typename T, typename Tuple>
    struct TypeIndex;

    template<typename T, typename... Types>
    struct TypeIndex<T, std::tuple<T, Types...>> 
        : public std::integral_constant<std::size_t, 0> 
    {
    };

    template<typename T, typename U, typename... Types>
    struct TypeIndex<T, std::tuple<U, Types...>> 
        : public std::integral_constant<std::size_t, 1 + TypeIndex<T, std::tuple<Types...>>::value> 
    {
    };

    template<typename T, typename Tuple>
    using TypeIndexV = TypeIndex<T, Tuple>::value;


    // function type traits
    template<auto MethodPointer, typename Signature>
    struct Method;

    template<auto MethodPointer, typename R, typename... Args>
    struct Method<MethodPointer, R(Args...)>
    {
        static constexpr auto pointer = MethodPointer;
        using return_type = R;
    };


    // node dispatch type traits
    template<typename T, typename Sum, typename Fn>
    concept ImplementsMethod = requires(const T& node, const Sum& sum)
    {
        { (node.*Fn::pointer)(sum) } -> std::same_as<typename Fn::return_type>;
    };

    template<typename T, typename Sum, typename Tuple, std::size_t... Is>
    concept SumTupleImpl = (ImplementsMethod<T, Sum, std::tuple_element_t<Is, Tuple>> && ...);

    template<typename T, typename Sum, typename Tuple>
    concept SumTuple = SumTupleImpl<T, Sum, Tuple, std::make_index_sequence<std::tuple_size_v<Tuple>>{}>;



    // maximum 255 subtypes (0 reserved for invalid)
    using TagType = std::uint8_t;
    using RegisterIndex = std::uint32_t;

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

    template<typename T>
    using ExpressionIndex = TypeIndex<T, ExpressionType>;


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

    template<typename T>
    using StatementIndex = TypeIndex<T, StatementType>;


    // expose a strict polymorphic interface for AST nodes
    template<typename Sum, typename Anchor>
    using ASTMethods = std::tuple
    <
        Method<&Anchor::atype, TagType()>,
        Method<&Anchor::format, std::string()>,
        Method<&Anchor::evaluate, Sum(const Sum&)>,
        Method<&Anchor::optimize, std::optional<Sum>(const Sum&)>,
        Method<&Anchor::check_types, bool(const Sum&)>
    >;

    using ExpressionMethods = ASTMethods<ExpressionStorage, ExpressionAnchor>;
    using StatementMethods = ASTMethods<StatementStorage, StatementAnchor>;

    template<typename T, typename Sum, typename Anchor>
    concept ASTNode = SumTuple<T, Sum, ASTMethods<Sum, Anchor>>;


    // TODO: verify that all of the SumStorage members are of StatementStorage/ExpressionAST (and by proxy that they implement the required methods)
    template<typename... Ts>
    struct SumStorage 
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



    template<typename Fn, typename Sum, typename Tuple, std::size_t... Is>
    consteval auto make_dispatch_table_impl(std::index_sequence<Is...>)
    {
        return std::array
        {
            [](const Sum& ast, RegisterIndex index) -> typename Fn::return_type
            {
                using T = typename std::tuple_element_t<Is, Tuple>;
                const auto& vector = ast.storage.template get<T>();
                return (vector[index].*Method::pointer)(ast);
            }...
        };
    }

    template<typename Fn, typename Sum, typename Tuple>
    consteval auto make_dispatch_table()
    {
        return make_dispatch_table_impl<Fn, Sum, Tuple>(
            std::make_index_sequence<std::tuple_size_v<Tuple>>{});
    }



    template<typename Sum, typename Tuple>
    struct SumHandle
    {
    public:
        using Anchor = std::tuple_element_t<0, Tuple>;

    public:
        const Sum& ast;
        RegisterIndex index;
        std::size_t tag;

    public:
        template<typename Method>
        decltype(auto) call() const
        {
            static constinit auto table =
                make_dispatch_table<Method, Sum, Tuple>();

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
    template<typename Node, typename Sum, typename Tuple>
    SumHandle<Sum, Tuple> make_handle(const Sum& ast, RegisterIndex index)
    {
        return SumHandle<Sum, Tuple>{ast, index, TypeIndex<Node, Tuple>::value};
    }


    // TODO: integrate Expression and Statement families
    struct ExpressionStorage : public ExpressionTypes<SumStorage>
    {   
    public:
        using Type = ExpressionType;
    };

    struct StatementStorage : public StatementTypes<SumStorage>
    {
    public:
        using Type = StatementType;
    };


    struct ValueStorage : public ValueTypes<SumStorage>
    {
    public:
        using Type = ValueType;
    };



#define MAKE_HANDLE(ast, index) make_handle<decltype(*this), std::decay_t<decltype(ast)>, typename std::decay_t<decltype(ast)>::Type>(ast, index)

    // TODO: Remake
    struct BinaryExpression
    {
    private:
        static constexpr auto tag = ExpressionIndex<BinaryExpression>::value;

    public:
        RegisterIndex left_index;   // index of left expression in the ExpressionAST storage
        RegisterIndex right_index;  // index of right expression
        char op;                   // '+', '-', '*', '/' etc.

        void print(const ExpressionStorage& ast) const
        {
            SumHandle<ExpressionStorage, ExpressionType> left(ast, left_index, tag);
            SumHandle<ExpressionStorage, ExpressionType> right(ast, right_index, tag);
            left.print();
            std::cout << " " << op << " ";
            right.print();
        }

        BinaryExpression evaluate(const ExpressionStorage& ast) const
        {
            const auto left_handle = MAKE_HANDLE(ast, left_index);
            const auto right_handle = MAKE_HANDLE(ast, right_index);

            const auto left_val = left_handle.evaluate();
            const auto right_val = right_handle.evaluate();

            // TODO: perform the operation based upon 'op'

            return *this;
        }

        BinaryExpression optimize(const ExpressionStorage& ast) const
        {
            SumHandle<ExpressionStorage, ExpressionType> left_handle(ast, left_index, tag);
            SumHandle<ExpressionStorage, ExpressionType> right_handle(ast, right_index, tag);
            auto left_opt = left_handle.optimize();
            auto right_opt = right_handle.optimize();
            return *this;
        }

        bool check_types(const ExpressionStorage& ast) const
        {
            SumHandle<ExpressionStorage, ExpressionType> left_handle(ast, left_index, tag);
            SumHandle<ExpressionStorage, ExpressionType> right_handle(ast, right_index, tag);
            return left_handle.check_types() && right_handle.check_types();
        }
    };


    // VALUE CLASS

    struct RegisterValue;
    struct StackValue;
    // global object in the data segment
    struct StaticValue;

    template<template<typename...> class T>
    using ValueTypes = T
    <
        RegisterValue,
        StackValue,
        StaticValue
    >;

    using ValueType = ValueTypes<std::tuple>;
    using ValueAnchor = std::tuple_element_t<0, ValueType>;

    // value mapper
    template<std::size_t I>
    using ValueTypeAt = std::tuple_element_t<I, ValueType>;

    template<typename T>
    using ValueIndex = TypeIndex<T, ValueType>;


    // expose a strict polymorphic interface for values
    template<typename Sum, typename Anchor>
    using ValueMethods = std::tuple
    <
        Method<&Anchor::vtype, TagType()>
        Method<&Anchor::format, std::string()>,
        Method<&Anchor::load_into, void(Generator&, RegisterIndex)>,
        Method<&Anchor::store_from, void(Generator&, RegisterIndex)>,
    >;

    using ValueMethod = ValueMethods<ValueStorage, ValueAnchor>;


    template<typename T, typename Sum, typename Anchor>
    concept Value = SumTuple<T, Sum, ValueMethods<Sum, Anchor>>;



    struct RegisterValue
    {
    public:
        using RegisterIndex = std::uint8_t;

    public:
        RegisterIndex index;

    public:
        TagType vtype() const
        {
            return 
        }

        std::string format() const
        {
            return std::format("r{}", index);
        }

        void load_into(Generator& generator, RegisterIndex destination) const
        {
            if (destination != index)
            {
                generator.move(destination, index);
            }
        }

        void store_from(Generator& generator, RegisterIndex source) const
        {
            if (source != index)
            {
                generator.move(index, source);
            }
        }
    };



}

#endif
