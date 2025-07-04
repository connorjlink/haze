#ifndef HAZE_TOOLCHAIN_H
#define HAZE_TOOLCHAIN_H

#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <io/FileManager.h>
#include <job/JobManager.h>
#include <toolchain/Generator.h>
#include <toolchain/ToolchainType.h>
#include <toolchain/Parser.h>
#include <toolchain/models/Token.h>

// Haze Toolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Toolchain
		: public ServiceTag<Toolchain>
		, public InjectService<Generator, JobManager, Parser>
		, public InjectSingleton<CommandLineOptions, ErrorReporter, FileManager>
	{
	protected:
		// filepath to vector<token> mapping
		std::unordered_map<std::string, std::vector<Token>> _tokens;

	private:
		std::int32_t _toolchain_task;

	public:
		Toolchain()
			: _tokens{}
		{
			_toolchain_task = -1;
		}
		virtual ~Toolchain() = default;

	public:
		void panic();

	public:
		virtual ToolchainType ttype() const = 0;
		// entry point filepath
		virtual void run(const std::string&) = 0;

	public:
		// entry point filepath
		void init(const std::string&);
		// is_panic
		void shut_down(bool);
	};
}

#endif
