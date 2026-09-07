#ifndef CGI_EXECUTOR_HPP
#define CGI_EXECUTOR_HPP

#include <string>
#include <vector>

#include "ICgiExecutor.hpp"

class CgiExecutor : public ICgiExecutor
{
public:
	CgiExecutor();
	virtual ~CgiExecutor();

	virtual bool start(const CgiRequest &cgi, HttpRequest &request,
					   CgiSession &session);
	virtual bool writeInput(CgiSession &session);
	virtual bool readOutput(CgiSession &session, bool &eof);
	virtual bool finish(CgiSession &session, HttpResponse &response);
	virtual void abort(CgiSession &session);

private:
	bool createPipes(int cgiStdinPipe[2], int cgiStdoutPipe[2]);

	pid_t startProcess(
		const CgiRequest &cgi,
		const HttpRequest &request,
		int cgiStdinPipe[2],
		int cgiStdoutPipe[2]);

	bool setupChildProcess(
		const CgiRequest &cgi,
		const HttpRequest &request,
		int cgiStdinPipe[2],
		int cgiStdoutPipe[2]);

	// Consumes `output`: the body is moved out of it, not copied.
	bool parseCgiOutput(
		std::string &output,
		HttpResponse &response);

	// `scriptFilename` becomes SCRIPT_FILENAME: how the script is named from
	// the directory the child runs in, which a chdir changes.
	std::vector<std::string> buildEnvironment(
		const CgiRequest &cgi,
		const HttpRequest &request,
		const std::string &scriptFilename);
};

#endif
