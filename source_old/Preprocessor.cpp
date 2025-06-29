import std;

#include "Preprocessor.h"
#include "ErrorReporter.h"
#include "Symbol.h"
#include "SymbolDatabase.h"
#include "SymbolExporter.h"
#include "FileManager.h"

// Haze Preprocessor.cpp
// (c) Connor J. Link. All Rights Reserved.

// for _old functions
namespace
{
	std::vector<std::string> split(const std::string& text, char delimiter)
	{
		std::vector<std::string> result{};

		std::stringstream input{ text };

		std::string segment;
		while (std::getline(input, segment, delimiter))
		{
			result.emplace_back(segment);
		}

		return result;
	}

	void replace(std::string& str, const std::string& from, const std::string& to)
	{
		if (from.empty())
		{
			return;
		}

		std::size_t start_pos = 0;

		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}

	static constexpr auto ws = " \t\n\r";

	bool isspace(char character)
	{
		return std::isspace(static_cast<int>(character));
	}

	// Thanks https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring :)
	// trim from end of string (right)
	std::string& rtrim(std::string& s, const char* t = ws)
	{
		s.erase(s.find_last_not_of(t) + 1);
		return s;
	}

	// trim from beginning of string (left)
	std::string& ltrim(std::string& s, const char* t = ws)
	{
		s.erase(0, s.find_first_not_of(t));
		return s;
	}

	// trim from both ends of string (right then left)
	std::string& trim(std::string& s, const char* t = ws)
	{
		return ltrim(rtrim(s, t), t);
	}
}

// for the new implementation only
namespace
{
	static const std::string MACRO_KEYWORD = ".macro";
	static const std::string INCLUDE_KEYWORD = ".include";

	auto is_identifier_first = [&](char c)
	{
		return (c >= 'a' && c <= 'z') ||
			   (c >= 'A' && c <= 'Z') ||
			   (c == '_');
	};

	auto is_identifier = [&](char c)
	{
		return is_identifier_first(c) || (c >= '0' && c <= '9');
	};
}

namespace hz
{
	void Preprocessor::preprocess_include_old()
	{
		std::regex rx_incl(R"(^(\.include \"(.*)\"\s*((;.*$)?|$)))", std::regex::optimize);
		std::smatch matches;

		while (std::regex_search(code, matches, rx_incl))
		{
			std::string filepath = matches[2];

			if (filepath == parent_filepath)
			{
				_error_reporter->post_error(std::format("invalid recursive include of `{}`", filepath), NULL_TOKEN);
				break;
			}

			std::ifstream file(filepath);

			if (!file.good())
			{
				_error_reporter->post_error(std::format("could not open file `{}` for reading", filepath), NULL_TOKEN);
				break;
			}

			const auto size = std::filesystem::file_size(filepath); //bytes
			std::string buffer(size, '\0');
			file.read(buffer.data(), static_cast<std::streamsize>(size));

			replace(code, matches[1], buffer);
		}
	}

	void Preprocessor::preprocess_macro_definition_old()
	{
		std::regex rx_mcro(R"((\.macro ([a-zA-Z_][a-zA-Z0-0_]*) = \((([a-zA-Z])(, ?[a-zA-Z])*)?\)\:\s*\{([^\}]*)\}))", std::regex::optimize);
		std::smatch matches;

		while (std::regex_search(code, matches, rx_mcro))
		{
			std::string name = matches[2];
			std::string args = matches[3];

			args.erase(std::remove_if(args.begin(), args.end(), ::isspace), args.end());
			const auto args_delim = split(args, ',');

			std::string mcode = matches[6];
			trim(mcode);

			Macro macro{ name, args_delim, mcode };

			if (defined_macros.contains(macro.name))
			{
				_error_reporter->post_error(std::format("invalid duplicate definition of macro {}", macro.name), NULL_TOKEN);
				break;
			}

			defined_macros.emplace(name, macro);

			replace(code, matches[1], "");
		}
	}

	void Preprocessor::preprocess_macro_invokation_old()
	{
		std::regex rx_invk(R"((\$([a-zA-Z_][a-zA-Z0-9_]*)\(([^\,\)]*\s*(\,[^\,\)]*)*)?\)))", std::regex::optimize);
		std::smatch matches;

		while (std::regex_search(code, matches, rx_invk))
		{
			std::string name = matches[2];
			std::string args = matches[3];

			args.erase(std::remove_if(args.begin(), args.end(), ::isspace), args.end());
			const auto args_delim = split(args, ',');

			Macro macro{ name, args_delim, "" }, defined_macro;

			if (!defined_macros.contains(macro.name))
			{
				_error_reporter->post_error(std::format("macro `{}` is undefined", macro.name), NULL_TOKEN);
				break;
			}

			if (defined_macro.args.size() != args_delim.size())
			{
				_error_reporter->post_error(std::format("macro `{}` was defined with {} arguments but called with {}", 
					defined_macro.name, args_delim.size(), defined_macro.args.size()), NULL_TOKEN);
				break;
			}

			if (!args_delim.empty())
			{
				std::string copy = defined_macro.code;

				for (auto i = 0; i < args_delim.size(); ++i)
				{
					replace(copy, std::string("[") + defined_macro.args[i] + std::string("]"), args_delim[i]);
				}

				replace(code, matches[1], copy);
			}
		}
	}

	std::string Preprocessor::preprocess_old()
	{
		preprocess_include_old();
		preprocess_macro_definition_old();
		preprocess_macro_invokation_old();
		return code;
	}


	//---------------------------------------------------------------------------

	void Preprocessor::register_macro_definition(const Macro& macro)
	{
		// only specifies the original definition, so invokations have to manually export each time
		const auto symbol = _database->add_define(macro.name, macro.token);
	}

	void Preprocessor::register_macro_invokation(Context& context, const std::string& name)
	{
		const auto forged = forge(context);
		// explicitly mark as visited
		const auto symbol = _database->reference_symbol(SymbolType::DEFINE, name, forged, true);
		_exporter->enqueue(symbol, forged);
	}

	std::string Preprocessor::read_identifier(Context& context, bool advance_context)
	{
		auto position = context.location.position;
		auto& content = context.content;

		auto start = position;

		// special case for the first character
		if (position >= content.size() || !is_identifier_first(content[position]))
		{
			return "";
		}

		position++;
		while (position < content.size() && is_identifier(content[position]))
		{
			position++;
		}

		auto end = position;

		const auto length = end - start;

		if (advance_context)
		{
			advance(context, length);
		}

		return content.substr(start, length);
	}

	std::string Preprocessor::substring_until(Context& context, char c)
	{
		const auto start = context.location.position;
		
		while (context.current() != '"' && context.location.position < context.content.length())
		{
			context.location.position++;
		}

		const auto string = context.content.substr(start, context.location.position - start);
		return string;
	}

	Token Preprocessor::forge(const Context& context) const
	{
		auto current = std::string{ context.current() };

		// default to identifier unless the contex holds a valid context for which to search
		auto type = TokenType::IDENTIFIER;
		if (_token_map.contains(current))
		{
			// has_value() strengthened
			type = _token_map.at(current).value();
		}

		return Token
		{
			.type = type,
			.text = current,
			.location = context.location,
		};
	}

	bool Preprocessor::expect(Context& context, char expected, bool consume_whitespace)
	{
		if (auto c = context.current(); c != expected)
		{
			_error_reporter->post_error(std::format(
				"expected token `{}` to specify an include filepath but got `{}`", expected, c), NULL_TOKEN);
			return false;
		}

		// if the character matches, consume it
		advance(context, 1);

		if (consume_whitespace)
		{
			// try consume as requested
			skip_whitespace(context, false);
		}
		return true;
	}

	bool Preprocessor::anticipate(Context& context, char expected, bool consume_whitespace)
	{
		auto try_consume = [&]
		{
			if (consume_whitespace)
			{
				// try consume as requested
				skip_whitespace(context, false);
			}
		};

		if (context.current() == expected)
		{
			advance(context, 1);
			try_consume();
			return true;
		}

		try_consume();
		return false;
	}

	void Preprocessor::skip_whitespace(Context& context, bool require)
	{
		int number_to_skip = 0;
		for (auto c = context.content[context.location.position]; std::isspace(c); number_to_skip++)
		{
			// this is a weird for... loop :D
		}

		if (require && number_to_skip == 0)
		{
			_error_reporter->post_error(std::format(
				"expectd one or more whitespace tokens but got `{}`", context.current()), NULL_TOKEN);
		}

		advance(context, number_to_skip);
	}

	bool Preprocessor::match_keyword(Context& context, const std::string& keyword)
	{
		const auto length = keyword.length();

		// match the same number of characters and ensure the current string piece is a valid token (delimited correctly)
		if (context.content.compare(context.location.position, length, keyword) == 0 &&
			(std::isspace(context.content[context.location.position + length]) || context.content[context.location.position + length] == '"')) 
		{
			return true;
		}
		return false;
	}

	bool Preprocessor::match_macro_invokation(Context& context)
	{
		// will return empty if there is not a valid macro invokation
		return read_identifier(context, false) != "";
	}

	void Preprocessor::advance(Context& context, std::size_t how_many)
	{
		for (int i = 0u; i < how_many; ++i)
		{
			// NOTE: this will not correctly handle Windows \r\n
			if (context.content[context.location.position] == '\n')
			{
				context.location.line++;
				context.location.column = 1;
			}
			else
			{
				context.location.column++;
			}
			// while line and column updates vary by input, the running index always increments
			context.location.position++;
		}
	}

	void Preprocessor::load_file(const std::string& filepath)
	{
		std::ifstream file(filepath, std::ios::binary);
		if (!file)
		{
			_error_reporter->post_error(std::format(
				"could not open file `{}`", filepath), forge({ "", { filepath, 0, 1, 1 } }));
			return;
		}

		const auto size = std::filesystem::file_size(filepath);

		// is this any faster than string('\0', size)?
		std::string content{};
		content.resize(size);

		file.read(content.data(), size);
		if (!file)
		{
			_error_reporter->post_error(std::format(
				"error reading file `{}`", filepath), forge({ "", { filepath, 0, 1, 1 } }));
			return;
		}

		_raw_file_cache.emplace(filepath, content);
		_files_to_process.emplace(Context{ content, { filepath, 0, 1, 1 } });
	}

	void Preprocessor::preprocess(const std::string& initial_file)
	{
		load_file(initial_file);

		while (!_files_to_process.empty())
		{
			auto& context = _files_to_process.top();

			// only pop once advanced past the last virtual location of the file in question
			if (context.eof())
			{
				// about to dispose context, so safe to move from here
				_processed_file_cache.emplace(initial_file, std::move(context.content));
				_files_to_process.pop();
				continue;
			}

			char c = context.current();
			switch (auto c = context.current())
			{
				case '.':
				{
					if (match_keyword(context, INCLUDE_KEYWORD))
					{
						handle_include(context);
					}
					else if (match_keyword(context, MACRO_KEYWORD))
					{
						handle_macro_definition(context);
					}
					else
					{
						// unrecognized, pass through
						advance(context);
					}
				} break;

				case '$':
				{
					if (match_macro_invokation(context))
					{
						handle_macro_invokation(context);
					}
					else
					{
						// unrecognized, pass through
						advance(context);
					}
				} break;

				default:
				{
					// unrecognized, pass through
					advance(context);
				} break;
			}
		}
	}

	const std::string& Preprocessor::get_preprocessed_source(const std::string& filepath)
	{
		if (!_processed_file_cache.contains(filepath))
		{
			preprocess(filepath);
		}

		if (!_processed_file_cache.contains(filepath))
		{
			// using NULL_TOKEN is okay since the context is probably not available if the operation failed
			_error_reporter->post_error(std::format(
				"failed to lazy-load preprocess source file `{}`", filepath), NULL_TOKEN);
			return "<error>";
		}

		// strengthened contains
		return _processed_file_cache.at(filepath);
	}

	void Preprocessor::handle_include(Context& context)
	{
		// consume keyword ".include" and any trailing whitespace
		advance(context, INCLUDE_KEYWORD.length());
		skip_whitespace(context);

		expect(context, '"');

		// e.g., test_file.hz
		const auto include_filepath = substring_until(context, '"');
		expect(context, '"');
		
		if (include_filepath == context.location.filepath)
		{
			_error_reporter->post_error(std::format(
				"invalid recursive include of file `{}`", include_filepath), forge(context));
			return;
		}

		load_file(include_filepath);
	}

	void Preprocessor::handle_macro_definition(Context& context)
	{
		// consume keyword ".macro" and any trailing whitespace
		advance(context, MACRO_KEYWORD.length());
		skip_whitespace(context);

		const auto name = read_identifier(context);
		advance(context, name.length());
		skip_whitespace(context);
		
		expect(context, '=');
		expect(context, '(');

		// greedily consume until a closing parenthesis
		std::vector<std::string> arguments{};
		while (context.current() != ')')
		{
			const auto argument = read_identifier(context);
			advance(context, argument.length());
			arguments.push_back(argument);
			anticipate(context, ',');
		}

		expect(context, ')');
		expect(context, ':');

		expect(context, '{');

		const auto body = substring_until(context, '}');
		expect(context, '}');

		Macro macro{ name, arguments, body, forge(context) };
		defined_macros.try_emplace(name, macro);

		// call stub you can integrate with your symbol DB
		register_macro_definition(macro);
	}

	void Preprocessor::handle_macro_invokation(Context& context)
	{
		const auto start = context.where();
		expect(context, '$');

		const auto name = read_identifier(context);

		expect(context, '(');

		// greedily consume until a closing parenthesis
		std::vector<std::string> arguments{};
		while (context.current() != ')')
		{
			const auto argument = read_identifier(context);
			arguments.push_back(argument);
			anticipate(context, ',');
		}

		expect(context, ')');

		// explicitly forbid any macro recursion at all. if this macro was called anywhere in the current
		// macro callstack, error out! this is the easiest way to prevent infinite recursion
		if (_macro_invokations.contains(name))
		{
			_error_reporter->post_error(std::format("invalid recursive macro `{}`", name), forge(context));
			return;
		}

		if (!defined_macros.contains(name))
		{
			_error_reporter->post_error(std::format("macro `{}` is undefined", name), forge(context));
			return;
		}

		const auto& macro = defined_macros.at(name);
		if (macro.args.size() != arguments.size())
		{
			_error_reporter->post_error("macro argument count mismatch", forge(context));
			return;
		}

		if (macro.args.size() != arguments.size())
		{
			_error_reporter->post_error(std::format("macro `{}` was defined with {} arguments but called with {}",
				macro.name, macro.args.size(), arguments.size()), forge(context));
			return;
		}

		_macro_invokations.emplace(name);

		// intentional copy
		auto expanded = macro.code;
		for (auto i = 0; i < arguments.size(); i++)
		{
			// if this substitution logic ever becomes a bottleneck, it might be wise to roll a custom re-scanner
			replace(expanded, std::format("[{}]", macro.args[i]), arguments[i]);
		}

		context.insert_inline(expanded);
		register_macro_invokation(context, name);

		_macro_invokations.erase(name);

		// NOTE: explicitly not advancing past the test substition because the new text might contains macros that need to again expand
		//advance(context, expanded.length());
	}
}
