#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>

#include "../../include/RequestHandler.hpp"
#include "StubCgiExecutor.hpp"

// =========================================================================
// Stub Classes
// =========================================================================

class StubConfig: public IConfig {
public:
    StubConfig() {
        _listenAddr.host = "0.0.0.0";
        _listenAddr.port = 8080;
    }

    bool resolveLocation(ServerId, const std::string& uriPath, ResolvedLocation& out) const {
		out.root = "./www";
		out.clientMaxBodySize = 1024 * 1024; // 1MB
		out.autoindex = false;
		out.redirect.enabled = false;
		out.cgi.enabled = false;
		// out.allowGet = true;
		// out.allowPost = false;
		// out.allowDelete = false;
        out.methods.clear(); out.methods.push_back("GET"); out.methods.push_back("POST"); out.methods.push_back("DELETE");

		if (uriPath == "/redirect_301") {
			out.redirect.enabled = true;
			out.redirect.statusCode = 301;
			out.redirect.target = "http://example.com/new_place";
			return true;
		}
		if (uriPath == "/redirect_302") {
			out.redirect.enabled = true;
			out.redirect.statusCode = 302;
			out.redirect.target = "/relative_target";
			return true;
		}

		if (uriPath.find("/upload") == 0) {
			out.methods.clear(); out.methods.push_back("POST");
			out.uploadStore = "./www/uploads";
			return true;
		}
		if (uriPath.find("/no_upload_store") == 0) {
			out.methods.clear(); out.methods.push_back("POST");
			out.uploadStore = "";
			return true;
		}

		if (uriPath.find("/delete_target") == 0) {
			out.methods.clear(); out.methods.push_back("DELETE");
			return true;
		}

		if (uriPath.find("/read_only") == 0) {
			out.methods.clear(); out.methods.push_back("GET");
			return true;
		}

		return true;
	}

	std::string mapUriToPath(const ResolvedLocation& loc, const std::string& uriPath) const {
		return loc.root + uriPath;
	}

	bool errorPagePath(ServerId, int statusCode, std::string& out) const {
		if (statusCode == 404) {
			out = "./www/404.html";
			return true;
		}
		return false;
	}

    std::size_t serverCount() const { return 1; }
	ServerConfig _listenAddr;
	const ServerConfig& listenAddress(ServerId) const {
        return _listenAddr;
    }
    std::size_t clientMaxBodySize(ServerId) const { return 1024 * 1024; } // 1MB

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

void test_redirect_301(RequestHandler& handler) {
	HttpRequest req; req.method = "GET"; req.path = "/redirect_301";
	HttpResponse res = handler.handle(0, req).response;

	EXPECT_EQ(res.statusCode, 301);
	EXPECT_EQ(res.headers["Location"], "http://example.com/new_place");
}

void test_redirect_302(RequestHandler& handler) {
	HttpRequest req; req.method = "GET"; req.path = "/redirect_302";
	HttpResponse res = handler.handle(0, req).response;

	EXPECT_EQ(res.statusCode, 302);
	EXPECT_EQ(res.headers["Location"], "/relative_target");
}

void test_post_success(RequestHandler& handler) {
	HttpRequest req;
	req.method = "POST";
	req.path = "/upload/test_file.txt";
	req.body = "Hello, this is POST body content!";

	HttpResponse res = handler.handle(0, req).response;

	EXPECT_EQ(res.statusCode, 201);
	EXPECT_EQ(res.headers["Location"], "/upload/test_file.txt");

	std::ifstream savedFile("./www/uploads/test_file.txt", std::ios::binary);
	EXPECT_EQ(savedFile.is_open(), true);
	std::string content((std::istreambuf_iterator<char>(savedFile)), std::istreambuf_iterator<char>());
	EXPECT_EQ(content, "Hello, this is POST body content!");
}

void test_post_no_upload_store(RequestHandler& handler) {
	HttpRequest req;
	req.method = "POST";
	req.path = "/no_upload_store/file.txt";
	req.body = "data";

	HttpResponse res = handler.handle(0, req).response;
	EXPECT_EQ(res.statusCode, 403);
}

void test_post_method_not_allowed(RequestHandler& handler) {
	HttpRequest req;
	req.method = "POST";
	req.path = "/read_only/file.txt";
	req.body = "data";

	HttpResponse res = handler.handle(0, req).response;
	EXPECT_EQ(res.statusCode, 405);
}

void test_post_payload_too_large(RequestHandler& handler) {
	HttpRequest req;
	req.method = "POST";
	req.path = "/upload/large_file.txt";
	req.body.assign(2000000, 'X');

	HttpResponse res = handler.handle(0, req).response;
	EXPECT_EQ(res.statusCode, 413);
}

void test_delete_success(RequestHandler& handler) {
	std::ofstream target("./www/delete_target/target.txt");
	target << "to be deleted";
	target.close();

	HttpRequest req;
	req.method = "DELETE";
	req.path = "/delete_target/target.txt";

	HttpResponse res = handler.handle(0, req).response;

	EXPECT_EQ(res.statusCode, 204); // 204 No Content

	struct stat st;
	EXPECT_EQ(stat("./www/delete_target/target.txt", &st) != 0, true);
}

void test_delete_file_not_found(RequestHandler& handler) {
	HttpRequest req;
	req.method = "DELETE";
	req.path = "/delete_target/not_exist.txt";

	HttpResponse res = handler.handle(0, req).response;
	EXPECT_EQ(res.statusCode, 404);
}

void test_delete_directory_forbidden(RequestHandler& handler) {
	HttpRequest req;
	req.method = "DELETE";
	req.path = "/delete_target";

	HttpResponse res = handler.handle(0, req).response;
	EXPECT_EQ(res.statusCode, 403);
}

void test_delete_method_not_allowed(RequestHandler& handler) {
	HttpRequest req;
	req.method = "DELETE";
	req.path = "/read_only/file.txt";

	HttpResponse res = handler.handle(0, req).response;
	EXPECT_EQ(res.statusCode, 405);
}

void setup_env() {
	std::system("mkdir -p ./www/uploads ./www/delete_target");
}

void teardown_env() {
	std::system("rm -rf ./www");
}

int main() {
	setup_env();

	std::cout << "========================================" << std::endl;
	std::cout << "  RequestHandler POST/DELETE/Redirect   " << std::endl;
	std::cout << "========================================" << std::endl;

	run_test("Redirect: 301 Moved Permanently", test_redirect_301);
	run_test("Redirect: 302 Found", test_redirect_302);

	// run_test("POST: Success Upload (201 Created)", test_post_success);
	// run_test("POST: Error 403 (No upload_store)", test_post_no_upload_store);
	// run_test("POST: Error 405 (Method Not Allowed)", test_post_method_not_allowed);
	// run_test("POST: Error 413 (Payload Too Large)", test_post_payload_too_large);

	run_test("DELETE: Success File Removal (204 No Content)", test_delete_success);
	run_test("DELETE: Error 404 (File Not Found)", test_delete_file_not_found);
	run_test("DELETE: Error 403 (Directory Target)", test_delete_directory_forbidden);
	run_test("DELETE: Error 405 (Method Not Allowed)", test_delete_method_not_allowed);

	std::cout << "========================================" << std::endl;
	std::cout << COLOR_GREEN << "Passed: " << g_passed_tests << COLOR_RESET
	          << ", " << COLOR_RED << "Failed: 0" << COLOR_RESET << std::endl;

	teardown_env();
	return 0;
}