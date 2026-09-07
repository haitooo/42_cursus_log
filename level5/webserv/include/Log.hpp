#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>
#include <string>

#define DEBUG 0
#define INFO  1
#define ERROR 2
#define NONE  3

#define LOG_LEVEL INFO

class Log
{
public:
	enum Level
	{
		LEVEL_INFO,
		LEVEL_DEBUG,
		LEVEL_ERROR
	};

	Log(Level level, const char *file, int line);
	~Log();

	template <typename T>
	Log &operator<<(const T &value)
	{
		_buffer << value;
		return (*this);
	}

private:
	Log();
	Log(const Log &other);
	Log &operator=(const Log &other);

	std::ostringstream _buffer;
	Level _level;
	const char *_file;
	int _line;
};


class LogSink
{
public:
	template <typename T>
	LogSink &operator<<(const T &)
	{
		return (*this);
	}
};

#if LOG_LEVEL <= DEBUG
# define LOG_DEBUG Log(Log::LEVEL_DEBUG, __FILE__, __LINE__)
#else
# define LOG_DEBUG LogSink()
#endif

#if LOG_LEVEL <= INFO
# define LOG_INFO Log(Log::LEVEL_INFO, __FILE__, __LINE__)
#else
# define LOG_INFO LogSink()
#endif

#if LOG_LEVEL <= ERROR
# define LOG_ERROR Log(Log::LEVEL_ERROR, __FILE__, __LINE__)
#else
# define LOG_ERROR LogSink()
#endif

#undef DEBUG
#undef INFO
#undef ERROR
#undef NONE
#undef LOG_LEVEL

#endif
