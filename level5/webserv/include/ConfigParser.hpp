#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <cstddef>
#include <string>

#include "IConfigParser.hpp"
#include "Config.hpp"

class ConfigParser : public IConfigParser
{
private:
	Config *config_;
	std::string errorMessage_;
	std::size_t errorLine_;

	ConfigParser(const ConfigParser &);
	ConfigParser &operator=(const ConfigParser &);

public:
	ConfigParser();
	~ConfigParser();

	bool parseFile(const std::string &path);
	const IConfig &getConfig() const;
	const std::string &getErrorMessage() const;
	std::size_t getErrorLine() const;
};

#endif
