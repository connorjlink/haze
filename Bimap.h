#ifndef HAZE_BIMAP_H
#define HAZE_BIMAP_H

#include <unordered_map>
#include <string>
#include <format>

#include "Log.h"

namespace hz
{
	template<typename T>
	struct Bimap_t
	{
		T value;
		std::string identifier;
	};

	template<typename T, typename... Ts>
	class Bimap
	{
	private:
		std::unordered_map<T, std::string> _forward;
		std::unordered_map<std::string, T> _backward;

	public:
		const std::string& at(const T& value) const
		{
			if (auto it = _forward.at(value);
				it != std::end(_forward))
			{
				return *it;
			}

			Log::error(std::format("Value {} was not defined", value));
		}

		T at(const std::string& identifier) const
		{
			if (auto it = _backward.at(identifier);
				it != std::end(_backward))
			{
				return *it;
			}

			Log::error(std::format("Identifier {} was not defined", identifier));
		}

	public:
		void add(Bimap_t<T> set)
		{
			_forward[set.value] = set.identifier;
			_backward[set.identifier] = set.value;
		}

	public:
		Bimap(Ts... sets)
		{
			add(sets...);
		}
	};
}

#endif 
