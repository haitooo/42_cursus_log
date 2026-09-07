#ifndef IREQUEST_HANDLER_HPP
# define IREQUEST_HANDLER_HPP

# include "ConfigTypes.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ICgiExecutor.hpp"

/*
** Routing, static files, uploads, deletion, redirects and CGI dispatch all
** live behind handle().
**
** `id` selects which server block's configuration applies; the socket layer
** carries it from the listening socket to every accepted client.
**
** handle() never throws. A failure is expressed by returning a response with
** the matching status code, so the poll loop is never unwound by an exception.
*/
struct HandleResult
{
	bool			pending;
	HttpResponse	response;
	CgiRequest		cgi;

	HandleResult() : pending(false), response(), cgi() {}
};

class IRequestHandler
{
public:
	virtual ~IRequestHandler() {}

	virtual HandleResult	handle(ServerId id,
								const HttpRequest &request) = 0;

	/*
	** Turns what the CGI produced into the response to send.
	**
	** `response` is in-out: on entry it holds what ICgiExecutor::finish()
	** parsed out of the CGI's output, on return the response for the client.
	** In place rather than by value so the body -- which the CGI may have made
	** very large -- is never copied.
	*/
	virtual void			finishCgi(ServerId id, const HttpRequest &request,
								bool ok, int errorStatus,
								HttpResponse &response) = 0;

	virtual HttpResponse	makeError(ServerId id, int statusCode) = 0;

	virtual bool			isCgiRoute(ServerId id,
								const HttpRequest &request) = 0;
};

#endif
