#ifndef HAZE_STRINGS_H
#define HAZE_STRINGS_H

// Haze Strings.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    namespace detail
    {
        // https://stackoverflow.com/questions/23999573/convert-a-number-to-a-string-literal-with-constexpr
        template<std::size_t... Digits>
        struct DigitsToStringLiteral 
        { 
            static const char value[];
        };

        template<std::size_t... Digits>
        constexpr char DigitsToStringLiteral<Digits...>::value[] = {('0' + Digits)..., 0};

        template<std::size_t Remainder, std::size_t... Digits>
        struct DigitsExpander : DigitsExpander<Remainder / 10, Remainder % 10, Digits...> 
        {
        };

        template<std::size_t... Digits>
        struct DigitsExpander<0, Digits...> : DigitsToStringLiteral<Digits...>
        {
        };
    }

    template<std::size_t N>
    struct IntegerToStringLiteral : detail::DigitsExpander<N>
    {
    };


    template<std::size_t N>
    struct CompileTimeString
    {
        char value[N + 1]{};

        constexpr CompileTimeString(const char(&str)[N + 1])
        {
            for (auto i = 0uz; i < N; ++i)
            {
                value[i] = str[i];
            }

            value[N] = '\0';
        }
    };

    template<CompileTimeString String, CompileTimeString... Strings>
    consteval auto compile_time_concatenate(String&& string, Strings&&... strings)
    {
        constexpr std::size_t total_size = (String.size + Strings.size + ...);
        std::array<char, total_size + 1> result{};

        if constexpr (sizeof...(Strings) == 0)
        {
            for (auto i = 0uz; i < String.size; ++i)
            {
                result[i] = string.value[i];
            }
        }
        else
        {
            auto offset = 0uz;

            auto append_string = [&](const auto& str)
            {
                for (auto i = 0uz; i < str.size; ++i)
                {
                    result[offset + i] = str.value[i];
                }
                offset += str.size;
            };

            append_string(string);
            (append_string(strings), ...);
        }

        return result;
    }

    template<typename T, typename... Ts>
    consteval auto compile_time_concat(T&& first, Ts&&... rest)
    {
        return compile_time_concatenate(CompileTimeString{ first }, CompileTimeString{ rest }...);
    }
}

#endif
