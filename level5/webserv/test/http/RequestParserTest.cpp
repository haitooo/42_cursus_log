#include <iostream>
#include <string>
#include <cstdlib>

#include "../../include/RequestParser.hpp"

// =========================================================================
// Test Framework
// =========================================================================

#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_RESET "\033[0m"

std::string g_test_name;
int g_passed_tests = 0;

// Comparison assertion (terminates test on mismatch)
template <typename T, typename U>
void expect_eq(const T& actual, const U& expected, const char* actual_str, const char* expected_str, int line)
{
	if (actual != expected) {
		std::cerr << "\n" << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name << "\n"
		          << "  L" << line << ": " << actual_str << " == " << expected_str << " failed.\n"
		          << "  Actual:   " << actual << "\n"
		          << "  Expected: " << expected << std::endl;
		std::exit(1);
	}
}

#define EXPECT_EQ(actual, expected) expect_eq(actual, expected, #actual, #expected, __LINE__)

// Test runner (prints PASS on success)
void run_test(const std::string& name, void (*test_func)())
{
	g_test_name = name;
	std::cout << "[RUN ] " << name << std::endl;
	test_func();
	++g_passed_tests;
	std::cout << COLOR_GREEN << "[ OK ] " << COLOR_RESET << name << std::endl;
}

// =========================================================================
// Helper Functions
// =========================================================================

inline ParseStatus append(RequestParser& parser, const std::string& data)
{
	return parser.append(data.data(), data.size());
}

inline ParseStatus appendBin(RequestParser& parser, const char* data, std::size_t size)
{
	return parser.append(data, size);
}

// =========================================================================
// Test Cases
// =========================================================================

void test_normal_get()
{
	RequestParser parser;
	ParseStatus status = append(parser, "GET /index.html HTTP/1.0\r\nHost: localhost\r\nUser-Agent: curl\r\n\r\n");
	
	EXPECT_EQ(status, PARSE_COMPLETE);
	EXPECT_EQ(parser.getRequest().method, "GET");
	EXPECT_EQ(parser.getRequest().path, "/index.html");
	EXPECT_EQ(parser.getRequest().version, "HTTP/1.0");
	EXPECT_EQ(parser.getRequest().headers.at("host"), "localhost");
}

void test_normal_chunked_receive()
{
	RequestParser parser;
	
	// std::string raw = "POST /api HTTP/1.1\r\nContent-Length: 5\r\n\r\nhello";
	std::string raw = "POST /api HTTP/1.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nhello";
	
	for (std::size_t i = 0; i < raw.size() - 1; ++i) {
		ParseStatus status = appendBin(parser, &raw[i], 1);
		EXPECT_EQ(status, PARSE_INCOMPLETE);
	}
	
	ParseStatus final_status = appendBin(parser, &raw[raw.size() - 1], 1);
	EXPECT_EQ(final_status, PARSE_COMPLETE);
	EXPECT_EQ(parser.getRequest().body, "hello");
}

void test_normal_binary_body()
{
	RequestParser parser;
	
	append(parser, "POST / HTTP/1.0\r\nContent-Length: 4\r\n\r\n");
	
	char binData[4] = {'A', '\0', 'B', '\0'};
	ParseStatus status = appendBin(parser, binData, 4);
	
	EXPECT_EQ(status, PARSE_COMPLETE);
	EXPECT_EQ(parser.getRequest().body.size(), (std::size_t)4);
	EXPECT_EQ(parser.getRequest().body[1], '\0');
}

void test_error_missing_request_line_parts()
{
	RequestParser parser;
	ParseStatus status = append(parser, "GET /index.html\r\n\r\n");
	EXPECT_EQ(status, PARSE_ERROR);
	EXPECT_EQ(parser.getErrorStatus(), 400); // Bad Request
}

void test_error_unsupported_http_version()
{
	RequestParser parser;
	ParseStatus status = append(parser, "GET / HTTP/2.0\r\n\r\n");
	EXPECT_EQ(status, PARSE_ERROR);
	EXPECT_EQ(parser.getErrorStatus(), 505); // HTTP Version Not Supported
}

void test_error_header_no_colon()
{
	RequestParser parser;
	ParseStatus status = append(parser, "GET / HTTP/1.0\r\nHost localhost\r\n\r\n");
	EXPECT_EQ(status, PARSE_ERROR);
	EXPECT_EQ(parser.getErrorStatus(), 400);
}

void test_error_header_empty_key()
{
	RequestParser parser;
	ParseStatus status = append(parser, "GET / HTTP/1.0\r\n: value\r\n\r\n");
	EXPECT_EQ(status, PARSE_ERROR);
	EXPECT_EQ(parser.getErrorStatus(), 400);
}

void test_error_content_length_not_number()
{
	RequestParser parser;
	ParseStatus status = append(parser, "POST / HTTP/1.0\r\nContent-Length: 10abc\r\n\r\n");
	EXPECT_EQ(status, PARSE_ERROR);
	EXPECT_EQ(parser.getErrorStatus(), 400);
}

int main()
{
	std::cout << "========================================" << std::endl;
	std::cout << "    RequestParser Strict Test Suite     " << std::endl;
	std::cout << "========================================" << std::endl;

	run_test("Normal: GET Request", test_normal_get);
	run_test("Normal: 1-Byte Chunked Receive", test_normal_chunked_receive);
	run_test("Normal: Binary Safety Body", test_normal_binary_body);

	run_test("Error: Missing Request Line Parts", test_error_missing_request_line_parts);
	run_test("Error: Unsupported HTTP Version", test_error_unsupported_http_version);
	run_test("Error: Header Missing Colon", test_error_header_no_colon);
	run_test("Error: Header Empty Key", test_error_header_empty_key);
	run_test("Error: Content-Length is Not Number", test_error_content_length_not_number);

	std::cout << "========================================" << std::endl;
	std::cout << COLOR_GREEN << "Passed: " << g_passed_tests << COLOR_RESET
	          << ", " << COLOR_RED << "Failed: 0" << COLOR_RESET << std::endl;
	return 0;
}


// #include <cstddef>
// #include <iostream>
// #include <map>
// #include <string>

// #include "../../include/RequestParser.hpp"

// namespace
// {
//     struct TestResult
//     {
//         int passed;
//         int failed;

//         TestResult() : passed(0), failed(0) {}
//     };

//     bool expect(bool condition, const std::string &message)
//     {
//         if (!condition)
//             std::cerr << "  " << message << std::endl;
//         return condition;
//     }

//     bool expectStatus(ParseStatus actual, ParseStatus expected, const std::string &label)
//     {
//         if (actual == expected)
//             return true;
//         std::cerr << "  " << label << ": expected status " << expected
//                   << ", got " << actual << std::endl;
//         return false;
//     }

//     bool expectField(const std::string &actual, const std::string &expected,
//                      const std::string &label)
//     {
//         if (actual == expected)
//             return true;
//         std::cerr << "  " << label << ": expected '" << expected
//                   << "', got '" << actual << "'" << std::endl;
//         return false;
//     }

//     bool expectHeader(const HttpRequest &request, const std::string &key,
//                       const std::string &expected)
//     {
//         std::map<std::string, std::string>::const_iterator it = request.headers.find(key);
//         if (it == request.headers.end())
//         {
//             std::cerr << "  missing header: " << key << std::endl;
//             return false;
//         }
//         if (it->second == expected)
//             return true;
//         std::cerr << "  header '" << key << "': expected '" << expected
//                   << "', got '" << it->second << "'" << std::endl;
//         return false;
//     }

//     bool expectNoHeader(const HttpRequest &request, const std::string &key)
//     {
//         if (request.headers.find(key) == request.headers.end())
//             return true;
//         std::cerr << "  unexpected header: " << key << std::endl;
//         return false;
//     }

//     bool testGetRequestSplitAcrossChunks()
//     {
//         RequestParser parser;
//         bool ok = true;

//         const std::string chunk1 = "GET /index.html HTTP/1.0\r\nHost: example.com\r\n";
//         const std::string chunk2 = "Accept: */*\r\nUser-Agent: test-suite\r\n";
//         const std::string chunk3 = "\r\n";

//         ok &= expectStatus(parser.append(chunk1.c_str(), chunk1.size()), PARSE_INCOMPLETE,
//                            "GET request should stay incomplete after partial headers");
//         ok &= expectStatus(parser.append(chunk2.c_str(), chunk2.size()), PARSE_INCOMPLETE,
//                            "GET request should stay incomplete before header terminator");
//         ok &= expectStatus(parser.append(chunk3.c_str(), chunk3.size()), PARSE_COMPLETE,
//                            "GET request should complete after header terminator");

//         const HttpRequest &request = parser.getRequest();
//         ok &= expectField(request.method, "GET", "method");
//         ok &= expectField(request.path, "/index.html", "path");
//         ok &= expectField(request.version, "HTTP/1.0", "version");
//         ok &= expectHeader(request, "Host", "example.com");
//         ok &= expectHeader(request, "Accept", "*/*");
//         ok &= expectHeader(request, "User-Agent", "test-suite");
//         ok &= expectField(request.body, "", "body");

//         return ok;
//     }

//     bool testPostBodySplitAcrossChunks()
//     {
//         RequestParser parser;
//         bool ok = true;

//         const std::string chunk1 =
//             "POST /upload HTTP/1.0\r\n"
//             "Content-Length: 5\r\n"
//             "Content-Type: text/plain\r\n"
//             "\r\n"
//             "he";
//         const std::string chunk2 = "llo";

//         ok &= expectStatus(parser.append(chunk1.c_str(), chunk1.size()), PARSE_INCOMPLETE,
//                            "POST request should wait for the rest of the body");
//         ok &= expectStatus(parser.append(chunk2.c_str(), chunk2.size()), PARSE_COMPLETE,
//                            "POST request should complete once the body length matches");

//         const HttpRequest &request = parser.getRequest();
//         ok &= expectField(request.method, "POST", "method");
//         ok &= expectField(request.path, "/upload", "path");
//         ok &= expectField(request.version, "HTTP/1.0", "version");
//         ok &= expectHeader(request, "Content-Length", "5");
//         ok &= expectHeader(request, "Content-Type", "text/plain");
//         ok &= expectField(request.body, "hello", "body");

//         return ok;
//     }

//     bool testDeleteRequestWithoutBody()
//     {
//         RequestParser parser;
//         bool ok = true;

//         const std::string requestText = "DELETE /resource/42 HTTP/1.0\r\n\r\n";

//         ok &= expectStatus(parser.append(requestText.c_str(), requestText.size()), PARSE_COMPLETE,
//                            "DELETE request should complete without a body");

//         const HttpRequest &request = parser.getRequest();
//         ok &= expectField(request.method, "DELETE", "method");
//         ok &= expectField(request.path, "/resource/42", "path");
//         ok &= expectField(request.version, "HTTP/1.0", "version");
//         ok &= expect(request.headers.empty(), "DELETE request should not need headers");
//         ok &= expectField(request.body, "", "body");

//         return ok;
//     }

//     bool testInvalidMethod()
//     {
//         RequestParser parser;
//         bool ok = true;

//         const std::string requestText = "PUT / HTTP/1.0\r\n\r\n";

//         ok &= expectStatus(parser.append(requestText.c_str(), requestText.size()), PARSE_ERROR,
//                            "unsupported method should fail");
//         ok &= expect(parser.getErrorStatus() == 405, "error status should be 405");

//         return ok;
//     }

//     bool testInvalidVersion()
//     {
//         RequestParser parser;
//         bool ok = true;

//         const std::string requestText = "GET / HTTP/2.0\r\n\r\n";

//         ok &= expectStatus(parser.append(requestText.c_str(), requestText.size()), PARSE_ERROR,
//                            "unsupported HTTP version should fail");
//         ok &= expect(parser.getErrorStatus() == 505, "error status should be 505");

//         return ok;
//     }

//     bool testAppendAfterComplete()
//     {
//         RequestParser parser;
//         bool ok = true;

//         const std::string requestText = "GET / HTTP/1.0\r\n\r\n";

//         ok &= expectStatus(parser.append(requestText.c_str(), requestText.size()), PARSE_COMPLETE,
//                            "request should complete");
//         ok &= expectStatus(parser.append("ignored", 7), PARSE_COMPLETE,
//                            "append after complete should stay complete");

//         return ok;
//     }

//     TestResult runAll()
//     {
//         TestResult result;

//         struct Case
//         {
//             const char *name;
//             bool (*fn)();
//         };

//         const Case cases[] =
//         {
//             {"GET request split across chunks", testGetRequestSplitAcrossChunks},
//             {"POST body split across chunks", testPostBodySplitAcrossChunks},
//             {"DELETE request without body", testDeleteRequestWithoutBody},
//             {"invalid method returns 405", testInvalidMethod},
//             {"invalid version returns 505", testInvalidVersion},
//             {"append after complete stays complete", testAppendAfterComplete},
//         };

//         const std::size_t caseCount = sizeof(cases) / sizeof(cases[0]);

//         for (std::size_t i = 0; i < caseCount; ++i)
//         {
//             std::cout << "[RUN ] " << cases[i].name << std::endl;
//             if (cases[i].fn())
//             {
//                 ++result.passed;
//                 std::cout << "[ OK ] " << cases[i].name << std::endl;
//             }
//             else
//             {
//                 ++result.failed;
//                 std::cout << "[FAIL] " << cases[i].name << std::endl;
//             }
//         }

//         return result;
//     }
// }

// int main()
// {
//     const TestResult result = runAll();

//     std::cout << std::endl
//               << "Passed: " << result.passed
//               << ", Failed: " << result.failed << std::endl;

//     return result.failed == 0 ? 0 : 1;
// }