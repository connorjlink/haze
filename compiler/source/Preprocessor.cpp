import std;

#include <error/ErrorReporter.h>
#include <io/FileManager.h>
#include <symbol/models/Symbol.h>
#include <symbol/SymbolDatabase.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/Preprocessor.h>
#include <toolchain/models/Token.h>
#include <utility/Constants.h>

// Haze Preprocessor.cpp
// (c) Connor J. Link. All Rights Reserved.

namespace
{
	void replace(std::string& str, const std::string& from, const std::string& to)
	{
		if (from.empty())
		{
			return;
		}

		auto start_pos = 0uz;

		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}

	// Thanks https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring :)
	// trim from end of string (right)
	std::string& rtrim(std::string& s, const char* t = hz::WHITESPACE)
	{
		s.erase(s.find_last_not_of(t) + 1);
		return s;
	}

	// trim from beginning of string (left)
	std::string& ltrim(std::string& s, const char* t = hz::WHITESPACE)
	{
		s.erase(0, s.find_first_not_of(t));
		return s;
	}

	// trim from both ends of string (right then left)
	std::string& trim(std::string& s, const char* t = hz::WHITESPACE)
	{
		return ltrim(rtrim(s, t), t);
	}
}

namespace hz
{
	void Preprocessor::register_macro_definition(const Macro& macro)
	{
		// only specifies the original definition, so invokations have to manually export each time
		const auto symbol = USE_SAFE(SymbolDatabase)->add_define(macro.name, macro.token);
	}

	void Preprocessor::register_macro_invokation(std::string_view name)
	{
		const auto forged = forge_token();
		// explicitly mark as visited
		const auto symbol = USE_SAFE(SymbolDatabase)->reference_define(name, forged, true);
		USE_SAFE(SymbolExporter)->enqueue(symbol, forged);
	}

	bool Preprocessor::match_macro_invokation()
	{
		// will return empty if there is not a valid macro invokation
		return read_identifier(false) != "";
	}

	void Preprocessor::load_file(const std::filesystem::path& filepath)
	{
		USE_SAFE(FileManager)->open_file(filepath);
		const auto contents = USE_SAFE(FileManager)->get_file(filepath).get_raw_contents();
		files_to_process.emplace(SourceContext{ .source = contents, .location = { filepath, 0, 1, 1 } });
	}

	void Preprocessor::handle_include()
	{
		auto include_filepath = std::string{};

		if (current() == '<')
		{
			// system headers
			expect('<');
			include_filepath = substring_until('>');
			expect('>');
		}
		else
		{
			// user headers
			expect('"');
			include_filepath = substring_until('"');
			expect('>');
		}
		
		if (include_filepath == wherein())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid recursive include of file `{}`", include_filepath), forge_token());
			return;
		}

		load_file(include_filepath);
	}

	void Preprocessor::handle_macro_definition()
	{
		const auto name = read_identifier();
		advance(name.length());
		skip_whitespace();

		if (current() == '(')
		{
			handle_functionlike_macro_definition(name);
		}
		else
		{
			handle_objectlike_macro_definition(name);
		}
	}

	void Preprocessor::handle_functionlike_macro_definition(std::string_view name)
	{
		expect('(');

		// greedily consume until a closing parenthesis
		auto arguments = std::vector<std::string_view>{};
		while (current() != ')')
		{
			const auto argument = read_identifier();
			advance(argument.length());
			arguments.push_back(argument);
			anticipate(',');
		}

		expect(')');

		const auto body = substring_until('\n');

		Macro macro{ name, arguments, body, forge_token() };
		defined_macros.try_emplace(name, macro);

		// call stub you can integrate with your symbol DB
		register_macro_definition(macro);
	}

	void Preprocessor::handle_objectlike_macro_definition(std::string_view name)
	{
#pragma message("TODO: combine lines together with \ character")
		const auto value = substring_until('\n');

	}

	void Preprocessor::handle_objectlike_macro_invokation(std::string_view name)
	{
		advance(name.length());

	}

	void Preprocessor::handle_functionlike_macro_invokation(std::string_view name)
	{
		expect('(');

		// greedily consume until a closing parenthesis
		std::vector<std::string> arguments{};
		while (current() != ')')
		{
			const auto argument = read_identifier();
			arguments.push_back(argument);
			anticipate(',');
		}

		expect(')');

		// explicitly forbid any macro recursion at all. if this macro was called anywhere in the current
		// macro callstack, error out! this is the easiest way to prevent infinite recursion
		if (macro_invokations.contains(name))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"invalid recursive macro `{}`", name), forge_token());
			return;
		}

		if (!defined_macros.contains(name))
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"macro `{}` is undefined", name), forge_token());
			return;
		}

		const auto& macro = defined_macros.at(name);
		if (macro.arguments.size() != arguments.size())
		{
			USE_SAFE(ErrorReporter)->post_error(
				"macro argument count mismatch", forge_token());
			return;
		}

		if (macro.arguments.size() != arguments.size())
		{
			USE_SAFE(ErrorReporter)->post_error(std::format(
				"macro `{}` was defined with {} arguments but called with {}",
					macro.name, macro.arguments.size(), arguments.size()), forge_token());
			return;
		}

		macro_invokations.emplace(name);

		// intentional copy
		auto expanded = macro.code;
		for (auto i = 0; i < arguments.size(); i++)
		{
			// if this substitution logic ever becomes a bottleneck, it might be wise to roll a custom re-scanner
			replace(expanded, std::format("[{}]", macro.arguments[i]), arguments[i]);
		}

		insert_adjacent(expanded);
		register_macro_invokation(name);

		macro_invokations.erase(name);

		// NOTE: explicitly not advancing past the test substition because the new text might contains macros that need to again expand
		//advance(context, expanded.length());
	}

	void handle_conditional_compilation()
	{
#pragma message("TODO")
	}

	void Preprocessor::preprocess()
	{
		load_file();

		// only pop once advanced past the last virtual location of the file in question
		if (eof())
		{
			USE_SAFE(FileManager)->update_file(context.location.filepath, context.source);
			files_to_process.pop();
			continue;
		}

		switch (const auto current = context.current())
		{
			case '#':
			{
				// consume #
				advance();

				if (const auto include_keyword = *token_map.at(TokenKind::INCLUDE); match_keyword(include_keyword))
				{
					advance(include_keyword.length());
					skip_whitespace();

					handle_include();
				}
				else if (const auto define_keyword = *token_map.at(TokenKind::DEFINE); match_keyword(define_keyword))
				{
					advance(define_keyword.length());
					skip_whitespace();

					handle_macro_definition();
				}
				else if (const auto if_keyword = *token_map.at(TokenKind::IF); match_keyword(if_keyword))
				{
					advance(if_keyword.length());
					skip_whitespace();

					handle_conditional_compilation();
				}
				else if (const auto elif_keyword = *token_map.at(TokenKind::ELIF); match_keyword(elif_keyword))
				{
					advance(elif_keyword.length());
					skip_whitespace();

					handle_conditional_compilation();
				}
				else if (const auto else_keyword = *token_map.at(TokenKind::ELSE); match_keyword(else_keyword))
				{
					advance(else_keyword.length());
					skip_whitespace();

					handle_conditional_compilation();
				}
				else if (const auto endif_keyword = *token_map.at(TokenKind::ENDIF); match_keyword(endif_keyword))
				{
					advance(endif_keyword.length());
					skip_whitespace();
					handle_conditional_compilation();
				}
				else if (const auto undef_keyword = *token_map.at(TokenKind::UNDEF); match_keyword(undef_keyword))
				{
					advance(undef_keyword.length());
					skip_whitespace();

					const auto name = read_identifier();
					advance(name.length());
					if (!defined_macros.contains(name))
					{
						USE_SAFE(ErrorReporter)->post_error(std::format(
							"reference to undefined macro `{}`", name), forge_token());
						return;
					}

					defined_macros.erase(name);
				}
				else
				{
					USE_SAFE(ErrorReporter)->post_error(
						"invalid preprocessor directive", forge_token());
					skip_until('\n');
				}

			} break;
		}


		if (match_macro_invokation())
		{
			handle_macro_invokation();
			return;
		}

		advance();
	}

	ScannerKind Preprocessor::scanner_kind_implementation() const noexcept
	{
		return ScannerKind::PREPROCESSOR;
	}

	Preprocessor::Preprocessor(const std::filesystem::path& filepath)
		: Scanner{ filepath }
	{
		auto contents = USE_SAFE(FileManager)->get_file(filepath).get_raw_contents();

		// moving the contents is safe because the filemanager returns a copy
		set_state(SourceContext{ .source = std::move(contents), .location = { filepath, 0, 1, 1 } });
		save_state();
	}
}
