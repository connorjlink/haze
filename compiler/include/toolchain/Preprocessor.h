#ifndef HAZE_PREPROCESSOR_H
#define HAZE_PREPROCESSOR_H

#include "Scanner.h"

// Haze Preprocessor.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected file manager service
	class Preprocessor : public Scanner
	{
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

	private:
		// filepath -> contents lazy-load :)
		std::unordered_map<std::string, std::string> _raw_file_cache;
		std::stack<SourceContext> _files_to_process;
		// used to prohibit recursive macros
		std::unordered_set<std::string> _macro_invokations;

	private:
		void register_macro_definition(const Macro&);
		void register_macro_invokation(const std::string&);

	private:
		void load_file(const std::string&);

	public:
		bool match_macro_invokation(void);
		void handle_include(void);
		void handle_macro_definition(void);
		void handle_macro_invokation(void);

	public:
		void preprocess(const std::string&); // initial file

	public:
		ScannerType stype(void) const noexcept;

	public:
		Preprocessor(const std::string&);
	};
}

#endif
