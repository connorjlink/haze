#ifndef HAZE_PREPROCESSOR_H
#define HAZE_PREPROCESSOR_H

#include <toolchain/Scanner.h>

// Haze Preprocessor.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	// inherits the injected file manager service
	struct Preprocessor : public Scanner
	{
	private:
		struct Macro
		{
			std::string_view name;
			std::vector<std::string_view> arguments;
			std::string_view code;
			Token token;
		};

	private:
		// name -> Macro
		std::unordered_map<std::string_view, Macro> defined_macros;

	private:
		// used to prohibit recursive macros
		std::unordered_set<std::string_view> macro_invokations;

	private:
		void register_macro_definition(const Macro&);
		void register_macro_invokation(std::string_view);

	private:
		void load_file(std::string_view);

	public:
		bool match_macro_invokation();
		void handle_include();
		void handle_macro_definition();
		void handle_functionlike_macro_definition(std::string_view);
		void handle_objectlike_macro_definition(std::string_view);
		void handle_macro_invokation();
		void handle_functionlike_macro_invokation(std::string_view);
		void handle_objectlike_macro_invokation(std::string_view);
		void handle_conditional_compilation();

	public:
		void preprocess();

	public:
		ScannerKind scanner_kind_implementation() const noexcept;

	public:
		Preprocessor(const std::filesystem::path&);
	};
}

#endif
