import std;

#include "Preprocessor.h"
#include "ErrorReporter.h"

// Haze Preprocessor.cpp
// (c) Connor J. Link. All Rights Reserved.

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

namespace
{
	static constexpr std::string MACRO_KEYWORD = ".macro";
	static constexpr std::string INCLUDE_KEYWORD = ".include";
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

			if (std::find_if(defined_macros.begin(), defined_macros.end(), [&](auto m)
				{
					return (m.name == macro.name);
				}) != std::end(defined_macros))
			{
				_error_reporter->post_error(std::format("invalid duplicate definition of macro {}", macro.name), NULL_TOKEN);
				break;
			}

			defined_macros.emplace_back(macro);

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

			if (std::find_if(defined_macros.begin(), defined_macros.end(), [&](auto m)
				{
					if (m.name == macro.name)
					{
						defined_macro = m;
						return true;
					}

					return false;
				}) == defined_macros.end())
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

	std::string Preprocessor::read_identifier(IncludeContext& context)
	{
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

		auto& position = context.location.position;
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
		return content.substr(start, end - start);
	}

	Token Preprocessor::forge(const IncludeContext& context) const
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

	bool Preprocessor::expect(IncludeContext& context, char expected)
	{
		if (auto c = context.current(); c != expected)
		{
			_error_reporter->post_error(std::format(
				"expected token `{}` to specify an include filepath but got `{}`", expected, c), NULL_TOKEN);
			return false;
		}
		// if the character matches, consume it
		advance(context, 1);
		return true;
	}

	bool Preprocessor::anticipate(IncludeContext& context, char expected)
	{
		if (context.current() == expected)
		{
			advance(context, 1);
			return true;
		}
		return false;
	}

	void Preprocessor::skip_whitespace(IncludeContext& context, bool require)
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

	bool Preprocessor::match_keyword(IncludeContext& context, const std::string& keyword)
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

	void Preprocessor::advance(IncludeContext& context, int howMany)
	{
		for (int i = 0; i < howMany; ++i)
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
		std::ifstream file(filepath);
		if (!file.good())
		{
			_error_reporter->post_error(std::format(
				"could not open file `{}`", filepath), forge({ "", { filepath, 0, 1, 1 } }));
			return;
		}

		const auto size = std::filesystem::file_size(filepath);

		// reserve capacity constructor
		std::string content('\0', size);
		content.assign(std::istreambuf_iterator<char>{ file }, 
					   std::istreambuf_iterator<char>{});

		context_stack.push_back({ content, { filepath, 0, 1, 1 } });
	}

	void Preprocessor::preprocess(const std::string& initial_file)
	{
		load_file(initial_file);

		while (!context_stack.empty())
		{
			auto& context = context_stack.back();

			// only pop once advanced past the last virtual location of the file in question
			if (context.position >= context.content.size())
			{
				context_stack.pop_back();
				continue;
			}

			char c = context.content[context.position];

			if (c == '.')
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
					append_char(c);
					advance(context, 1);
				}
			}
			else if (c == '$')
			{
				if (peek_macro_invocation(context))
				{
					handle_macro_invocation(context);
				}
				else
				{
					// unrecognized, pass through
					append_char(c);
					advance(context, 1);
				}
			}
			else
			{
				// unrecognized, pass through
				append_char(c);
				advance(context, 1);
			}
		}
	}

	void Preprocessor::handle_include(IncludeContext& context)
	{
		// consume keyword ".include" and any trailing whitespace
		advance(context, INCLUDE_KEYWORD.length());
		skip_whitespace(context);

		expect(context, '"');

		// scan until reaching the closing quote to substring only once
		std::string include_filepath;
		const auto start = context.location.position;
		while (context.current() != '"' && context.location.position < context.content.length())
		{
			++context.location.position;
		}
		include_filepath = context.content.substr(start, context.location.position - start);

		expect(context, '"');
		
		if (include_filepath == context.location.filepath)
		{
			_error_reporter->post_error(std::format(
				"invalid recursive include of file `{}`", include_filepath), NULL_TOKEN));
			return;
		}

		load_file(include_filepath);
	}

	void Preprocessor::handle_macro_definition(IncludeContext& context)
	{
		// consume keyword ".macro" and any trailing whitespace
		advance(context, MACRO_KEYWORD.length());
		skip_whitespace(context);

		const auto name = read_identifier(context);
		skip_whitespace(context);

		expect(context, '=');
		skip_whitespace(context);

		expect(context, '(');

		// greedily consume until a closing parenthesis
		std::vector<std::string> arguments{};
		while (context.current() != ')')
		{
			arguments.push_back(read_identifier(context));
			anticipate(context, ',');
			skip_whitespace(context);
		}

		context.position++; // skip ')'
		skip_whitespace(context);

		if (context.content[context.position] != ':') return;
		context.position++;
		skip_whitespace(context);

		if (context.content[context.position] != '{') return;
		context.position++;

		std::string body;
		while (context.content[context.position] != '}' && context.position < context.content.size()) {
			body += context.content[context.position++];
		}

		context.position++; // skip '}'

		Macro macro{ name, args, body, { context.file_path, context.line, context.column } };
		defined_macros.push_back(macro);

		// call stub you can integrate with your symbol DB
		register_macro_definition(macro);
	}

	void Preprocessor::handle_macro_invokation(IncludeContext& ctx)
	{
		size_t start = ctx.position;
		ctx.position++; // skip '$'
		std::string name = read_identifier(ctx);

		if (ctx.content[ctx.position] != '(') return;
		ctx.position++;

		std::vector<std::string> args;
		while (ctx.content[ctx.position] != ')') {
			args.push_back(read_argument(ctx));
			if (ctx.content[ctx.position] == ',') ctx.position++;
		}

		ctx.position++; // skip ')'

		// Lookup macro
		auto it = std::find_if(defined_macros.begin(), defined_macros.end(), [&](const Macro& m) {
			return m.name == name;
			});

		if (it == defined_macros.end()) {
			_error_reporter->post_error("macro `" + name + "` is undefined", NULL_TOKEN);
			return;
		}

		if (it->args.size() != args.size()) {
			_error_reporter->post_error("macro argument count mismatch", NULL_TOKEN);
			return;
		}

		std::string expanded = it->body;
		for (size_t i = 0; i < args.size(); ++i) {
			replace(expanded, "[" + it->args[i] + "]", args[i]);
		}

		code.insert(ctx.position, expanded); // insert macro code at position
		register_macro_invocation(name, ctx.file_path, ctx.line, ctx.column);
	}
}
