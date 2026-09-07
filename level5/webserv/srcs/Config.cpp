#include "Config.hpp"

Config::Config(const std::vector<ServerBlock> &servers)
	: servers_(servers),
	  listenAddresses_()
{
	std::size_t i = 0;
	while (i < servers_.size())
	{
		ServerConfig addr;
		addr.host = servers_[i].host;
		addr.port = servers_[i].port;
		listenAddresses_.push_back(addr);
		i++;
	}
}

std::size_t Config::serverCount() const
{
	return (servers_.size());
}

const ServerConfig &Config::listenAddress(ServerId id) const
{
	return (listenAddresses_[id]);
}

std::size_t Config::clientMaxBodySize(ServerId id) const
{
	const ServerBlock	&server = servers_[id];
	std::size_t			largest = server.clientMaxBodySize;
	std::size_t			i = 0;

	while (i < server.locations.size())
	{
		if (server.locations[i].clientMaxBodySize > largest)
			largest = server.locations[i].clientMaxBodySize;
		i++;
	}
	return (largest);
}

// Is locPath a prefix of uriPath? /foo does not match /foobar.
bool Config::isPrefixMatch(const std::string &locPath, const std::string &uriPath)
{
	if (uriPath.size() < locPath.size())
		return (false);
	if (uriPath.compare(0, locPath.size(), locPath) != 0)
		return (false);
	if (uriPath.size() == locPath.size())
		return (true);
	return (locPath[locPath.size() - 1] == '/' || uriPath[locPath.size()] == '/');
}

ResolvedLocation Config::toResolved(const LocationConfig &loc)
{
	ResolvedLocation out;
	out.locationPath = loc.path;
	out.root = loc.root;
	out.index = loc.index;
	out.autoindex = loc.autoindex;
	out.methods = loc.methods;
	out.uploadStore = loc.uploadStore;
	out.clientMaxBodySize = loc.clientMaxBodySize;
	out.redirect.enabled = loc.redirectEnabled;
	out.redirect.statusCode = loc.redirectStatus;
	out.redirect.target = loc.redirectTarget;
	out.cgi.enabled = (loc.hasCgiExtension && loc.hasCgiPath);
	out.cgi.extension = loc.cgiExtension;
	out.cgi.interpreterPath = loc.cgiPath;
	return (out);
}

bool Config::resolveLocation(ServerId id,
							 const std::string &uriPath,
							 ResolvedLocation &out) const
{
	const std::vector<LocationConfig> &locs = servers_[id].locations;
	std::size_t bestLen = 0;
	const LocationConfig *best = NULL;

	std::size_t i = 0;
	while (i < locs.size())
	{
		if (isPrefixMatch(locs[i].path, uriPath) && locs[i].path.size() >= bestLen)
		{
			bestLen = locs[i].path.size();
			best = &locs[i];
		}
		i++;
	}
	if (!best)
		return (false);
	out = toResolved(*best);
	return (true);
}

std::string Config::mapUriToPath(const ResolvedLocation &loc,
								 const std::string &uriPath) const
{
	// Strip the location prefix:
	//   locationPath "/images" + uriPath "/images/cat.jpg" -> "/cat.jpg"
	if (loc.root.empty())
		return (std::string());

	std::string remainder = uriPath.substr(loc.locationPath.size());
	// For location "/" the substr above also eats the leading '/'.
	if (!remainder.empty() && remainder[0] != '/')
		remainder = '/' + remainder;
	// remainder already starts with '/', so drop a trailing one from root to
	// avoid doubling it up.
	std::string root = loc.root;
	if (root.size() > 1 && root[root.size() - 1] == '/')
		root.erase(root.size() - 1);
	//   root "/var/www" + remainder "/cat.jpg" -> "/var/www/cat.jpg"
	return (root + remainder);
}

bool Config::errorPagePath(ServerId id, int statusCode, std::string &out) const
{
	const std::map<int, std::string> &pages = servers_[id].errorPages;
	std::map<int, std::string>::const_iterator it = pages.find(statusCode);
	if (it == pages.end())
		return (false);
	out = it->second;
	return (true);
}
