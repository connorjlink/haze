#ifndef HAZE_ENVIRONMENT_H
#define HAZE_ENVIRONMENT_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze Environment.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	struct Environment
		: public InjectSingleton<ErrorReporter>
	{
	private:
		using ScopeMap = std::unordered_map<std::string_view, T>;
		struct Scope
		{
			ScopeMap map;
			// establish cut points for dynamic evaluation along the call stack 
			bool is_boundary;
		};


	private:
		std::vector<ScopeMap> scopes;

	public:
		void enter_block()
		{
			scopes.emplace_back({}, false);
		}

		void enter_function()
		{
			scopes.emplace_back({}, true);
		}

		void exit_block()
		{
			if (scopes.size() > 1 && !scopes.back().is_boundary)
			{
				scopes.pop_back();
			}
		}

		void exit_function()
		{
			while (scopes.size() > 1)
			{
				const auto is_boundary = scopes.back().is_boundary;
				scopes.pop_back();

				if (is_boundary)
				{
					break;
				}
			}
		}

		void define(std::string_view name, T value)
		{
			// define in most nested scope
			scopes.back().map[name] = std::move(value);
		}

		void define_global(std::string_view name, T value)
		{
			// define in least nested, top-level scope
			scopes.front().map[name] = std::move(value);
		}

		std::optional<T> find(std::string_view name)
		{
			// reverse iteration to look upwards through the scopes
			for (auto it = scopes.rbegin(); it != scopes.rend(); ++it)
			{
				const auto found = it->find(name);
				if (found != it->end())
				{
					return found->second;
				}

				// break search if reaching a boundary not to leak caller's scope
				if (it->is_boundary)
				{
					break;
				}
			}

			// search global scope as fallback if not found locally
			if (!scopes.front().map.contains(name))
			{
				return std::nullopt;
			}

			return scopes.front().map[name];
		}

	public:
		Environment()
		{
			enter_block();
		}
	};
}

#endif
