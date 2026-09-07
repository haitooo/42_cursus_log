#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/stat.h>
#include <sstream>

#include "IConfig.hpp"
#include "RequestHandler.hpp"
#include "./StubCgiExecutor.hpp"

// =========================================================================
// Stub Classes
// =========================================================================

class StubConfig : public IConfig
{
public:
    virtual ~StubConfig() {}

    virtual std::size_t serverCount() const { return 1; }

    virtual const ServerConfig &listenAddress(ServerId id) const {
        static ServerConfig addr;
        (void)id;
        addr.host = "127.0.0.1";
        addr.port = 8080;
        return addr;
    }

    virtual std::size_t clientMaxBodySize(ServerId) const {
        return 1024 * 1024;
    }

    virtual bool resolveLocation(ServerId id, const std::string& uriPath, ResolvedLocation& out) const {
        (void)id;
        out.locationPath = "/";
        out.root = "./www";
        out.index = "index.html";
        out.autoindex = false;
        out.clientMaxBodySize = 1024 * 1024;
        out.cgi.enabled = false;
        out.methods.clear(); out.methods.push_back("GET"); out.methods.push_back("POST");

        if (uriPath.find("/cgi-bin/") == 0) {
            out.locationPath = "/cgi-bin";
            out.cgi.enabled = true;
            out.cgi.extension = ".py";
            out.cgi.interpreterPath = "/usr/bin/python3";
            return true;
        }
        return true;
    }

    virtual std::string mapUriToPath(const ResolvedLocation& loc, const std::string& uriPath) const {
        return loc.root + uriPath;
    }

    virtual bool errorPagePath(ServerId, int, std::string&) const {
        return false;
    }
};

// =========================================================================
// Test Framework
// =========================================================================

#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

std::string g_test_name;
int g_passed_tests = 0;

template <typename T, typename U>
void expect_eq(const T& actual, const U& expected, int line) {
    if (actual != expected) {
        std::cerr << "\n" << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name << "\n"
                  << "  Line " << line << " mismatch!\n"
                  << "  Actual:   " << actual << "\n"
                  << "  Expected: " << expected << std::endl;
        std::exit(1);
    }
}
#define EXPECT_EQ(actual, expected) expect_eq(actual, expected, __LINE__)

void expect_contains(const std::string& body, const std::string& expected, int line) {
    if (body.find(expected) == std::string::npos) {
        std::cerr << "\n" << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name << "\n"
                  << "  Line " << line << ": Expected to find substring.\n"
                  << "  Substring: '" << expected << "'\n"
                  << "  Actual Body:\n" << body << std::endl;
        std::exit(1);
    }
}
#define EXPECT_CONTAINS(body, expected) expect_contains(body, expected, __LINE__)


void setup_test_environment() {
    std::system("mkdir -p www/cgi-bin");
    
    std::system("echo 'print(\"hello\")' > www/cgi-bin/hello.py");
    std::system("chmod 755 www/cgi-bin/hello.py");

    std::system("echo 'print(\"secret\")' > www/cgi-bin/no_exec.py");
    std::system("chmod 000 www/cgi-bin/no_exec.py");

    std::system("mkdir -p www/cgi-bin/dir.py");

    std::system("echo 'plain text' > www/cgi-bin/hello.txt");
}

void teardown_test_environment() {
    std::system("chmod 777 www/cgi-bin/no_exec.py 2>/dev/null");
}

void run_test(const std::string& name, void (*test_func)(RequestHandler&)) {
    g_test_name = name;
    std::cout << "[RUN ] " << name << std::endl;
    StubConfig config;
    StubCgiExecutor cgi;
    // RequestHandler holds a SessionStore reference, so it has to
    // outlive the handler. Each test gets a fresh, empty store.
    SessionStore sessions(SESSION_TTL_SEC, SESSION_MAX_COUNT);
    RequestHandler handler(config, cgi, sessions);

    test_func(handler);
    ++g_passed_tests;
    std::cout << COLOR_GREEN << "[ OK ] " << COLOR_RESET << name << std::endl;
}

bool eof;
void test_cgi_simple_uri(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/hello.py";

    CgiSession session; session.request = handler.handle(0, req).cgi;
    StubCgiExecutor cgi; cgi.readOutput(session, eof);
    HttpResponse res; cgi.finish(session, res);
    
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_CONTAINS(res.body, "<p>Script Path: ./www/cgi-bin/hello.py</p>");
    EXPECT_CONTAINS(res.body, "<p>Path Info: </p>");
    EXPECT_CONTAINS(res.body, "<p>Query String: </p>");
}

void test_cgi_query_string_only(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/hello.py?name=world&age=20";

    CgiSession session; session.request = handler.handle(0, req).cgi;
    StubCgiExecutor cgi; cgi.readOutput(session, eof);
    HttpResponse res; cgi.finish(session, res);
    
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_CONTAINS(res.body, "<p>Path Info: </p>");
    EXPECT_CONTAINS(res.body, "<p>Query String: name=world&age=20</p>");
}

void test_cgi_path_info_only(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/hello.py/users/123";

    CgiSession session; session.request = handler.handle(0, req).cgi;
    StubCgiExecutor cgi; cgi.readOutput(session, eof);
    HttpResponse res; cgi.finish(session, res);
    
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_CONTAINS(res.body, "<p>Path Info: /users/123</p>");
    EXPECT_CONTAINS(res.body, "<p>Query String: </p>");
}

void test_cgi_path_info_and_query(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/hello.py/users/123?action=edit";

    CgiSession session; session.request = handler.handle(0, req).cgi;
    StubCgiExecutor cgi; cgi.readOutput(session, eof);
    HttpResponse res; cgi.finish(session, res);
    
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_CONTAINS(res.body, "<p>Path Info: /users/123</p>");
    EXPECT_CONTAINS(res.body, "<p>Query String: action=edit</p>");
}

void test_cgi_complex_edge_uri(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/hello.py/fuga.py?/piyo.?nfw.f";

    CgiSession session; session.request = handler.handle(0, req).cgi;
    StubCgiExecutor cgi; cgi.readOutput(session, eof);
    HttpResponse res; cgi.finish(session, res);
    
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_CONTAINS(res.body, "<p>Script Path: ./www/cgi-bin/hello.py</p>");
    EXPECT_CONTAINS(res.body, "<p>Path Info: /fuga.py</p>"); 
    EXPECT_CONTAINS(res.body, "<p>Query String: /piyo.?nfw.f</p>"); 
}

void test_cgi_multiple_question_marks(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/hello.py?q=1?&b=2";

    CgiSession session; session.request = handler.handle(0, req).cgi;
    StubCgiExecutor cgi; cgi.readOutput(session, eof);
    HttpResponse res; cgi.finish(session, res);
    
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_CONTAINS(res.body, "<p>Path Info: </p>");
    EXPECT_CONTAINS(res.body, "<p>Query String: q=1?&b=2</p>");
}

void test_cgi_non_cgi_extension(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/hello.txt";

    CgiSession session;
    HandleResult result = handler.handle(0, req);
    HttpResponse res = result.response;
    // session.request = result.cgi;
    // StubCgiExecutor cgi; cgi.readOutput(session, eof);
    // HttpResponse res; cgi.finish(session, res);
    
    EXPECT_EQ(result.pending, false);
    EXPECT_EQ(res.statusCode, 200);
    if (res.body.find("CGI Response") != std::string::npos) {
        std::cerr << "\n[FAIL] CGI Executor was called for a non-CGI extension file!" << std::endl;
        std::exit(1);
    }
    EXPECT_CONTAINS(res.body, "plain text");
}

void test_cgi_script_not_found(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/not_exist.py";

    HandleResult result = handler.handle(0, req);

    EXPECT_EQ(result.pending, true);
    EXPECT_EQ(result.cgi.scriptPath, std::string("./www/cgi-bin/not_exist.py"));
    EXPECT_EQ(result.cgi.interpreterPath, std::string("/usr/bin/python3"));

    HttpResponse res;
    res.body = "partial cgi output";
    handler.finishCgi(0, req, false, 502, res);
    EXPECT_EQ(res.statusCode, 502);
}

void test_cgi_permission_denied(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/no_exec.py";

    HandleResult result = handler.handle(0, req);

    EXPECT_EQ(result.pending, false);
    EXPECT_EQ(result.response.statusCode, 403);
}

void test_cgi_is_directory(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/cgi-bin/dir.py";

    HandleResult result = handler.handle(0, req);

    EXPECT_EQ(result.pending, false);
    EXPECT_EQ(result.response.statusCode, 403);
}

int main() {
    setup_test_environment();

    std::cout << "========================================" << std::endl;
    std::cout << "  RequestHandler CGI Edge-Case Tests    " << std::endl;
    std::cout << "========================================" << std::endl;

    run_test("CGI URI: Simple Script", test_cgi_simple_uri);
    run_test("CGI URI: Query String Only", test_cgi_query_string_only);
    run_test("CGI URI: Path Info Only", test_cgi_path_info_only);
    run_test("CGI URI: Path Info & Query String", test_cgi_path_info_and_query);
    run_test("CGI URI: Complex Edge Case (Dot in PathInfo, ? in Query)", test_cgi_complex_edge_uri);
    run_test("CGI URI: Multiple Question Marks", test_cgi_multiple_question_marks);

    run_test("CGI Ext: Non-CGI Extension file in cgi-bin", test_cgi_non_cgi_extension);

    run_test("CGI FS: Script Not Found -> Delegate to CGI", test_cgi_script_not_found);
    run_test("CGI FS: Permission Denied (403)", test_cgi_permission_denied);
    run_test("CGI FS: Target is a Directory (403)", test_cgi_is_directory);

    std::cout << "========================================" << std::endl;
    std::cout << COLOR_GREEN << "Passed: " << g_passed_tests << COLOR_RESET
              << ", " << COLOR_RED << "Failed: 0" << COLOR_RESET << std::endl;

    teardown_test_environment();
    return 0;
}