#include "TokenStream.hpp"

TokenStream::TokenStream(const std::vector<ConfigToken> &tokens)
	: _tokens(tokens),
	  _index(0)
{
}

bool TokenStream::hasCurrent() const
{
	return (_tokens.size() > _index);
}

// Current token, or NULL when the stream is exhausted.
const ConfigToken *TokenStream::current() const
{
	if (hasCurrent())
	{
		return (&_tokens[_index]);
	}
	return (0);
}

// Returns the current token and advances the stream.
const ConfigToken *TokenStream::advance()
{
	if (hasCurrent())
	{
		return (&_tokens[_index++]);
	}
	return (0);
}

std::size_t TokenStream::lastLine() const
{
	if (_tokens.empty())
		return (1);
	return (_tokens.back().line);
}

bool TokenStream::expect(const char *value,
						 std::string &errorMessage,
						 std::size_t &errorLine)
{
	const ConfigToken *t = current();

	if (!t)
	{
		errorMessage = std::string("expected '") + value + "', but reached end of file";
		errorLine = lastLine();
		return (false);
	}

	if (t->value != value)
	{
		errorMessage = std::string("expected '") + value + "', but got '" + t->value + "'";
		errorLine = t->line;
		return (false);
	}

	advance();
	return (true);
}
