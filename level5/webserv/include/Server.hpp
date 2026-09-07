#ifndef SERVER_HPP
# define SERVER_HPP

# define RECV_BUFFER_SIZE 65536

/*
** Seconds a client may stay silent, and seconds one exchange may take in
** total. Both are needed: see the comment on Client::lastActivity.
*/
# define CLIENT_IDLE_TIMEOUT_SEC 10
# define CLIENT_TOTAL_TIMEOUT_SEC 60

/*
** How long poll() may block while clients are connected. Nothing wakes the
** loop on the passage of time alone, so an infinite wait would make the
** timeouts above unreachable.
*/
# define POLL_TIMEOUT_MS 1000

# include <vector>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <cerrno>
# include <cstring>
# include <set>
# include <sstream>
# include <ctime>
# include <sys/types.h>
# include <netdb.h>
# include "IConfig.hpp"
# include "IEventManager.hpp"
# include "PollEventManager.hpp"
# include "Client.hpp"
# include "RequestHandler.hpp"
# include "CgiExecutor.hpp"
# include "SessionStore.hpp"

# define CLIENT_MAX_REQUESTS 100
# define MAX_CONCURRENT_CGI 10

struct Listener
{
	int			fd;
	std::string	host;
	int			port;
	std::size_t	configIndex;

	Listener() : fd(-1), port(0), configIndex(0) {}
};

class Server {
	private:
		const IConfig				&_config;
		std::vector<Listener>		_listeners;
		PollEventManager			_events;
		std::map<int, Client>		_clients;
		std::vector<int>			_pendingClose;
		std::map<int, int>			_cgiOwner;
		std::vector<int>			_cgiQueue;
		std::size_t					_activeCgi;
		CgiExecutor					_cgiExecutor;
		SessionStore				_sessions;
		RequestHandler				_handler;
		bool						_running;


		Server();
		Server(const Server& other);
		Server& operator=(const Server& other);

		void				initialize();
		void				createListeningSocket();
		struct sockaddr_in	makeAddr(const ServerConfig &conf);
		void				closeAllFd();
		void				errorCreateListeningSocket(int fd, const std::string &syscall);

		void				registerListeners();
		bool				findListenerIndex(int fd, std::size_t &index) const;
		void				dispatchSafely(const Event &event);
		void				dispatch(const Event &event);
		void				acceptClient(std::size_t listenerIndex);
		void				receiveRequest(int fd);
		void				respond(int fd, ParseStatus status);
		void				startCgi(int fd, const CgiRequest &cgi);
		void				launchCgi(int fd);
		void				dispatchCgi(int fd, const Event &event);
		void				clientEventDuringCgi(int fd, const Event &event);
		void				abandonCgi(int fd);
		void				completeCgi(int fd, bool ok);
		void				releaseCgi(int fd);
		void				pumpCgiQueue();
		bool				acquireCgiSlot(int fd);
		void				releaseCgiSlot(int fd);
		void				classifyCgi(int fd);
		void				resetForNextRequest(int fd);
		void				stopReading(int fd);
		void				sendResponse(int fd);
		void				checkCgiTimeouts();
		void				closeTimedOut();
		void				respondTimeout(int fd);
		void				scheduleClose(int fd);
		void				flushPendingClose();

	public:
		Server(const IConfig &config);
		~Server();

		void	run();
};

#endif
