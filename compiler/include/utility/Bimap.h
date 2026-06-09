#ifndef HAZE_BIMAP_H
#define HAZE_BIMAP_H

// Haze Bimap.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T, typename U>
		requires (!std::is_same_v<T, U>)
	struct bimap_t
	{
		T first;
		U second;
	};

	template<typename T, typename U>
		requires (!std::is_same_v<T, U>)
	struct Bimap
	{
	private:
		std::unordered_map<T, U> forward;
		std::unordered_map<U, T> backward;

	public:
		bool contains(const T& value) const
		{
			return forward.contains(value);
		}

		std::optional<U> at(const T& value) const
		{
			if (forward.contains(value))
			{
				return { forward.at(value) };
			}

			return std::nullopt;
		}

		bool contains(const U& value) const
		{
			return backward.contains(value);
		}

		std::optional<T> at(const U& value) const
		{
			if (backward.contains(value))
			{
				return { backward.at(value) };
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
			forward[set.first] = set.second;
			backward[set.second] = set.first;
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
