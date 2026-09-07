#ifndef TOKEN_STREAM_HPP
#define TOKEN_STREAM_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "Tokenizer.hpp"

class TokenStream
{
private:
	const std::vector<ConfigToken> &_tokens;
	std::size_t _index;

public:
	TokenStream(const std::vector<ConfigToken> &tokens);

	bool hasCurrent() const;
	const ConfigToken *current() const;
	const ConfigToken *advance();
	std::size_t lastLine() const;
	bool expect(const char *value,
				std::string &errorMessage,
				std::size_t &errorLine);
};

#endif
