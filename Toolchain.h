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
		std::string _filepath;

	protected:
		// filepath to vector<token> mapping
		std::unordered_map<std::string, std::vector<Token>> _tokens;

	private:
		std::int32_t _toolchain_task;

	public:
		Toolchain()
			: _tokens{}
		{
			_filepath = "";
			_toolchain_task = -1;
		}

	public:
		virtual ~Toolchain() = default;

	public:
		void panic();

	public:
		virtual ToolchainType ttype() const = 0;
		virtual void run() = 0;

	public:
		void init(const std::string&);
		void shut_down(bool is_panic);
	};

	extern Toolchain* _toolchain;
}

#endif
