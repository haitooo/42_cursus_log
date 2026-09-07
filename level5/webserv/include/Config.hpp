#ifndef CONFIG_HPP
#define CONFIG_HPP

#define DEFAULT_CONFIG_PATH "conf/default.conf"

#include <cstddef>
#include <vector>

#include "IConfig.hpp"
#include "ConfigInternals.hpp"

class Config : public IConfig
{
private:
	std::vector<ServerBlock> servers_;
	std::vector<ServerConfig> listenAddresses_;

	static bool isPrefixMatch(const std::string &locPath,
							  const std::string &uriPath);
	static ResolvedLocation toResolved(const LocationConfig &loc);

public:
	explicit Config(const std::vector<ServerBlock> &servers);

	std::size_t serverCount() const;
	const ServerConfig &listenAddress(ServerId id) const;
	std::size_t clientMaxBodySize(ServerId id) const;
	bool resolveLocation(ServerId id,
						 const std::string &uriPath,
						 ResolvedLocation &out) const;
	std::string mapUriToPath(const ResolvedLocation &loc,
							 const std::string &uriPath) const;
	bool errorPagePath(ServerId id,
					   int statusCode,
					   std::string &out) const;
};

#endif
