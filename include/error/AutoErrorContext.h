#ifndef HAZE_AUTOERRORCONTEXT_H
#define HAZE_AUTOERRORCONTEXT_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze AutoErrorContext.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ErrorFrame;

	class AutoErrorContext : public InjectSingleton<ErrorReporter>
	{
	private:
		std::size_t _initial_context_count;
		ErrorFrame _frame;

	private:
		std::string _file, _task;

	public:
		AutoErrorContext(const std::string&, const std::string&);
		~AutoErrorContext();
	};
}

#endif
