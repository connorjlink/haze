import std;

#include "HookTopicBuilder.h"
#include "ErrorReporter.h"

// Haze HookTopicBuilder.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	constexpr std::string project(Project _project)
	{
		using enum Project;
		switch (_project)
		{
			case HAZE: return "haze";
			case GEO: return "geo";

			default:
			{
				USE_UNSAFE(ErrorReporter).post_error("invalid project type", NULL_TOKEN);
				return "ERROR";
			} break;
		}
	}

	constexpr std::string subproject(Subproject _subproject)
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

			default:
			{
				USE_UNSAFE(ErrorReporter).post_error("invalid namespace type", NULL_TOKEN);
				return "ERROR";
			} break;
		}
	}

	constexpr std::string datapoint(Datapoint _datapoint)
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

			default:
			{
				USE_UNSAFE(ErrorReporter).post_error("invalid datapoint type", NULL_TOKEN);
				return "ERROR";
			} break;
		}
	}

	constexpr std::string operation(Operation _operation)
	{
		using enum Operation;
		switch (_operation)
		{
			case GET: return "get";
			case SET: return "set";
			case BROADCAST: return "broadcast";

			default:
			{
				USE_UNSAFE(ErrorReporter).post_error("invalid operation type", NULL_TOKEN);
				return "ERROR";
			} break;
		}
	}

	std::string build_topic(Project _project, Subproject _subproject, Datapoint _datapoint, Operation _operation)
	{
		constexpr auto PREFIX = "cjl";
		return std::format("{}/{}/{}/{}/{}", 
			PREFIX, project(_project), subproject(_subproject), datapoint(_datapoint), operation(_operation));
	}
}