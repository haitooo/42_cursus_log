#include "Parser.hpp"
#include "TokenStream.hpp"

#include <cctype>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <cerrno>

bool Parser::isAllDigits(const std::string &s)
{
	if (s.empty())
		return (false);
	std::size_t i = 0;
	while (i < s.size())
	{
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return (false);
		i++;
	}
	return (true);
}

bool Parser::isSpecialToken(const std::string &s)
{
	return (s == "{" || s == "}" || s == ";");
}

bool Parser::isIntegerInRange(const std::string &s, long min, long max)
{
	if (!isAllDigits(s))
		return (false);
	if (s.size() > 1 && s[0] == '0')
		return (false);
	errno = 0;
	const long value = std::strtol(s.c_str(), 0, 10);
	if (errno == ERANGE)
		return (false);
	return (value >= min && value <= max);
}

bool Parser::isValidPortToken(const std::string &s)
{
	if (!isAllDigits(s))
		return (false);
	if (s.size() > 1 && s[0] == '0')
		return (false);
	errno = 0;
	const long port = std::strtol(s.c_str(), 0, 10);
	if (errno == ERANGE)
		return (false);
	return (port >= 1 && port <= 65535);
}

std::vector<std::string> Parser::split(const std::string &s, char delimiter)
{
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delimiter))
		result.push_back(item);
	return (result);
}

bool Parser::isValidIpv4Token(const std::string &s)
{
	// Reject a leading or trailing '.', e.g. .172.16.0.0 or 172.16.0.0.
	if (s.empty() || s[0] == '.' || s[s.size() - 1] == '.')
		return (false);
	const std::vector<std::string> octets = split(s, '.');
	if (octets.size() != 4)
		return (false);
	std::size_t i = 0;
	while (i < octets.size())
	{
		if (!isAllDigits(octets[i]))
			return (false);
		// Leading zeros are invalid.
		if (octets[i].size() > 1 && octets[i][0] == '0')
			return (false);
		errno = 0;
		const long value = std::strtol(octets[i].c_str(), 0, 10);
		if (errno == ERANGE)
			return (false);
		if (value < 0 || value > 255)
			return (false);
		++i;
	}
	return (true);
}

bool Parser::isValidListenAddressToken(const std::string &s)
{
	const std::size_t colonPos = s.find(':');
	// Port only.
	if (colonPos == std::string::npos)
		return (isValidPortToken(s));
	const std::string host = s.substr(0, colonPos);
	const std::string port = s.substr(colonPos + 1);
	if (host.empty() || port.empty())
		return (false);
	// host:port.
	return (isValidIpv4Token(host) && isValidPortToken(port));
}

bool Parser::parseListenDirective(TokenStream &ts,
								  ServerBlock &server,
								  std::string &errorMessage,
								  std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *address = ts.advance();
	if (!address)
	{
		errorMessage = "missing argument for directive 'listen'";
		errorLine = directive->line;
		return (false);
	}
	if (!isValidListenAddressToken(address->value))
	{
		errorMessage = std::string("invalid listen address: '") + address->value + "'";
		errorLine = address->line;
		return (false);
	}
	const std::size_t colonPos = address->value.find(':');
	if (colonPos == std::string::npos)
	{
		server.port = static_cast<int>(std::strtol(address->value.c_str(), 0, 10));
		server.hasPort = true;
	}
	else
	{
		server.host = address->value.substr(0, colonPos);
		server.hasHost = true;
		server.port = static_cast<int>(std::strtol(address->value.substr(colonPos + 1).c_str(), 0, 10));
		server.hasPort = true;
	}
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseRootDirective(TokenStream &ts,
								std::string &out,
								std::string &errorMessage,
								std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *path = ts.advance();
	if (!path)
	{
		errorMessage = "missing argument for directive 'root'";
		errorLine = directive->line;
		return (false);
	}
	out = path->value;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseIndexDirective(TokenStream &ts,
								 std::string &out,
								 std::string &errorMessage,
								 std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *filename = ts.advance();
	if (!filename)
	{
		errorMessage = "missing argument for directive 'index'";
		errorLine = directive->line;
		return (false);
	}
	out = filename->value;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseClientMaxBodySizeDirective(TokenStream &ts,
											 std::size_t &out,
											 std::string &errorMessage,
											 std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *size = ts.advance();
	if (!size)
	{
		errorMessage = "missing argument for directive 'client_max_body_size'";
		errorLine = directive->line;
		return (false);
	}
	// Any number is allowed as long as it has no leading zero.
	if (!isAllDigits(size->value) || (size->value.size() > 1 && size->value[0] == '0'))
	{
		errorMessage = std::string("invalid client_max_body_size: '") + size->value + "'";
		errorLine = size->line;
		return (false);
	}
	out = static_cast<std::size_t>(std::strtoul(size->value.c_str(), 0, 10));
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseErrorPageDirective(TokenStream &ts,
									 ServerBlock &server,
									 std::string &errorMessage,
									 std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *status = ts.advance();
	if (!status)
	{
		errorMessage = "missing argument for directive 'error_page'";
		errorLine = directive->line;
		return (false);
	}
	if (!isIntegerInRange(status->value, 400, 599))
	{
		errorMessage = std::string("invalid error status code: '") + status->value + "'";
		errorLine = status->line;
		return (false);
	}
	const int statusCode = std::atoi(status->value.c_str());
	if (server.errorPages.count(statusCode))
	{
		errorMessage = std::string("duplicate error_page status code: '") + status->value + "'";
		errorLine = status->line;
		return (false);
	}
	const ConfigToken *path = ts.advance();
	if (!path || isSpecialToken(path->value))
	{
		errorMessage = "missing argument for directive 'error_page'";
		errorLine = status->line;
		return (false);
	}
	server.errorPages[statusCode] = path->value;
	server.hasErrorPages = true;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseMethodsDirective(TokenStream &ts,
								   LocationConfig &location,
								   std::string &errorMessage,
								   std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	bool hasAny = false;
	while (ts.hasCurrent())
	{
		const ConfigToken *t = ts.current();
		if (t->value == ";")
		{
			ts.advance();
			if (!hasAny)
			{
				errorMessage = "directive 'methods' expects at least 1 argument";
				errorLine = directive->line;
				return (false);
			}
			location.hasMethods = true;
			return (true);
		}
		const std::string &method = t->value;
		if (method != "GET" && method != "POST" && method != "DELETE"
			&& method != "HEAD")
		{
			errorMessage = std::string("invalid HTTP method: '") + method + "'";
			errorLine = t->line;
			return (false);
		}
		std::size_t i = 0;
		while (i < location.methods.size())
		{
			if (location.methods[i] == method)
			{
				errorMessage = std::string("duplicate HTTP method: '") + method + "'";
				errorLine = t->line;
				return (false);
			}
			i++;
		}
		location.methods.push_back(method);
		hasAny = true;
		ts.advance();
	}
	errorMessage = "missing ';' after directive 'methods'";
	errorLine = directive->line;
	return (false);
}

bool Parser::parseAutoindexDirective(TokenStream &ts,
									 LocationConfig &location,
									 std::string &errorMessage,
									 std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *value = ts.advance();
	if (!value)
	{
		errorMessage = "missing argument for directive 'autoindex'";
		errorLine = directive->line;
		return (false);
	}
	if (value->value != "on" && value->value != "off")
	{
		errorMessage = std::string("invalid autoindex value: '") + value->value + "'";
		errorLine = value->line;
		return (false);
	}
	location.autoindex = (value->value == "on");
	location.hasAutoindex = true;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseUploadStoreDirective(TokenStream &ts,
									   LocationConfig &location,
									   std::string &errorMessage,
									   std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *path = ts.advance();
	if (!path)
	{
		errorMessage = "missing argument for directive 'upload_store'";
		errorLine = directive->line;
		return (false);
	}
	location.uploadStore = path->value;
	location.hasUploadStore = true;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseReturnDirective(TokenStream &ts,
								  LocationConfig &location,
								  std::string &errorMessage,
								  std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *status = ts.advance();
	if (!status)
	{
		errorMessage = "missing argument for directive 'return'";
		errorLine = directive->line;
		return (false);
	}
	// Only 301, 302, 307 and 308 are accepted as redirect status codes.
	if (status->value != "301" && status->value != "302" && status->value != "307" && status->value != "308")
	{
		errorMessage = std::string("invalid return status: '") + status->value + "'";
		errorLine = status->line;
		return (false);
	}
	const ConfigToken *target = ts.advance();
	if (!target || isSpecialToken(target->value))
	{
		errorMessage = "missing argument for directive 'return'";
		errorLine = status->line;
		return (false);
	}
	location.redirectEnabled = true;
	location.hasRedirect = true;
	location.redirectStatus = std::atoi(status->value.c_str());
	location.redirectTarget = target->value;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseCgiExtensionDirective(TokenStream &ts,
										LocationConfig &location,
										std::string &errorMessage,
										std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *extension = ts.advance();
	if (!extension)
	{
		errorMessage = "missing argument for directive 'cgi_extension'";
		errorLine = directive->line;
		return (false);
	}
	if (extension->value[0] != '.')
	{
		errorMessage = std::string("extension must start with '.': ") + extension->value;
		errorLine = directive->line;
		return (false);
	}
	location.cgiExtension = extension->value;
	location.hasCgiExtension = true;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseCgiPathDirective(TokenStream &ts,
								   LocationConfig &location,
								   std::string &errorMessage,
								   std::size_t &errorLine)
{
	const ConfigToken *directive = ts.advance();
	const ConfigToken *path = ts.advance();
	if (!path)
	{
		errorMessage = "missing argument for directive 'cgi_path'";
		errorLine = directive->line;
		return (false);
	}
	location.cgiPath = path->value;
	location.hasCgiPath = true;
	if (!ts.expect(";", errorMessage, errorLine))
		return (false);
	return (true);
}

bool Parser::parseLocationBlock(TokenStream &ts,
								LocationConfig &location,
								std::string &errorMessage,
								std::size_t &errorLine)
{
	if (!ts.expect("location", errorMessage, errorLine))
		return (false);
	const ConfigToken *path = ts.advance();
	if (!path)
	{
		errorMessage = "missing location path";
		errorLine = ts.lastLine();
		return (false);
	}
	if (path->value.empty() || path->value[0] != '/')
	{
		errorMessage = std::string("location path must start with '/': ") + path->value;
		errorLine = path->line;
		return (false);
	}
	location.path = path->value;
	if (!ts.expect("{", errorMessage, errorLine))
		return (false);

	while (ts.hasCurrent())
	{
		const ConfigToken *t = ts.current();
		if (t->value == "}")
		{
			if (location.hasCgiExtension != location.hasCgiPath)
			{
				errorMessage = "cgi_extension and cgi_path must be used together";
				errorLine = path->line;
				return (false);
			}
			return (ts.expect("}", errorMessage, errorLine));
		}
		if (t->value == "location")
		{
			errorMessage = "nested location block is not allowed";
			errorLine = t->line;
			return (false);
		}
		if (t->value == "server")
		{
			errorMessage = "server block inside location is not allowed";
			errorLine = t->line;
			return (false);
		}
		if (t->value == "methods")
		{
			if (location.hasMethods)
			{
				errorMessage = "duplicate 'methods' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseMethodsDirective(ts, location, errorMessage, errorLine))
				return (false);
		}
		else if (t->value == "root")
		{
			if (location.hasRoot)
			{
				errorMessage = "duplicate 'root' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseRootDirective(ts, location.root, errorMessage, errorLine))
				return (false);
			location.hasRoot = true;
		}
		else if (t->value == "index")
		{
			if (location.hasIndex)
			{
				errorMessage = "duplicate 'index' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseIndexDirective(ts, location.index, errorMessage, errorLine))
				return (false);
			location.hasIndex = true;
		}
		else if (t->value == "autoindex")
		{
			if (location.hasAutoindex)
			{
				errorMessage = "duplicate 'autoindex' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseAutoindexDirective(ts, location, errorMessage, errorLine))
				return (false);
		}
		else if (t->value == "client_max_body_size")
		{
			if (location.hasClientMaxBodySize)
			{
				errorMessage = "duplicate 'client_max_body_size' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseClientMaxBodySizeDirective(ts, location.clientMaxBodySize,
					errorMessage, errorLine))
				return (false);
			location.hasClientMaxBodySize = true;
		}
		else if (t->value == "upload_store")
		{
			if (location.hasUploadStore)
			{
				errorMessage = "duplicate 'upload_store' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseUploadStoreDirective(ts, location, errorMessage, errorLine))
				return (false);
		}
		else if (t->value == "return")
		{
			if (location.hasRedirect)
			{
				errorMessage = "duplicate 'return' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseReturnDirective(ts, location, errorMessage, errorLine))
				return (false);
		}
		else if (t->value == "cgi_extension")
		{
			if (location.hasCgiExtension)
			{
				errorMessage = "duplicate 'cgi_extension' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseCgiExtensionDirective(ts, location, errorMessage, errorLine))
				return (false);
		}
		else if (t->value == "cgi_path")
		{
			if (location.hasCgiPath)
			{
				errorMessage = "duplicate 'cgi_path' directive in location block";
				errorLine = t->line;
				return (false);
			}
			if (!parseCgiPathDirective(ts, location, errorMessage, errorLine))
				return (false);
		}
		else
		{
			errorMessage = std::string("unknown or invalid directive in location block: '") + t->value + "'";
			errorLine = t->line;
			return (false);
		}
	}
	errorMessage = "missing '}' to close location block";
	errorLine = path->line;
	return (false);
}

bool Parser::parseServerBlock(TokenStream &ts,
							  ServerBlock &server,
							  std::string &errorMessage,
							  std::size_t &errorLine)
{
	bool hasListen = false;

	if (!ts.expect("server", errorMessage, errorLine))
		return (false);
	if (!ts.expect("{", errorMessage, errorLine))
		return (false);

	while (ts.hasCurrent())
	{
		const ConfigToken *t = ts.current();

		if (t->value == "}")
		{
			if (!hasListen)
			{
				errorMessage = "server block requires one 'listen' directive";
				errorLine = t->line;
				return (false);
			}
			return (ts.expect("}", errorMessage, errorLine));
		}
		if (t->value == "server")
		{
			errorMessage = "nested server block is not allowed";
			errorLine = t->line;
			return (false);
		}
		if (t->value == "location")
		{
			const std::size_t locationLine = t->line;
			LocationConfig location;
			if (!parseLocationBlock(ts, location, errorMessage, errorLine))
				return (false);
			std::size_t i = 0;
			while (i < server.locations.size())
			{
				if (server.locations[i].path == location.path)
				{
					errorMessage = std::string("duplicate location path: '") + location.path + "'";
					errorLine = locationLine;
					return (false);
				}
				i++;
			}
			server.locations.push_back(location);
		}
		else if (t->value == "listen")
		{
			if (hasListen)
			{
				errorMessage = "duplicate 'listen' directive in server block";
				errorLine = t->line;
				return (false);
			}
			hasListen = true;
			if (!parseListenDirective(ts, server, errorMessage, errorLine))
				return (false);
		}
		else if (t->value == "root")
		{
			if (server.hasRoot)
			{
				errorMessage = "duplicate 'root' directive in server block";
				errorLine = t->line;
				return (false);
			}
			if (!parseRootDirective(ts, server.root, errorMessage, errorLine))
				return (false);
			server.hasRoot = true;
		}
		else if (t->value == "index")
		{
			if (server.hasIndex)
			{
				errorMessage = "duplicate 'index' directive in server block";
				errorLine = t->line;
				return (false);
			}
			if (!parseIndexDirective(ts, server.index, errorMessage, errorLine))
				return (false);
			server.hasIndex = true;
		}
		else if (t->value == "client_max_body_size")
		{
			if (server.hasClientMaxBodySize)
			{
				errorMessage = "duplicate 'client_max_body_size' directive in server block";
				errorLine = t->line;
				return (false);
			}
			if (!parseClientMaxBodySizeDirective(ts, server.clientMaxBodySize,
					errorMessage, errorLine))
				return (false);
			server.hasClientMaxBodySize = true;
		}
		else if (t->value == "error_page")
		{
			if (!parseErrorPageDirective(ts, server, errorMessage, errorLine))
				return (false);
		}
		else
		{
			errorMessage = std::string("unknown or invalid directive in server block: '") + t->value + "'";
			errorLine = t->line;
			return (false);
		}
	}
	errorMessage = "missing '}' to close server block";
	errorLine = ts.lastLine();
	return (false);
}

bool Parser::parseTokens(const std::vector<ConfigToken> &tokens,
						 std::string &errorMessage,
						 std::size_t &errorLine)
{
	errorMessage.clear();
	errorLine = 0;
	servers_.clear();

	TokenStream ts(tokens);
	while (ts.hasCurrent())
	{
		ServerBlock server;
		if (!parseServerBlock(ts, server, errorMessage, errorLine))
			return (false);
		resolveInheritance(server);
		servers_.push_back(server);
	}
	return (true);
}

const std::vector<ServerBlock> &Parser::getServers() const
{
	return (servers_);
}

void Parser::resolveInheritance(ServerBlock &server)
{
	std::size_t i = 0;
	while (i < server.locations.size())
	{
		LocationConfig &loc = server.locations[i];
		if (!loc.hasRoot && server.hasRoot)
			loc.root = server.root;
		if (!loc.hasIndex && server.hasIndex)
			loc.index = server.index;
		if (!loc.hasClientMaxBodySize && server.hasClientMaxBodySize)
			loc.clientMaxBodySize = server.clientMaxBodySize;
		i++;
	}
}
