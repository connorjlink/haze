#ifndef HAZE_LAZY_H
#define HAZE_LAZY_H

// Haze Lazy.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// NOTE: not thread safe
	template<typename T>
	struct Lazy
	{
	private:
		mutable std::optional<T> value;
		std::function<T()> functor;

	public:
		template<typename Self>
		auto&& get(this Self&& self)
		{
			if (!self.value)
			{
				self.value.emplace(self.functor());
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

		void reset()
		{
			value.reset();
		}

		bool has_value() const
		{
			return value.has_value();
		}

	public:
		operator T&()
		{
			return get();
		}

		operator const T& () const
		{
			return get();
		}

		operator T && ()&&
		{
			return std::move(get());
		}

	public:
		template<typename F, typename... ArgumentsTs>
		Lazy(F&& f, ArgumentsTs&&... arguments)
			: value{}
			, functor { [f = std::forward<F>(f), ...args = std::forward<ArgumentsTs>(arguments)]() mutable
			{
				return std::invoke(f, args...);
			} }
		{
		}
	};
}

#endif
