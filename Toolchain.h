#ifndef HAZE_TOOLCHAIN_H
#define HAZE_TOOLCHAIN_H

#include "ToolchainType.h"
#include "FileManager.h"
#include "Token.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace hz
{
	class Toolchain
	{
	protected:
		FileManager _file_manager;

	protected:
		std::vector<std::string> _filepaths;

	protected:
		// filepath to vector<token> mapping
		std::unordered_map<std::string, std::vector<Token>> _tokens;

	private:
		std::int32_t _toolchain_task;

	public:
		Toolchain()
			: _file_manager{}, _filepaths{}, _tokens{}
		{
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
		void init(const std::vector<std::string>&);
		void shut_down();
	};

	extern Toolchain* _toolchain;
}

#endif
