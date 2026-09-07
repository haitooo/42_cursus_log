#ifndef REQUEST_PARSER_HPP
# define REQUEST_PARSER_HPP

# include <string>

# include "IRequestParser.hpp"

/*
** Total bytes allowed for the request line and headers, so that a single
** client that never sends the CRLFCRLF terminator cannot exhaust memory.
*/
# define REQUEST_HEADER_LIMIT 8192
# define REQUEST_URI_LIMIT 8192

class RequestParser : public IRequestParser
{
	public:
		RequestParser();
		~RequestParser();

		void				setBodyLimit(std::size_t limit);
		ParseStatus			append(const char *data, std::size_t size);
		ParseStatus			resume();
		const HttpRequest	&getRequest() const;

		/*
		** The parsed request, for a caller that needs to take its body over
		** instead of copying it (see HttpRequest::takeFrom). Only valid after
		** append() returned PARSE_COMPLETE.
		*/
		HttpRequest			&mutableRequest();
		int					getErrorStatus() const;
		bool				isKeepAlive() const;

		/*
		** True while a request has been started but not finished.
		**
		** Lets the timeout sweep tell "the client began sending and stopped"
		** (408) from "a reused connection is waiting for the next request"
		** (just close it).
		*/
		bool				hasPartialRequest() const;
		bool				isReadingBody() const;
		bool				headersComplete() const;

		// Resets the parser so the same connection can read another request.
		// Unused while every response closes the connection; kept as the entry
		// point for keep-alive.
		void				reset();

	private:
		enum InternalState
		{
			STATE_REQUEST_LINE,
			STATE_HEADERS,
			STATE_BODY,
			STATE_CHUNK_SIZE,
			STATE_CHUNKED_BODY,
			STATE_COMPLETE,
			STATE_ERROR
		};

		std::string		_buffer;
		HttpRequest		_request;
		std::size_t		_headerBytes;
		std::size_t		_contentLength;
		std::size_t		_bodyLimit;
		int				_errorStatus;
		InternalState	_state;

		ParseStatus	parse();
		bool		parseRequestLine();
		bool		parseHeaders();
		bool		onHeaderParsed();
		bool		parseBody();
		bool		parseChunkSize();
		bool		parseChunkedBody();
		bool		invalidRequest(int status);

		static bool	isChunkedEncoding(const std::string &value);
		static bool	headerHasToken(const std::string &value,
						const std::string &token);

		static bool	parseContentLength(const std::string &value, std::size_t &out);
		static void	toLower(std::string &s);
		static bool	trimHeaderValue(const std::string &raw, std::string &out);
};

#endif
