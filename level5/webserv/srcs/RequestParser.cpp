#include <cctype>

#include "RequestParser.hpp"
#include "HttpStatus.hpp"
#include "ConfigInternals.hpp"

RequestParser::RequestParser()
	: _buffer(),
	  _request(),
	  _headerBytes(0),
	  _contentLength(0),
	  _bodyLimit(DEFAULT_CLIENT_MAX_BODYSIZE),
	  _errorStatus(0),
	  _state(STATE_REQUEST_LINE)
{
}

RequestParser::~RequestParser()
{
}

void	RequestParser::setBodyLimit(std::size_t limit)
{
	_bodyLimit = limit;
}

/*
** Prepares the parser for the next request on the same connection.
**
** _buffer is deliberately kept: a client may have pipelined the head of the
** next request into the same read, and clearing here would drop those bytes
** and stall the connection. Everything that describes the request just served
** is cleared instead.
*/
void	RequestParser::reset()
{
	_request.clear();
	_buffer.clear();
	_headerBytes = 0;
	_contentLength = 0;
	_errorStatus = 0;
	_state = STATE_REQUEST_LINE;
}

ParseStatus	RequestParser::append(const char *data, std::size_t size)
{
	if (_state == STATE_COMPLETE)
		return (PARSE_COMPLETE);
	if (_state == STATE_ERROR)
		return (PARSE_ERROR);

	if (_state == STATE_REQUEST_LINE || _state == STATE_HEADERS)
		_headerBytes += size;
	_buffer.append(data, size);
	return (parse());
}

ParseStatus	RequestParser::resume()
{
	if (_state == STATE_COMPLETE)
		return (PARSE_COMPLETE);
	if (_state == STATE_ERROR)
		return (PARSE_ERROR);
	return (parse());
}

const HttpRequest	&RequestParser::getRequest() const
{
	return (_request);
}

HttpRequest	&RequestParser::mutableRequest()
{
	return (_request);
}

int	RequestParser::getErrorStatus() const
{
	return (_errorStatus);
}

ParseStatus	RequestParser::parse()
{
	while (_state != STATE_COMPLETE && _state != STATE_ERROR)
	{
		bool	consumed = false;

		if (_state == STATE_REQUEST_LINE)
			consumed = parseRequestLine();
		else if (_state == STATE_HEADERS)
			consumed = parseHeaders();
		else if (_state == STATE_BODY)
			consumed = parseBody();
		else if (_state == STATE_CHUNK_SIZE)
			consumed = parseChunkSize();
		else if (_state == STATE_CHUNKED_BODY)
			consumed = parseChunkedBody();

		if (consumed == false)
			break ;
	}

	/*
	** Measured as the running total spent on the header section. Looking at
	** _buffer alone would miss a client that keeps sending complete header
	** lines forever, because each one is consumed as it arrives.
	*/
	if ((_state == STATE_REQUEST_LINE || _state == STATE_HEADERS)
		&& _headerBytes > REQUEST_HEADER_LIMIT)
		invalidRequest(STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE);

	if (_state == STATE_COMPLETE)
		return (PARSE_COMPLETE);
	if (_state == STATE_ERROR)
		return (PARSE_ERROR);
	return (PARSE_INCOMPLETE);
}

bool	RequestParser::parseRequestLine()
{
	std::size_t	pos = _buffer.find("\r\n");

	if (pos == std::string::npos)
		return (false); // incomplete

	std::string	requestLine = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 2);

	std::size_t	methodEnd = requestLine.find(' ');
	if (methodEnd == std::string::npos)
		return (invalidRequest(STATUS_BAD_REQUEST));

	std::size_t	uriEnd = requestLine.find(' ', methodEnd + 1);
	if (uriEnd == std::string::npos)
		return (invalidRequest(STATUS_BAD_REQUEST));

	// Exactly two spaces: a raw space inside the URI is invalid.
	if (requestLine.find(' ', uriEnd + 1) != std::string::npos)
		return (invalidRequest(STATUS_BAD_REQUEST));

	_request.method = requestLine.substr(0, methodEnd);
	_request.path = requestLine.substr(methodEnd + 1, uriEnd - methodEnd - 1);
	_request.version = requestLine.substr(uriEnd + 1);

	// An empty method / URI / version would be indexed later, so reject here.
	if (_request.method.empty() || _request.path.empty()
		|| _request.version.empty())
		return (invalidRequest(STATUS_BAD_REQUEST));
	// return 400 if method contains invalid characters
	if (_request.method.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos)
		return (invalidRequest(STATUS_BAD_REQUEST));
	if (_request.path.size() > REQUEST_URI_LIMIT)
		return (invalidRequest(STATUS_URI_TOO_LONG));

	/*
	** 405 means "the method is known but not allowed on this resource" and
	** requires an Allow header. A method the server does not implement is 501.
	** The per-resource 405 is decided by RequestHandler::handle().
	**
	** HEAD is known here even though a location only serves it when `methods`
	** lists it: whether a resource allows it is a 405 question, not a 501 one.
	*/
	const bool	isKnownMethod = _request.method == "GET"
		|| _request.method == "POST"
		|| _request.method == "DELETE"
		|| _request.method == "HEAD";
	if (!isKnownMethod)
		return (invalidRequest(STATUS_NOT_IMPLEMENTED));

	const bool	isKnownVersion = _request.version == HTTP_VERSION_1_1
		|| _request.version == HTTP_VERSION_1_0;
	if (!isKnownVersion)
		return (invalidRequest(STATUS_HTTP_VERSION_NOT_SUPPORTED));

	_state = STATE_HEADERS;
	return (true);
}

void	RequestParser::toLower(std::string &s)
{
	for (std::size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
			s[i] = static_cast<char>(s[i] - 'A' + 'a');
	}
}

// An empty header value (`X-Foo:`) is valid per RFC 7230; keep it as "".
bool	RequestParser::trimHeaderValue(const std::string &raw, std::string &out)
{
	std::size_t	first = raw.find_first_not_of(" \t");

	if (first == std::string::npos)
	{
		out.clear();
		return (true);
	}
	out = raw.substr(first, raw.find_last_not_of(" \t") - first + 1);
	return (true);
}

bool	RequestParser::parseHeaders()
{
	std::size_t	pos = _buffer.find("\r\n");

	if (pos == std::string::npos)
		return (false); // incomplete

	while (pos != 0)
	{
		std::string	headerLine = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 2);

		std::size_t	colonPos = headerLine.find(':');
		if (colonPos == std::string::npos)
			return (invalidRequest(STATUS_BAD_REQUEST));

		// Empty name, or whitespace between name and colon: 400 per RFC 7230.
		if (colonPos == 0 || headerLine[colonPos - 1] == ' '
			|| headerLine[colonPos - 1] == '\t')
			return (invalidRequest(STATUS_BAD_REQUEST));

		std::string	headerName = headerLine.substr(0, colonPos);
		toLower(headerName);
		// Host and Content-Length must not be repeated
		if ((headerName == "host" || headerName == "content-length") &&
			_request.headers.find(headerName) != _request.headers.end())
			return (invalidRequest(STATUS_BAD_REQUEST));

		std::string	headerValue;
		trimHeaderValue(headerLine.substr(colonPos + 1), headerValue);
		if (headerValue.size() > REQUEST_HEADER_LIMIT)
			return (invalidRequest(STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE));
		_request.headers[headerName] = headerValue;

		pos = _buffer.find("\r\n");
		if (pos == std::string::npos)
			return (false); // incomplete
	}

	_buffer.erase(0, 2); // the blank line that terminates the headers
	return (onHeaderParsed());
}

/*
** Reads a decimal Content-Length into a size_t. Overflow is checked by hand
** because leaving it to stringstream is implementation defined.
*/
bool	RequestParser::parseContentLength(const std::string &value,
	std::size_t &out)
{
	if (value.empty() || value.find_first_not_of("0123456789") != std::string::npos)
		return (false);

	std::size_t	result = 0;

	for (std::size_t i = 0; i < value.size(); ++i)
	{
		const std::size_t	digit = static_cast<std::size_t>(value[i] - '0');

		if (result > (static_cast<std::size_t>(-1) - digit) / 10)
			return (false);
		result = result * 10 + digit;
	}
	out = result;
	return (true);
}

bool	RequestParser::onHeaderParsed()
{
	// Host is mandatory for HTTP/1.1 (RFC 7230).
	if (_request.version == HTTP_VERSION_1_1)
	{
		if (_request.headers.find("host") == _request.headers.end())
			return (invalidRequest(STATUS_BAD_REQUEST));
		if (_request.headers["host"].empty())
			return (invalidRequest(STATUS_BAD_REQUEST));
	}

	/*
	** Only "chunked" is decoded. Any other Transfer-Encoding is still refused:
	** letting one through as a zero-length body would make the encoded data
	** look like the next request on this connection (request smuggling).
	*/
	std::map<std::string, std::string>::const_iterator	encodingIt
		= _request.headers.find("transfer-encoding");
	if (encodingIt != _request.headers.end())
	{
		if (!isChunkedEncoding(encodingIt->second))
			return (invalidRequest(STATUS_NOT_IMPLEMENTED));
		/*
		** A request carrying both Transfer-Encoding and Content-Length is the
		** classic smuggling shape: two framings that can disagree. Refuse it
		** rather than picking a winner (RFC 7230 3.3.3).
		*/
		if (_request.headers.find("content-length") != _request.headers.end())
			return (invalidRequest(STATUS_BAD_REQUEST));
		_contentLength = 0;
		_state = STATE_CHUNK_SIZE;
		return (true);
	}

	std::map<std::string, std::string>::const_iterator	it
		= _request.headers.find("content-length");

	if (_request.method == "POST" && it == _request.headers.end())
		return (invalidRequest(STATUS_LENGTH_REQUIRED));
	if (it == _request.headers.end())
		_contentLength = 0;
	else
	{
		if (parseContentLength(it->second, _contentLength) == false)
			return (invalidRequest(STATUS_BAD_REQUEST));
		if (_contentLength > _bodyLimit)
			return (invalidRequest(STATUS_PAYLOAD_TOO_LARGE));
	}

	if (_contentLength == 0)
		_state = STATE_COMPLETE;
	else
		_state = STATE_BODY;
	return (true);
}

bool	RequestParser::parseBody()
{
	if (_buffer.size() < _contentLength)
		return (false); // incomplete

	/*
	** When the buffer holds exactly this body and nothing behind it -- the
	** common case, since a pipelined request arrives in a later recv() -- it
	** is handed over instead of copied. Otherwise a 100 MB upload exists
	** twice between the substr() and the erase().
	*/
	if (_buffer.size() == _contentLength)
	{
		std::string().swap(_request.body);
		_request.body.swap(_buffer);
	}
	else
	{
		_request.body = _buffer.substr(0, _contentLength);
		_buffer.erase(0, _contentLength);
	}
	_state = STATE_COMPLETE;
	return (true);
}

/*
** Chunked framing helpers. Both are file-local: they are implementation
** detail of the chunk decoder, not part of the parser's contract.
*/
static std::size_t	hexCharToDecimal(char c)
{
	if (c >= '0' && c <= '9')
		return (static_cast<std::size_t>(c - '0'));
	else if (c >= 'a' && c <= 'f')
		return (static_cast<std::size_t>(c - 'a' + 10));
	else if (c >= 'A' && c <= 'F')
		return (static_cast<std::size_t>(c - 'A' + 10));
	return (0);
}

/*
** True only for a Transfer-Encoding this parser can actually decode, i.e. a
** lone "chunked". A list such as "gzip, chunked" is not accepted: we do not
** implement the outer coding, and guessing would reframe the body.
*/
bool	RequestParser::isChunkedEncoding(const std::string &value)
{
	std::size_t	begin = value.find_first_not_of(" \t");
	std::size_t	end = value.find_last_not_of(" \t");

	if (begin == std::string::npos)
		return (false);

	std::string	trimmed = value.substr(begin, end - begin + 1);

	if (trimmed.size() != 7)
		return (false);
	for (std::size_t i = 0; i < trimmed.size(); ++i)
	{
		if (tolower(static_cast<unsigned char>(trimmed[i])) != "chunked"[i])
			return (false);
	}
	return (true);
}

bool	RequestParser::parseChunkSize()
{
	if(_buffer.find("\r\n") == std::string::npos)
		return (false); // incomplete
	const std::string chunkSizeLine = _buffer.substr(0, _buffer.find("\r\n"));

	if(chunkSizeLine.empty())
		return (invalidRequest(STATUS_BAD_REQUEST));
	if(chunkSizeLine.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
		return (invalidRequest(STATUS_BAD_REQUEST));

	std::size_t chunkSize = 0;
	for(std::size_t i = 0; i < chunkSizeLine.size(); ++i)
	{
		if(chunkSize > (static_cast<std::size_t>(-1) - hexCharToDecimal(chunkSizeLine[i])) / 16)
			return (invalidRequest(STATUS_BAD_REQUEST));
		chunkSize = chunkSize * 16 + hexCharToDecimal(chunkSizeLine[i]);
	}
	
	/*
	** The limit is on the decoded body as a whole, not on one chunk. Checking
	** per chunk would let a client send unlimited chunks of _bodyLimit bytes
	** each and grow _request.body without bound.
	*/
	if (chunkSize > _bodyLimit - _request.body.size())
		return (invalidRequest(STATUS_PAYLOAD_TOO_LARGE));

	if(chunkSize == 0)
	{
		if(_buffer.size() < chunkSizeLine.size() + 4) // +4 for CRLFCRLF
			return (false); // incomplete
		if(_buffer.find("\r\n\r\n", chunkSizeLine.size()) != chunkSizeLine.size())
			return (invalidRequest(STATUS_BAD_REQUEST));
		_buffer.erase(0, chunkSizeLine.size() + 4); // +4 for CRLFCRLF
		_state = STATE_COMPLETE;
		return (true);
	}
	_contentLength = chunkSize;
	_state = STATE_CHUNKED_BODY;
	_buffer.erase(0, chunkSizeLine.size() + 2);
	return (true);
}

bool	RequestParser::parseChunkedBody()
{
	if(_buffer.size() < _contentLength + 2) // +2 for CRLF
		return (false); // incomplete
	_request.body.append(_buffer, 0, _contentLength);
	_buffer.erase(0, _contentLength + 2); // +2 for CRLF
	_state = STATE_CHUNK_SIZE;
	_contentLength = 0;
	return (true);
}

bool	RequestParser::hasPartialRequest() const
{
	if (_state == STATE_COMPLETE || _state == STATE_ERROR)
		return (false);
	return (_buffer.empty() == false || _state != STATE_REQUEST_LINE);
}

bool	RequestParser::isReadingBody() const
{
	return (_state == STATE_BODY || _state == STATE_CHUNK_SIZE
		|| _state == STATE_CHUNKED_BODY);
}

bool	RequestParser::headersComplete() const
{
	return (_state != STATE_REQUEST_LINE && _state != STATE_HEADERS
		&& _state != STATE_ERROR);
}

bool	RequestParser::isKeepAlive() const
{
	if (_state != STATE_COMPLETE)
		return (false);

	/*
	** parseHeaders() lowercases every field name, so this must look the name
	** up in lower case -- FIELD_CONNECTION ("Connection") never matches and
	** would make "Connection: close" silently behave as keep-alive.
	*/
	std::map<std::string, std::string>::const_iterator	it
		= _request.headers.find("connection");

	if (it == _request.headers.end())
		return (_request.version == HTTP_VERSION_1_1);
	if (headerHasToken(it->second, "close"))
		return (false);
	if (headerHasToken(it->second, "keep-alive"))
		return (true);
	return (_request.version == HTTP_VERSION_1_1);
}

/*
** Connection carries a comma separated token list and the tokens are
** case-insensitive, so an exact string compare is not enough.
*/
bool	RequestParser::headerHasToken(const std::string &value,
	const std::string &token)
{
	std::size_t	pos = 0;

	while (pos <= value.size())
	{
		std::size_t	comma = value.find(',', pos);
		std::size_t	end = (comma == std::string::npos) ? value.size() : comma;
		std::size_t	begin = value.find_first_not_of(" \t", pos);

		if (begin != std::string::npos && begin < end)
		{
			std::size_t	last = value.find_last_not_of(" \t", end - 1);
			std::string	item = value.substr(begin, last - begin + 1);

			if (item.size() == token.size())
			{
				std::size_t	i = 0;
				while (i < item.size()
					&& tolower(static_cast<unsigned char>(item[i])) == token[i])
					i++;
				if (i == item.size())
					return (true);
			}
		}
		if (comma == std::string::npos)
			break ;
		pos = comma + 1;
	}
	return (false);
}

bool	RequestParser::invalidRequest(int status)
{
	_state = STATE_ERROR;
	_errorStatus = status;
	return (false);
}
