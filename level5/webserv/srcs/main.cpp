#include <iostream>
#include <string>

#include "Config.hpp"
#include "ConfigParser.hpp"
#include "Log.hpp"
#include "Server.hpp"
#include "Signal.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "usage: ./webserv [config_path]" << std::endl;
		return (1);
	}

	setupSignalHandlers();

	const std::string configPath = (argc == 2 ? argv[1] : DEFAULT_CONFIG_PATH);

	LOG_INFO << "configuration file: " << configPath;

	ConfigParser parser;

	if (!parser.parseFile(configPath))
	{
		if (parser.getErrorLine() == 0)
			LOG_ERROR << parser.getErrorMessage() << ": " << configPath;
		else
			LOG_ERROR << "configuration file error: " << configPath << ":"
					  << parser.getErrorLine() << ": "
					  << parser.getErrorMessage();
		return (1);
	}

	try
	{
		Server server(parser.getConfig());

		server.run();

		if (g_signalReceived != 0)
			LOG_INFO << "shutdown signal=" << g_signalReceived;
	}
	catch (const std::exception &e)
	{
		LOG_ERROR << e.what();
		return (1);
	}
	return (0);
}
