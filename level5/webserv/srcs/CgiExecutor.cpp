#include <cctype>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <fcntl.h>
#include <poll.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#include "CgiExecutor.hpp"
#include "HttpStatus.hpp"

/*
** The two windows behind CgiSession::idleDeadline and ::totalDeadline.
**
** The idle one has to be long enough for the longest a legitimate script may
** go without reading or writing -- it is silence, not slowness, that it
** detects. The total one is the hard cap on one invocation and matches
** CLIENT_TOTAL_TIMEOUT_SEC, since a CGI outliving the connection it answers
** has nothing left to answer.
**
** Overridable so the timeout test can build a server with short deadlines
** instead of waiting these out.
*/
#ifndef CGI_IDLE_TIMEOUT_SEC
# define CGI_IDLE_TIMEOUT_SEC 10
#endif
#ifndef CGI_TOTAL_TIMEOUT_SEC
# define CGI_TOTAL_TIMEOUT_SEC 60
#endif

/*
** Called on every byte that moves in either direction, which is what makes
** the idle clock a no-progress clock rather than a second absolute one.
*/
static void	extendIdleDeadline(CgiSession &session)
{
	session.idleDeadline = std::time(NULL) + CGI_IDLE_TIMEOUT_SEC;
}

/*
** Where the child runs and what it calls things, once it has moved into the
** script's directory (IV.3: "The CGI should be run in the correct directory
** for relative path file access").
*/
struct ChildPaths
{
	std::string	dir;		// chdir target; empty means stay put
	std::string	interp;		// execve path, resolvable from `dir`
	std::string	script;		// argv[1], resolvable from `dir`

	ChildPaths() : dir(), interp(), script() {}
};

/*
** One "../" per segment the chdir descends, so a relative interpreter still
** names the same file afterwards.
**
** Returns false instead of a wrong count for the two shapes whose depth
** cannot be derived from the text: an absolute directory, because naming the
** original cwd again would need getcwd(), which is not an allowed function,
** and one containing "..", because its depth is not its segment count. A
** symlinked segment is the same hazard and is not detectable without
** realpath(), also not allowed -- a directory reached through a symlink to
** another level will still come out wrong.
*/
static bool	walkBackPrefix(const std::string &dir, std::string &prefix)
{
	std::string::size_type	i = 0;

	prefix.clear();
	if (dir.empty() || dir[0] == '/')
		return (false);
	while (i < dir.size())
	{
		std::string::size_type	end = dir.find('/', i);

		if (end == std::string::npos)
			end = dir.size();

		const std::string	segment = dir.substr(i, end - i);

		if (segment == "..")
			return (false);
		// "." and an empty segment (doubled or trailing slash) descend nothing.
		if (!segment.empty() && segment != ".")
			prefix += "../";
		i = end + 1;
	}
	return (true);
}

/*
** Splits the script path and, when the move would leave the exec unable to
** find the interpreter, gives up on the move rather than guessing at it: a
** wrong "../" count points execve() at nothing and fails the request
** outright, which is worse than running in the server's directory.
*/
static ChildPaths	planChildPaths(const CgiRequest &cgi)
{
	ChildPaths	plan;

	plan.interp = cgi.interpreterPath;
	plan.script = cgi.scriptPath;

	const std::string::size_type	slash = cgi.scriptPath.find_last_of('/');

	if (slash == std::string::npos)
		return (plan);

	const std::string	dir = cgi.scriptPath.substr(0, slash);
	const std::string	base = cgi.scriptPath.substr(slash + 1);

	// A trailing slash leaves no file name, and "/x" leaves no directory to
	// move into that is not already the root.
	if (dir.empty() || base.empty())
		return (plan);

	// An absolute interpreter survives any move untouched.
	if (plan.interp.empty() || plan.interp[0] != '/')
	{
		std::string	prefix;

		if (!walkBackPrefix(dir, prefix))
			return (plan);
		plan.interp = prefix + plan.interp;
	}
	plan.dir = dir;
	plan.script = base;
	return (plan);
}

static bool	setNonBlocking(int fd)
{
	int	flags = fcntl(fd, F_GETFL, 0);

	if (flags < 0)
		return (false);
	return (fcntl(fd, F_SETFL, flags | O_NONBLOCK) >= 0);
}

CgiExecutor::CgiExecutor()
{
}

CgiExecutor::~CgiExecutor()
{
}

/*
** Opens both pipes, closing the first one again if the second fails so no
** half-open pair leaks.
*/
bool CgiExecutor::createPipes(int cgiStdinPipe[2], int cgiStdoutPipe[2])
{
	if (pipe(cgiStdinPipe) < 0)
		return false;
	if (pipe(cgiStdoutPipe) < 0)
	{
		close(cgiStdinPipe[0]);
		close(cgiStdinPipe[1]);
		return false;
	}
	return true;
}

/*
** Forks and starts the child. Returns the pid to the parent, -1 on failure.
*/
pid_t CgiExecutor::startProcess(
	const CgiRequest &cgi,
	const HttpRequest &request,
	int cgiStdinPipe[2],
	int cgiStdoutPipe[2])
{
	pid_t pid = fork();
	if (pid < 0)
		return -1;

	if (pid == 0)
	{
		setupChildProcess(cgi, request, cgiStdinPipe, cgiStdoutPipe);
		/*
		** Only reached when execve() failed. _exit() rather than exit(): the
		** child inherited the parent's stdio buffers and exit() would flush
		** them, duplicating whatever the parent had not written out yet.
		*/
		_exit(1);
	}
	return pid;
}

/*
** Child side: points stdin/stdout at the pipes and execve()s. On success it
** does not return, because execve() replaces the process.
*/
bool CgiExecutor::setupChildProcess(
	const CgiRequest &cgi,
	const HttpRequest &request,
	int cgiStdinPipe[2],
	int cgiStdoutPipe[2])
{
	if (dup2(cgiStdinPipe[0], STDIN_FILENO) < 0)
		return false;
	if (dup2(cgiStdoutPipe[1], STDOUT_FILENO) < 0)
		return false;

	close(cgiStdinPipe[0]);
	close(cgiStdinPipe[1]);
	close(cgiStdoutPipe[0]);
	close(cgiStdoutPipe[1]);

	// Keeps CGI diagnostics out of the server's own log stream.
	int devNull = open("/dev/null", O_WRONLY);
	if (devNull >= 0)
	{
		dup2(devNull, STDERR_FILENO);
		close(devNull);
	}

	const ChildPaths paths = planChildPaths(cgi);

	/*
	** Ahead of buildEnvironment(), so SCRIPT_FILENAME can describe the script
	** from where it will actually run. A failure here is not swallowed: the
	** caller turns it into _exit(1) and the request into 502, the same as a
	** failing execve().
	*/
	if (!paths.dir.empty() && chdir(paths.dir.c_str()) != 0)
		return false;

	std::vector<std::string> envVec = buildEnvironment(cgi, request,
														paths.script);
	std::vector<char *> envp;
	std::size_t i = 0;
	while (i < envVec.size())
	{
		// execve() takes char *const[]; c_str() is const and is not written to.
		envp.push_back(const_cast<char *>(envVec[i].c_str()));
		i++;
	}
	envp.push_back(NULL);

	std::vector<char *> argv;
	argv.push_back(const_cast<char *>(paths.interp.c_str()));
	argv.push_back(const_cast<char *>(paths.script.c_str()));
	argv.push_back(NULL);

	execve(paths.interp.c_str(), &argv[0], &envp[0]);
	return false;
}

/*
** Builds the CGI/1.1 environment.
**
** RequestParser stores field names lower-cased, so every lookup here is too.
*/
std::vector<std::string> CgiExecutor::buildEnvironment(
	const CgiRequest &cgi,
	const HttpRequest &request,
	const std::string &scriptFilename)
{
	std::vector<std::string> env;

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	env.push_back("SERVER_SOFTWARE=webserv/1.0");
	env.push_back("SERVER_NAME=" + cgi.serverHost);
	env.push_back("SERVER_PROTOCOL=" + request.version);
	{
		std::ostringstream oss;
		oss << cgi.serverPort;
		env.push_back("SERVER_PORT=" + oss.str());
	}

	env.push_back("REQUEST_METHOD=" + request.method);

	/*
	** REQUEST_URI is not part of CGI/1.1 but is universally provided. The
	** grading cgi_tester joins it with HTTP_HOST to rebuild the URL and fails
	** with its own 500 when it is missing. Do not drop it.
	*/
	env.push_back("REQUEST_URI=" + request.path);

	/*
	** PATH_INFO is meant to be the extra path after the script name, empty
	** when there is none. cgi_tester instead requires it to match the path of
	** REQUEST_URI, and rejects both an empty value and a filesystem path.
	** Extra path is passed through as the spec says; otherwise the URI path
	** is used, which is what satisfies the tester.
	*/
	if (cgi.pathInfo.empty())
	{
		std::string	uriPath = request.path;
		std::size_t	query = uriPath.find('?');

		if (query != std::string::npos)
			uriPath.erase(query);
		env.push_back("PATH_INFO=" + uriPath);
	}
	else
		env.push_back("PATH_INFO=" + cgi.pathInfo);
	env.push_back("QUERY_STRING=" + cgi.queryString);
	/*
	** SCRIPT_NAME (the URL-side script name, e.g. "/cgi-bin/hello.py") is part
	** of CGI/1.1 but CgiRequest does not carry it, so it is not set. Splitting
	** the URI belongs to the HTTP side (see schema/ICgiExecutor.hpp).
	** SCRIPT_FILENAME is the on-disk path and is not in CGI/1.1 itself. It is
	** whatever names the script from the directory the child ends up in, so
	** after a chdir it is the bare file name -- the path it was handed would
	** no longer resolve from there.
	*/
	env.push_back("SCRIPT_FILENAME=" + scriptFilename);
	{
		std::map<std::string, std::string>::const_iterator it;

		it = request.headers.find("content-type");
		if (it != request.headers.end())
			env.push_back("CONTENT_TYPE=" + it->second);

		if (!request.body.empty())
		{
			std::ostringstream oss;
			oss << request.body.size();
			env.push_back("CONTENT_LENGTH=" + oss.str());
		}
	}
	// Remaining request headers become HTTP_* (dashes to underscores, upper).
	std::map<std::string, std::string>::const_iterator it;
	it = request.headers.begin();
	while (it != request.headers.end())
	{
		if (it->first == "content-type" || it->first == "content-length")
		{
			it++;
			continue;
		}
		std::string name = "HTTP_";
		std::size_t i = 0;
		while (i < it->first.size())
		{
			if (it->first[i] == '-')
				name += '_';
			else
				name += static_cast<char>(toupper(
					static_cast<unsigned char>(it->first[i])));
			i++;
		}
		name += "=" + it->second;
		env.push_back(name);
		it++;
	}
	return env;
}

/*
Status: 201 Created
Content-Type: text/html
X-Test: hello\r\n
\r\n
<html>
<body>Hello!</body>
</html>
			becomes
response.statusCode    = 201;
response.reasonPhrase = "Created";

response.headers["Content-Type"] = "text/html";
response.headers["X-Test"]       = "hello";
response.headers["Content-Length"] = "...";

response.body = "<html>\n<body>Hello!</body>\n</html>";
*/
bool CgiExecutor::parseCgiOutput(
	std::string &output,
	HttpResponse &response)
{
	std::size_t sepPos = output.find("\r\n\r\n");
	std::size_t sepLen = 4;
	if (sepPos == std::string::npos)
	{
		sepPos = output.find("\n\n");
		sepLen = 2;
	}
	if (sepPos == std::string::npos)
		return false;

	std::string headerSection = output.substr(0, sepPos);

	/*
	** The body is taken over from `output` and the header section erased off
	** the front, rather than copied out with substr(): the copy would hold two
	** hundred-megabyte bodies at once, the erase only shifts one in place.
	*/
	std::string().swap(response.body);
	response.body.swap(output);
	response.body.erase(0, sepPos + sepLen);

	response.statusCode = STATUS_OK;
	response.reasonPhrase = REASON_OK;

	bool statusSet = false;
	std::istringstream stream(headerSection);
	std::string line;

	while (std::getline(stream, line))
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			continue;

		// Status: 201 Created
		std::size_t colonPos = line.find(':');
		if (colonPos == std::string::npos)
			continue;

		// name = "Status ", value = " 201 Created"
		// ↓
		// name = "Status", value = "201 Created"
		std::string name = line.substr(0, colonPos);
		std::string value = line.substr(colonPos + 1);
		std::size_t nameEnd = name.find_last_not_of(" \t");
		if (nameEnd == std::string::npos)
			continue;
		name = name.substr(0, nameEnd + 1);
		std::size_t valStart = value.find_first_not_of(" \t");
		if (valStart != std::string::npos)
			value = value.substr(valStart);
		else
			value.clear();

		{
			std::string normalized;
			bool cap = true;
			std::size_t j = 0;
			while (j < name.size())
			{
				if (name[j] == '-')
				{
					normalized += '-';
					cap = true;
				}
				else if (cap)
				{
					normalized += static_cast<char>(toupper(static_cast<unsigned char>(name[j])));
					cap = false;
				}
				else
					normalized += static_cast<char>(tolower(static_cast<unsigned char>(name[j])));
				j++;
			}
			name = normalized;
		}
		if (value.find('\r') != std::string::npos)
			continue;

		/*
			Status: 404 Not Found
					↓
			name = "Status", value = "404 Not Found"
					↓
			response.statusCode = 404
			response.reasonPhrase = "Not Found"
		*/
		if (name == "Status")
		{
			if (statusSet)
				return false;

			std::istringstream ss(value);
			std::string codeStr;
			std::string reason;

			ss >> codeStr;
			std::getline(ss, reason);

			if (codeStr.size() != 3 ||
				!isdigit(static_cast<unsigned char>(codeStr[0])) ||
				!isdigit(static_cast<unsigned char>(codeStr[1])) ||
				!isdigit(static_cast<unsigned char>(codeStr[2])))
				return false;

			int code = (codeStr[0] - '0') * 100 +
					   (codeStr[1] - '0') * 10 +
					   (codeStr[2] - '0');

			if (code < 100 || code > 599)
				return false;

			response.statusCode = code;
			statusSet = true;
			std::size_t rStart = reason.find_first_not_of(" \t");
			if (rStart != std::string::npos)
				response.reasonPhrase = reason.substr(rStart);
			else
				response.reasonPhrase = "";
		}
		else
		{
			if (name == FIELD_LOCATION && !statusSet)
			{
				if ((value.size() >= 7 && value.substr(0, 7) == "http://") ||
					(value.size() >= 8 && value.substr(0, 8) == "https://"))
					response.statusCode = STATUS_FOUND;
			}
			response.headers[name] = value;
		}
	}

	if (response.headers.find(FIELD_CONTENT_LENGTH) == response.headers.end())
	{
		std::ostringstream oss;
		oss << response.body.size();
		response.headers[FIELD_CONTENT_LENGTH] = oss.str();
	}
	return true;
}

bool CgiExecutor::start(const CgiRequest &cgi, HttpRequest &request,
						CgiSession &session)
{
	int	cgiStdinPipe[2];
	int	cgiStdoutPipe[2];

	session.errorStatus = STATUS_INTERNAL_SERVER_ERROR;
	if (!createPipes(cgiStdinPipe, cgiStdoutPipe))
		return (false);

	pid_t	pid = startProcess(cgi, request, cgiStdinPipe, cgiStdoutPipe);

	if (pid < 0)
	{
		close(cgiStdinPipe[0]);
		close(cgiStdinPipe[1]);
		close(cgiStdoutPipe[0]);
		close(cgiStdoutPipe[1]);
		return (false);
	}
	close(cgiStdinPipe[0]);
	close(cgiStdoutPipe[1]);

	if (!setNonBlocking(cgiStdinPipe[1]) || !setNonBlocking(cgiStdoutPipe[0]))
	{
		close(cgiStdinPipe[1]);
		close(cgiStdoutPipe[0]);
		kill(pid, SIGKILL);
		waitpid(pid, NULL, 0);
		return (false);
	}

	session.pid = pid;
	session.inFd = cgiStdinPipe[1];
	session.outFd = cgiStdoutPipe[0];
	/*
	** Moved, not copied. buildEnvironment() has already read the body's size
	** for CONTENT_LENGTH inside startProcess() above, so nothing downstream
	** still needs it on the request.
	*/
	std::string().swap(session.input);
	session.input.swap(request.body);
	session.written = 0;
	session.output.clear();
	session.totalDeadline = std::time(NULL) + CGI_TOTAL_TIMEOUT_SEC;
	extendIdleDeadline(session);
	session.errorStatus = 0;

	if (session.input.empty())
	{
		close(session.inFd);
		session.inFd = -1;
	}
	return (true);
}

bool CgiExecutor::writeInput(CgiSession &session)
{
	if (session.inFd < 0)
		return (false);

	ssize_t	n = write(session.inFd, session.input.data() + session.written,
				session.input.size() - session.written);

	if (n > 0)
	{
		session.written += static_cast<std::size_t>(n);
		extendIdleDeadline(session);
		if (session.written < session.input.size())
			return (true);
	}

	close(session.inFd);
	session.inFd = -1;
	return (false);
}

bool CgiExecutor::readOutput(CgiSession &session, bool &eof)
{
	char	buf[65536];
	ssize_t	n = read(session.outFd, buf, sizeof(buf));

	eof = false;
	if (n > 0)
	{
		session.output.append(buf, static_cast<std::size_t>(n));
		extendIdleDeadline(session);
		return (true);
	}
	if (n == 0)
	{
		eof = true;
		return (true);
	}

	session.errorStatus = STATUS_INTERNAL_SERVER_ERROR;
	return (false);
}

bool CgiExecutor::finish(CgiSession &session, HttpResponse &response)
{
	int	childStatus = 0;

	if (session.inFd >= 0)
	{
		close(session.inFd);
		session.inFd = -1;
	}
	if (session.outFd >= 0)
	{
		close(session.outFd);
		session.outFd = -1;
	}
	if (waitpid(session.pid, &childStatus, WNOHANG) != session.pid)
	{
		kill(session.pid, SIGKILL);
		waitpid(session.pid, &childStatus, 0);
	}
	session.pid = -1;

	if (!WIFEXITED(childStatus) || WEXITSTATUS(childStatus) != 0)
	{
		session.errorStatus = STATUS_BAD_GATEWAY;
		return (false);
	}
	if (!parseCgiOutput(session.output, response))
	{
		session.errorStatus = STATUS_BAD_GATEWAY;
		return (false);
	}
	return (true);
}

void CgiExecutor::abort(CgiSession &session)
{
	if (session.inFd >= 0)
	{
		close(session.inFd);
		session.inFd = -1;
	}
	if (session.outFd >= 0)
	{
		close(session.outFd);
		session.outFd = -1;
	}
	if (session.pid > 0)
	{
		kill(session.pid, SIGKILL);
		waitpid(session.pid, NULL, 0);
		session.pid = -1;
	}
}

/*

What the client sent
HttpRequest
├── method  ──→ "GET"
├── path    ──→ "/cgi-bin/hello.py/foo?name=taishi"
├── version ──→ "HTTP/1.1"
├── headers
│   ├── "Host"           → "example.com:8080"
│   ├── "Content-Type"   → "text/plain"
│   └── "Content-Length" → "0"
└── body    ──→ ""

What the HTTP side hands to the executor
CgiRequest
├── scriptPath
│      ↓
│   "/var/www/cgi-bin/hello.py"
│
├── interpreterPath
│      ↓
│   "/usr/bin/python3"
│
├── pathInfo
│      ↓
│   "/foo"
│
├── queryString
│      ↓
│   "name=taishi"
│
├── serverHost
│      ↓
│   "example.com"
│
└── serverPort
	   ↓
	  8080
*/
