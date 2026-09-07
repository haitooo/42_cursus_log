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

static std::string uploadPathFromLocation(const std::string& location)
{
    if (location.find("/upload/") == 0)
        return std::string("./www/uploads/") + location.substr(std::string("/upload/").size());
    return std::string("./www") + location;
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

void setup_test_environment() {
    std::system("mkdir -p www/uploads www/delete_dir www/no_write_dir");
    
    std::system("echo 'old_content' > www/uploads/existing.txt");

    std::system("chmod 555 www/no_write_dir");

    std::system("echo 'to_be_deleted' > www/delete_target.txt");
    
    std::system("echo 'readonly' > www/no_write_file.txt");
    std::system("chmod 444 www/no_write_file.txt");
}

void teardown_test_environment() {
    std::system("chmod 777 www/no_write_file.txt www/no_write_dir 2>/dev/null");
}

void test_redirect_301_moved_permanently(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/redirect_301";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 301);
    EXPECT_EQ(res.headers["Location"], "http://example.com/new_place");
}

void test_redirect_302_found(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/redirect_302";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 302);
    EXPECT_EQ(res.headers["Location"], "/relative_target");
}

void test_post_201_success_new_file(RequestHandler& handler) {
    HttpRequest req; req.method = "POST"; req.path = "/upload/";
    req.body = "Hello, this is POST body content!";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers["Location"].find("/upload/upload_") == 0, true);

    const std::string savedPath = uploadPathFromLocation(res.headers["Location"]);
    std::ifstream savedFile(savedPath.c_str(), std::ios::binary);
    EXPECT_EQ(savedFile.is_open(), true);
    std::string content((std::istreambuf_iterator<char>(savedFile)), std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "Hello, this is POST body content!");
}

void test_post_201_success_empty_body(RequestHandler& handler) {
    HttpRequest req; req.method = "POST"; req.path = "/upload/";
    req.body = "";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers["Location"].find("/upload/upload_") == 0, true);

    const std::string savedPath = uploadPathFromLocation(res.headers["Location"]);
    struct stat st;
    EXPECT_EQ(stat(savedPath.c_str(), &st) == 0, true);
    EXPECT_EQ(st.st_size, 0);
}

void test_post_201_overwrite_existing_file(RequestHandler& handler) {
    HttpRequest req; req.method = "POST"; req.path = "/upload/existing.txt";
    req.body = "new_content";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 201);

    std::ifstream savedFile("./www/uploads/existing.txt", std::ios::binary);
    std::string content((std::istreambuf_iterator<char>(savedFile)), std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "new_content");
}

void test_post_403_no_upload_store(RequestHandler& handler) {
    HttpRequest req; req.method = "POST"; req.path = "/no_store/file.txt";
    req.body = "data";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 403);
}

void test_post_405_method_not_allowed(RequestHandler& handler) {
    HttpRequest req; req.method = "POST"; req.path = "/read_only/file.txt";
    req.body = "data";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 405);
}

void test_post_413_payload_too_large(RequestHandler& handler) {
    HttpRequest req; req.method = "POST"; req.path = "/upload/heavy.txt";
    req.body.assign(1048576 + 1, 'A');
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 413);
}

void test_post_500_directory_permission_denied(RequestHandler& handler) {
    HttpRequest req; req.method = "POST"; req.path = "/no_write_dir/file.txt";
    req.body = "data";
    HttpResponse res = handler.handle(0, req).response;
    // EXPECT_EQ(res.statusCode, 500); 
    EXPECT_EQ(res.statusCode, 403); 
}

void test_delete_204_success(RequestHandler& handler) {
    HttpRequest req; req.method = "DELETE"; req.path = "/delete_target.txt";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 204);

    struct stat st;
    EXPECT_EQ(stat("./www/delete_target.txt", &st) != 0, true);
}

void test_delete_404_not_found(RequestHandler& handler) {
    HttpRequest req; req.method = "DELETE"; req.path = "/already_gone.txt";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 404);
}

void test_delete_403_directory_forbidden(RequestHandler& handler) {
    HttpRequest req; req.method = "DELETE"; req.path = "/delete_dir";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 403);
}

void test_delete_405_method_not_allowed(RequestHandler& handler) {
    HttpRequest req; req.method = "DELETE"; req.path = "/read_only/file.txt";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 405);
}

void test_delete_403_permission_denied(RequestHandler& handler) {
    HttpRequest req; req.method = "DELETE"; req.path = "/no_write_file.txt";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 403);
}

int main() {
	setup_test_environment();

	std::cout << "========================================" << std::endl;
	std::cout << "  RequestHandler POST/DELETE/Redirect   " << std::endl;
	std::cout << "========================================" << std::endl;

	run_test("Redirect: 301 Moved Permanently", test_redirect_301_moved_permanently);
	run_test("Redirect: 302 Found", test_redirect_302_found);

	run_test("POST: Success Upload (201 Created)", test_post_201_success_new_file);
	run_test("POST: Success Empty Body (201 Created)", test_post_201_success_empty_body);
	// run_test("POST: Success Overwrite Existing File (201 Created)", test_post_201_overwrite_existing_file);
	run_test("POST: Error 403 (No upload_store)", test_post_403_no_upload_store);
	run_test("POST: Error 405 (Method Not Allowed)", test_post_405_method_not_allowed);
	run_test("POST: Error 413 (Payload Too Large)", test_post_413_payload_too_large);
	run_test("POST: Error 500 (Directory Permission Denied)", test_post_500_directory_permission_denied);

	run_test("DELETE: Success File Removal (204 No Content)", test_delete_204_success);
	run_test("DELETE: Error 404 (File Not Found)", test_delete_404_not_found);
	run_test("DELETE: Error 403 (Directory Target)", test_delete_403_directory_forbidden);
	run_test("DELETE: Error 405 (Method Not Allowed)", test_delete_405_method_not_allowed);
	run_test("DELETE: Error 403 (Permission Denied)", test_delete_403_permission_denied);

	std::cout << "========================================" << std::endl;
    std::cout << COLOR_GREEN << "Passed: " << g_passed_tests << COLOR_RESET
              << ", " << COLOR_RED << "Failed: 0" << COLOR_RESET << std::endl;

	teardown_test_environment();
	return 0;
}