#ifndef HAZE_AUTOERRORCONTEXT_H
#define HAZE_AUTOERRORCONTEXT_H

#include <data/DependencyInjector.h>
#include <error/ErrorReporter.h>

// Haze AutoErrorContext.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	struct ErrorFrame;

	struct AutoErrorContext : public InjectSingleton<ErrorReporter>
	{
	private:
		std::size_t initial_context_count;
		ErrorFrame frame;

	private:
		std::string file, task;

	public:
		AutoErrorContext(const std::string&, const std::string&);
		~AutoErrorContext();
	};
}

#endif
