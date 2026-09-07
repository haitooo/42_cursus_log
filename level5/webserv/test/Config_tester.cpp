#include <dirent.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "ConfigParser.hpp"

#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

#define PASS (std::string(COLOR_GREEN) + "[PASS]" + std::string(COLOR_RESET))
#define FAIL (std::string(COLOR_RED) + "[FAIL]" + std::string(COLOR_RESET))

typedef bool (*ConfigValidator)(const IConfig &config);

struct ConfigTest
{
	std::string path;
	bool expected;
	ConfigValidator validator;
};

static bool hasConfExtension(const std::string &name)
{
	return (name.size() > 5
		&& name.substr(name.size() - 5) == ".conf");
}

static bool isExpectedSuccess(const std::string &name)
{
	return (name.size() > 3 && name.substr(0, 3) == "ok_");
}

static std::vector<std::string> makeMethods(const char *first,
	const char *second = NULL, const char *third = NULL,
	const char *fourth = NULL)
{
	std::vector<std::string> methods;
	if (first != NULL)
		methods.push_back(first);
	if (second != NULL)
		methods.push_back(second);
	if (third != NULL)
		methods.push_back(third);
	if (fourth != NULL)
		methods.push_back(fourth);
	return (methods);
}

static const std::vector<std::string> methodsG = makeMethods("GET");
static const std::vector<std::string> methodsP = makeMethods("POST");
static const std::vector<std::string> methodsD = makeMethods("DELETE");
static const std::vector<std::string> methodsGP = makeMethods("GET", "POST");
static const std::vector<std::string> methodsGD = makeMethods("GET", "DELETE");
static const std::vector<std::string> methodsPD = makeMethods("POST", "DELETE");
static const std::vector<std::string> methodsGH = makeMethods("GET", "HEAD");
static const std::vector<std::string> methodsGPD = makeMethods("GET", "POST", "DELETE");
static const std::vector<std::string> methodsGPDH = makeMethods("GET", "POST", "DELETE", "HEAD");

static bool expectListen(const IConfig &config, ServerId id,
	const std::string &host, int port)
{
	const ServerConfig &listen = config.listenAddress(id);
	return (listen.host == host && listen.port == port);
}

static bool expectLocation(const IConfig &config, ServerId id,
	const std::string &path, const std::vector<std::string> &methods,
	const std::string &root, const std::string &index, bool autoindex)
{
	ResolvedLocation location;
	if (!config.resolveLocation(id, path, location))
		return (false);
	return (location.locationPath == path
		&& location.methods == methods
		&& location.root == root
		&& location.index == index
		&& location.autoindex == autoindex);
}

static bool validateOkAllDirectives(const IConfig &config)
{
	if (config.serverCount() != 1 || !expectListen(config, 0, "127.0.0.1", 8080))
		return (false);
	if (!expectLocation(config, 0, "/", methodsG, "./www", "home.html", false))
		return (false);
	if (!expectLocation(config, 0, "/upload", methodsP, "./www", "index.html", false))
		return (false);
	if (!expectLocation(config, 0, "/files", methodsGD, "./storage", "index.html", true))
		return (false);
	if (!expectLocation(config, 0, "/cgi-bin", methodsGP, "./cgi-bin", "index.html", false))
		return (false);
	ResolvedLocation cgi;
	if (!config.resolveLocation(0, "/cgi-bin", cgi)
		|| !cgi.cgi.enabled || cgi.cgi.extension != ".py"
		|| cgi.cgi.interpreterPath != "/usr/bin/python3")
		return (false);
	return (true);
}

static bool validateOkAutoindexOff(const IConfig &config)
{
	return (config.serverCount() == 1
		&& expectLocation(config, 0, "/", methodsG, "", "", false));
}

static bool validateOkAutoindexOn(const IConfig &config)
{
	return (config.serverCount() == 1
		&& expectLocation(config, 0, "/", methodsG, "", "", true));
}

static bool validateOkBasic(const IConfig &config)
{
	return (config.serverCount() == 2
		&& expectListen(config, 0, "127.0.0.1", 8080)
		&& expectListen(config, 1, "", 8080)
		&& expectLocation(config, 0, "/", methodsGPD, "./www", "index.html", false)
		&& config.clientMaxBodySize(0) == 1048576);
}

static bool validateOkCgi(const IConfig &config)
{
	ResolvedLocation location;
	return (config.serverCount() == 1
		&& expectLocation(config, 0, "/cgi-bin", methodsGP, "./cgi-bin", "", false)
		&& config.resolveLocation(0, "/cgi-bin", location)
		&& location.cgi.enabled
		&& location.cgi.extension == ".py"
		&& location.cgi.interpreterPath == "/usr/bin/python3");
}

static bool validateOkClientMaxBodySizeZero(const IConfig &config)
{
	return (config.serverCount() == 1 && config.clientMaxBodySize(0) == 0);
}

static bool validateOkLocationClientMaxBodySize(const IConfig &config)
{
	ResolvedLocation	location;

	if (config.serverCount() != 1 || config.clientMaxBodySize(0) != 10485760)
		return (false);
	if (!config.resolveLocation(0, "/", location)
		|| location.clientMaxBodySize != 1000)
		return (false);
	if (!config.resolveLocation(0, "/small", location)
		|| location.clientMaxBodySize != 100)
		return (false);
	if (!config.resolveLocation(0, "/big", location)
		|| location.clientMaxBodySize != 10485760)
		return (false);
	return (true);
}

static bool validateOkDefault(const IConfig &config)
{
	return (config.serverCount() == 1
		&& expectListen(config, 0, "127.0.0.1", 8080)
		&& config.clientMaxBodySize(0) == 1048576
		&& expectLocation(config, 0, "/", methodsGPD, "./www", "index.html", false)
		&& expectLocation(config, 0, "/upload", methodsP, "./www", "index.html", false));
}

static bool validateOkErrorPageMultiple(const IConfig &config)
{
	std::string path;
	return (config.serverCount() == 1
		&& config.errorPagePath(0, 400, path) && path == "./errors/400.html"
		&& config.errorPagePath(0, 404, path) && path == "./errors/404.html"
		&& config.errorPagePath(0, 500, path) && path == "./errors/500.html"
		&& config.errorPagePath(0, 599, path) && path == "./errors/599.html");
}

static bool validateOkListenIpv4AllZeros(const IConfig &config)
{
	return (config.serverCount() == 1
		&& expectListen(config, 0, "0.0.0.0", 8080));
}

static bool validateOkListenIpv4MaxOctet(const IConfig &config)
{
	return (config.serverCount() == 1
		&& expectListen(config, 0, "255.255.255.255", 8080));
}

static bool validateOkListenPortMax(const IConfig &config)
{
	return (config.serverCount() == 1 && expectListen(config, 0, "", 65535));
}

static bool validateOkListenPortMin(const IConfig &config)
{
	return (config.serverCount() == 1 && expectListen(config, 0, "", 1));
}

static bool validateOkListenPortOnly(const IConfig &config)
{
	return (config.serverCount() == 1 && expectListen(config, 0, "", 8080));
}

static bool validateOkMethodsAll(const IConfig &config)
{
	return (config.serverCount() == 1
		&& expectLocation(config, 0, "/", methodsGPDH, "", "", false));
}

static bool validateOkMethodsHead(const IConfig &config)
{
	return (config.serverCount() == 1
		&& expectLocation(config, 0, "/", methodsGH, "", "", false));
}

static bool validateOkMinimal(const IConfig &config)
{
	return (config.serverCount() == 1 && expectListen(config, 0, "", 8080));
}

static bool validateOkMultipleServers(const IConfig &config)
{
	return (config.serverCount() == 3
		&& expectListen(config, 0, "127.0.0.1", 8080)
		&& expectListen(config, 1, "127.0.0.1", 8081)
		&& expectListen(config, 2, "", 8082));
}

static bool validateOkReturn(const IConfig &config)
{
	const int statuses[] = {301, 302, 307, 308};
	for (std::size_t i = 0; i < 4; ++i)
	{
		ResolvedLocation location;
		std::string path = "/";
		if (i == 0)
			path = "/301";
		else if (i == 1)
			path = "/302";
		else if (i == 2)
			path = "/307";
		else
			path = "/308";
		if (!config.resolveLocation(0, path, location)
			|| !location.redirect.enabled
			|| location.redirect.statusCode != statuses[i]
			|| location.redirect.target != "/new")
			return (false);
	}
	return (true);
}

static ConfigValidator validatorFor(const std::string &name)
{
	if (name == "ok_all_directives.conf") return (validateOkAllDirectives);
	if (name == "ok_autoindex_off.conf") return (validateOkAutoindexOff);
	if (name == "ok_autoindex_on.conf") return (validateOkAutoindexOn);
	if (name == "ok_basic.conf") return (validateOkBasic);
	if (name == "ok_cgi.conf") return (validateOkCgi);
	if (name == "ok_client_max_body_size_zero.conf") return (validateOkClientMaxBodySizeZero);
	if (name == "ok_default.conf") return (validateOkDefault);
	if (name == "ok_error_page_multiple.conf") return (validateOkErrorPageMultiple);
	if (name == "ok_listen_ipv4_all_zeros.conf") return (validateOkListenIpv4AllZeros);
	if (name == "ok_listen_ipv4_max_octet.conf") return (validateOkListenIpv4MaxOctet);
	if (name == "ok_listen_port_max.conf") return (validateOkListenPortMax);
	if (name == "ok_listen_port_min.conf") return (validateOkListenPortMin);
	if (name == "ok_listen_port_only.conf") return (validateOkListenPortOnly);
	if (name == "ok_location_client_max_body_size.conf") return (validateOkLocationClientMaxBodySize);
	if (name == "ok_methods_all.conf") return (validateOkMethodsAll);
	if (name == "ok_methods_head.conf") return (validateOkMethodsHead);
	if (name == "ok_minimal.conf") return (validateOkMinimal);
	if (name == "ok_multiple_servers.conf") return (validateOkMultipleServers);
	if (name == "ok_return.conf") return (validateOkReturn);
	return (NULL);
}

static bool collectTests(const std::string &directory,
	std::vector<ConfigTest> &tests)
{
	DIR *dir = opendir(directory.c_str());
	if (dir == NULL)
	{
		std::cerr << "failed to open test directory: " << directory << std::endl;
		return (false);
	}

	struct dirent *entry = readdir(dir);
	while (entry != NULL)
	{
		const std::string name = entry->d_name;
		if (name != "." && name != ".." && hasConfExtension(name))
		{
			if (name.substr(0, 3) != "ok_" && name.substr(0, 3) != "ng_")
			{
				std::cerr << "ignored file without ok_/ng_ prefix: "
					<< name << std::endl;
			}
			else
			{
				ConfigTest test;
				test.path = directory + "/" + name;
				test.expected = isExpectedSuccess(name);
				test.validator = validatorFor(name);
				tests.push_back(test);
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (true);
}

static bool runTest(const ConfigTest &test)
{
	ConfigParser parser;
	const bool actual = parser.parseFile(test.path);
	bool valuesValid = true;
	if (actual && test.expected)
	{
		if (test.validator == NULL)
			valuesValid = false;
		else
			valuesValid = test.validator(parser.getConfig());
	}
	const bool passed = (actual == test.expected && valuesValid);

	std::cout << (passed ? PASS : FAIL) << " " << test.path
		<< " expected=" << (test.expected ? "accept" : "reject")
		<< " actual=" << (actual ? "accept" : "reject");
	if (actual && test.expected)
		std::cout << " values=" << (valuesValid ? "valid" : "invalid");
	if (!actual)
		std::cout << " (line " << parser.getErrorLine() << ": "
			<< parser.getErrorMessage() << ")";
	std::cout << std::endl;
	return (passed);
}

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "usage: config_tester [test_directory]" << std::endl;
		return (1);
	}

	const std::string directory = (argc == 2 ? argv[1] : "conf/tests");
	std::vector<ConfigTest> tests;
	if (!collectTests(directory, tests))
		return (1);
	if (tests.empty())
	{
		std::cerr << "no .conf tests found in " << directory << std::endl;
		return (1);
	}

	std::size_t passed = 0;
	for (std::size_t i = 0; i < tests.size(); ++i)
	{
		if (runTest(tests[i]))
			++passed;
	}

	std::cout << passed << "/" << tests.size() << " tests passed"
		<< std::endl;
	return (passed == tests.size() ? 0 : 1);
}
