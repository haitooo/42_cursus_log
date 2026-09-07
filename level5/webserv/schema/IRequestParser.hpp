#ifndef IREQUEST_PARSER_HPP
# define IREQUEST_PARSER_HPP

# include <cstddef>

# include "HttpRequest.hpp"

enum ParseStatus
{
	PARSE_INCOMPLETE,
	PARSE_COMPLETE,
	PARSE_ERROR
};

class IRequestParser
{
public:
	virtual ~IRequestParser() {}

	/*
	** Maximum body size in bytes. Must be called before the first append().
	** Takes the server-level value, IConfig::clientMaxBodySize(id). A larger
	** body makes append() return PARSE_ERROR with getErrorStatus() == 413.
	** The location-level limit is checked again by IRequestHandler.
	*/
	virtual void		setBodyLimit(std::size_t limit) = 0;

	// Raw bytes as returned by recv(). data need not be NUL-terminated.
	virtual ParseStatus	append(const char *data, std::size_t size) = 0;

	// Only valid after append() returned PARSE_COMPLETE.
	virtual const HttpRequest	&getRequest() const = 0;

	// Only valid after append() returned PARSE_ERROR. Returns an HTTP status.
	virtual int	getErrorStatus() const = 0;
};

#endif
