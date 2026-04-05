#ifndef HAZE_VALUESUM_H
#define HAZE_VALUESUM_H

#include <ast/new/Sum.h>
#include <ast/new/Typing.h>

// Haze ValueSum.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    class RegisterValue;
    class StackValue;
    // global object in the data segment
    class StaticValue;


    using ValueTypes = SumTypeList
    <
        RegisterValue,
        StackValue,
        StaticValue
    >;

    // value mapper
    template<std::size_t I>
    using ValueTypeAt = std::tuple_element_t<I, ValueType>;

    template<typename T>
    using ValueIndex = TypeIndex<T, ValueType>;

    // expose a strict polymorphic interface for values
    template<typename SumStorageT>
    using ValueMethods = std::tuple
    <
        Method<&(typename SumStorageT::Anchor)::ttype, TagType()>,
        Method<&(typename SumStorageT::Anchor)::format, std::string()>,
        Method<&(typename SumStorageT::Anchor)::load_into, void(Generator&, RegisterIndex)>,
        Method<&(typename SumStorageT::Anchor)::store_from, void(Generator&, RegisterIndex)>,
    >;

    using ValueMethod = ValueMethods<ValueStorage>;

    template<typename SumMemberT, typename SumStorageT>
    concept Value = SumTuple<SumMemberT, SumStorageT, ValueMethods<SumStorageT>>;


    class RegisterValue 
        : public InjectTagType<ValueIndex, RegisterValue>
    {
    public:
        using RegisterIndex = std::uint8_t;

    public:
        RegisterIndex index;

    public:
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
