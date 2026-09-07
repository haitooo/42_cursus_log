#ifndef CONFIG_TYPES_HPP
# define CONFIG_TYPES_HPP

# include <cstddef>
# include <string>
# include <vector>

/*
** Configuration values shared across modules, as they are once the
** configuration file has been parsed. Tokenizer and parser internals do not
** belong here.
**
** An empty string member means "not set".
*/

/*
** Identifies a server block, numbered 0, 1, 2, ... in the order the blocks
** appear in the configuration file. A listening socket remembers its ServerId
** and hands it to every client accepted on it.
*/
typedef std::size_t	ServerId;

/*
** listen 127.0.0.1:8080;
*/
struct ServerConfig
{
	std::string	host;
	int			port;

	ServerConfig()
		: host(),
		  port(0)
	{
	}
};

/*
** return 301 /new;
*/
struct RedirectRule
{
	bool		enabled;
	int			statusCode;
	std::string	target;

	RedirectRule()
		: enabled(false),
		  statusCode(0),
		  target()
	{
	}
};

/*
** cgi_extension .py;
** cgi_path /usr/bin/python3;
**
** Both directives must be present or both absent; the parser rejects the
** half-configured case, so only `enabled` needs to be checked here.
*/
struct CgiRule
{
	bool		enabled;
	std::string	extension;
	std::string	interpreterPath;

	CgiRule()
		: enabled(false),
		  extension(),
		  interpreterPath()
	{
	}
};

/*
** The configuration that finally applies to one request path.
** Inheritance (server -> location) and longest-prefix location matching are
** already resolved by IConfig::resolveLocation().
*/
struct ResolvedLocation
{
	// Prefix of the matched location, e.g. "/cgi-bin".
	// mapUriToPath() strips it from the URI.
	std::string					locationPath;

	std::string					root;
	std::string					index;
	bool						autoindex;

	// Methods this location accepts: "GET" / "POST" / "DELETE".
	// The Allow header of a 405 is built by joining these.
	std::vector<std::string>	methods;

	std::string					uploadStore;

	// Location-level limit, checked again by the request handler because the
	// location is not known yet while RequestParser reads the body.
	std::size_t					clientMaxBodySize;

	RedirectRule				redirect;
	CgiRule						cgi;

	ResolvedLocation()
		: locationPath(),
		  root(),
		  index(),
		  autoindex(false),
		  methods(),
		  uploadStore(),
		  clientMaxBodySize(0),
		  redirect(),
		  cgi()
	{
	}

	bool	isMethodAllowed(const std::string &method) const
	{
		for (std::size_t i = 0; i < methods.size(); ++i)
		{
			if (methods[i] == method)
				return (true);
		}
		return (false);
	}
};

#endif
