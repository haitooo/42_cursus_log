// ==========================================
// StubConfig.hpp
// ==========================================
#ifndef STUB_CONFIG_HPP
#define STUB_CONFIG_HPP

#include "IConfig.hpp"
#include <string>

class StubConfig : public IConfig {
public:
    StubConfig() {
        listenAddr.host = "127.0.0.1";
        listenAddr.port = 8080;
    }

    bool resolveLocation(ServerId, const std::string& uriPath, ResolvedLocation& out) const {
        out.root = "./www";
        out.clientMaxBodySize = 1024 * 1024; // 1MB
        out.redirect.enabled = false;
        out.cgi.enabled = false;
        const char* methods[] = {"GET", "POST", "DELETE"};
        std::vector<std::string> methodsVec(methods, methods + sizeof(methods) / sizeof(methods[0]));
        out.methods = methodsVec;

        if (uriPath == "/not_found" || uriPath == "/not_found.html") {
            return false;
        }
        if (uriPath.find("/upload") == 0) {
            out.methods.clear();
            out.methods.push_back("POST");
            out.uploadStore = "./www/uploads";
            out.index = "";
            return true;
        }
        if (uriPath.find("/admin") == 0) {
            // out.allowGet = false;
            out.methods.clear();
        }
        if (uriPath.find("/auto_on") == 0) {
            out.index = "";
            out.autoindex = true;
        }
        else if (uriPath.find("/auto_off") == 0) {
            out.index = "";
            out.autoindex = false;
        }
        else {
            out.index = "index.html";
            out.autoindex = false;
        }
        out.locationPath = uriPath;
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
    const ServerConfig& listenAddress(ServerId) const {
        return listenAddr;
    }
    std::size_t clientMaxBodySize(ServerId) const { return 1024 * 1024; } // 1MB
    
    private:
        ServerConfig listenAddr;
};

#endif