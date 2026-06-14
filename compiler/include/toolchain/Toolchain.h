#ifndef HAZE_TOOLCHAIN_H
#define HAZE_TOOLCHAIN_H

#include <cli/CommandLineOptions.h>
#include <data/DependencyInjector.h>
#include <io/FileManager.h>
#include <job/JobManager.h>
#include <toolchain/defs/ToolchainKind.h>
#include <toolchain/Generator.h>
#include <toolchain/Parser.h>
#include <toolchain/models/Token.h>
#include <toolchain/Linker.h>

// Haze Toolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename T>
	struct Toolchain
		: public ServiceTag<Toolchain>
		, public InjectService<Generator, JobManager, Parser<T>>
		, public InjectSingleton<CommandLineOptions, ErrorReporter, FileManager, Linker>
	{
	protected:
		// filepath to vector<token> mapping
		std::unordered_map<std::string, std::vector<Token>> tokens;

	private:
		std::int32_t toolchain_task;

	public:
		Toolchain()
			: tokens{}, toolchain_task{ -1 }
		{
		}
		virtual ~Toolchain() = default;

	public:
		void panic();

	public:
		virtual ToolchainKind tag_type() const = 0;
		virtual std::vector<Linkable> run(const std::string&) = 0;

	public:
		// entry point filepath
		void init(const std::string&);
		// is_panic
		void shut_down(bool);
	};
}

#endif
