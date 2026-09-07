#ifndef STUB_CGI_EXECUTOR_HPP
# define STUB_CGI_EXECUTOR_HPP

# include "ICgiExecutor.hpp"

class StubCgiExecutor : public ICgiExecutor
{
public:
    virtual ~StubCgiExecutor() {}

	bool	start(const CgiRequest &cgi, HttpRequest &request, CgiSession &session) {
        (void)cgi;
        (void)request;
        (void)session;
        return true;
    }

	bool	writeInput(CgiSession &session) {
        (void)session;
        return false;
    }

	bool	readOutput(CgiSession &session, bool &eof) {
        CgiRequest &cgi = session.request;
        std::ostringstream body;

        body << "<html><body><h1>CGI Response (stub)</h1>\n"
            //  << "<p>Method: " << request.method << "</p>\n"
             << "<p>Body Size: " << session.input.size() << "</p>\n"
             << "<p>Script Path: " << cgi.scriptPath << "</p>\n"
             << "<p>Interpreter Path: " << cgi.interpreterPath << "</p>\n"
             << "<p>Path Info: " << cgi.pathInfo << "</p>\n"
             << "<p>Query String: " << cgi.queryString << "</p>\n"
             << "<p>Server Host: " << cgi.serverHost << "</p>\n"
             << "<p>Server Port: " << cgi.serverPort << "</p>\n"
             << "</body></html>";
        session.output = body.str();
        eof = true;
        return false;
    }

	bool	finish(CgiSession &session, HttpResponse &response) {
        response.statusCode = 200;
        response.reasonPhrase = "OK";
        response.body = session.output;
        return true;
    }

	void	abort(CgiSession &session) {
        (void)session;
    }

    // bool execute(const CgiRequest &cgi, const HttpRequest &request,
    //                      HttpResponse &response)
    // {
    //     std::ostringstream body;

    //     body << "<html><body><h1>CGI Response (stub)</h1>\n"
    //          << "<p>Method: " << request.method << "</p>\n"
    //          << "<p>Body Size: " << request.body.size() << "</p>\n"
    //          << "<p>Script Path: " << cgi.scriptPath << "</p>\n"
    //          << "<p>Interpreter Path: " << cgi.interpreterPath << "</p>\n"
    //          << "<p>Path Info: " << cgi.pathInfo << "</p>\n"
    //          << "<p>Query String: " << cgi.queryString << "</p>\n"
    //          << "<p>Server Host: " << cgi.serverHost << "</p>\n"
    //          << "<p>Server Port: " << cgi.serverPort << "</p>\n"
    //          << "</body></html>";

    //     response.statusCode = 200;
    //     response.reasonPhrase = "OK";
    //     response.body = body.str();

    //     std::ostringstream contentLength;
    //     contentLength << response.body.size();
    //     response.headers["Content-Type"] = "text/html";
    //     response.headers["Content-Length"] = contentLength.str();
    //     return true;
    // }

    // virtual int getErrorStatus() const
    // {
    //     return 500;
    // }
};

#endif