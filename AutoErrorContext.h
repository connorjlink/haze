#ifndef HAZE_AUTOERRORCONTEXT_H
#define HAZE_AUTOERRORCONTEXT_H

// Haze AutoErrorContext.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class AutoErrorContext
	{
	private:
		std::size_t _initial_context_count;
		TestParameters _frame;

	private:
		std::string _file, _task;

	public:
		AutoErrorContext(const std::string&, const std::string&) noexcept;
		~AutoErrorContext() noexcept;
	};
}

#endif
