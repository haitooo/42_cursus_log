#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <map>
#include <string>

struct HttpRequest
{
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;

    HttpRequest()
        : method(),
          path(),
          version(),
          headers(),
          body()
    {
    }

    void clear()
    {
        method.clear();
        path.clear();
        version.clear();
        headers.clear();
        // Swapped rather than cleared: clear() keeps the capacity, and a body
        // that was hundreds of megabytes would stay resident on a keep-alive
        // connection until the client disconnects.
        std::string().swap(body);
    }

    /*
    ** Takes `src` over, moving the body instead of copying it.
    **
    ** Plain assignment leaves both objects holding the same bytes for as long
    ** as they are both alive; for a CGI upload that is a second copy of the
    ** whole body. `src` keeps its metadata and is left with an empty body.
    */
    void takeFrom(HttpRequest &src)
    {
        method = src.method;
        path = src.path;
        version = src.version;
        headers = src.headers;
        std::string().swap(body);
        body.swap(src.body);
    }
};

#endif
