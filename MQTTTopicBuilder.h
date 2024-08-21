#ifndef HAZE_MQTTTOPICBUILDER_H
#define HAZE_MQTTTOPICBUILDER_H

#include <string>
#include <string_view>

#include "Log.h"

namespace hz
{
	enum class Project
	{
		HAZE,
		GEO,
	};

	enum class Subproject
	{
		// haze-related
		COMPILER,
		ASSEMBLER,
		INTERPRETER,

		// geo-related
		ENGINE,
		FRONTEND,
	};

	enum class Datapoint
	{
		// haze-related
		

		// geo-related
		FRAMETIME,
		CONTEXT,


		// common
		HEALTH,
	};

	enum class Operation
	{
		GET,
		SET,
		BROADCAST
	};

	constexpr auto project(Project _project)
	{
		using enum Project;
		switch (_project)
		{
			case HAZE: return "haze";
			case GEO: return "geo";

			default: Log::error("Invalid project type");
		}
	}

	constexpr auto subproject(Subproject _subproject)
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

	constexpr auto datapoint(Datapoint _datapoint)
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

	constexpr auto operation(Operation _operation)
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

	/*
		The MQTT topic format utilized for all projects is:
			cjl/{project}/{subproject}/{datapoint}/{operation}

		So, if the interpreter has run and wants to update the engine on the results:
			Interpreter: cjl/haze/interpreter/context/set -- payload == new context

		Or, if the frontend wants to determine current performance of the engine:
			Frontend: cjl/geo/engine/frametime/get
			Engine (response): cjl/geo/engine/frametime/broadcast -- payload == current frametime
	*/

	std::string build(Project _project, Subproject _subproject, Datapoint _datapoint, Operation _operation)
	{
		constexpr auto PREFIX = "cjl";
		return std::format("{}/{}/{}/{}/{}", PREFIX, project(_project), subproject(_subproject), datapoint(_datapoint), operation(_operation));
	}
}

#endif
