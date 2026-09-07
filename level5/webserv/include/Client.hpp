#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <cstddef>
# include <ctime>

# include "ICgiExecutor.hpp"
# include "RequestParser.hpp"

/*
** Bodies at or below this are sent together with the header. Chosen to cover
** every page the server generates itself -- error pages, autoindex listings,
** small static files -- while leaving real payloads on the zero-copy path.
*/
# define SEND_COALESCE_LIMIT 65536

struct Client
{
	int				fd;
	std::size_t		listenerIndex;
	RequestParser	parser;

	/*
	** The response, kept as two buffers instead of one.
	**
	** sendBody is moved out of HttpResponse rather than copied, so a CGI body
	** of hundreds of megabytes is never duplicated just to glue the status
	** line in front of it. sentSize is the cursor over the two concatenated.
	*/
	std::string		sendHead;
	std::string		sendBody;
	std::size_t		sentSize;
	bool			closing;
	bool			keepAlive;
	std::size_t		servedRequests;
	std::string		pendingInput;

	bool			throttled;
	bool			cgiClassified;
	bool			cgiBound;
	bool			cgiSlot;
	bool			cgiActive;
	CgiSession		cgi;
	HttpRequest		cgiRequest;

	/*
	** Two clocks, because either one alone leaves a hole.
	**
	** lastActivity catches the connection that goes quiet, but a client that
	** dribbles one byte at a time keeps refreshing it and would live forever.
	** connectedAt caps the whole exchange regardless of how busy it looks.
	*/
	std::time_t		lastActivity;
	std::time_t		connectedAt;

	Client() : fd(-1), listenerIndex(0), parser(), sendHead(), sendBody(),
		sentSize(0),
		closing(false), keepAlive(false), servedRequests(0), pendingInput(),
		throttled(false), cgiClassified(false), cgiBound(false), cgiSlot(false),
		cgiActive(false), cgi(), cgiRequest(),
		lastActivity(0), connectedAt(0) {}

	std::size_t	responseSize() const
	{
		return (sendHead.size() + sendBody.size());
	}

	// True from the moment a response is built until resetForNextRequest()
	// clears it. Marks the window in which the request it answers is over.
	bool		hasResponse() const
	{
		return (!sendHead.empty() || !sendBody.empty());
	}

	// Takes the response over: `response` is left with an empty body.
	void		setResponse(HttpResponse &response)
	{
		sendHead = response.serializeHead();
		std::string().swap(sendBody);
		/*
		** A small body is copied in behind the head so the whole response
		** leaves in a single send().
		**
		** Splitting it puts a second, tiny segment on the wire, and on a
		** keep-alive connection that is write-write-read: Nagle holds the
		** second segment until the peer acknowledges the first, and the peer's
		** delayed ACK takes ~40 ms. Measured at 44 ms per request against 3.5
		** ms once coalesced -- the 42 tester's 20x5000 GET flood went from
		** minutes to seconds.
		**
		** A large body is still moved, never copied. There the extra segment
		** costs nothing beside the transfer, and the copy would be the very
		** thing this buffer split exists to avoid.
		*/
		if (response.body.size() <= SEND_COALESCE_LIMIT)
			sendHead += response.body;
		else
			sendBody.swap(response.body);
		std::string().swap(response.body);
		sentSize = 0;
	}

	void		clearResponse()
	{
		std::string().swap(sendHead);
		std::string().swap(sendBody);
		sentSize = 0;
	}
};

#endif
