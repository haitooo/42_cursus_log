#ifndef ICONFIG_HPP
# define ICONFIG_HPP

# include <cstddef>
# include <string>

# include "ConfigTypes.hpp"

/*
** Read-only view of the parsed configuration. Every rule for interpreting the
** configuration file lives behind this interface: longest-prefix location
** matching, server -> location inheritance, and URI-to-filesystem path
** resolution. Callers must not re-implement any of them.
**
** Contract:
** - Read only. The configuration never changes once the server is running.
** - Valid ServerId range is [0, serverCount()). Out of range is undefined
**   behaviour; the caller guarantees it.
** - Nothing here throws. Invalid configurations are already rejected by
**   IConfigParser.
*/
class IConfig
{
public:
	virtual ~IConfig() {}

	// Number of server blocks. Parsing guarantees this is at least 1.
	virtual std::size_t			serverCount() const = 0;

	// host and port to pass to bind(). An empty host means "not set".
	virtual const ServerConfig	&listenAddress(ServerId id) const = 0;

	/*
	** Largest client_max_body_size any location under this server allows,
	** used as the provisional limit while RequestParser reads the body. The
	** location-level limit is checked again later through
	** ResolvedLocation::clientMaxBodySize.
	*/
	virtual std::size_t			clientMaxBodySize(ServerId id) const = 0;

	/*
	** Resolves the configuration that applies to a request URI path: picks the
	** longest matching location prefix, applies server-level inheritance and
	** writes the result to `out`.
	**
	** Returns false when no location matches (treat as 404); `out` is left
	** untouched in that case.
	*/
	virtual bool				resolveLocation(ServerId id,
									const std::string &uriPath,
									ResolvedLocation &out) const = 0;

	/*
	** Translates a URI path into a filesystem path by stripping the matched
	** location prefix and joining the remainder onto root.
	**
	**   location /files { root ./storage; }
	**   request  /files/report.txt   ->   ./storage/report.txt
	**
	** `loc` must be what resolveLocation() returned for the same id and
	** uriPath. Appending index when the result is a directory is up to the
	** caller.
	*/
	virtual std::string			mapUriToPath(const ResolvedLocation &loc,
									const std::string &uriPath) const = 0;

	/*
	** Path of the custom error page for a status code.
	**
	** Returns false when no error_page is configured for that code; the caller
	** then generates a built-in body. True does not guarantee the file is
	** readable -- the caller opens it.
	*/
	virtual bool				errorPagePath(ServerId id,
									int statusCode,
									std::string &out) const = 0;
};

#endif
