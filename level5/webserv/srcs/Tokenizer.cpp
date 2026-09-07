#include "Tokenizer.hpp"

#include <fstream>

bool Tokenizer::isWhitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

bool Tokenizer::isSpecialToken(char c)
{
	return (c == '{' || c == '}' || c == ';');
}

bool Tokenizer::tokenizeFile(const std::string &path,
							 std::vector<ConfigToken> &outTokens,
							 std::string &errorMessage,
							 std::size_t &errorLine) const
{
	std::string content;
	std::string line;

	outTokens.clear();
	errorMessage.clear();
	errorLine = 0;

	std::ifstream ifs(path.c_str());

	if (!ifs.is_open())
	{
		errorMessage = "failed to open file"; // std::strerror(errno) would say why
		return (false);
	}
	while (std::getline(ifs, line))
	{
		content += line;
		content += '\n'; // getline() strips the newline, so put it back
	}
	if (ifs.bad())
	{
		errorMessage = "I/O error while reading file";
		return false;
	}
	if (content.empty())
	{
		errorMessage = "configuration file is empty";
		return (false);
	}
	return (tokenizeString(content, outTokens, errorMessage, errorLine));
}

bool Tokenizer::tokenizeString(const std::string &input,
							   std::vector<ConfigToken> &outTokens,
							   std::string &errorMessage,
							   std::size_t &errorLine) const
{
	std::size_t i = 0;
	std::size_t line = 1;

	outTokens.clear();
	errorMessage.clear();
	errorLine = 0;

	while (i < input.size())
	{
		const char c = input[i];

		// '#' starts a comment: skip to the end of the line.
		if (c == '#')
		{
			while (i < input.size() && input[i] != '\n')
				++i;
			continue;
		}

		if (isWhitespace(c))
		{
			if (c == '\n')
				++line;
			++i;
			continue;
		}

		if (isSpecialToken(c))
		{
			outTokens.push_back(ConfigToken(std::string(1, c), line));
			++i;
			continue;
		}

		// A bare word such as "server" or "listen".
		{
			const std::size_t tokenLine = line;
			std::string token;

			while (i < input.size() && !isWhitespace(input[i]) && !isSpecialToken(input[i]) && input[i] != '#')
			{
				token += input[i];
				++i;
			}
			outTokens.push_back(ConfigToken(token, tokenLine));
		}
	}
	if (outTokens.empty())
	{
		errorMessage = "configuration file contains no directives";
		errorLine = 1;
		return (false);
	}
	return (true);
}
