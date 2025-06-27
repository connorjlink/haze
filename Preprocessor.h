#ifndef HAZE_PREPROCESSOR_H
#define HAZE_PREPROCESSOR_H

#include "Token.h"

// Haze Preprocessor.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	class Preprocessor
	{
	private:
		std::string& code;
		std::string parent_filepath;

	public:
		Preprocessor(std::string& code, const std::string& parent_filepath)
			: code{ code }, parent_filepath{ parent_filepath }
		{
		}

	private:
		struct Macro
		{
			std::string name;
			std::vector<std::string> args;
			std::string code;
			Token token;
		};

	private:
		std::vector<Macro> defined_macros, invoked_macros;

	// NOTE: OLD CONTENT! HERE ONLY FOR LEGACY/BACKWARDS-COMPATIBILITY PURPOSES
	private:
		void preprocess_include_old();
		void preprocess_macro_definition_old();
		void preprocess_macro_invokation_old();

	public:
		std::string preprocess_old();


	// NOTE: new content
	private:
		struct IncludeContext
		{
			std::string content;
			SourceLocation location;

			char current(void) const
			{
				if (location.position < content.length())
				{
					return content[location.position];
				}
				return '\0';
			}
		};

	private:
		// filepath -> contents lazy-load :)
		std::unordered_map<std::string, std::string> _file_contents;

	private:
		void register_macro_definition(const Macro&);
		void register_macro_invokation(const std::string&, SourceLocation);

	private:
		std::string read_identifier(IncludeContext&);
		Token forge(const IncludeContext&) const;
		bool expect(IncludeContext&, char);
		bool anticipate(IncludeContext&, char);
		void skip_whitespace(IncludeContext&, bool = false);
		void advance(IncludeContext&, std::int32_t);
		void load_file(const std::string&);
		bool match_keyword(IncludeContext&, const std::string&);
		void handle_include(IncludeContext&);
		void handle_macro_definition(IncludeContext&);
		void handle_macro_invokation(IncludeContext&);

	public:
		void preprocess(const std::string&); // initial file

	};
}

#endif
