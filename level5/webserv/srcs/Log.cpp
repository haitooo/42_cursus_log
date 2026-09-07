#include "Log.hpp"
#include <iomanip>

static std::string shortFileName(const char *file)
{
	std::string path(file);
	std::string::size_type separator = path.find_last_of("/\\");

	if (separator == std::string::npos)
		return (path);
	return (path.substr(separator + 1));
}

Log::Log(Level level, const char *file, int line)
	: _buffer(), _level(level), _file(file), _line(line)
{
}

Log::~Log()
{
	std::ostringstream location;

	location << "[" << shortFileName(_file) << ":" << _line << "]";
	if (_level == LEVEL_ERROR)
		std::cerr << "\033[31m[ERROR]\033[0m " << std::left
				  << std::setw(24) << location.str() << _buffer.str()
				  << std::endl;
	else if (_level == LEVEL_DEBUG)
		std::cout << "\033[36m[DEBUG]\033[0m " << std::left
				  << std::setw(24) << location.str() << _buffer.str()
				  << std::endl;
	else
		std::cout << "\033[32m[INFO ]\033[0m " << std::left
				  << std::setw(24) << location.str() << _buffer.str()
				  << std::endl;
}
