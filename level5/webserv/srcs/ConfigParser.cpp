#include "ConfigParser.hpp"
#include "Parser.hpp"
#include "Tokenizer.hpp"

ConfigParser::ConfigParser()
	: config_(NULL),
	  errorMessage_(),
	  errorLine_(0)
{
}

ConfigParser::~ConfigParser()
{
	delete config_;
}

bool ConfigParser::parseFile(const std::string &path)
{
	errorMessage_.clear();
	errorLine_ = 0;

	Tokenizer tok;
	std::vector<ConfigToken> tokens;
	if (!tok.tokenizeFile(path, tokens, errorMessage_, errorLine_))
		return (false);

	Parser parser;
	if (!parser.parseTokens(tokens, errorMessage_, errorLine_))
		return (false);

	delete config_;
	config_ = new Config(parser.getServers());
	return (true);
}

const IConfig &ConfigParser::getConfig() const
{
	return (*config_);
}

const std::string &ConfigParser::getErrorMessage() const
{
	return (errorMessage_);
}

std::size_t ConfigParser::getErrorLine() const
{
	return (errorLine_);
}
