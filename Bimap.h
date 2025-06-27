#ifndef HAZE_BIMAP_H
#define HAZE_BIMAP_H

// Haze Bimap.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T, typename U>
		requires !std::is_same_v<T, U>
	struct bimap_t
	{
		T _first;
		U _second;
	};

	template<typename T, typename U>
		requires !std::is_same_v<T, U>
	class Bimap
	{
	private:
		std::unordered_map<T, U> _forward;
		std::unordered_map<U, T> _backward;

	public:
		bool contains(const T& value) const
		{
			return _forward.contains(value);
		}

		std::optional<U> at(const T& value) const
		{
			if (_forward.contains(value))
			{
				return { _forward.at(value) };
			}

			return std::nullopt;
		}

		bool contains(const U& value) const
		{
			return _backward.contains(value);
		}

		std::optional<T> at(const U& value) const
		{
			if (_backward.contains(value))
			{
				return { _backward.at(value) };
			}

			return std::nullopt;
		}

	private:
		template<typename P, typename... Rs>
		void add_impl(P&& p, Rs&&... rs)
		{
			add(p);
			add_impl(rs...);
		}

		void add_impl()
		{
		}

	public:
		void add(bimap_t<T, U> set)
		{
			_forward[set._first] = set._second;
			_backward[set._second] = set._first;
		}

	public:
		template<typename... Vs>
		Bimap(Vs... vs)
		{
			add_impl(vs...);
		}
	};
}

#endif 
