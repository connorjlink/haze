#ifndef HAZE_BIMAP_H
#define HAZE_BIMAP_H

#include <string>
#include <format>
#include <optional>

#include <type_traits>
#include <unordered_map>

#include "Log.h"

namespace hz
{
	template<typename T, typename U>
	struct bimap_t
	{
		T _first;
		U _second;
	};

	template<typename T, typename U>
	class Bimap
	{
	private:
		std::unordered_map<T, U> _forward;
		std::unordered_map<U, T> _backward;

	public:
		std::optional<U> at(const T& value) const
		{
			if (auto it = _forward.find(value);
				it != _forward.end())
			{
				return { it->second };
			}

			return std::nullopt;
		}

		std::optional<T> at(const U& value) const
		{
			if (auto it = _backward.find(value);
				it != _backward.end())
			{
				return { it->second };
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
