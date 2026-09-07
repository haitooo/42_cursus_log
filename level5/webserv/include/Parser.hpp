#ifndef PARSER_HPP
#define PARSER_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "ConfigInternals.hpp"
#include "Tokenizer.hpp"

class TokenStream;

class Parser
{
public:
	bool parseTokens(const std::vector<ConfigToken> &tokens,
					 std::string &errorMessage,
					 std::size_t &errorLine);
	const std::vector<ServerBlock> &getServers() const;

private:
	std::vector<ServerBlock> servers_;

	// server directives
	static bool parseListenDirective(TokenStream &ts,
									 ServerBlock &server,
									 std::string &errorMessage,
									 std::size_t &errorLine);
	static bool parseRootDirective(TokenStream &ts,
								   std::string &out,
								   std::string &errorMessage,
								   std::size_t &errorLine);
	static bool parseIndexDirective(TokenStream &ts,
									std::string &out,
									std::string &errorMessage,
									std::size_t &errorLine);
	static bool parseClientMaxBodySizeDirective(TokenStream &ts,
												std::size_t &out,
												std::string &errorMessage,
												std::size_t &errorLine);
	static bool parseErrorPageDirective(TokenStream &ts,
										ServerBlock &server,
										std::string &errorMessage,
										std::size_t &errorLine);
	// location directives
	static bool parseMethodsDirective(TokenStream &ts,
									  LocationConfig &location,
									  std::string &errorMessage,
									  std::size_t &errorLine);
	static bool parseAutoindexDirective(TokenStream &ts,
										LocationConfig &location,
										std::string &errorMessage,
										std::size_t &errorLine);
	static bool parseUploadStoreDirective(TokenStream &ts,
										  LocationConfig &location,
										  std::string &errorMessage,
										  std::size_t &errorLine);
	static bool parseReturnDirective(TokenStream &ts,
									 LocationConfig &location,
									 std::string &errorMessage,
									 std::size_t &errorLine);
	static bool parseCgiExtensionDirective(TokenStream &ts,
										   LocationConfig &location,
										   std::string &errorMessage,
										   std::size_t &errorLine);
	static bool parseCgiPathDirective(TokenStream &ts,
									  LocationConfig &location,
									  std::string &errorMessage,
									  std::size_t &errorLine);
	// blocks
	static bool parseLocationBlock(TokenStream &ts,
								   LocationConfig &location,
								   std::string &errorMessage,
								   std::size_t &errorLine);
	static bool parseServerBlock(TokenStream &ts,
								 ServerBlock &server,
								 std::string &errorMessage,
								 std::size_t &errorLine);
	static void resolveInheritance(ServerBlock &server);
	// utilities
	static bool isAllDigits(const std::string &s);
	static bool isValidPortToken(const std::string &s);
	static bool isSpecialToken(const std::string &s);
	static bool isIntegerInRange(const std::string &s, long min, long max);
	static std::vector<std::string> split(const std::string &s, char delimiter);
	static bool isValidIpv4Token(const std::string &s);
	static bool isValidListenAddressToken(const std::string &s);
};

#endif
