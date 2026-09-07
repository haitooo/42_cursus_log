#ifndef ICONFIG_PARSER_HPP
# define ICONFIG_PARSER_HPP

# include <cstddef>
# include <string>

# include "IConfig.hpp"

/*
** Entry point for loading the configuration file. Only main() calls it;
** everything else receives a const IConfig reference, so there is no path
** through which the configuration can change while the server runs.
**
** Invalid configurations are rejected before the server starts listening, and
** reported through return values rather than exceptions.
**
** Usage:
**
**   ConfigParser	parser;
**
**   if (!parser.parseFile(path))
**   {
**       std::cerr << "webserv: " << path << ":"
**                 << parser.getErrorLine() << ": "
**                 << parser.getErrorMessage() << std::endl;
**       return (1);
**   }
**
**   const IConfig	&config = parser.getConfig();
*/
class IConfigParser
{
public:
	virtual ~IConfigParser() {}

	/*
	** Reads and validates the configuration file. On failure returns false and
	** fills getErrorMessage() and getErrorLine().
	**
	** Rejected: missing, unreadable or empty file; no server block; syntax
	** errors (missing `{` `}` `;`, unknown directive); invalid values (port,
	** method, duplicate directive); only one of cgi_extension / cgi_path.
	**
	** See section 8 of docs/CONFIG_SPEC.ja.md for the full list.
	*/
	virtual bool				parseFile(const std::string &path) = 0;

	/*
	** Only valid after parseFile() returned true. The parser owns the returned
	** reference, so it must outlive every user of the configuration.
	*/
	virtual const IConfig		&getConfig() const = 0;

	// Only valid after parseFile() returned false.
	// Human readable message, without the file name or line number.
	virtual const std::string	&getErrorMessage() const = 0;

	// Only valid after parseFile() returned false.
	// 1-based line number, or 0 when no line can be identified.
	virtual std::size_t			getErrorLine() const = 0;
};

#endif
