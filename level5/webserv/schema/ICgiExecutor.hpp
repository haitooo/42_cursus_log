#ifndef ICGI_EXECUTOR_HPP
# define ICGI_EXECUTOR_HPP

# include <csignal>
# include <ctime>
# include <string>
# include <sys/types.h>

# include "ConfigTypes.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

/*
** Boundary between HTTP handling and CGI execution.
**
** Everything process related -- fork / execve / pipe / dup2 / waitpid, the
** environment, and turning CGI output (Status:, Content-Type:) into an HTTP
** response -- lives behind this interface.
**
** Splitting the URI into script path, PATH_INFO and query string stays on the
** HTTP side; the executor trusts the values it is handed.
*/

/*
** Everything needed for one CGI invocation, filled in by the caller.
*/
struct CgiRequest
{
	// Filesystem path of the script, as returned by IConfig::mapUriToPath().
	// Becomes SCRIPT_FILENAME.
	std::string	scriptPath;

	// Interpreter to run the script with, from CgiRule::interpreterPath.
	// Becomes the first argument to execve().
	std::string	interpreterPath;

	// Part of the URI after the script name, empty when there is none.
	// Becomes PATH_INFO.
	std::string	pathInfo;

	// Part of the URI after '?', excluding the '?' itself.
	// Becomes QUERY_STRING.
	std::string	queryString;

	// listen address of the server block that received the request.
	// Becomes SERVER_NAME / SERVER_PORT.
	std::string	serverHost;
	int			serverPort;

	CgiRequest()
		: scriptPath(),
		  interpreterPath(),
		  pathInfo(),
		  queryString(),
		  serverHost(),
		  serverPort(0)
	{
	}
};

struct CgiSession
{
	pid_t		pid;
	int			inFd;
	int			outFd;
	std::string	input;
	std::size_t	written;
	std::string	output;

	/*
	** Two clocks, for the same reason Client keeps two.
	**
	** idleDeadline is pushed back every time the script consumes input or
	** produces output, so it catches the script that has stopped doing
	** anything without ever firing on one that is simply moving a lot of
	** bytes: a hundred-megabyte body takes far longer than any sane
	** no-progress window, and a single absolute deadline would kill it
	** mid-transfer.
	**
	** totalDeadline is never pushed back, because a script that dribbles one
	** byte per second refreshes the first clock forever and would outlive it.
	*/
	std::time_t	idleDeadline;
	std::time_t	totalDeadline;

	int			errorStatus;
	CgiRequest	request;

	CgiSession()
		: pid(-1),
		  inFd(-1),
		  outFd(-1),
		  input(),
		  written(0),
		  output(),
		  idleDeadline(0),
		  totalDeadline(0),
		  errorStatus(0),
		  request()
	{
	}
};

class ICgiExecutor
{
public:
	virtual ~ICgiExecutor() {}

	/*
	** `request` is non-const so the body can be moved into the session rather
	** than copied; on return its body is empty. Everything else is untouched.
	*/
	virtual bool	start(const CgiRequest &cgi, HttpRequest &request,
						CgiSession &session) = 0;

	virtual bool	writeInput(CgiSession &session) = 0;

	virtual bool	readOutput(CgiSession &session, bool &eof) = 0;

	virtual bool	finish(CgiSession &session, HttpResponse &response) = 0;

	virtual void	abort(CgiSession &session) = 0;
};

#endif
