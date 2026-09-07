*This project has been created as part of the 42 curriculum by haito, ywada, totake.*

# webserv

## Description

`webserv` is an HTTP server written from scratch in C++98. It serves static content, accepts file uploads, and executes CGI scripts, with its behaviour driven entirely by an NGINX-style configuration file.

The server is single-threaded and fully non-blocking. Every socket operation between the server and its clients — including `accept()` — goes through a single `poll()` call, so one process handles many simultaneous connections without ever blocking on a single slow client.

Goals of the project:

- Understand the HTTP request/response cycle by implementing it, rather than using a library
- Learn event-driven I/O multiplexing (`poll()`) and non-blocking socket programming
- Build a server that stays responsive and never crashes, even under abrupt client disconnections

## Instructions

### Requirements

- A C++ compiler supporting C++98 (`c++` / `clang++` / `g++`)
- GNU Make
- Linux (developed and tested on Linux; not tested on macOS)
- Python 3, for the example CGI scripts in `cgi-bin/`

No external libraries are used.

### Build

```bash
make
```

| Rule | Effect |
|---|---|
| `all` (default) | Builds the `webserv` binary |
| `clean` | Removes the `objs/` directory (object and dependency files) |
| `fclean` | `clean` + removes the `webserv` binary |
| `re` | `fclean` followed by `all` |

The build uses `-Wall -Wextra -Werror -std=c++98` and header dependency tracking (`-MMD -MP`), so editing a header only rebuilds the sources that include it.

### Run

```bash
./webserv [configuration file]
```

If no path is given, `conf/default.conf` is used.

```bash
./webserv                      # uses conf/default.conf
./webserv conf/default.conf    # explicit path
```

The server runs in the foreground and logs to standard output. Stop it with `Ctrl-C`.

### Try it

`conf/default.conf` is arranged so that every feature can be exercised from the
command line. With the server running:

```bash
curl http://127.0.0.1:8080/                       # static file, via index
curl http://127.0.0.1:8080/listing/               # directory listing (autoindex)
curl -i http://127.0.0.1:8080/old                 # 301 redirect
curl -i http://127.0.0.1:8080/nothing-here        # 404, custom error page
curl -i -X DELETE http://127.0.0.1:8080/          # 405, method not in this route

curl -i -X POST --data-binary @- http://127.0.0.1:8080/small <<< short   # 201
head -c 200 /dev/zero | curl -i -X POST --data-binary @- \
    http://127.0.0.1:8080/small                   # 413, over this route's limit

curl -i -X POST --data-binary 'hello' http://127.0.0.1:8080/upload  # 201 + Location
curl http://127.0.0.1:8080/upload/                # what has been uploaded
curl -X DELETE http://127.0.0.1:8080/upload/<name># 204

curl http://127.0.0.1:8080/cgi-bin/hello.py       # CGI, GET
curl -X POST -d body http://127.0.0.1:8080/cgi-bin/echo.py   # CGI, POST
curl http://127.0.0.1:8080/cgi-bin/cwd.py         # CGI reads the file beside it
curl -i http://127.0.0.1:8080/cgi-bin/hang.py     # 504 after the CGI deadline

curl http://127.0.0.1:8081/                       # a second site, another port
```

### Configuration file

The configuration file follows an NGINX-inspired `server` / `location` block syntax. See `conf/default.conf` for a working example, and `docs/CONFIG_SPEC.ja.md` / `docs/CONFIG_BNF.ja.md` for the full grammar.

```nginx
server {
    listen 127.0.0.1:8080;

    root ./www;
    index index.html;
    client_max_body_size 1048576;
    error_page 404 ./errors/404.html;

    location / {
        methods GET HEAD;
        autoindex off;
    }

    location /listing {
        methods GET;
        root ./www/listing;
        autoindex on;
    }

    location /old {
        methods GET;
        return 301 /index.html;
    }

    location /upload {
        methods GET POST DELETE;
        root ./uploads;
        upload_store ./uploads;
        autoindex on;
    }

    location /cgi-bin {
        methods GET POST HEAD;
        root ./cgi-bin;
        cgi_extension .py;
        cgi_path /usr/bin/python3;
    }
}
```

| Directive | Scope | Effect |
|---|---|---|
| `listen <host>:<port>` | server | Address to bind. A bare port listens on every interface |
| `root` | server, location | Filesystem prefix the URI is resolved against |
| `index` | server, location | File served when a directory is requested |
| `client_max_body_size` | server, location | Largest request body accepted, in bytes |
| `error_page <code> <path>` | server | Replaces the built-in page for that status |
| `methods` | location | Accepted HTTP methods. Anything else gets 405 with `Allow` |
| `autoindex on\|off` | location | Whether a directory without an index file is listed |
| `return <code> <target>` | location | Redirect instead of serving |
| `upload_store` | location | Directory `POST` bodies are written to |
| `cgi_extension` / `cgi_path` | location | Extension that triggers CGI, and the interpreter to run |

A directive written in a `location` overrides the one inherited from its `server`.

## Project structure

| Path | Contents |
|---|---|
| `srcs/` | Implementation files |
| `include/` | Headers internal to a single module |
| `schema/` | Interfaces shared between all three members. Changes here must be agreed on first |
| `conf/` | Configuration files: `default.conf` for demonstration, `tests/` for the parser tests |
| `test/` | Test suites and their fixtures |
| `docs/` | Design documents and specifications |
| `www/`, `errors/`, `cgi-bin/`, `uploads/` | Document root, error pages, example CGI scripts, and the upload destination used by `conf/default.conf` |

`schema/` is the contract between the three modules. It defines `IConfig`,
`IConfigParser`, `IRequestParser`, `IRequestHandler`, `ICgiExecutor`, and the
shared data types (`HttpRequest`, `HttpResponse`, `ServerConfig`,
`ResolvedLocation`, `CgiRequest`, `CgiSession`). Each member codes against these
headers, which lets the three parts be developed in parallel against stubs —
`test/http/StubConfig.hpp` and `test/http/StubCgiExecutor.hpp` are what remain of
that arrangement, now used by the unit tests.

## Team and scope

| Member | Scope | Main files | Makefile variable |
|---|---|---|---|
| **haito** | Network and connection management: sockets, the `poll()` loop, `accept()`, client lifecycle, timeouts, send/receive buffers | `srcs/Server.cpp`, `srcs/PollEventManager.cpp` | `SRCS_SERVER` |
| **ywada** | HTTP: request parsing, routing, GET / POST / DELETE / HEAD, static files, directory listing, uploads, redirects, error responses | `srcs/RequestParser.cpp`, `srcs/RequestHandler.cpp` | `SRCS_HTTP`, `SRCS_HANDLER` |
| **totake** | Configuration file parsing and CGI execution | `srcs/Config.cpp`, `srcs/ConfigParser.cpp`, `srcs/Parser.cpp`, `srcs/Tokenizer.cpp`, `srcs/TokenStream.cpp`, `srcs/CgiExecutor.cpp` | `SRCS_CONFIG`, `SRCS_CGI` |

Sources are listed per module in the `Makefile` so that the three members edit different lines and avoid merge conflicts. The full role breakdown is in `docs/webserv_role_design.md`.

## Current status

All three modules are implemented and integrated; the stubs they were built
against are now only used by the tests.

| Area | State |
|---|---|
| Listening sockets (`socket` / `setsockopt` / `bind` / `listen` / non-blocking / `FD_CLOEXEC`) | Done |
| Event loop — one `poll()` for every socket and pipe, deferred close | Done |
| Configuration file — tokenizer, parser, `server` / `location` inheritance | Done |
| HTTP request parsing — request line, headers, `Content-Length` and chunked bodies | Done |
| GET / POST / DELETE / HEAD, static files, directory listing, uploads, redirects | Done |
| Custom and built-in error pages | Done |
| Keep-alive, request pipelining, idle and total connection timeouts | Done |
| CGI — non-blocking pipes, chunked bodies un-chunked, EOF-terminated output, run in the script's directory | Done |
| CGI deadlines — killed and answered 504 when a script stalls or overruns | Done |

The design of the network layer, including the reasoning behind choosing `poll()` over `epoll()` and the ordering rules of the event loop, is documented in `docs/event_manager_design.md`.

### Behaviour worth knowing

These are decisions rather than accidents, and each one is deliberate:

- **A location's `methods` list is exhaustive.** A method absent from it is
  answered 405 with an `Allow` header, whatever the location would otherwise do.
- **`POST` needs an `upload_store`.** Without one there is nowhere to put the
  body, so the route answers 403.
- **A concurrency cap on CGI.** At most `MAX_CONCURRENT_CGI` scripts run at
  once; further requests wait for a slot rather than forking without limit.
- **Two clocks per connection and two per CGI.** An idle clock catches whatever
  has gone quiet, and a total clock caps the whole exchange, so a peer that
  dribbles one byte at a time cannot hold a slot open indefinitely.
- **`PATH_INFO` carries the script's own URI path when there is no extra path.**
  CGI/1.1 wants it empty there; the interpreter used for grading requires it to
  match `REQUEST_URI`, and that requirement won.

## Testing

Each suite builds what it needs and cleans up after itself, so they can be run
individually from the repository root.

```bash
sh test/run_config_tester.sh          # 101 configuration files: accepted/rejected and parsed values
sh test/run_integration_test.sh       # end-to-end over a real socket, against test/test.conf
sh test/run_http_tests.sh             # every unit suite under test/http/
```

| Suite | Covers |
|---|---|
| `test/run_config_tester.sh` | Every file in `conf/tests/`, checking both the accept/reject decision and the values produced |
| `test/run_integration_test.sh` | The server as a black box: status codes, headers, keep-alive, uploads, CGI, error pages |
| `test/http/run_request_parser_test.sh` | Request line, headers, limits, split and byte-at-a-time delivery |
| `test/http/run_chunked_keep_alive_test.sh` | Chunked decoding and connection reuse |
| `test/http/run_handler_test.sh` | Routing, index resolution, autoindex, error statuses |
| `test/http/run_post_delete_test.sh` | Uploads and deletion |
| `test/http/run_multipart_form_test.sh` | `multipart/form-data` bodies |
| `test/http/run_cgi_test.sh` | Environment construction and CGI output parsing |
| `test/http/run_cgi_timeout_test.sh` | A stalled script is killed; a slow but productive one is not |
| `test/http/run_cgi_chdir_test.sh` | The script runs in its own directory, for every shape of configured path |
| `test/http/run_cgi_disconnect_test.sh` | A script is killed when the client it answers goes away |

The CGI timeout suite rebuilds the server with short deadlines rather than
waiting out the production ones, which is why `CGI_IDLE_TIMEOUT_SEC` and
`CGI_TOTAL_TIMEOUT_SEC` are overridable at compile time.

## Resources

### HTTP and CGI

- RFC 1945 — Hypertext Transfer Protocol / HTTP 1.0
- RFC 9110 / RFC 9112 — HTTP Semantics and HTTP/1.1 (current revisions of RFC 7230–7231)
- RFC 3875 — The Common Gateway Interface (CGI) Version 1.1
- NGINX documentation — used as a reference implementation to compare header and status-code behaviour

### Sockets and I/O multiplexing

- Beej's Guide to Network Programming
- `man 2 poll`, `man 2 socket`, `man 2 accept`, `man 7 socket`, `man 7 tcp`
- The C10K problem — background on why event-driven servers exist

### Use of AI

AI was used across the whole project — the network layer, the HTTP layer, and
the configuration and CGI layers alike — in three roles: as a design partner
before code existed, as a reviewer of code that already did, and to carry out
the fixes those reviews called for. 

**Design.** Choosing `poll()` over `epoll()` and `kqueue()`, and shaping the
`IEventManager` abstraction that keeps the backend replaceable. The module
boundaries in `schema/`, which let the three parts be built in parallel against
stubs. The two-clock timeout scheme used for connections and for CGI
invocations alike, where one clock catches whatever has gone quiet and another
caps the whole exchange — a single absolute deadline would have killed a large
upload mid-transfer. Running a CGI child in the script's own directory without
`getcwd()` or `realpath()`, neither of which this project may use.

**Review.** Reading the subject and the evaluation sheet against the
implementation and recording every gap, which is what surfaced the CGI timeout
that was declared but never checked, the listening and client sockets leaking
into CGI children, `inet_addr()` sitting outside the allowed function list, and
a `read()` that was not driven by `poll()`. Reviewing each module's pull
requests as they were opened. Cross-checking the sources against `schema/` for
interface drift.

**Fixes and tests.** Where a review found a concrete defect, the fix was
written and measured: the CGI deadlines and the 504 they produce, killing a CGI
whose client has disconnected instead of letting it run for output nobody will
read, the `Connection: close` that a 408 announced without acting on, the
status code for a directory whose configured index is missing, and the default
for an unset `client_max_body_size`. Each arrived with a test that failed
before the change and passed after it — `test/http/run_cgi_timeout_test.sh`,
`run_cgi_chdir_test.sh`, and `run_cgi_disconnect_test.sh` were written this way.

**Documentation.** README
