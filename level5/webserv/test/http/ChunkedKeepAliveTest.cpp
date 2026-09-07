#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../../include/HttpStatus.hpp"
#include "../../include/RequestParser.hpp"

namespace
{
    // =========================================================================
    // Test Framework
    // =========================================================================

    #define COLOR_GREEN "\033[32m"
    #define COLOR_RED   "\033[31m"
    #define COLOR_RESET "\033[0m"

    std::string g_test_name;

    // =========================================================================
    // Helper Functions
    // =========================================================================

    bool expect(bool condition, const std::string &message)
    {
        if (!condition)
        {
            std::cerr << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name
                      << ": " << message << std::endl;
            return false;
        }
        return true;
    }

    bool expectStatus(ParseStatus actual, ParseStatus expected,
                      const std::string &label)
    {
        if (actual == expected)
            return true;
        std::cerr << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name
                  << ": " << label << " expected " << expected
                  << ", got " << actual << std::endl;
        return false;
    }

    bool expectHeader(const HttpRequest &request, const std::string &name,
                      const std::string &expected, const std::string &label)
    {
        std::map<std::string, std::string>::const_iterator it = request.headers.find(name);
        if (it == request.headers.end())
        {
            std::cerr << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name
                      << ": " << label << " missing header " << name << std::endl;
            return false;
        }
        if (it->second == expected)
            return true;
        std::cerr << COLOR_RED << "[FAIL] " << COLOR_RESET << g_test_name
                  << ": " << label << " expected header " << name
                  << "='" << expected << "' but got '" << it->second << "'" << std::endl;
        return false;
    }

    ParseStatus append(RequestParser &parser, const std::string &data)
    {
        return parser.append(data.data(), data.size());
    }

    struct TestResult
    {
        int passed;
        int failed;

        TestResult() : passed(0), failed(0) {}
    };

    typedef bool (*TestFunction)();

    void runTest(const std::string &name, TestFunction function, TestResult &result)
    {
        g_test_name = name;
        std::cout << "[RUN ] " << name << std::endl;
        if (function())
        {
            ++result.passed;
            std::cout << COLOR_GREEN << "[ OK ] " << COLOR_RESET << name << std::endl;
        }
        else
        {
            ++result.failed;
            std::cout << COLOR_RED << "[FAIL] " << COLOR_RESET << name << std::endl;
        }
    }

    // =========================================================================
    // Test Cases
    // =========================================================================

    // =========================================================================
    // Normal Cases
    // =========================================================================

    bool testChunkedBodyReassembled()
    {
        RequestParser parser;
        const std::string request =
            "POST /chunked HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "4\r\nWiki\r\n"
            "5\r\npedia\r\n"
            "0\r\n\r\n";

        ParseStatus status = PARSE_INCOMPLETE;
        for (std::size_t i = 0; i < request.size(); ++i)
            status = parser.append(&request[i], 1);

        return expectStatus(status, PARSE_COMPLETE, "final parse status")
            && expect(parser.getRequest().method == "POST", "method should be POST")
            && expectHeader(parser.getRequest(), "transfer-encoding", "chunked",
                            "transfer-encoding header")
            && expect(parser.getRequest().body == "Wikipedia",
                      "chunk data should be reassembled without framing");
    }

    bool testChunkedBodyCanBeSplitAtEveryBoundary()
    {
        RequestParser parser;
        const std::string headers =
            "POST /chunked HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n\r\n";

        return expectStatus(append(parser, headers), PARSE_INCOMPLETE,
                            "headers wait for first chunk")
            && expectStatus(append(parser, "3\r\nabc\r\n"), PARSE_INCOMPLETE,
                            "non-final chunk waits")
            && expectStatus(append(parser, "1\r\nd\r\n"), PARSE_INCOMPLETE,
                            "partial final chunk waits")
            && expectStatus(append(parser, "0\r\n\r\n"), PARSE_COMPLETE,
                            "zero chunk completes")
            && expect(parser.getRequest().body == "abcd",
                      "chunk body should be exact");
    }

    bool testChunkedMultipleChunksAndEmptySegments()
    {
        RequestParser parser;
        const std::string request =
            "POST /x HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "3\r\nabc\r\n"
            "0\r\n"
            "\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_COMPLETE, "multiple chunk completion")
            && expect(parser.getRequest().body == "abc",
                      "chunk body should keep exact content");
    }

    bool testChunkedBinaryBody()
    {
        RequestParser parser;
        const std::string chunkBody = std::string("A", 1) + std::string("\0", 1)
            + std::string("B", 1) + std::string("\xFF", 1) + std::string("C", 1);

        std::string request =
            "POST /bin HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n";
        request += "5\r\n";
        request += chunkBody;
        request += "\r\n0\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_COMPLETE, "binary chunked request")
            && expect(parser.getRequest().body.size() == 5,
                      "binary body size should be preserved")
            && expect(parser.getRequest().body[1] == '\0',
                      "binary NUL byte must be preserved")
            && expect(static_cast<unsigned char>(parser.getRequest().body[3]) == 0xFF,
                      "binary 0xFF byte must be preserved");
    }

    bool testChunkedLowercaseHexAccepted()
    {
        RequestParser parser;
        const std::string request =
            "POST /hex HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "a\r\n0123456789\r\n"
            "0\r\n\r\n";

        return expectStatus(append(parser, request), PARSE_COMPLETE,
                            "lowercase chunk size")
            && expect(parser.getRequest().body == "0123456789",
                      "lowercase hex should be accepted");
    }

    bool testChunkedRejectsNonHexSize()
    {
        RequestParser parser;
        const std::string request =
            "POST /bad HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "g\r\nabc\r\n"
            "0\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "non-hex chunk size")
            && expect(parser.getErrorStatus() == STATUS_BAD_REQUEST,
                      "invalid chunk size should be 400");
    }

    bool testChunkedRejectsChunkExtension()
    {
        RequestParser parser;
        const std::string request =
            "POST /bad HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "4;foo=bar\r\nWiki\r\n"
            "0\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "chunk extension rejected")
            && expect(parser.getErrorStatus() == STATUS_BAD_REQUEST,
                      "chunk extension should be invalid here");
    }

    bool testChunkedRejectsOversizedChunk()
    {
        RequestParser parser;
        parser.setBodyLimit(3);
        const std::string request =
            "POST /big HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "4\r\nWiki\r\n"
            "0\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "oversized chunk")
            && expect(parser.getErrorStatus() == STATUS_PAYLOAD_TOO_LARGE,
                      "oversized chunk should be 413");
    }

    bool testChunkedRejectsTruncatedFinalZero()
    {
        RequestParser parser;
        const std::string request =
            "POST /trunc HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Transfer-Encoding: chunked\r\n"
            "\r\n"
            "3\r\nabc\r\n"
            "0\r\n";

        return expectStatus(append(parser, request), PARSE_INCOMPLETE,
                            "truncated final zero remains incomplete");
    }

    bool testHeadersTrimWhitespaceAndLowercase()
    {
        RequestParser parser;
        const std::string request =
            "GET /trim HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "X-Test:   value   \t \r\n"
            "Connection: keep-alive\r\n"
            "\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_COMPLETE, "header trimming")
            && expectHeader(parser.getRequest(), "x-test", "value",
                            "trimmed header value")
            && expectHeader(parser.getRequest(), "connection", "keep-alive",
                            "connection header");
    }

    bool testHostIsRequiredForHTTP11()
    {
        RequestParser parser;
        const std::string request =
            "GET /missing-host HTTP/1.1\r\n"
            "User-Agent: curl\r\n"
            "\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "HTTP/1.1 without Host")
            && expect(parser.getErrorStatus() == STATUS_BAD_REQUEST,
                      "missing Host must be 400");
    }

    bool testHostIsOptionalForHTTP10()
    {
        RequestParser parser;
        const std::string request =
            "GET /nohost HTTP/1.0\r\n"
            "User-Agent: curl\r\n"
            "\r\n";

        return expectStatus(append(parser, request), PARSE_COMPLETE,
                            "HTTP/1.0 no Host accepted");
    }

    bool testContentLengthRejectsNonNumericValue()
    {
        RequestParser parser;
        const std::string request =
            "POST /bad HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 10abc\r\n"
            "\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "non-numeric Content-Length")
            && expect(parser.getErrorStatus() == STATUS_BAD_REQUEST,
                      "bad Content-Length should be 400");
    }

    bool testContentLengthRejectsOverflow()
    {
        RequestParser parser;
        std::string huge(20, '9');
        const std::string request =
            "POST /overflow HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: " + huge + "\r\n"
            "\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "Content-Length overflow")
            && expect(parser.getErrorStatus() == STATUS_BAD_REQUEST,
                      "overflow should be rejected");
    }

    bool testBodyLimitRejectsLargeContentLength()
    {
        RequestParser parser;
        parser.setBodyLimit(4);
        const std::string request =
            "POST /large HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 5\r\n"
            "\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "body limit")
            && expect(parser.getErrorStatus() == STATUS_PAYLOAD_TOO_LARGE,
                      "body limit should be 413");
    }

    bool testUnknownMethodRejected()
    {
        RequestParser parser;
        const std::string request = "PATCH / HTTP/1.1\r\nHost: localhost\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "unknown method")
            && expect(parser.getErrorStatus() == STATUS_NOT_IMPLEMENTED,
                      "unknown methods should be 501");
    }

    bool testUnknownVersionRejected()
    {
        RequestParser parser;
        const std::string request = "GET / HTTP/2.0\r\nHost: localhost\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "unknown version")
            && expect(parser.getErrorStatus() == STATUS_HTTP_VERSION_NOT_SUPPORTED,
                      "unsupported version should be 505");
    }

    bool testRequestLineRejectsExtraSpaces()
    {
        RequestParser parser;
        const std::string request = "GET  /bad HTTP/1.1\r\nHost: localhost\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "extra spaces")
            && expect(parser.getErrorStatus() == STATUS_BAD_REQUEST,
                      "extra spaces are invalid");
    }

    bool testRequestLineRejectsMissingParts()
    {
        RequestParser parser;
        const std::string request = "GET /bad\r\n\r\n";

        ParseStatus status = append(parser, request);
        return expectStatus(status, PARSE_ERROR, "missing request parts")
            && expect(parser.getErrorStatus() == STATUS_BAD_REQUEST,
                      "missing version should be 400");
    }

    bool testKeepAliveResetClearsState()
    {
        RequestParser parser;
        const std::string first =
            "GET /first HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Connection: keep-alive\r\n\r\n";
        const std::string second =
            "POST /second HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Length: 5\r\n"
            "Connection: close\r\n\r\n"
            "hello";

        return expectStatus(append(parser, first), PARSE_COMPLETE,
                            "first request complete")
            && expectHeader(parser.getRequest(), "connection", "keep-alive",
                            "first keep-alive header")
            && (parser.reset(), true)
            && expectStatus(append(parser, second), PARSE_COMPLETE,
                            "second request complete after reset")
            && expect(parser.getRequest().method == "POST", "second method should update")
            && expect(parser.getRequest().path == "/second", "second path should update")
            && expect(parser.getRequest().body == "hello", "second body should be preserved")
            && expectHeader(parser.getRequest(), "connection", "close",
                            "second connection header");
    }

    bool testResetOnPartialRequestClearsBuffer()
    {
        RequestParser parser;
        const std::string partial = "POST /buf HTTP/1.1\r\nHost: localhost\r\nContent-Length: 7\r\n\r\nhel";

        return expectStatus(append(parser, partial), PARSE_INCOMPLETE,
                            "partial request incomplete")
            && (parser.reset(), true)
            && expectStatus(append(parser, "GET /fresh HTTP/1.1\r\nHost: localhost\r\n\r\n"),
                            PARSE_COMPLETE,
                            "fresh request after reset");
    }

    bool testMultipleResetCyclesForKeepAlive()
    {
        RequestParser parser;
        const std::string a = "GET /a HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n";
        const std::string b = "DELETE /b HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
        const std::string c = "GET /c HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n";

        return expectStatus(append(parser, a), PARSE_COMPLETE, "first request")
            && (parser.reset(), true)
            && expectStatus(append(parser, b), PARSE_COMPLETE, "second request")
            && (parser.reset(), true)
            && expectStatus(append(parser, c), PARSE_COMPLETE, "third request")
            && expect(parser.getRequest().path == "/c",
                      "third request should replace previous state");
    }

    bool testCompletedParserDoesNotAcceptMoreData()
    {
        RequestParser parser;
        const std::string request = "GET /done HTTP/1.1\r\nHost: localhost\r\n\r\n";

        return expectStatus(append(parser, request), PARSE_COMPLETE,
                            "first complete parse")
            && expectStatus(append(parser, "GET /later HTTP/1.1\r\nHost: localhost\r\n\r\n"),
                            PARSE_COMPLETE,
                            "completed parser stays completed");
    }
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Chunked and Keep-Alive Test Suite     " << std::endl;
    std::cout << "========================================" << std::endl;

    const struct TestCase
    {
        const char *name;
        TestFunction function;
    } tests[] = {
        {"chunked: body reassembled", testChunkedBodyReassembled},
        {"chunked: split at every boundary", testChunkedBodyCanBeSplitAtEveryBoundary},
        {"chunked: multiple chunks and empty segment", testChunkedMultipleChunksAndEmptySegments},
        {"chunked: binary body preserved", testChunkedBinaryBody},
        {"chunked: lowercase hex accepted", testChunkedLowercaseHexAccepted},
        {"chunked: rejects non-hex chunk size", testChunkedRejectsNonHexSize},
        {"chunked: rejects chunk extensions", testChunkedRejectsChunkExtension},
        {"chunked: rejects oversized chunk", testChunkedRejectsOversizedChunk},
        {"chunked: truncated final zero stays incomplete", testChunkedRejectsTruncatedFinalZero},
        {"headers: trim whitespace and normalize case", testHeadersTrimWhitespaceAndLowercase},
        {"http/1.1: Host required", testHostIsRequiredForHTTP11},
        {"http/1.0: Host optional", testHostIsOptionalForHTTP10},
        {"Content-Length: rejects non-numeric value", testContentLengthRejectsNonNumericValue},
        {"Content-Length: rejects overflow", testContentLengthRejectsOverflow},
        {"body limit: large Content-Length rejected", testBodyLimitRejectsLargeContentLength},
        {"method: unknown methods rejected", testUnknownMethodRejected},
        {"version: unknown versions rejected", testUnknownVersionRejected},
        {"request line: extra spaces rejected", testRequestLineRejectsExtraSpaces},
        {"request line: missing parts rejected", testRequestLineRejectsMissingParts},
        {"keep-alive: reset clears state", testKeepAliveResetClearsState},
        {"keep-alive: reset on partial request clears buffer", testResetOnPartialRequestClearsBuffer},
        {"keep-alive: multiple resets work", testMultipleResetCyclesForKeepAlive},
        {"keep-alive: completed parser does not consume more data", testCompletedParserDoesNotAcceptMoreData}
    };

    TestResult result;
    for (std::size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i)
        runTest(tests[i].name, tests[i].function, result);

    std::cout << "========================================" << std::endl;
    std::cout << COLOR_GREEN << "Passed: " << result.passed << COLOR_RESET
              << ", " << COLOR_RED << "Failed: " << result.failed
              << COLOR_RESET << std::endl;
    return result.failed == 0 ? 0 : 1;
}