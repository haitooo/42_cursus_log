#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>

#include "../../include/HttpStatus.hpp"
#include "../../include/RequestHandler.hpp"
#include "StubCgiExecutor.hpp"

namespace
{
    // =========================================================================
    // Test Framework
    // =========================================================================

    #define COLOR_GREEN "\033[32m"
    #define COLOR_RED   "\033[31m"
    #define COLOR_RESET "\033[0m"

    std::string g_test_name;

    struct TestResult
    {
        int passed;
        int failed;
        TestResult() : passed(0), failed(0) {}
    };

    // =========================================================================
    // Stub Classes
    // =========================================================================

    class MultipartConfig : public IConfig
    {
    public:
        explicit MultipartConfig(const std::string &store) : _store(store)
        {
            _server.host = "127.0.0.1";
            _server.port = 8080;
        }

        virtual std::size_t serverCount() const { return 1; }
        virtual const ServerConfig &listenAddress(ServerId) const { return _server; }
        virtual std::size_t clientMaxBodySize(ServerId) const { return 1024 * 1024; }

        virtual bool resolveLocation(ServerId, const std::string &path,
                                     ResolvedLocation &location) const
        {
            location.root = "./www";
            location.locationPath = "/upload";
            location.methods.push_back("POST");
            location.uploadStore = _store;
            location.clientMaxBodySize = 1024 * 1024;
            return path.find("/upload") == 0;
        }

        virtual std::string mapUriToPath(const ResolvedLocation &location,
                                         const std::string &path) const
        {
            return location.root + path;
        }

        virtual bool errorPagePath(ServerId, int, std::string &) const { return false; }

    private:
        std::string _store;
        ServerConfig _server;
    };

    // =========================================================================
    // Helper Functions
    // =========================================================================

    bool check(bool condition, const std::string &message)
    {
        if (condition)
            return true;
        std::cerr << "  " << COLOR_RED << message << COLOR_RESET << std::endl;
        return false;
    }

    bool expectStatus(const HttpResponse &response, int expected)
    {
        std::ostringstream message;
        message << "expected status " << expected << ", got " << response.statusCode;
        return check(response.statusCode == expected, message.str());
    }

    bool expectHeader(const HttpResponse &response, const std::string &name,
                      const std::string &expected)
    {
        std::map<std::string, std::string>::const_iterator it = response.headers.find(name);
        if (it == response.headers.end())
            return check(false, "missing response header: " + name);
        return check(it->second == expected, "unexpected response header: " + name);
    }

    bool fileEquals(const std::string &path, const std::string &expected)
    {
        std::ifstream file(path.c_str(), std::ios::binary);
        if (!file.is_open())
            return check(false, "file was not created: " + path);
        const std::string actual((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
        return check(actual == expected, "saved file content differs");
    }

    HttpResponse sendMultipart(const std::string &contentType,
                               const std::string &body,
                               const std::string &store = "./www/uploads")
    {
        MultipartConfig config(store);
        StubCgiExecutor cgi;
        // RequestHandler holds a SessionStore reference, so it has to
        // outlive the handler. Each test gets a fresh, empty store.
        SessionStore sessions(SESSION_TTL_SEC, SESSION_MAX_COUNT);
        RequestHandler handler(config, cgi, sessions);
        HttpRequest request;

        request.method = "POST";
        request.path = "/upload/";
        request.headers["content-type"] = contentType;
        request.body = body;
        return handler.handle(0, request).response;
    }

    std::string oneFile(const std::string &boundary, const std::string &headers,
                        const std::string &body)
    {
        return "--" + boundary + "\r\n" + headers + "\r\n\r\n" + body
            + "\r\n--" + boundary + "--\r\n";
    }

    bool testOneFile()
    {
        const std::string body = oneFile(
            "AaB03x",
            "Content-Disposition: form-data; name=\"file\"; filename=\"one.txt\"\r\n"
            "Content-Type: text/plain",
            "hello multipart");
        const HttpResponse response = sendMultipart("multipart/form-data; boundary=AaB03x", body);
        return expectStatus(response, STATUS_CREATED)
            && expectHeader(response, "Location", "/upload/")
            && fileEquals("./www/uploads/one.txt", "hello multipart");
    }

    bool testMultipleFiles()
    {
        const std::string body =
            "--multi\r\nContent-Disposition: form-data; name=\"a\"; filename=\"a.txt\"\r\n\r\na\r\n"
            "--multi\r\nContent-Disposition: form-data; name=\"b\"; filename=\"b.txt\"\r\n"
            "Content-Type: text/plain\r\n\r\nb\r\n--multi--\r\n";
        const HttpResponse response = sendMultipart("multipart/form-data; boundary=multi", body);
        return expectStatus(response, STATUS_CREATED)
            && fileEquals("./www/uploads/a.txt", "a")
            && fileEquals("./www/uploads/b.txt", "b");
    }

    bool testBinaryFile()
    {
        const std::string prefix =
            "--binary\r\nContent-Disposition: form-data; name=\"f\"; filename=\"b.bin\"\r\n"
            "Content-Type: application/octet-stream\r\n\r\n";
        const char bytes[] = {'A', '\0', 'B', '\xff', 'C'};
        const std::string body = prefix + std::string(bytes, sizeof(bytes))
            + "\r\n--binary--\r\n";
        const HttpResponse response = sendMultipart(
            "multipart/form-data; boundary=binary", body);
        return expectStatus(response, STATUS_CREATED)
            && fileEquals("./www/uploads/b.bin", std::string(bytes, sizeof(bytes)));
    }

    bool testAllowedBoundaryCharacters()
    {
        const std::string boundary = "0123abcXYZ'()+_,-./:=?";
        const std::string body = oneFile(boundary,
            "Content-Disposition: form-data; name=\"f\"; filename=\"chars.txt\"", "ok");
        return expectStatus(sendMultipart("multipart/form-data; boundary=" + boundary, body),
                            STATUS_CREATED);
    }

    bool testCaseInsensitivePartHeaders()
    {
        const std::string body = oneFile("case",
            "content-disposition: form-data; name=\"f\"; filename=\"case.txt\"\r\n"
            "content-type: text/plain", "case");
        return expectStatus(sendMultipart("multipart/form-data; boundary=case", body), STATUS_CREATED)
            && fileEquals("./www/uploads/case.txt", "case");
    }

    bool badContentType(const std::string &contentType)
    {
        return expectStatus(sendMultipart(contentType, "ignored"), STATUS_BAD_REQUEST);
    }

    bool testMissingBoundary() { return badContentType("multipart/form-data"); }
    bool testWrongPrefix() { return badContentType("application/json; multipart/form-data; boundary=x"); }
    bool testMissingSemicolon() { return badContentType("multipart/form-data boundary=x"); }
    bool testEmptyBoundary() { return badContentType("multipart/form-data; boundary="); }
    bool testBoundaryWhitespace() { return badContentType("multipart/form-data;  boundary=x"); }

    bool testBoundaryTooLong()
    {
        return badContentType("multipart/form-data; boundary=" + std::string(71, 'a'));
    }

    bool testBoundaryInvalidCharacter()
    {
        return badContentType("multipart/form-data; boundary=bad boundary");
    }

    bool testBodyWithoutOpeningBoundary()
    {
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", "not-x"),
                            STATUS_BAD_REQUEST);
    }

    bool testOpeningBoundaryWithoutCrlf()
    {
        const std::string body = "--x Content-Disposition: form-data; name=\"f\"; filename=\"x\"\r\n"
            "\r\nbody\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_BAD_REQUEST);
    }

    bool testPartHeaderSeparatorMissing()
    {
        const std::string body = "--x\r\nContent-Disposition: form-data; name=\"f\"; filename=\"x\"\r\n"
            "body\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_BAD_REQUEST);
    }

    bool testPartHeaderColonMissing()
    {
        const std::string body = "--x\r\nContent-Disposition form-data; name=\"f\"; filename=\"x\"\r\n"
            "\r\nbody\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_BAD_REQUEST);
    }

    bool testDispositionMissing()
    {
        const std::string body = "--x\r\nContent-Type: text/plain\r\n\r\nbody\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_BAD_REQUEST);
    }

    bool testNameMissing()
    {
        const std::string body = "--x\r\nContent-Disposition: form-data; filename=\"x\"\r\n\r\nbody\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_BAD_REQUEST);
    }

    bool testNameEmpty()
    {
        const std::string body = "--x\r\nContent-Disposition: form-data; name=\"\"; filename=\"x\"\r\n\r\nbody\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_BAD_REQUEST);
    }

    bool testFilenameMissing()
    {
        const std::string body = "--x\r\nContent-Disposition: form-data; name=\"f\"\r\n\r\nbody\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_CREATED);
    }

    bool testFilenameEmpty()
    {
        const std::string body = "--x\r\nContent-Disposition: form-data; name=\"f\"; filename=\"\"\r\n\r\nbody\r\n--x--\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_CREATED);
    }

    bool testClosingBoundaryMissing()
    {
        const std::string body = "--x\r\nContent-Disposition: form-data; name=\"f\"; filename=\"x\"\r\n\r\nbody\r\n";
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body), STATUS_BAD_REQUEST);
    }

    bool testUploadFileCannotOpen()
    {
        const std::string body = oneFile("x",
            "Content-Disposition: form-data; name=\"f\"; filename=\"x.txt\"", "body");
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body,
                                          "./www/no-such-directory"), STATUS_INTERNAL_SERVER_ERROR);
    }

    bool testUploadStoreMissing()
    {
        const std::string body = oneFile("x",
            "Content-Disposition: form-data; name=\"f\"; filename=\"x.txt\"", "body");
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body, ""),
                            STATUS_FORBIDDEN);
    }

    bool testFilenamePathTraversal()
    {
        const std::string body = oneFile("x",
            "Content-Disposition: form-data; name=\"f\"; filename=\"../outside.txt\"",
            "body");
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body),
                            STATUS_BAD_REQUEST);
    }

    bool testFilenameContainsDirectorySeparator()
    {
        const std::string body = oneFile("x",
            "Content-Disposition: form-data; name=\"f\"; filename=\"dir/file.txt\"",
            "body");
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body),
                            STATUS_BAD_REQUEST);
    }

    bool testDispositionParameterQuoteMissing()
    {
        const std::string body = oneFile("x",
            "Content-Disposition: form-data; name=\"f; filename=\"x.txt\"",
            "body");
        return expectStatus(sendMultipart("multipart/form-data; boundary=x", body),
                            STATUS_BAD_REQUEST);
    }

    typedef bool (*TestFunction)();

    void runTest(const std::string &name, TestFunction function, TestResult &result)
    {
        g_test_name = name;
        std::cout << "[RUN ] " << name << std::endl;
        if (function())
        {
            ++result.passed;
            std::cout << COLOR_GREEN << "[ OK ] " << COLOR_RESET
                      << name << std::endl;
        }
        else
        {
            ++result.failed;
            std::cout << COLOR_RED << "[FAIL] " << COLOR_RESET
                      << name << std::endl;
        }
    }
}

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "  Multipart Form Test Suite             " << std::endl;
    std::cout << "========================================" << std::endl;

    std::string str = "----abc123\r\nContent-Disposition: form-data; name=\"file1\"; filename=\"a.txt\"\r\n\r\nAAA\r\n----abc123\r\nContent-Disposition: form-data; name=\"file2\"; filename=\"b.txt\"\r\n\r\nBBB\r\n----abc123\r\nContent-Disposition: form-data; name=\"file3\"; filename=\"c.txt\"\r\n\r\nCCC\r\n----abc123--\r\n";
    std::cout << "[INFO] Multipart body size: " << str.size() << std::endl;
    const struct TestCase
    {
        const char *name;
        TestFunction function;
    } tests[] =
    {
        {"normal: one text file", testOneFile},
        {"normal: multiple files", testMultipleFiles},
        {"normal: binary file", testBinaryFile},
        {"normal: allowed boundary characters", testAllowedBoundaryCharacters},
        {"normal: case-insensitive part headers", testCaseInsensitivePartHeaders},
        {"error 400: missing boundary", testMissingBoundary},
        {"error 400: wrong content-type prefix", testWrongPrefix},
        {"error 400: missing semicolon", testMissingSemicolon},
        {"error 400: empty boundary", testEmptyBoundary},
        {"error 400: boundary whitespace", testBoundaryWhitespace},
        {"error 400: boundary too long", testBoundaryTooLong},
        {"error 400: invalid boundary character", testBoundaryInvalidCharacter},
        {"error 400: opening boundary missing", testBodyWithoutOpeningBoundary},
        {"error 400: opening boundary CRLF missing", testOpeningBoundaryWithoutCrlf},
        {"error 400: part separator missing", testPartHeaderSeparatorMissing},
        {"error 400: part header colon missing", testPartHeaderColonMissing},
        {"error 400: disposition missing", testDispositionMissing},
        {"error 400: name missing", testNameMissing},
        {"error 400: name empty", testNameEmpty},
        {"error 400: filename missing", testFilenameMissing},
        {"error 400: filename empty", testFilenameEmpty},
        {"error 400: closing boundary missing", testClosingBoundaryMissing},
        {"error 500: upload file cannot open", testUploadFileCannotOpen},
        {"error 403: upload store missing", testUploadStoreMissing},
        {"security 400: filename path traversal", testFilenamePathTraversal},
        {"security 400: filename directory separator", testFilenameContainsDirectorySeparator},
        {"error 400: disposition quote missing", testDispositionParameterQuoteMissing}
    };
    TestResult result;

    std::system("mkdir -p www/uploads");
    for (std::size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i)
        runTest(tests[i].name, tests[i].function, result);
    std::cout << "========================================" << std::endl;
    std::cout << COLOR_GREEN << "Passed: " << result.passed << COLOR_RESET
              << ", " << COLOR_RED << "Failed: " << result.failed
              << COLOR_RESET << std::endl;
    return result.failed == 0 ? 0 : 1;
}
