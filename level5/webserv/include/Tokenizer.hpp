#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <vector>
#include <cstddef> // For std::size_t

// One token. `line` is 1-based and is used in error messages.
struct ConfigToken
{
	std::string value;
	std::size_t line;

	ConfigToken()
		: value(),
		  line(0)
	{
	}

	ConfigToken(const std::string &v, std::size_t l)
		: value(v),
		  line(l)
	{
	}
};

/*
** Splits a configuration file into tokens.
**
**   server {              [ ConfigToken("server", 1),
**       listen 8080;        ConfigToken("{",      1),
**   }               ->      ConfigToken("listen", 2),
**                           ConfigToken("8080",   2),
**                           ConfigToken(";",      2),
**                           ConfigToken("}",      3) ]
*/
class Tokenizer
{
private:
	// Deliberately not std::isspace: only space, tab, newline and carriage
	// return count as whitespace here.
	static bool isWhitespace(char c);
	static bool isSpecialToken(char c);

public:
	// Opens the file and hands its contents to tokenizeString().
	bool tokenizeFile(const std::string &path,
					  std::vector<ConfigToken> &outTokens,
					  std::string &errorMessage,
					  std::size_t &errorLine) const;

	// Tokenizes raw file contents: skips comments and whitespace, and splits
	// out `{`, `}`, `;` and bare words.
	bool tokenizeString(const std::string &input,
						std::vector<ConfigToken> &outTokens,
						std::string &errorMessage,
						std::size_t &errorLine) const;
};

#endif
