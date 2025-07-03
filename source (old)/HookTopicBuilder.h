#ifndef HAZE_HOOKTOPICBUILDER_H
#define HAZE_HOOKTOPICBUILDER_H

// Haze HookTopicBuilder.h
// (c) Connor J. Link. All Rights Reserved.

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

	constexpr std::string project(Project);
	constexpr std::string subproject(Subproject);
	constexpr std::string datapoint(Datapoint);
	constexpr std::string operation(Operation);

	std::string build_topic(Project, Subproject, Datapoint, Operation);
}

#endif
