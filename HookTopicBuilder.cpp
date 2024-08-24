#include "HookTopicBuilder.h"

#include <format>

namespace hz
{
	constexpr std::string_view project(Project _project)
	{
		using enum Project;
		switch (_project)
		{
			case HAZE: return "haze";
			case GEO: return "geo";

			default: Log::error("Invalid project type");
		}
	}

	constexpr std::string_view subproject(Subproject _subproject)
	{
		using enum Subproject;
		switch (_subproject)
		{
			// haze-related
			case COMPILER: return "compiler";
			case ASSEMBLER: return "assembler";
			case INTERPRETER: return "interpreter";

			// geo-related
			case ENGINE: return "engine";
			case FRONTEND: return "frontend";

			default: Log::error("Invalid namespace type");
		}
	}

	constexpr std::string_view datapoint(Datapoint _datapoint)
	{
		using enum Datapoint;
		switch (_datapoint)
		{
			// haze-related


			// geo-related
			case FRAMETIME: return "frametime";
			case CONTEXT: return "context";

			// common
			case HEALTH: return "health";

			default: Log::error("Invalid datapoint type");
		}
	}

	constexpr std::string_view operation(Operation _operation)
	{
		using enum Operation;
		switch (_operation)
		{
			case GET: return "get";
			case SET: return "set";
			case BROADCAST: return "broadcast";

			default: Log::error("Invalid operation type");
		}
	}

	std::string build_topic(Project _project, Subproject _subproject, Datapoint _datapoint, Operation _operation)
	{
		constexpr auto PREFIX = "cjl";
		return std::format("{}/{}/{}/{}/{}", 
			PREFIX, project(_project), subproject(_subproject), datapoint(_datapoint), operation(_operation));
	}
}