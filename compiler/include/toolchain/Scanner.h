#ifndef HAZE_SCANNER_H
#define HAZE_SCANNER_H

#include <data/DependencyInjector.h>
#include <data/UndoableRedoable.h>
#include <error/ErrorReporter.h>
#include <io/FileManager.h>
#include <symbol/SymbolDatabase.h>
#include <symbol/SymbolExporter.h>
#include <toolchain/models/Token.h>

// Haze Scanner.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	enum struct ScannerKind
	{
		PREPROCESSOR,
		LEXER,
	};

	struct SourceContext
	{
	public:
#pragma message("TODO: determine cheapest ownership model here for source storage")
		std::string source;
		SourceLocation location;

	public:
		std::size_t whereat() const
		{
			return location.position;
		}

		const std::filesystem::path& wherein() const
		{
			return location.filepath;
		}

		bool eof() const
		{
			return whereat() >= source.length();
		}

		char current() const
		{
			if (eof())
			{
				return '\0';
			}

			return source[whereat()];
		}

		char lookahead() const
		{
			if (whereat() + 1 >= source.length())
			{
				return '\0';
			}

			return source[whereat() + 1];
		}
	};

	struct Scanner 
		: public UndoableRedoable<SourceContext>
		, public InjectSingleton<ErrorReporter, FileManager, SymbolDatabase, SymbolExporter>
	{
	private:
		SourceContext current_context;

	public:
		const SourceContext& get_context() const
		{
			return current_context;
		}
		void set_state(const SourceContext& context)
		{
#pragma message("TODO: fix and determine copy versus reference semantics")
			current_context = context;
		}

	protected:
		template<typename T>
		T commit(std::function<T(SourceContext&)> callable)
		{
			callable(current_context);
			set_state(current_context);
			save_state();
		}

	protected:
		// positional queries
		char current() const;
		char lookahead() const;
		std::size_t whereat() const;
		const std::filesystem::path& wherein() const;
		bool eof() const;

	protected:
		// source modifications
		// NOTE: inserts at the current source position
		void insert_adjacent(const std::string&);

	protected:
		// error handling/recovery
		void advance(std::size_t = 1u);
		bool expect(char, bool = true);
		bool anticipate(char, bool = true);
		Token forge_token() const;
		Token forge_token(std::string_view) const;
		Token error_token(std::string_view);

	protected:
		// advance the cursor context?
		std::string_view read_identifier(bool = false);
		bool match_keyword(std::string_view);
		void skip_whitespace(bool = false);
		void skip_until(char);
		void skip_while(bool(*)(char));
		std::string_view substring_until(char, bool = false);

		template<typename FunctorT>
			requires std::predicate<FunctorT, char> && std::convertible_to<std::invoke_result_t<FunctorT, char>, bool>
		std::string_view substring_while(FunctorT&& functor, bool = false)
		{
			const auto source_length = current_context.source.length();
			const auto start = whereat();

			auto position = start;
			while (position < source_length && functor(current_context.source[position]))
			{
				position++;
			}

			const auto length = position - start;
			if (advance_while)
			{
				advance(length);
			}

			return std::string_view{ current_context.source }.substr(start, length);
		}

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
		inline static bool my_isoctal(char c)
		{
			return c >= '0' && c <= '7';
		}
		inline static bool my_isbinary(char c)
		{
			return c == '0' || c == '1';
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
		template<typename Self>
		ScannerKind scanner_kind(this Self&& self)
		{
			return self.scanner_kind_implementation();
		}

	public:
		Scanner(const std::filesystem::path&);
		Scanner(std::string);
		~Scanner();
	};
}

#endif
