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
	class FileManager;

	enum class ScannerType
	{
		PREPROCESSOR,
		LEXER,
	};

	struct SourceContext
	{
		std::string source;
		SourceLocation location;
	};

	// represents a generic lexical analyzer instance 
	class Scanner : 
		public Inject<FileManager, ErrorReporter>, 
		public UndoableRedoable<SourceContext>
	{
	private:
		SourceContext _current_context;

	protected:
		const SourceContext& get_state(void) const
		{
			return _current_context;
		}
		void set_state(const SourceContext& context)
		{
			_current_context = context;
		}

	private:
		void commit(void(*callable)(SourceContext&))
		{
			if (callable == nullptr)
			{
				USE(ErrorReporter).post_error(std::format(
					"invalid nullish entity tracking instance"), NULL_TOKEN);
				return;
			}

			callable(_current_context);
			set_state(_current_context);
			save_state();
		}

	protected:
		// positional queries
		char current(void) const;
		std::size_t where(void) const;
		bool eof(void) const;

	protected:
		// source modifications
		void insert_adjacent(const std::string&);

	protected:
		// error handling/recovery
		void advance(std::size_t);
		void expect(char);
		void expect(const std::string&);
		bool anticipate(char, bool);
		Token forge_token(void) const;

	protected:
		bool peek_match_identifier(void) const;

	protected:
		void skip_whitespace(bool = false);
		std::string substring_until(char, bool = false);

	public:
		virtual ScannerType stype(void) const noexcept = 0;
		virtual void run(void) noexcept = 0;

	public:
		Scanner(const std::string&);
		virtual ~Scanner() = default;
	};
}

#endif
