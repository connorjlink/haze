#ifndef HAZE_TOOLCHAIN_H
#define HAZE_TOOLCHAIN_H

#include "ToolchainType.h"
#include "FileManager.h"
#include "Token.h"

// Haze Toolchain.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Toolchain
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

	public:
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

	extern Toolchain* _toolchain;
}

#endif
