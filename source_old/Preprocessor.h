#ifndef HAZE_PREPROCESSOR_H
#define HAZE_PREPROCESSOR_H

#include "Token.h"
#include "Scanner.h"

// Haze Preprocessor.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected file manager service
	class Preprocessor : public Scanner
	{
	public:
		Preprocessor(const std::string& filepath)
			: Scanner{ filepath }
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
		// name -> Macro
		std::unordered_map<std::string, Macro> defined_macros;

	// NOTE: OLD CONTENT! HERE ONLY FOR LEGACY/BACKWARDS-COMPATIBILITY PURPOSES
	private:
		void preprocess_include_old();
		void preprocess_macro_definition_old();
		void preprocess_macro_invokation_old();

	public:
		std::string preprocess_old();


	// NOTE: new content
	private:
		struct Context
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

			std::size_t where(void) const
			{
				return location.position;
			}

			bool eof() const
			{
				return location.position >= content.size();
			}

			void insert_inline(const std::string& code)
			{
				content.insert(where(), code);
			}
		};

	private:
		// filepath -> contents lazy-load :)
		std::unordered_map<std::string, std::string> _raw_file_cache;
		std::unordered_map<std::string, std::string> _processed_file_cache;
		std::stack<Context> _files_to_process;

		// used to prohibit recursive macros
		std::unordered_set<std::string> _macro_invokations;

	private:
		void register_macro_definition(const Macro&);
		void register_macro_invokation(Context& , const std::string&);

	private:
		// uses C identifier formatting, optionally advance the cursor tracking information
		std::string read_identifier(Context&, bool = false);
		// NOTE: does not consume the terminator character
		std::string substring_until(Context&, char);
		// generate a falsified token containing source location information from the specified context
		Token forge(const Context&) const;
		bool expect(Context&, char, bool = true);
		bool anticipate(Context&, char, bool = true);
		void skip_whitespace(Context&, bool = false);
		void advance(Context&, std::size_t = 1u);
		void load_file(const std::string&);
		bool match_keyword(Context&, const std::string&);
		bool match_macro_invokation(Context&);
		void handle_include(Context&);
		void handle_macro_definition(Context&);
		// parent macro optional, to prevent infinite macro substitution recursion
		void handle_macro_invokation(Context&);

	public:
		void preprocess(const std::string&); // initial file
		const std::string& get_preprocessed_source(const std::string&); // for which filepath
	};
}

#endif
