#include "HttpStatus.hpp"
#include "Log.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Signal.hpp"

static std::string eventTypesToString(int types)
{
	std::string result;

	if (types & EVENT_READ)
		result += "READ";
	if (types & EVENT_WRITE)
		result += (result.empty() ? "WRITE" : "|WRITE");
	if (types & EVENT_ERROR)
		result += (result.empty() ? "ERROR" : "|ERROR");
	return (result);
}

Server::Server(const IConfig &config)
	: _config(config), _cgiOwner(), _cgiQueue(), _activeCgi(0),
	  _cgiExecutor(),
	  _sessions(SESSION_TTL_SEC, SESSION_MAX_COUNT),
	  _handler(config, _cgiExecutor, _sessions), _running(false)
{
	initialize();
}

Server::~Server()
{
	std::map<int, Client>::iterator it;

	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		_cgiExecutor.abort(it->second.cgi);
		close(it->first);
	}
	_clients.clear();
	closeAllFd();
}

void Server::initialize()
{
	if (_config.serverCount() == 0)
		throw std::runtime_error("no server configuration");

	createListeningSocket();
}

void Server::createListeningSocket()
{
	std::size_t i = 0;

	while (i < _config.serverCount())
	{
		const ServerConfig &listenAddr = _config.listenAddress(i);

		Listener listener;

		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
		{
			errorCreateListeningSocket(-1, "socket");
		}
		if(fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
		{
			errorCreateListeningSocket(fd, "fcntl");
		}

		int opt = 1;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		{
			errorCreateListeningSocket(fd, "setsockopt");
		}

		struct sockaddr_in addr = makeAddr(listenAddr);
		if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{
			errorCreateListeningSocket(fd, "bind");
		}

		if (listen(fd, SOMAXCONN) == -1)
		{
			errorCreateListeningSocket(fd, "listen");
		}

		if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		{
			errorCreateListeningSocket(fd, "fcntl");
		}

		listener.fd = fd;
		listener.configIndex = i;
		listener.host = (listenAddr.host.empty()
							 ? std::string("0.0.0.0")
							 : listenAddr.host);
		listener.port = listenAddr.port;
		_listeners.push_back(listener);
		LOG_INFO << "listen host=" << listener.host
				 << " port=" << listener.port << " fd=" << listener.fd
				 << " (http://" << listener.host
				 << ":" << listener.port << ")";
		i++;
	}
}

struct sockaddr_in Server::makeAddr(const ServerConfig &conf)
{
	struct sockaddr_in addr;

	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(conf.port);
	/*
	** An empty host means "not set" (schema/ConfigTypes.hpp), which is what
	** `listen 8080;` produces, so listen on every interface.
	**
	** inet_addr("") returns INADDR_NONE (255.255.255.255); passing that
	** through would bind successfully to an address nobody can reach.
	*/
	if (conf.host.empty())
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else {
		struct addrinfo hints;
		struct addrinfo *res = NULL;
		addr.sin_addr.s_addr = static_cast<in_addr_t>(-1);
		std::memset(&hints, 0, sizeof(hints));

		hints.ai_family = AF_INET; // IPv4
		hints.ai_flags = AI_NUMERICHOST; // Don't resolve hostnames
		int status = getaddrinfo(conf.host.c_str(), NULL, &hints, &res);
		if (status != 0 || res == NULL) {
			if (res != NULL)
				freeaddrinfo(res);
			return addr; // Return with addr.sin_addr.s_addr set to -1
		}
		struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(res->ai_addr);
		addr.sin_addr.s_addr = ipv4->sin_addr.s_addr;
		freeaddrinfo(res);
	}
	return (addr);
}

void Server::closeAllFd()
{
	std::size_t i = 0;

	while (i < _listeners.size())
	{
		if (_listeners[i].fd != -1)
		{
			close(_listeners[i].fd);
			_listeners[i].fd = -1;
		}
		i++;
	}
}

void Server::errorCreateListeningSocket(int fd, const std::string &syscall)
{
	int saved = errno;
	if (fd != -1)
		close(fd);
	closeAllFd();
	std::ostringstream msg;
	msg << syscall << ": " << std::strerror(saved);
	throw std::runtime_error(msg.str());
}

void Server::run()
{
	std::vector<Event> events;

	registerListeners();
	_running = true;
	while (_running && g_signalReceived == 0)
	{
		/*
		** Block indefinitely only while no client can time out. With one
		** connected, poll has to come back on its own so closeTimedOut() gets
		** a chance to run.
		*/
		int ready = _events.wait(events,
								 _clients.empty() ? -1 : POLL_TIMEOUT_MS);

		if (ready == -1)
		{
			if (g_signalReceived != 0)
				break ;
			LOG_ERROR << "poll error=failed action=shutdown";
			break;
		}
		LOG_DEBUG << "poll ready=" << ready;
		/*
		** Last resort. dispatchSafely() already contains anything one client
		** can throw, so reaching here means the failure was outside a single
		** connection. Log it and take the next poll round rather than letting
		** it unwind out of run().
		*/
		try
		{
			for (std::size_t i = 0; i < events.size(); ++i)
				dispatchSafely(events[i]);
			checkCgiTimeouts();
			closeTimedOut();
			_sessions.sweep(std::time(NULL));
			flushPendingClose();
		}
		catch (const std::exception &e)
		{
			LOG_ERROR << "event_loop error=\"" << e.what() << "\"";
		}
	}
}

/*
** One failing client must not take the whole server down (General rules: the
** program "must not crash under any circumstances (even if it runs out of
** memory)").
**
** dispatch() allocates in several places -- the receive buffer, the send
** buffer, serialize(), and reading a file into memory -- so std::bad_alloc is
** reachable. Anything thrown ends that one connection and nothing else.
*/
void Server::dispatchSafely(const Event &event)
{
	try
	{
		dispatch(event);
	}
	catch (const std::exception &e)
	{
		LOG_ERROR << "dispatch fd=" << event.fd
				  << " error=\"" << e.what() << "\"";
		scheduleClose(event.fd);
	}
	catch (...)
	{
		LOG_ERROR << "dispatch fd=" << event.fd
				  << " error=\"unknown exception\"";
		scheduleClose(event.fd);
	}
}

void Server::registerListeners()
{
	for (std::size_t i = 0; i < _listeners.size(); ++i)
		_events.add(_listeners[i].fd, EVENT_READ);
}

bool Server::findListenerIndex(int fd, std::size_t &index) const
{
	for (std::size_t i = 0; i < _listeners.size(); ++i)
	{
		if (_listeners[i].fd == fd)
		{
			index = i;
			return (true);
		}
	}
	return (false);
}

void Server::dispatch(const Event &event)
{
	std::size_t listenerIndex;

	LOG_DEBUG << "event fd=" << event.fd
			  << " types=" << eventTypesToString(event.types);
	if (findListenerIndex(event.fd, listenerIndex))
	{
		if (event.types & EVENT_READ)
			acceptClient(listenerIndex);
		return;
	}

	std::map<int, int>::iterator owner = _cgiOwner.find(event.fd);

	if (owner != _cgiOwner.end())
	{
		dispatchCgi(owner->second, event);
		return;
	}

	std::map<int, Client>::iterator client = _clients.find(event.fd);

	if (client == _clients.end())
		return;
	if (client->second.cgiActive)
	{
		if (!client->second.closing)
			clientEventDuringCgi(event.fd, event);
		return;
	}
	if ((event.types & EVENT_READ) && !client->second.closing)
		receiveRequest(event.fd);
	if ((event.types & EVENT_WRITE) && !client->second.closing)
		sendResponse(event.fd);
	if ((event.types & EVENT_ERROR) && !client->second.closing)
		scheduleClose(event.fd);
}

void Server::acceptClient(std::size_t listenerIndex)
{
	int fd = accept(_listeners[listenerIndex].fd, NULL, NULL);

	if (fd == -1)
	{
		LOG_DEBUG << "accept result=empty";
		return;
	}
	
	if (fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
	{
		LOG_ERROR << "accept fd=" << fd << " error=fcntl action=drop";
		close(fd);
		return;
	}

	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		LOG_ERROR << "accept fd=" << fd << " error=fcntl action=drop";
		close(fd);
		return;
	}

	/*
	** Without this, any response that leaves in more than one send() stalls on
	** a keep-alive connection: Nagle withholds the trailing segment until the
	** peer ACKs, and the peer delays that ACK ~40 ms. setResponse() already
	** coalesces small bodies for the common case; this covers what it cannot,
	** such as a large body whose final send() is a short remainder.
	*/
	int	nodelay = 1;

	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay)) == -1)
		LOG_ERROR << "accept fd=" << fd << " error=setsockopt(TCP_NODELAY)";

	Client client;

	client.fd = fd;
	client.listenerIndex = listenerIndex;
	client.connectedAt = std::time(NULL);
	client.lastActivity = client.connectedAt;

	client.parser.setBodyLimit(
		_config.clientMaxBodySize(_listeners[listenerIndex].configIndex));
	_clients[fd] = client;
	_events.add(fd, EVENT_READ);
	LOG_INFO << "accept fd=" << fd
			 << " host=" << _listeners[listenerIndex].host
			 << " port=" << _listeners[listenerIndex].port;
}

/*
** Stops reading from a client for good, because recv() reported that nothing
** more will arrive. Leaving READ interest on would spin: at end of file poll()
** reports the fd readable on every round.
**
** With nothing left to write the connection is finished; otherwise the write
** side keeps its turn, which is what recv() == 0 asks for. A peer that does
** shutdown(SHUT_WR) and then waits for the answer is saying "I am done
** sending", not "drop the response".
*/
void Server::stopReading(int fd)
{
	Client &client = _clients[fd];

	if (client.sentSize == client.responseSize())
	{
		scheduleClose(fd);
		return;
	}
	_events.modify(fd, EVENT_WRITE);
	LOG_DEBUG << "interest fd=" << fd << " events=WRITE";
}

void Server::receiveRequest(int fd)
{
	char buffer[RECV_BUFFER_SIZE];
	ssize_t received = recv(fd, buffer, sizeof(buffer), 0);

	if (received <= 0)
	{
		LOG_DEBUG << "recv fd=" << fd << " bytes=" << received
				  << " action=stop_reading";
		stopReading(fd);
		return;
	}

	Client &client = _clients[fd];

	client.lastActivity = std::time(NULL);
	LOG_DEBUG << "recv fd=" << fd << " bytes=" << received;

	/*
	** Once a response exists the request it answers is over, so nothing that
	** arrives now may reach handle() again. append() returns PARSE_COMPLETE
	** without looking at the data when the parser is already complete, so
	** without this the response below would be rebuilt and restarted from
	** byte zero in the middle of being sent.
	**
	** On a keep-alive connection those bytes are the *next* request, so they
	** are parked instead of dropped and replayed once the response is out.
	** Dropping them would lose a pipelined request; parsing them now would
	** rebuild the response being sent.
	**
	** When the connection is closing the bytes are still read and dropped
	** rather than left unread: close() on a socket that still holds unread
	** data sends an RST, which would discard whatever the peer has not picked
	** up yet -- including the response.
	*/
	if (client.hasResponse())
	{
		if (client.keepAlive)
		{
			/*
			** The park has to be bounded, otherwise a client that never reads
			** its response can make us buffer without limit. One header block
			** is enough for a pipelined request to be worth keeping.
			*/
			if (client.pendingInput.size() + static_cast<std::size_t>(received) > REQUEST_HEADER_LIMIT)
			{
				LOG_INFO << "pipeline fd=" << fd
						 << " bytes=" << REQUEST_HEADER_LIMIT
						 << " limit_exceeded=true action=close";
				scheduleClose(fd);
				return;
			}
			client.pendingInput.append(buffer,
									   static_cast<std::size_t>(received));
			LOG_DEBUG << "pipeline fd=" << fd << " bytes=" << received
					  << " action=park";
			return;
		}
		LOG_DEBUG << "pipeline fd=" << fd << " bytes=" << received
				  << " action=drop";
		return;
	}

	ParseStatus status = client.parser.append(buffer,
											  static_cast<std::size_t>(received));

	if (status == PARSE_INCOMPLETE)
	{
		classifyCgi(fd);
		return;
	}
	respond(fd, status);
}

void Server::classifyCgi(int fd)
{
	Client &client = _clients[fd];

	if (client.parser.headersComplete() == false || client.cgiClassified)
		return;
	client.cgiClassified = true;

	const ServerId id = _listeners[client.listenerIndex].configIndex;

	client.cgiBound = _handler.isCgiRoute(id, client.parser.getRequest());
	if (client.cgiBound == false)
		return;
	if (acquireCgiSlot(fd) == false)
	{
		_events.modify(fd, 0);
		client.throttled = true;
		_cgiQueue.push_back(fd);
		LOG_DEBUG << "cgi fd=" << fd << " slot=busy body_read=deferred";
	}
}

bool Server::acquireCgiSlot(int fd)
{
	Client &client = _clients[fd];

	if (client.cgiSlot)
		return (true);
	if (_activeCgi >= MAX_CONCURRENT_CGI)
		return (false);
	_activeCgi++;
	client.cgiSlot = true;
	return (true);
}

void Server::releaseCgiSlot(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);

	if (it == _clients.end() || it->second.cgiSlot == false)
		return;
	it->second.cgiSlot = false;
	if (_activeCgi > 0)
		_activeCgi--;
	pumpCgiQueue();
}


/*
** Turns a finished parse into the response to send.
**
** Split out of receiveRequest() because resetForNextRequest() needs the same
** path: on a keep-alive connection the next request can already be sitting in
** the parser buffer, with no further recv() coming to drive it.
*/
void Server::respond(int fd, ParseStatus status)
{
	Client &client = _clients[fd];
	const ServerId id = _listeners[client.listenerIndex].configIndex;
	HttpResponse response;

	if (status == PARSE_ERROR)
	{
		response = _handler.makeError(id, client.parser.getErrorStatus());
		LOG_INFO << "request fd=" << fd << " parse_error=true"
				 << " status=" << response.statusCode;
	}
	else
	{
		const HttpRequest &request = client.parser.getRequest();

		HandleResult result = _handler.handle(id, request);

		if (result.pending)
		{
			/*
			** handle() has already run, so nothing needs the body on the
			** parser's request any more and it is moved rather than copied.
			** `request` stays valid: only the body is taken.
			*/
			client.cgiRequest.takeFrom(client.parser.mutableRequest());
			LOG_INFO << "request fd=" << fd << " method=" << request.method
					 << " path=" << request.path << " handler=cgi";
			startCgi(fd, result.cgi);
			return;
		}
		response = result.response;
		LOG_INFO << "request fd=" << fd << " method=" << request.method
				 << " path=" << request.path
				 << " status=" << response.statusCode;
	}
	releaseCgiSlot(fd);

	/*
	** isKeepAlive() needs the completed request, so this must run before
	** resetForNextRequest() clears it. A parse error leaves the byte stream
	** out of sync, so that connection is never reused.
	*/
	client.servedRequests++;
	client.keepAlive = (status != PARSE_ERROR) && client.parser.isKeepAlive() && client.servedRequests < CLIENT_MAX_REQUESTS;
	response.headers[FIELD_CONNECTION] = client.keepAlive ? "keep-alive" : "close";

	client.setResponse(response);
	_events.modify(fd, EVENT_READ | EVENT_WRITE);
	LOG_DEBUG << "interest fd=" << fd << " events=READ|WRITE";
}

void Server::startCgi(int fd, const CgiRequest &cgi)
{
	Client &client = _clients[fd];

	client.cgiActive = true;
	client.cgi = CgiSession();
	client.cgi.request = cgi;
	/*
	** Reading, not to parse -- the request being answered has already been
	** read -- but because the socket is the only thing that can say the peer
	** has gone. Dropping it from the poll set is what let a script keep
	** running for output nobody would read (IV.1: "properly handle client
	** disconnections when necessary").
	*/
	_events.modify(fd, EVENT_READ);
	if (acquireCgiSlot(fd) == false)
	{
		client.throttled = true;
		_cgiQueue.push_back(fd);
		LOG_DEBUG << "cgi fd=" << fd << " queued=true running=" << _activeCgi;
		return;
	}
	client.throttled = false;
	launchCgi(fd);
}

void Server::launchCgi(int fd)
{
	Client &client = _clients[fd];

	if (_cgiExecutor.start(client.cgi.request, client.cgiRequest, client.cgi) == false)
	{
		LOG_ERROR << "cgi fd=" << fd << " error=start_failed";
		client.cgi.errorStatus = STATUS_INTERNAL_SERVER_ERROR;
		completeCgi(fd, false);
		return;
	}
	if (client.cgi.inFd >= 0)
	{
		_cgiOwner[client.cgi.inFd] = fd;
		_events.add(client.cgi.inFd, EVENT_WRITE);
	}
	_cgiOwner[client.cgi.outFd] = fd;
	_events.add(client.cgi.outFd, EVENT_READ);
	LOG_DEBUG << "cgi fd=" << fd << " started=true pid=" << client.cgi.pid
			  << " running=" << _activeCgi << "/" << MAX_CONCURRENT_CGI;
}

void Server::dispatchCgi(int fd, const Event &event)
{
	std::map<int, Client>::iterator it = _clients.find(fd);

	if (it == _clients.end() || it->second.cgiActive == false)
		return;

	Client &client = it->second;

	if (event.fd == client.cgi.inFd && (event.types & (EVENT_WRITE | EVENT_ERROR)))
	{
		if (_cgiExecutor.writeInput(client.cgi) == false)
		{
			_events.remove(event.fd);
			_cgiOwner.erase(event.fd);
			client.cgi.inFd = -1;
		}
		return;
	}
	if (event.fd != client.cgi.outFd)
		return;

	bool eof = false;

	if (_cgiExecutor.readOutput(client.cgi, eof) == false)
	{
		completeCgi(fd, false);
		return;
	}
	if (eof)
		completeCgi(fd, true);
}

/*
** The client socket while its CGI runs. Two things can arrive on it, and they
** mean opposite things.
**
** A FIN or an error means nobody is left to read the answer, so the script is
** killed now instead of running its deadline out -- otherwise up to a minute
** of CPU and one of MAX_CONCURRENT_CGI slots go to a dead connection.
**
** Bytes mean a pipelined next request. They are parked under the same bound
** as the ones that arrive while a response is being sent, and replayed by
** resetForNextRequest() once this response is out. Handing them to the parser
** here would rebuild the very request the running CGI was started from.
**
** recv() < 0 is deliberately ignored. Telling a spurious wakeup from a real
** error needs errno, which may not be read after recv(), and a real error
** raises POLLERR/POLLHUP too -- which arrives here as EVENT_ERROR. Guessing
** would SIGKILL a healthy script.
*/
void Server::clientEventDuringCgi(int fd, const Event &event)
{
	Client	&client = _clients[fd];

	if (event.types & EVENT_ERROR)
	{
		abandonCgi(fd);
		return;
	}
	if ((event.types & EVENT_READ) == 0)
		return;

	char	buffer[RECV_BUFFER_SIZE];
	ssize_t	received = recv(fd, buffer, sizeof(buffer), 0);

	if (received == 0)
	{
		abandonCgi(fd);
		return;
	}
	if (received < 0)
		return;
	if (client.pendingInput.size() + static_cast<std::size_t>(received)
		> REQUEST_HEADER_LIMIT)
	{
		LOG_INFO << "pipeline fd=" << fd << " bytes=" << REQUEST_HEADER_LIMIT
				 << " limit_exceeded=true action=close";
		abandonCgi(fd);
		return;
	}
	client.pendingInput.append(buffer, static_cast<std::size_t>(received));
	LOG_DEBUG << "pipeline fd=" << fd << " bytes=" << received
			  << " source=cgi action=park";
}

/*
** Tears the CGI down for a client that is no longer there.
**
** Unlike completeCgi() there is no response to build, because the socket it
** would go to is gone. Releasing the slot is the part that matters beyond
** this one connection: a queued request starts now rather than after this
** script's deadline expires.
**
** Also reached by a session still queued for a slot, which owns no process
** and no pipes yet -- releaseCgi() and abort() both tolerate that, and
** marking the client closing first makes pumpCgiQueue() skip its stale
** entry.
*/
void Server::abandonCgi(int fd)
{
	Client &client = _clients[fd];

	LOG_INFO << "cgi fd=" << fd << " client_gone=true pid=" << client.cgi.pid;
	releaseCgi(fd);
	_cgiExecutor.abort(client.cgi);
	std::string().swap(client.cgi.input);
	std::string().swap(client.cgi.output);
	client.cgiRequest.clear();
	scheduleClose(fd);
	releaseCgiSlot(fd);
}

void Server::completeCgi(int fd, bool reachedEof)
{
	Client &client = _clients[fd];
	HttpResponse cgiResponse;
	bool ok = false;

	releaseCgi(fd);
	if (reachedEof)
		ok = _cgiExecutor.finish(client.cgi, cgiResponse);
	else
		_cgiExecutor.abort(client.cgi);

	const ServerId id = _listeners[client.listenerIndex].configIndex;

	// In place: cgiResponse goes in as the CGI's output and comes back out as
	// the response to send, so its body is never copied.
	_handler.finishCgi(id, client.cgiRequest, ok,
			client.cgi.errorStatus ? client.cgi.errorStatus : STATUS_BAD_GATEWAY,
			cgiResponse);

	HttpResponse &response = cgiResponse;

	LOG_INFO << "cgi fd=" << fd << " status=" << response.statusCode;
	client.servedRequests++;
	client.keepAlive = client.parser.isKeepAlive()
		&& client.servedRequests < CLIENT_MAX_REQUESTS;
	response.headers[FIELD_CONNECTION] = client.keepAlive ? "keep-alive" : "close";
	client.setResponse(response);
	client.lastActivity = std::time(NULL);
	_events.modify(fd, EVENT_READ | EVENT_WRITE);

	std::string().swap(client.cgi.input);
	std::string().swap(client.cgi.output);
	client.cgiRequest.clear();

	releaseCgiSlot(fd);
}

void Server::releaseCgi(int fd)
{
	Client &client = _clients[fd];

	if (client.cgi.inFd >= 0)
	{
		_events.remove(client.cgi.inFd);
		_cgiOwner.erase(client.cgi.inFd);
	}
	if (client.cgi.outFd >= 0)
	{
		_events.remove(client.cgi.outFd);
		_cgiOwner.erase(client.cgi.outFd);
	}
	client.cgiActive = false;
}

void Server::pumpCgiQueue()
{
	while (!_cgiQueue.empty() && _activeCgi < MAX_CONCURRENT_CGI)
	{
		int next = _cgiQueue.front();

		_cgiQueue.erase(_cgiQueue.begin());

		std::map<int, Client>::iterator it = _clients.find(next);

		if (it == _clients.end() || it->second.closing)
			continue ;
		if (acquireCgiSlot(next) == false)
			break ;
		if (it->second.cgiActive)
			launchCgi(next);
		else
		{
			it->second.throttled = false;
			it->second.lastActivity = std::time(NULL);
			it->second.connectedAt = it->second.lastActivity;
			_events.modify(next, EVENT_READ);
			LOG_DEBUG << "cgi fd=" << next << " body_read=resumed";
		}
	}
}

void Server::sendResponse(int fd)
{
	Client &client = _clients[fd];
	const std::size_t total = client.responseSize();

	if (client.sentSize == total)
	{
		scheduleClose(fd);
		return;
	}

	/*
	** Head and body live in separate buffers (see Client), so one send() works
	** on whichever of the two the cursor is currently in. The head is short
	** and goes first; everything after it is body.
	*/
	const char *data;
	std::size_t remaining;

	if (client.sentSize < client.sendHead.size())
	{
		data = client.sendHead.data() + client.sentSize;
		remaining = client.sendHead.size() - client.sentSize;
	}
	else
	{
		const std::size_t offset = client.sentSize - client.sendHead.size();

		data = client.sendBody.data() + offset;
		remaining = client.sendBody.size() - offset;
	}

	ssize_t sent = send(fd, data, remaining, 0);

	if (sent <= 0)
	{
		LOG_DEBUG << "send fd=" << fd << " bytes=" << sent
				  << " action=close";
		scheduleClose(fd);
		return;
	}

	client.sentSize += static_cast<std::size_t>(sent);
	client.lastActivity = std::time(NULL);
	LOG_DEBUG << "send fd=" << fd << " progress=" << client.sentSize
			  << "/" << total;
	if (client.sentSize == total)
	{
		LOG_INFO << "response fd=" << fd << " bytes=" << total
				 << " sent=true";
		if (client.keepAlive == false)
		{
			scheduleClose(fd);
			return;
		}
		resetForNextRequest(fd);
	}
}

/*
** Prepares a keep-alive connection for the request after the one just sent.
**
** RequestParser::reset() keeps its buffer on purpose, so anything pipelined
** into the same recv() is still there. Together with the bytes parked while
** the response was going out, that can already be a whole request -- and the
** peer is waiting for our answer, so no further recv() will arrive to drive
** it. Both are therefore fed back in here rather than waiting for readiness.
*/
void Server::resetForNextRequest(int fd)
{
	Client &client = _clients[fd];

	client.parser.reset();
	client.cgiClassified = false;
	client.cgiBound = false;
	client.clearResponse();
	client.lastActivity = std::time(NULL);
	/*
	** The total-time cap is per exchange, not per connection: a browser holds
	** a keep-alive socket open for minutes and must not be cut at 60 seconds
	** just for being reused. The idle clock still bounds a silent peer.
	*/
	client.connectedAt = client.lastActivity;
	_events.modify(fd, EVENT_READ);
	LOG_DEBUG << "interest fd=" << fd << " events=READ keep_alive=true"
			  << " served=" << client.servedRequests;

	ParseStatus status;

	if (client.pendingInput.empty())
		status = client.parser.resume();
	else
	{
		std::string parked;

		parked.swap(client.pendingInput);
		status = client.parser.append(parked.data(), parked.size());
	}
	if (status != PARSE_INCOMPLETE)
	{
		LOG_DEBUG << "request fd=" << fd
				  << " source=pipeline action=process";
		respond(fd, status);
	}
}

/*
** Enforces the CGI deadlines, which closeTimedOut() below cannot: a client
** waiting on a CGI is excluded from the client clocks, because it is the
** script that owes the answer and the peer has done nothing wrong.
**
** Only a session with a live process is examined. One still queued for a slot
** (pid < 0) is left alone deliberately: waiting behind MAX_CONCURRENT_CGI is
** congestion, not a hang, and it cannot last forever because every slot is
** already bounded -- while the body is read by the client clocks, and once
** the script runs by the deadlines here. Killing on queue time would answer
** 504 to requests whose only fault is arriving during a busy patch.
*/
void Server::checkCgiTimeouts()
{
	const std::time_t				now = std::time(NULL);
	std::vector<int>				expired;
	std::map<int, Client>::iterator	it;

	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		const CgiSession &session = it->second.cgi;

		if (it->second.cgiActive == false || session.pid <= 0)
			continue ;
		if (now < session.idleDeadline && now < session.totalDeadline)
			continue ;
		expired.push_back(it->first);
	}

	/*
	** Collected first and killed after, because completeCgi() releases the
	** slot, which pumps the queue, which can launch the next CGI -- all of it
	** reaching back into _clients while the sweep above would still be
	** walking it.
	*/
	for (std::size_t i = 0; i < expired.size(); ++i)
	{
		std::map<int, Client>::iterator client = _clients.find(expired[i]);

		if (client == _clients.end() || client->second.cgiActive == false)
			continue ;

		const bool stalled = (now >= client->second.cgi.idleDeadline);

		LOG_INFO << "cgi fd=" << expired[i] << " timeout=true"
				 << " reason=" << (stalled ? "no_progress" : "total")
				 << " pid=" << client->second.cgi.pid;
		/*
		** completeCgi() does the rest: abort() kills the process and reaps
		** it, finishCgi() turns this status into the error page, and the
		** response leaves on the normal write path.
		*/
		client->second.cgi.errorStatus = STATUS_GATEWAY_TIMEOUT;
		completeCgi(expired[i], false);
	}
}

/*
** Enforces the two deadlines on every open client ("A request to your server
** should never hang indefinitely").
**
** Nothing here depends on an event arriving, which is the whole point: a peer
** that connects and then says nothing produces no readiness at all.
*/
void Server::closeTimedOut()
{
	const std::time_t now = std::time(NULL);
	std::map<int, Client>::iterator it;

	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client &client = it->second;

		if (client.closing)
			continue;

		const long idle = static_cast<long>(
			std::difftime(now, client.lastActivity));
		const long total = static_cast<long>(
			std::difftime(now, client.connectedAt));

		if (client.throttled || client.cgiActive)
			continue ;
		if (idle < CLIENT_IDLE_TIMEOUT_SEC && total < CLIENT_TOTAL_TIMEOUT_SEC)
			continue;
		LOG_INFO << "timeout fd=" << it->first << " idle=" << idle
				 << "s total=" << total << "s";
		/*
		** An empty send buffer means the request never completed, which is
		** what 408 describes. Once a response is on its way there is no room
		** for a status line, so drop the connection instead.
		*/
		/*
		** 408 describes a request that started and never finished. A reused
		** connection sitting idle between requests has not started one, so it
		** is simply closed -- answering 408 there would look like a failure
		** for a request the client never sent.
		*/
		if (!client.hasResponse() && client.parser.hasPartialRequest())
			respondTimeout(it->first);
		else
			scheduleClose(it->first);
	}
}

/*
** Answers 408 and stops reading. The client is not marked closing, so the
** normal write path gets one poll round to flush the response; if the peer
** never becomes writable, the next closeTimedOut() takes the other branch and
** closes for good.
*/
void Server::respondTimeout(int fd)
{
	Client &client = _clients[fd];
	const ServerId id = _listeners[client.listenerIndex].configIndex;
	HttpResponse response = _handler.makeError(id, STATUS_REQUEST_TIMEOUT);

	response.headers[FIELD_CONNECTION] = "close";
	/*
	** The header alone is not the decision -- sendResponse() closes on
	** client.keepAlive, and nothing between requests clears it, so a reused
	** connection still carries the true its previous request set. Announcing
	** "close" and then keeping the socket open made the header a lie and left
	** the peer waiting a whole idle sweep for an EOF that had been promised.
	**
	** The two are set together here because this is the one response path
	** that does not run through respond() or completeCgi(), which compute the
	** flag and the header from each other.
	*/
	client.keepAlive = false;
	client.setResponse(response);
	_events.modify(fd, EVENT_WRITE);
	LOG_INFO << "timeout_response fd=" << fd
			 << " status=" << response.statusCode;
}

void Server::scheduleClose(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);

	if (it == _clients.end() || it->second.closing)
		return;
	it->second.closing = true;
	_pendingClose.push_back(fd);
	LOG_DEBUG << "close fd=" << fd << " scheduled=true";
}

void Server::flushPendingClose()
{
	/*
	** Take the fd off the list before closing it. Clearing the list only at
	** the end would leave already closed fds queued if anything in between
	** threw, and the second close() would land on whatever client inherited
	** the number in the meantime.
	*/
	while (!_pendingClose.empty())
	{
		int fd = _pendingClose.back();

		_pendingClose.pop_back();
		_events.remove(fd);
		close(fd);
		releaseCgiSlot(fd);
		_clients.erase(fd);
		LOG_INFO << "close fd=" << fd << " completed=true";
	}
}
