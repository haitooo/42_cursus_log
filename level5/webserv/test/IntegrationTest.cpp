#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include "HttpResponse.hpp"

// =========================================================================
// Test Framework
// =========================================================================

#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

std::string g_test_name;
int g_passed_tests = 0;
int g_failed_tests = 0;
int g_is_failed = 0;

template <typename T, typename U>
void expect_eq(const T& actual, const U& expected, int line) {
    if (actual != expected) {
        std::cerr << "\n" << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name << "\n"
                  << "  L" << line << ": mismatch.\n"
                  << "  Actual:   " << actual << "\n"
                  << "  Expected: " << expected << std::endl;
        g_is_failed = 1;
        // std::exit(1);
    }
}

#define EXPECT_EQ(actual, expected) expect_eq(actual, expected, __LINE__)

void run_test(const std::string& name, void (*test_func)()) {
    g_is_failed = 0;
    g_test_name = name;
    std::cout << "[RUN ] " << name << std::endl;
    test_func();
    if (!g_is_failed) {
        ++g_passed_tests;
        std::cout << COLOR_GREEN << "[ OK ] " << COLOR_RESET << name << std::endl;
    }
    else {
        ++g_failed_tests;
    }
}

#define NL "\r\n"
#define HTTP_1_1 "HTTP/1.1"
#define HTTP_1_0 "HTTP/1.0"
#define CONNECTION_KEEP_ALIVE "Connection: keep-alive"
#define CONNECTION_CLOSE "Connection: close"

int connect_server(const std::string& host, int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return -1;

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

std::string send_and_recv_raw(int fd, const std::string& request_text) {
    ssize_t total_sent = send(fd, request_text.data(), request_text.size(), 0);
    if(request_text.empty() == false) {
            if (total_sent < 0)
            return "";
    }

    std::string response;
    char buf[4096];
    while (true) {
        ssize_t n = recv(fd, buf, sizeof(buf), 0);
        if (n <= 0) break;
        response.append(buf, static_cast<std::size_t>(n));

        std::size_t header_end = response.find("\r\n\r\n");
        if (header_end != std::string::npos) {
            std::size_t cl_pos = response.find("Content-Length: ");
            if (cl_pos != std::string::npos && cl_pos < header_end) {
                std::size_t cl_val_start = cl_pos + 16;
                std::size_t cl_val_end = response.find("\r\n", cl_val_start);
                std::size_t content_length = std::strtoul(
                    response.substr(cl_val_start, cl_val_end - cl_val_start).c_str(), NULL, 10);
                if (response.size() >= header_end + 4 + content_length)
                    break;
            }
        }
    }
    return response;
}

HttpResponse parse_raw_response(const std::string& raw) {
    HttpResponse res;
    std::size_t header_end = raw.find("\r\n\r\n");
    if (header_end == std::string::npos) return res;

    std::string header_part = raw.substr(0, header_end);
    res.body = raw.substr(header_end + 4);

    std::istringstream stream(header_part);
    std::string line;

    if (std::getline(stream, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        std::size_t sp1 = line.find(' ');
        std::size_t sp2 = line.find(' ', sp1 + 1);
        if (sp1 != std::string::npos && sp2 != std::string::npos) {
            res.version = line.substr(0, sp1);
            res.statusCode = std::atoi(line.substr(sp1 + 1, sp2 - sp1 - 1).c_str());
            res.reasonPhrase = line.substr(sp2 + 1);
        }
    }

    while (std::getline(stream, line)) {
        if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
        if (line.empty()) continue;
        std::size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string val = line.substr(colon + 1);
            std::size_t first = val.find_first_not_of(" \t");
            if (first != std::string::npos) val = val.substr(first);
            res.headers[key] = val;
        }
    }
    return res;
}

HttpResponse request_http(const std::string& request_text) {
    int fd = connect_server("127.0.0.1", 8080);
    if (fd < 0) {
        std::cerr << "Failed to connect to server on 127.0.0.1:8080" << std::endl;
        std::exit(1);
    }
    std::string raw = send_and_recv_raw(fd, request_text);
    close(fd);
    return parse_raw_response(raw);
}

void test_get_root() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);

    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.reasonPhrase, "OK");
    EXPECT_EQ(res.headers["Content-Type"], "text/html");
}

void test_unsupported_version() {
    std::string req = "GET / " "HTTP/2.0" NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 505);
    EXPECT_EQ(res.reasonPhrase, "HTTP Version Not Supported");
}

void test_excess_spaces_in_request_line() {
    std::string req = "GET    /    " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_lack_request_line() {
    std::string req = "GET /" NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_missing_host_header() {
    std::string req = "GET / " HTTP_1_1 NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_missing_host_header_with_http_1_0() {
    std::string req = "GET / " HTTP_1_0 NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.reasonPhrase, "OK");
}

void test_no_colon_in_header() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_no_crlf_after_header() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Connection: close" NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 408);
    EXPECT_EQ(res.reasonPhrase, "Request Timeout");
}

void test_no_crlf_after_request_line() {
    std::string req = "GET / " HTTP_1_1;
    req += "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_empty_header_name() {
    std::string req = "GET / " HTTP_1_1 NL
                      ": no-name" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_empty_host_header_value() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: " NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_empty_header_value() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: localhost" NL
                      "X-Empty-Header: " NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.reasonPhrase, "OK");
}

void test_unknown_method() {
    std::string req = "HOGE / " HTTP_1_1 NL
                        "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 501);
    EXPECT_EQ(res.reasonPhrase, "Not Implemented");
}

void test_lowercase_method() {
    std::string req = "get / " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_get_futu_file() {
    std::string req = "GET /test.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);

    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Content-Type"], "text/plain");
    EXPECT_EQ(res.body, "This is a test file for integration testing.");
}

void test_get_non_existent_file() {
    std::string req = "GET /non_existent_file.html " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);

    EXPECT_EQ(res.statusCode, 404);
    EXPECT_EQ(res.reasonPhrase, "Not Found");
    EXPECT_EQ(res.body, "<html><body><h1>Custom 404 Not Found</h1></body></html>");
}

void test_get_index_in_directory() {
    std::string req = "GET /subdir/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);

    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Content-Type"], "text/html");
    EXPECT_EQ(res.body, "<html><body><h1>Subdirectory Index</h1></body></html>");
}

void test_get_not_found() {
    std::string req = "GET /subdir/non_existent_file.html " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);

    EXPECT_EQ(res.statusCode, 404);
    EXPECT_EQ(res.reasonPhrase, "Not Found");
}

void test_bad_request_line() {
    std::string req = "GET / too many spaces " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);

    EXPECT_EQ(res.statusCode, 400);
}

void test_autoindex_on() {
    std::string req = "GET /autoindex_on/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Content-Type"], "text/html");
}

void test_autoindex_off() {
    std::string req = "GET /autoindex_off/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
}

void test_autoindex_xss() {
    std::string req = "GET /autoindex_xss/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Content-Type"], "text/html");
    EXPECT_EQ(res.body.find("<script>alert('XSS');</script>"), std::string::npos);
}

void test_autoindex_priority() {
    std::string req = "GET /autoindex_priority/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Content-Type"], "text/html");
    EXPECT_EQ(res.body, "<html><head><title>index.html</title></head><body><h1>index.html</h1></body></html>");
}

void test_no_permission_file() {
    std::string req = "GET /no_permission_file.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
}

void test_no_permission_directory() {
    std::string req = "GET /no_permission_dir/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
}

void test_no_permission_index_file() {
    std::string req = "GET /no_permission_index/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
}

void test_not_allowed_method() {
    std::string req = "POST /readonly/ " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Content-Length: 0" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 405);
    EXPECT_EQ(res.reasonPhrase, "Method Not Allowed");
}

void test_space_in_method() {
    std::string req = "GE T / " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_slash_in_path() {
    std::string req = "GET ///subdir///subdir_index.html " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body, "<html><body><h1>Subdirectory Index</h1></body></html>");
}

void test_get_path_traversal() {
    std::string req = "GET /../test.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    // EXPECT_EQ(res.statusCode, 403);
    // EXPECT_EQ(res.reasonPhrase, "Forbidden");
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_get_path_traversal_within_root() {
    std::string req = "GET /subdir/../test.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body, "This is a test file for integration testing.");
}

void test_URI_too_long() {
    std::string long_path(8192, 'a');
    std::string req = "GET /" + long_path + " " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 414);
    EXPECT_EQ(res.reasonPhrase, "URI Too Long");
}

void test_invalid_http_version() {
    std::string req = "GET / " "http/1.1" NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
}

void test_tab_in_request_line() {
    std::string req = "GET\t/\t" HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_empty_request_line() {
    std::string req = "" NL
                      "GET / " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_double_host_header() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Host: example.com" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_space_in_header_name() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host : localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_ows_in_header_value() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Connection:  \t  close  \t " NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Connection"], "close");
}

void test_header_mixed_case() {
    std::string req = "GET / " HTTP_1_1 NL
                      "hOsT: localhost" NL
                      "cOnNeCtIoN: close" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Connection"], "close");
}

void test_too_large_header_value() {
    std::string long_value(8193, 'A');
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: localhost" NL
                      "X-Long-Header: " + long_value + NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 431);
    EXPECT_EQ(res.reasonPhrase, "Request Header Fields Too Large");
}

void test_fragmentation_request() {
    std::string req = "GET / " HTTP_1_1 NL "Host: localhost" NL NL;
    int fd = connect_server("127.0.0.1", 8080);
    for (std::size_t i = 0; i < req.size(); ++i) {
        send(fd, &req[i], 1, 0);
        usleep(1000);
    }
    std::string raw = send_and_recv_raw(fd, "");
    close(fd);
    HttpResponse res = parse_raw_response(raw);
    EXPECT_EQ(res.statusCode, 200);
}

void test_timeout_request() {
    // std::string req = "GET / " HTTP_1_1 NL "Host: localhost" NL NL;
    // int fd = connect_server("127.0.0.1", 8080);
    // sleep(75);
    // std::string raw = send_and_recv_raw(fd, req); close(fd);
    // HttpResponse res = parse_raw_response(raw);
    // EXPECT_EQ(res.statusCode, 408);
    // EXPECT_EQ(res.reasonPhrase, "Request Timeout");
    // EXPECT_EQ(res.body, "</title></head><body><h1>408 Request Timeout</h1></body></html>");

    int fd = connect_server("127.0.0.1", 8080);

    std::string incomplete_req = "GET / HTTP/1.1\r\nHost: localhost\r\n";
    send(fd, incomplete_req.data(), incomplete_req.size(), 0);

    sleep(11);

    std::string raw;
    char buf[4096];
    while (true) {
        ssize_t n = recv(fd, buf, sizeof(buf), 0);
        if (n <= 0) break;
        raw.append(buf, static_cast<std::size_t>(n));
    }
    close(fd);

    HttpResponse res = parse_raw_response(raw);
    EXPECT_EQ(res.statusCode, 408);
    EXPECT_EQ(res.reasonPhrase, "Request Timeout");
    EXPECT_EQ(res.body, "<html><head><title>408 Request Timeout</title></head><body><h1>408 Request Timeout</h1></body></html>");
}

void test_timeout_silent_close_then_send() {
    int fd = connect_server("127.0.0.1", 8080);
    sleep(13);

    std::string req = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    ssize_t sent = send(fd, req.data(), req.size(), MSG_NOSIGNAL);

    char buf[1024];
    ssize_t n = recv(fd, buf, sizeof(buf), 0);
    buf[n] = '\0';
    int recv_err = errno;
    close(fd);

    bool disconnected_detected = false;

    if (sent < 0 && (errno == EPIPE || errno == ECONNRESET)) {
        disconnected_detected = true;
    } else if (n == 0) {
        disconnected_detected = true;
    } else if (n < 0 && (recv_err == ECONNRESET || recv_err == EPIPE)) {
        disconnected_detected = true;
    }

    EXPECT_EQ(disconnected_detected, true);
    EXPECT_EQ(n, 0);
    if(n != 0)
        std::cout << "Responce: " << std::string(buf, n) << std::endl;
}

void test_post_upload() {
    std::string body = "Hello, webserv integration test!";
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
    std::string request = "GET " + res.headers["Location"] + " " HTTP_1_1 NL
                               "Host: localhost" NL NL;
    HttpResponse res2 = request_http(request);
    EXPECT_EQ(res2.statusCode, 200);
    EXPECT_EQ(res2.body, body);
}

void test_zero_length_post() {
    std::string req = "POST /upload " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Content-Length: 0" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
}

void test_post_with_no_content_length() {
    std::string body = "Hello, webserv integration test!";
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 411);
    EXPECT_EQ(res.reasonPhrase, "Length Required");
}

void test_post_with_invalid_content_length() {
    std::string body = "Hello, webserv integration test!";
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: invalid" NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

// void test_post_exsisting_file() {
//     std::string body = "Hello, webserv integration test!";
//     std::stringstream ss;
//     ss << "POST /upload/existing_file.txt " HTTP_1_1 NL
//        << "Host: localhost" NL
//        << "Content-Length: " << body.size() << NL
//        << "Content-Type: text/plain" NL NL
//        << body;

//     HttpResponse res = request_http(ss.str());
//     HttpResponse res2 = request_http(ss.str());
//     EXPECT_EQ(res.statusCode, 409);
//     EXPECT_EQ(res.reasonPhrase, "Conflict");
// }

void test_post_payload_too_large() {
    std::string body(1024 * 1024 * 10, 'A');
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 413);
    EXPECT_EQ(res.reasonPhrase, "Payload Too Large");
}

void test_post_with_no_content_type() {
    std::string body = "Hello, webserv integration test!";
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_post_no_upload_directory() {
    std::string body = "Hello, webserv integration test!";
    std::stringstream ss;
    ss << "POST /no_store " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
}

void test_post_no_permission_to_write() {
    std::string body = "Hello, webserv integration test!";
    std::stringstream ss;
    ss << "POST /no_permission_upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    // EXPECT_EQ(res.statusCode, 403);
    // EXPECT_EQ(res.reasonPhrase, "Forbidden");
    EXPECT_EQ(res.statusCode, 500);
    EXPECT_EQ(res.reasonPhrase, "Internal Server Error");
}

void test_overflow_content_length() {
    std::string body = "Hello, webserv integration test!";
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: 18446744073709551616" NL // 2^64
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_double_content_length_header() {
    std::string req = "POST /upload " HTTP_1_1 NL "Host: localhost" NL
                      "Content-Length: 5" NL "Content-Length: 10" NL NL
                      "Hello";
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_multipart_one_file() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"" NL
                       "Content-Type: text/plain" NL NL "This is a test file." NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
}

void test_multipart_multiple_files() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    char binary_data[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file1\"; filename=\"test1.txt\"" NL
                       "Content-Type: text/plain" NL NL "This is test file 1." NL
                       "--" + boundary + NL
                      "Content-Disposition: form-data; name=\"file2\"; filename=\"test2.bin\"" NL
                      "Content-Type: application/octet-stream" NL NL;
    body.append(binary_data, sizeof(binary_data));
    body += NL "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
}

void test_multipart_no_boundary() {
    std::string body = "This is a test file without boundary.";
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_multipart_path_traversal() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file\"; filename=\"../../etc/passwd\"" NL
                       "Content-Type: text/plain" NL NL "This is a test file." NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_multipart_boundary_in_file_content() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string file_content = "This is a test file with boundary in content: " + boundary;
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"" NL
                       "Content-Type: text/plain" NL NL + file_content + NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
}

void test_multipart_no_filename() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file\"" NL
                       "Content-Type: text/plain" NL NL "This is a test file." NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 201);
}

void test_multipart_space_in_filename() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file\"; filename=\"test file.txt\"" NL
                       "Content-Type: text/plain" NL NL "This is a test file." NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 201);
}

void test_multipart_missing_content_disposition() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string body = "--" + boundary + NL
                       "Content-Type: text/plain" NL NL "This is a test file." NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_multipart_missing_NL() {
    std::string boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"" NL
                       "Content-Type: text/plain" NL
                       "This is a test file." NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_multipart_too_long_boundary() {
    std::string boundary(81, 'A');
    std::string body = "--" + boundary + NL
                       "Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"" NL
                       "Content-Type: text/plain" NL NL "This is a test file." NL
                       "--" + boundary + "--" NL;

    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: multipart/form-data; boundary=" << boundary << NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_chunked_transfer_encoding() {
    std::string body = "4\r\nWiki\r\n5\r\npedia\r\nE\r\n in\r\n\r\nchunks.\r\n0\r\n\r\n";
    std::stringstream ss;
    ss << "POST /upload " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Transfer-Encoding: chunked" NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
}

void test_discrete_chunked_transfer_encoding() {
    std::string body = "1\r\nA\r\n1\r\nB\r\n1\r\nC\r\n1\r\nD\r\n1\r\nE\r\n0\r\n\r\n";
    std::string request = "POST /upload " HTTP_1_1 NL "Host: localhost" NL
                          "Transfer-Encoding: chunked" NL "Content-Type: text/plain" NL NL
                          + body;
    HttpResponse res = request_http(request);
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
}

void test_chunked_hex_lower_and_upper() {
    std::string body = "a\r\nabcdefghij\r\nA\r\nABCDEFGHIJ\r\n0\r\n\r\n";
    std::string request = "POST /upload " HTTP_1_1 NL "Host: localhost" NL
                          "Transfer-Encoding: chunked" NL "Content-Type: text/plain" NL NL
                          + body;
    HttpResponse res = request_http(request);
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.headers.find("Location") != res.headers.end(), true);
}

void test_chunked_invalid_size() {
    std::string body = "G\r\ninvalidChunkSize\r\n0\r\n\r\n";
    std::string request = "POST /upload " HTTP_1_1 NL "Host: localhost" NL
                          "Transfer-Encoding: chunked" NL "Content-Type: text/plain" NL NL
                          + body;
    HttpResponse res = request_http(request);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_chunked_too_large_chunk() {
    std::string body = "100000000\r\n" + std::string(0x10000000, 'A') + "\r\n0\r\n\r\n"; // 256MB
    std::string request = "POST /upload " HTTP_1_1 NL "Host: localhost" NL
                          "Transfer-Encoding: chunked" NL "Content-Type: text/plain" NL NL
                          + body;
    HttpResponse res = request_http(request);
    EXPECT_EQ(res.statusCode, 413);
    EXPECT_EQ(res.reasonPhrase, "Payload Too Large");
}

void test_missing_chunked_terminator() {
    std::string body = "4\r\nWiki\r\n5\r\npedia\r\nE\r\n in\r\n\r\nchunks.\r\n";
    std::string request = "POST /upload " HTTP_1_1 NL "Host: localhost" NL
                          "Transfer-Encoding: chunked" NL "Content-Type: text/plain" NL NL
                          + body;
    HttpResponse res = request_http(request);
    // EXPECT_EQ(res.statusCode, 400);
    // EXPECT_EQ(res.reasonPhrase, "Bad Request");
    EXPECT_EQ(res.statusCode, 408);
    EXPECT_EQ(res.reasonPhrase, "Request Timeout");
}

void test_chunked_size_with_leading_zeros() {
    std::string req = "POST /upload " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Transfer-Encoding: chunked" NL NL
                      "000005" NL "Hello" NL
                      "0" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.reasonPhrase, "Created");
}

void test_chunked_incorrect_size() {
    std::string req = "POST /upload " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Transfer-Encoding: chunked" NL NL
                      "10" NL "Hello" NL
                      "0" NL NL;
    HttpResponse res = request_http(req);
    // EXPECT_EQ(res.statusCode, 400);
    // EXPECT_EQ(res.reasonPhrase, "Bad Request");
    EXPECT_EQ(res.statusCode, 408);
    EXPECT_EQ(res.reasonPhrase, "Request Timeout");
}

void test_chunked_incorrect_size2() {
    std::string req = "POST /upload " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Transfer-Encoding: chunked" NL NL
                      "5" NL "HelloWorld" NL
                      "0" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 400);
    EXPECT_EQ(res.reasonPhrase, "Bad Request");
}

void test_chunked_trailer() {
    std::string req = "POST /upload " HTTP_1_1 NL
                      "Host: localhost" NL
                      "Transfer-Encoding: chunked" NL NL
                      "5" NL "Hello" NL
                      "0" NL
                      "X-Trailer-Header: TrailerValue" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 201);
    EXPECT_EQ(res.reasonPhrase, "Created");
}


void test_delete_existing_file() {
    std::system("touch www/delete_me.txt");
    std::string req = "DELETE /delete_me.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 204);
    EXPECT_EQ(res.reasonPhrase, "No Content");
    EXPECT_EQ(access("www/delete_me.txt", F_OK), -1);
}

void test_delete_non_existent_file() {
    std::string req = "DELETE /non_existent_file.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 404);
    EXPECT_EQ(res.reasonPhrase, "Not Found");
}

void test_delete_no_permission_file() {
    std::system("touch www/delete_dir/no_permission_file.txt && chmod 000 www/delete_dir/no_permission_file.txt");
    std::string req = "DELETE /no_permission_file.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
    EXPECT_EQ(access("www/delete_dir/no_permission_file.txt", F_OK), 0);
    std::system("chmod 644 www/delete_dir/no_permission_file.txt && rm www/delete_dir/no_permission_file.txt");
}

void test_delete_no_permission_directory() {
    std::string req = "DELETE /no_permission_delete_dir/delete_me.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
    // EXPECT_EQ(res.statusCode, 500);
    // EXPECT_EQ(res.reasonPhrase, "Internal Server Error");
}

void test_delete_directory() {
    std::string req = "DELETE /subdir/ " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
    EXPECT_EQ(access("www/subdir/", F_OK), 0);
}

void test_delete_no_permission_location() {
    std::string req = "DELETE /readonly/file.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 405);
    EXPECT_EQ(res.reasonPhrase, "Method Not Allowed");
}

void test_redirect_302() {
    std::string req = "GET /redirect302 " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 302);
    EXPECT_EQ(res.reasonPhrase, "Found");
    EXPECT_EQ(res.headers["Location"], "/relative_target");
}

void test_redirect_301() {
    std::string req = "GET /redirect301 " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 301);
    EXPECT_EQ(res.reasonPhrase, "Moved Permanently");
    EXPECT_EQ(res.headers["Location"], "http://example.com/new_place");
}

void test_another_port() {
    std::string req = "GET / " HTTP_1_1 NL
                      "Host: localhost:8081" NL NL;
    int fd = connect_server("127.0.0.1", 8081);
    std::string raw = send_and_recv_raw(fd, req);
    HttpResponse res = parse_raw_response(raw); close(fd);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body, "<html><body><h1>Port 8081</h1></body></html>");
}

void test_cgi() {
    std::string req = "GET /cgi-bin/test.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Content-Type"], "text/html");
    EXPECT_EQ(res.body.find("<h1>Hello from CGI!</h1>") != std::string::npos, true);
}

void test_cgi_query_string() {
    std::string req = "GET /cgi-bin/test.py?name=webserv " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.headers["Content-Type"], "text/html");
    EXPECT_EQ(res.body.find("query_string: name=webserv") != std::string::npos, true);
}

void test_cgi_path_info() {
    std::string req = "GET /cgi-bin/test.py/extra/path " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body.find("path_info: /extra/path") != std::string::npos, true);
}

void test_cgi_query_string_and_path_info() {
    std::string req = "GET /cgi-bin/test.py/extra/path.py?name=webserv/test " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body.find("query_string: name=webserv/test") != std::string::npos, true);
    EXPECT_EQ(res.body.find("path_info: /extra/path.py") != std::string::npos, true);
}

void test_cgi_post() {
    std::string body = "Hello from POST!";
    std::stringstream ss;
    ss << "POST /cgi-bin/test.py " HTTP_1_1 NL
       << "Host: localhost" NL
       << "Content-Length: " << body.size() << NL
       << "Content-Type: text/plain" NL NL
       << body;

    HttpResponse res = request_http(ss.str());
    EXPECT_EQ(res.statusCode, 200);
    EXPECT_EQ(res.body.find("request_method: POST") != std::string::npos, true);
    EXPECT_EQ(res.body.find("content_length: 16") != std::string::npos, true);
    EXPECT_EQ(res.body.find("Hello from POST!") != std::string::npos, true);
}

void test_cgi_no_permission() {
    std::string req = "GET /cgi-bin/no_permission.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 403);
    EXPECT_EQ(res.reasonPhrase, "Forbidden");
}

void test_cgi_not_found() {
    std::string req = "GET /cgi-bin/non_existent.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 404);
    EXPECT_EQ(res.reasonPhrase, "Not Found");
}

void test_not_cgi_extension() {
    std::string req = "GET /cgi-bin/test.txt " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
}

void test_cgi_infinite_loop() {
    std::string req = "GET /cgi-bin/infinite_loop.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 504);
    EXPECT_EQ(res.reasonPhrase, "Gateway Timeout");
}

void test_cgi_error() {
    std::string req = "GET /cgi-bin/error.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 502);
    EXPECT_EQ(res.reasonPhrase, "Bad Gateway");
}

void test_cgi_invalid_output() {
    std::string req = "GET /cgi-bin/invalid_output.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 502);
    EXPECT_EQ(res.reasonPhrase, "Bad Gateway");
}

void test_cgi_custom_header() {
    std::string req = "GET /cgi-bin/test.py " HTTP_1_1 NL
                      "Host: localhost" NL
                      "X-Custom-Header: CustomValue" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
}

void test_cgi_status_code() {
    std::string req = "GET /cgi-bin/status_code.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 418);
    EXPECT_EQ(res.reasonPhrase, "I'm a teapot");
}

void test_cgi_sigpipe() {
    std::string req = "GET /cgi-bin/sigpipe.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 502);
    EXPECT_EQ(res.reasonPhrase, "Bad Gateway");

    std::string req2 = "GET /cgi-bin/test.py " HTTP_1_1 NL
                       "Host: localhost" NL NL;
    HttpResponse res2 = request_http(req2);
    EXPECT_EQ(res2.statusCode, 200);
}

void test_cgi_check_fd_leak() {
    std::string req = "GET /cgi-bin/check_fd_leak.py " HTTP_1_1 NL
                      "Host: localhost" NL NL;
    HttpResponse res = request_http(req);
    EXPECT_EQ(res.statusCode, 200);
    std::cout << "CGI check_fd_leak.py response body: " << res.body << std::endl;
}

void test_keep_alive() {
    int fd = connect_server("127.0.0.1", 8080);
    EXPECT_EQ(fd >= 0, true);

    std::string req1 = "GET / " HTTP_1_1 NL "Host: localhost" NL
                       "Connection: keep-alive" NL NL;
    std::string raw1 = send_and_recv_raw(fd, req1);
    HttpResponse res1 = parse_raw_response(raw1);
    EXPECT_EQ(res1.statusCode, 200);

    std::string req2 = "GET / " HTTP_1_1 NL "Host: localhost" NL
                       "Connection: close" NL NL;
    std::string raw2 = send_and_recv_raw(fd, req2);
    HttpResponse res2 = parse_raw_response(raw2);
    EXPECT_EQ(res2.statusCode, 200);
    EXPECT_EQ(res2.headers["Connection"], "close");

    close(fd);
}

void test_keep_alive_no_connection_header() {
    int fd = connect_server("127.0.0.1", 8080);
    EXPECT_EQ(fd >= 0, true);

    std::string req1 = "GET / " HTTP_1_1 NL "Host: localhost" NL NL;
    std::string raw1 = send_and_recv_raw(fd, req1);
    HttpResponse res1 = parse_raw_response(raw1);
    EXPECT_EQ(res1.statusCode, 200);

    std::string req2 = "GET / " HTTP_1_1 NL "Host: localhost" NL NL;
    std::string raw2 = send_and_recv_raw(fd, req2);
    HttpResponse res2 = parse_raw_response(raw2);
    EXPECT_EQ(res2.statusCode, 200);

    close(fd);
}

void test_close_http_1_0_connection() {
    int fd = connect_server("127.0.0.1", 8080);
    EXPECT_EQ(fd >= 0, true);

    std::string req1 = "GET / " HTTP_1_0 NL "Host: localhost" NL NL;
    std::string raw1 = send_and_recv_raw(fd, req1);
    HttpResponse res1 = parse_raw_response(raw1);
    EXPECT_EQ(res1.statusCode, 200);
    
    std::string raw2 = send_and_recv_raw(fd, req1);
    EXPECT_EQ(raw2.empty(), true);
    close(fd);
}

void setup_env() {
    std::system("chmod 000 www/no_permission_dir");
    std::system("chmod 000 www/cgi-bin/no_permission.py");
    std::system("chmod 000 www/no_permission_file.txt");
    std::system("chmod 000 www/no_permission_index/index.html");
    std::system("chmod 555 www/no_permission_delete_dir/delete_me.txt");
    std::system("mkdir -p www/upload");
    std::system("mkdir -p www/delete_dir");
    std::system("mkdir -p www/autoindex_xss");
    std::system("mkdir -p www/no_permission_upload && chmod 555 www/no_permission_upload");
}

void cleanup_env() {
    std::system("rm -rf www/upload/*");
    std::system("chmod 755 www/no_permission_dir");
    std::system("chmod 755 www/cgi-bin/no_permission.py");
    std::system("chmod 755 www/no_permission_file.txt");
    std::system("chmod 755 www/no_permission_index/index.html");
    std::system("chmod 755 www/no_permission_delete_dir/delete_me.txt");
    std::system("chmod 755 www/no_permission_upload");
}

int main() {
    setup_env();

    run_test("GET / (Root Index)", test_get_root);
    run_test("Unsupported HTTP Version", test_unsupported_version);
    run_test("Excess Spaces in Request Line", test_excess_spaces_in_request_line);
    run_test("Lack Request Line", test_lack_request_line);
    run_test("Missing Host Header", test_missing_host_header);
    run_test("Missing Host Header with HTTP/1.0", test_missing_host_header_with_http_1_0);
    run_test("No Colon in Header", test_no_colon_in_header);
    run_test("No CRLF after Header", test_no_crlf_after_header);
    run_test("No CRLF after Request Line", test_no_crlf_after_request_line);
    run_test("Empty Header Name", test_empty_header_name);
    run_test("Empty Host Header Value", test_empty_host_header_value);
    run_test("Empty Header Value", test_empty_header_value);
    run_test("Unknown Method", test_unknown_method);
    run_test("Lowercase Method", test_lowercase_method);
    run_test("GET /test.txt (Existing File)", test_get_futu_file);
    run_test("GET /non_existent_file.html (Non-existent File)", test_get_non_existent_file);
    run_test("GET /subdir/ (Directory with Index)", test_get_index_in_directory);
    run_test("GET /non_existent_file_12345.html (Not Found)", test_get_not_found);
    run_test("Bad Request Line", test_bad_request_line);
    run_test("Autoindex On", test_autoindex_on);
    run_test("Autoindex Off", test_autoindex_off);
    run_test("Autoindex XSS", test_autoindex_xss);
    run_test("Autoindex Priority", test_autoindex_priority);
    run_test("No Permission File", test_no_permission_file);
    run_test("No Permission Directory", test_no_permission_directory);
    run_test("No Permission Index File", test_no_permission_index_file);
    run_test("Not Allowed Method", test_not_allowed_method);
    run_test("GET space in method", test_space_in_method);
    run_test("GET slash in path", test_slash_in_path);
    run_test("GET path traversal", test_get_path_traversal);
    run_test("GET path traversal within root", test_get_path_traversal_within_root);
    run_test("GET URI too long", test_URI_too_long);
    run_test("GET invalid http version", test_invalid_http_version);
    run_test("GET tab in request line", test_tab_in_request_line);
    run_test("GET empty request line", test_empty_request_line);
    run_test("GET double host header", test_double_host_header);
    run_test("GET space in header name", test_space_in_header_name);
    run_test("GET OWS in header value", test_ows_in_header_value);
    run_test("GET header mixed case", test_header_mixed_case);
    run_test("GET too large header value", test_too_large_header_value);
    run_test("GET fragmentaion request", test_fragmentation_request);
    run_test("GET Timeout Request", test_timeout_request);
    run_test("GET Timeout Silent Request", test_timeout_silent_close_then_send);
    run_test("POST /upload (Upload File)", test_post_upload);
    run_test("POST /upload (Zero Length)", test_zero_length_post);
    run_test("POST /upload (No Content-Length)", test_post_with_no_content_length);
    run_test("POST /upload (Invalid Content-Length)", test_post_with_invalid_content_length);
    // run_test("POST /upload/existing_file.txt (Conflict)", test_post_exsisting_file);
    run_test("POST /upload (Payload Too Large)", test_post_payload_too_large);
    // run_test("POST /upload (No Content-Type)", test_post_with_no_content_type);
    run_test("POST /no_upload_dir (No Upload Directory)", test_post_no_upload_directory);
    run_test("POST /no_permission_to_write (No Permission to Write)", test_post_no_permission_to_write);
    run_test("POST /upload (Overflow Content-Length)", test_overflow_content_length);
    run_test("POST double Content-Length Header", test_double_content_length_header);
    run_test("POST /upload (Multipart One File)", test_multipart_one_file);
    run_test("POST /upload (Multipart Multiple Files)", test_multipart_multiple_files);
    run_test("POST /upload (Multipart No Boundary)", test_multipart_no_boundary);
    run_test("POST /upload (Multipart Path Traversal)", test_multipart_path_traversal);
    run_test("POST /upload (Multipart Boundary in File Content)", test_multipart_boundary_in_file_content);
    run_test("POST /upload (Multipart No Filename)", test_multipart_no_filename);
    run_test("POST /upload (Multipart Space in Filename)", test_multipart_space_in_filename);
    run_test("POST /upload (Multipart Missing Content-Disposition)", test_multipart_missing_content_disposition);
    run_test("POST /upload (Multipart Missing NL)", test_multipart_missing_NL);
    run_test("POST /upload (Multipart Too Long Boundary)", test_multipart_too_long_boundary);
    run_test("POST /upload (Chunked Transfer-Encoding)", test_chunked_transfer_encoding);
    run_test("POST /upload (Discrete Chunked Transfer-Encoding)", test_discrete_chunked_transfer_encoding);
    run_test("POST /upload (Chunked Hex Lower and Upper)", test_chunked_hex_lower_and_upper);
    run_test("POST /upload (Chunked Invalid Size)", test_chunked_invalid_size);
    run_test("POST /upload (Chunked Too Large Chunk)", test_chunked_too_large_chunk);
    run_test("POST /upload (Missing Chunked Terminator)", test_missing_chunked_terminator);
    run_test("POST /upload (Chunked Size with Leading Zeros)", test_chunked_size_with_leading_zeros);
    run_test("POST /upload (Chunked Incorrect Size)", test_chunked_incorrect_size);
    run_test("POST /upload (Chunked Incorrect Size 2)", test_chunked_incorrect_size2);
    // run_test("POST /upload (Chunked Trailer)", test_chunked_trailer);
    run_test("DELETE /delete_me.txt (Existing File)", test_delete_existing_file);
    run_test("DELETE /non_existent_file.txt (Non-existent File)", test_delete_non_existent_file);
    run_test("DELETE /no_permission_file.txt (No Permission File)", test_delete_no_permission_file);
    run_test("DELETE /no_permission_dir/ (No Permission Directory)", test_delete_no_permission_directory);
    run_test("DELETE /subdir/ (Directory)", test_delete_directory);
    run_test("DELETE /no_permission_to_delete/test.txt (No Permission Location)", test_delete_no_permission_location);
    run_test("GET /redirect302 (Redirect 302)", test_redirect_302);
    run_test("GET /redirect301 (Redirect 301)", test_redirect_301);
    run_test("GET / (Another Port)", test_another_port);
    run_test("GET /cgi-bin/test.py (CGI)", test_cgi);
    run_test("GET /cgi-bin/test.py?name=webserv (CGI Query String)", test_cgi_query_string);
    run_test("GET /cgi-bin/test.py/extra/path (CGI Path Info)", test_cgi_path_info);
    run_test("GET /cgi-bin/test.py/extra/path.py?name=webserv/test (CGI Query String and Path Info)", test_cgi_query_string_and_path_info);
    run_test("POST /cgi-bin/test.py (CGI POST)", test_cgi_post);
    run_test("GET /cgi-bin/no_permission.py (CGI No Permission)", test_cgi_no_permission);
    // run_test("GET /cgi-bin/non_existent.py (CGI Not Found)", test_cgi_not_found);
    run_test("GET /cgi-bin/test.txt (Not CGI Extension)", test_not_cgi_extension);
    run_test("GET /cgi-bin/infinite_loop.py (CGI Infinite Loop)", test_cgi_infinite_loop);
    run_test("GET /cgi-bin/error.py (CGI Error)", test_cgi_error);
    run_test("GET /cgi-bin/invalid_output.py (CGI Invalid Output)", test_cgi_invalid_output);
    run_test("GET /cgi-bin/test.py (CGI Custom Header)", test_cgi_custom_header);
    run_test("GET /cgi-bin/status_code.py (CGI Status Code)", test_cgi_status_code);
    run_test("GET /cgi-bin/sigpipe.py (CGI SIGPIPE)", test_cgi_sigpipe);
    run_test("GET /cgi-bin/check_fd_leak.py (CGI Check FD Leak)", test_cgi_check_fd_leak);
    run_test("Keep-Alive Connection", test_keep_alive);
    run_test("Keep-Alive Connection (No Connection Header)", test_keep_alive_no_connection_header);
    run_test("Close HTTP/1.0 Connection", test_close_http_1_0_connection);

    std::cout << "\n" << COLOR_GREEN << g_passed_tests << " tests passed" << COLOR_RESET
                << ", " << COLOR_RED << g_failed_tests << " tests failed" << COLOR_RESET << std::endl;
    
    cleanup_env();
    return g_failed_tests == 0 ? 0 : 1;
}