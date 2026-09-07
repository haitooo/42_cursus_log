#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <map>
#include <sstream>
#include <string>

struct HttpResponse
{
    std::string version;
    int statusCode;
    std::string reasonPhrase;
    std::map<std::string, std::string> headers;
    std::string body;

    HttpResponse()
        : version("HTTP/1.0"),
          statusCode(200),
          reasonPhrase("OK"),
          headers(),
          body()
    {
    }

    /*
    ** Status line and headers, up to and including the blank line that ends
    ** them. The body is deliberately left out.
    **
    ** Callers that send the response should use this and send the body from
    ** its own buffer: concatenating the two costs a full extra copy of the
    ** body, which for CGI output can be hundreds of megabytes.
    */
    std::string serializeHead() const
    {
        std::ostringstream result;

        result << version << " "
               << statusCode << " "
               << reasonPhrase << "\r\n";

        std::map<std::string, std::string>::const_iterator it;

        for (it = headers.begin(); it != headers.end(); ++it)
        {
            result << it->first
                   << ": "
                   << it->second
                   << "\r\n";
        }

        result << "\r\n";

        return result.str();
    }

    // Head and body in one buffer. Convenient, but it duplicates the body;
    // prefer serializeHead() plus the body on any path that can be large.
    std::string serialize() const
    {
        std::string result = serializeHead();

        result += body;

        return result;
    }
};

#endif
