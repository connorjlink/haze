#ifndef HAZE_PREPROCESSOR_H
#define HAZE_PREPROCESSOR_H

#include <string>
#include <string_view>
#include <vector>

namespace hz
{
	class Preprocessor
	{
	private:
		std::string code;
		std::string parent_filepath;

	public:
		Preprocessor(std::string&& code, std::string parent_filepath)
			: code(std::move(code)), parent_filepath(std::move(parent_filepath))
		{
		}

	private:
		struct Macro
		{
			std::string name;
			std::vector<std::string> args;
			std::string code;
		};

	private:
		std::vector<Macro> defined_macros, invoked_macros;

	private:
		void preprocess_include();
		void preprocess_macro_definition();
		void preprocess_macro_invokation();

	public:
		std::string preprocess();
	};
}

#endif //HAZE_PREPROCESSOR_H
