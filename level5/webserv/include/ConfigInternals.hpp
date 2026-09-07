#ifndef CONFIG_INTERNALS_HPP
#define CONFIG_INTERNALS_HPP

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#define DEFAULT_CLIENT_MAX_BODYSIZE (1024 * 1024)

struct LocationConfig
{
	std::string path;

	std::vector<std::string> methods;
	bool hasMethods;

	std::string root;
	bool hasRoot;

	std::string index;
	bool hasIndex;

	bool autoindex;
	bool hasAutoindex;

	std::string uploadStore;
	bool hasUploadStore;

	bool redirectEnabled;
	bool hasRedirect;
	int redirectStatus;
	std::string redirectTarget;

	std::string cgiExtension;
	bool hasCgiExtension;

	std::string cgiPath;
	bool hasCgiPath;

	std::size_t clientMaxBodySize;
	bool hasClientMaxBodySize;

	LocationConfig()
		: path(),
		  methods(),
		  hasMethods(false),
		  root(),
		  hasRoot(false),
		  index(),
		  hasIndex(false),
		  autoindex(false),
		  hasAutoindex(false),
		  uploadStore(),
		  hasUploadStore(false),
		  redirectEnabled(false),
		  hasRedirect(false),
		  redirectStatus(0),
		  redirectTarget(),
		  cgiExtension(),
		  hasCgiExtension(false),
		  cgiPath(),
		  hasCgiPath(false),
		  clientMaxBodySize(DEFAULT_CLIENT_MAX_BODYSIZE),
		  hasClientMaxBodySize(false)
	{
	}
};

struct ServerBlock
{
	std::string host;
	int port;
	bool hasHost;
	bool hasPort;

	std::string root;
	bool hasRoot;

	std::string index;
	bool hasIndex;

	std::size_t clientMaxBodySize;
	bool hasClientMaxBodySize;

	std::map<int, std::string> errorPages;
	bool hasErrorPages;

	std::vector<LocationConfig> locations;

	ServerBlock()
		: host(),
		  port(0),
		  hasHost(false),
		  hasPort(false),
		  root(),
		  hasRoot(false),
		  index(),
		  hasIndex(false),
		  clientMaxBodySize(DEFAULT_CLIENT_MAX_BODYSIZE),
		  hasClientMaxBodySize(false),
		  errorPages(),
		  hasErrorPages(false),
		  locations()
	{
	}
};

#endif
