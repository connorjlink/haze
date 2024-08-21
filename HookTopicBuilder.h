#ifndef HAZE_HOOKTOPICBUILDER_H
#define HAZE_HOOKTOPICBUILDER_H

#include <string>
#include <string_view>

#include "Log.h"

namespace hz
{
	/*
		The MQTT topic format utilized for all projects is:
			cjl/{project}/{subproject}/{datapoint}/{operation}

		So, if the interpreter has run and wants to update the engine on the results:
			Interpreter: cjl/haze/interpreter/context/set -- payload == new context

		Or, if the frontend wants to determine current performance of the engine:
			Frontend: cjl/geo/engine/frametime/get
			Engine (response): cjl/geo/engine/frametime/broadcast -- payload == current frametime
	*/

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

	constexpr std::string_view project(Project);
	constexpr std::string_view subproject(Subproject);
	constexpr std::string_view datapoint(Datapoint);
	constexpr std::string_view operation(Operation);

	std::string build_topic(Project, Subproject, Datapoint, Operation);
}

#endif
