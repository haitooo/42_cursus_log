#include <iostream>
#include <cstdlib>

#include "StubConfig.hpp"
#include "StubCgiExecutor.hpp"
#include "RequestHandler.hpp"

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
                  << "  L" << line << ": mismatch.\n"
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

// =========================================================================
// Test Cases
// =========================================================================

void test_get_normal_file(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/test.txt";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body, "hello world\n");
    EXPECT_EQ(res.headers["Content-Type"], "text/plain");
}

void test_get_directory_index(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body, "<h1>Index</h1>\n");
}

void test_error_404_custom_page(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/not_found.html";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 404);
    EXPECT_EQ(res.body, "<h1>Custom 404</h1>\n");
}

void test_error_404_no_location(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/not_found";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 404);
}

void test_error_403_autoindex_off(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/auto_off/";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.body.find("403 Forbidden") != std::string::npos, true);
}

void test_error_404_index_missing(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/no_index/";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 404);
    EXPECT_EQ(res.body, "<h1>Custom 404</h1>\n");
}

void test_get_autoindex_on(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/auto_on/";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body.find("Index of") != std::string::npos, true);
    std::cout << "[INFO] Autoindex response body:\n" << res.body << std::endl;
}

void test_error_405_method_not_allowed(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/admin/secret.txt";
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 405);
}

void test_error_413_payload_too_large(RequestHandler& handler) {
    HttpRequest req; req.method = "GET"; req.path = "/";
    req.body.assign(2000000, 'A');
    HttpResponse res = handler.handle(0, req).response;
    EXPECT_EQ(res.statusCode, 413);
}

// =========================================================================
// Test Setup and Main
// =========================================================================

void setup_test_environment() {
    std::system("mkdir -p www/admin www/auto_on www/auto_off www/no_index");
    std::system("echo '<h1>Index</h1>' > www/index.html");
    std::system("echo 'hello world' > www/test.txt");
    std::system("echo '<h1>Custom 404</h1>' > www/404.html");
    std::system("echo 'secret' > www/admin/secret.txt");
}

void teardown_test_environment() {
}

int main() {
    setup_test_environment();

    std::cout << "========================================" << std::endl;
    std::cout << " RequestHandler GET & Error Test Suite  " << std::endl;
    std::cout << "========================================" << std::endl;

    run_test("Normal: GET existing file", test_get_normal_file);
    run_test("Normal: GET directory (resolves index.html)", test_get_directory_index);
    run_test("Normal: GET directory with autoindex ON", test_get_autoindex_on);
    
    run_test("Error: 404 Not Found (Custom Error Page)", test_error_404_custom_page);
    run_test("Error: 404 Location Not Found", test_error_404_no_location);
    run_test("Error: 403 (autoindex OFF, no index)", test_error_403_autoindex_off);
    run_test("Error: 404 (index configured but missing)", test_error_404_index_missing);
    run_test("Error: 405 Method Not Allowed", test_error_405_method_not_allowed);
    run_test("Error: 413 Payload Too Large", test_error_413_payload_too_large);

    std::cout << "========================================" << std::endl;
    std::cout << COLOR_GREEN << "Passed: " << g_passed_tests << COLOR_RESET
              << ", " << COLOR_RED << "Failed: 0" << COLOR_RESET << std::endl;

    teardown_test_environment();
    return 0;
}