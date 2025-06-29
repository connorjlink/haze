#ifndef HAZE_SCANNER_H
#define HAZE_SCANNER_H

#include "Token.h"
#include "FileManager.h"
#include "DependencyInjector.h"
#include "UndoableRedoable.h"
#include "ErrorReporter.h"

// Haze Scanner.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum class ScannerType
	{
		PREPROCESSOR,
		LEXER,
	};

	struct SourceContext
	{
	public:
		std::string source;
		SourceLocation location;

	public:
		std::size_t whereat(void) const
		{
			return location.position;
		}

		std::string wherein(void) const
		{
			return location.filepath;
		}

		bool eof(void) const
		{
			return whereat() >= source.length();
		}

		char current(void) const
		{
			if (!eof())
			{
				return source[whereat()];
			}
			return '\0';
		}

		char lookahead(void) const
		{
			if (whereat() + 1 < source.length())
			{
				return source[whereat() + 1];
			}
			return '\0';
		}
	};

	class Scanner : 
		public InjectSingleton<FileManager, ErrorReporter>,
		public UndoableRedoable<SourceContext>
	{
	private:
		SourceContext _current_context;

	public:
		const SourceContext& get_state(void) const
		{
			return _current_context;
		}
		void set_state(const SourceContext& context)
		{
			_current_context = context;
		}

	protected:
		template<typename T>
		T commit(std::function<T(SourceContext&)> callable)
		{
			callable(_current_context);
			set_state(_current_context);
			save_state();
		}

	protected:
		// positional queries
		char current(void) const;
		char lookahead(void) const;
		std::size_t whereat(void) const;
		std::string wherein(void) const;
		bool eof(void) const;

	protected:
		// source modifications
		// NOTE: inserts at the current source position
		void insert_adjacent(const std::string&);

	protected:
		// error handling/recovery
		void advance(std::size_t = 1u);
		bool expect(char, bool = true);
		bool anticipate(char, bool = true);
		Token forge_token(void) const;
		Token forge_token(const std::string&) const;
		Token error_token(const std::string&);

	protected:
		// advance the cursor context?
		std::string read_identifier(bool = false);
		bool match_keyword(std::string_view);
		void skip_whitespace(bool = false);
		void skip_until(char);
		void skip_while(bool(*)(char));
		std::string substring_until(char, bool = false);
		std::string substring_while(bool(*)(char), bool = false);

	protected:
		inline static bool my_isidentifierfirst(char c)
		{
			return std::isalpha(c) || c == '_';
		}
		inline static bool my_isidentifier(char c)
		{
			return std::isalnum(c) || c == '_';
		}
		inline static bool my_isspace(char c)
		{
			return !!std::isspace(static_cast<int>(c));
		}
		inline static bool my_isdigit(char c)
		{
			return !!std::isdigit(static_cast<int>(c));
		}
		inline static bool my_isalpha(char c)
		{
			return !!std::isalpha(static_cast<int>(c));
		}
		inline static bool my_isalnum(char c)
		{
			return !!std::isalnum(static_cast<int>(c));
		}
		inline static bool my_ishex(char c)
		{
			return !!std::isdigit(static_cast<int>(c)) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
		}
		
	public:
		virtual ScannerType stype(void) const noexcept = 0;

	public:
		Scanner(const std::string&);
		virtual ~Scanner();
	};
}

#endif
