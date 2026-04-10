#ifndef HAZE_LAZY_H
#define HAZE_LAZY_H

// Haze Lazy.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
    // NOTE: not thread safe
    template<typename T, typename FunctorT, typename... Ts>
    class Lazy
    {
    private:
        mutable std::optional<T> value;
        mutable std::decay_t<FunctorT> functor;
        mutable std::tuple<std::decay_t<Ts>...> arguments;

    public:
        template<typename Self>
        auto&& get(this Self&& self)
        {
            if (!self.value)
            {
                self.value.emplace(std::apply([&](auto&... args) {
                    return std::invoke(self.functor, args...);
                }, self.arguments));
            }

            // handle the case where the Lazy is rvalue temporary, need to return an rvalue ref to preserve move semantics
            if constexpr (std::is_lvalue_reference_v<Self>)
            {
                return *self.value;
            }
            else
            {
                return std::move(*self.value);
            }
        }

    public:
        // this is maybe a bad idea, but too tempting to pass up!
        template<typename Self>
        auto&& operator T(this Self&& self)
        {
            return self.get();
        }

    public:
        template<typename F, typename... Args>
        Lazy(F&& f, Args&&... args)
            : value{}
            , functor{ std::forward<F>(f) }
            , arguments{ std::forward<Args>(args)... }
        {
        }
    };
}

#endif
