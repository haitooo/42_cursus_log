#!/bin/sh

# T-B: "The CGI should be run in the correct directory for relative path file
# access." (subject IV.3)
#
# Moving the child's cwd also moves the base of every relative path the exec
# still needs, so the interesting cases are the ones where that undoing is
# possible and the ones where it is not. Both are covered here: the second
# group only asserts that nothing regressed, because honouring the requirement
# there would break the exec outright.

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t run-cgi-chdir-test)
CONF="$BUILD_DIR/cgi_chdir.conf"
FIXTURE="$ROOT/test/chdir_fixture"
PORT=8096

SERVER_PID=""
failures=0

cleanup() {
    if [ -n "$SERVER_PID" ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        kill "$SERVER_PID" 2>/dev/null || true
        wait "$SERVER_PID" 2>/dev/null || true
    fi
    rm -f "$FIXTURE/py"
    rm -rf "$BUILD_DIR"
}

trap cleanup EXIT INT TERM

PYTHON=$(command -v python3 || true)
if [ -z "$PYTHON" ]; then
    echo "SKIP: python3 not found"
    exit 0
fi

# A *relative* interpreter path is what makes this test worth having: an
# absolute one survives any chdir untouched.
ln -sf "$PYTHON" "$FIXTURE/py"

make -s -C "$ROOT" >/dev/null

cat > "$CONF" <<EOF
server {
    listen 127.0.0.1:$PORT;

    root ./www;
    index index.html;

    location / {
        methods GET;
        autoindex off;
    }

    # Relative root + relative interpreter: the descent can be undone, so the
    # script must end up in its own directory.
    location /rel {
        methods GET;
        root ./test/chdir_fixture/scripts;
        cgi_extension .py;
        cgi_path ./test/chdir_fixture/py;
    }

    # Absolute root: naming the original cwd again would need getcwd(), which
    # is not an allowed function. Must still answer, not 502.
    location /abs {
        methods GET;
        root $FIXTURE/scripts;
        cgi_extension .py;
        cgi_path ./test/chdir_fixture/py;
    }

    # Root containing "..": its depth is not its segment count, so the number
    # of "../" cannot be derived. Must still answer, not 502.
    location /dotdot {
        methods GET;
        root ./test/chdir_fixture/x/../scripts;
        cgi_extension .py;
        cgi_path ./test/chdir_fixture/py;
    }

    # Absolute interpreter, relative root: the common case.
    location /cgi-bin {
        methods GET;
        root ./cgi-bin;
        cgi_extension .py;
        cgi_path $PYTHON;
    }
}
EOF

cd "$ROOT"
"$ROOT/webserv" "$CONF" >"$BUILD_DIR/server.log" 2>&1 &
SERVER_PID=$!

waited=0
while [ "$waited" -lt 50 ]; do
    if curl -s -o /dev/null --max-time 2 "http://127.0.0.1:$PORT/" 2>/dev/null; then
        break
    fi
    if ! kill -0 "$SERVER_PID" 2>/dev/null; then
        echo "FAIL: server exited during startup"
        cat "$BUILD_DIR/server.log"
        exit 1
    fi
    waited=$((waited + 1))
    sleep 0.1
done

check() {
    # $1 = name, $2 = expected, $3 = actual
    if [ "$2" = "$3" ]; then
        echo "ok   - $1 ($3)"
    else
        echo "FAIL - $1: expected '$2', got '$3'"
        failures=$((failures + 1))
    fi
}

get() {
    curl -s --max-time 10 "http://127.0.0.1:$PORT$1" 2>/dev/null || echo "curl_failed"
}
code() {
    curl -s -o /dev/null -w '%{http_code}' --max-time 10 \
        "http://127.0.0.1:$PORT$1" 2>/dev/null || echo "curl_failed"
}

# --- the requirement itself -------------------------------------------------

body=$(get /rel/probe.py)
check "relative root + relative interpreter: reads its neighbour" \
    "read=OK" "$(printf '%s\n' "$body" | grep -o 'read=[A-Za-z:]*' || true)"
check "relative root + relative interpreter: cwd is the script's directory" \
    "scripts" "$(printf '%s\n' "$body" | sed -n 's|^cwd=.*/\([^/]*\)$|\1|p')"

body=$(get /cgi-bin/cwd.py)
check "absolute interpreter: reads its neighbour" \
    "read=OK" "$(printf '%s\n' "$body" | grep -o 'read=[A-Za-z]*' || true)"
check "absolute interpreter: cwd is the script's directory" \
    "cgi-bin" "$(printf '%s\n' "$body" | sed -n 's|^cwd=.*/\([^/]*\)$|\1|p')"

# --- no regression where the descent cannot be undone -----------------------

check "absolute root + relative interpreter still answers" "200" "$(code /abs/probe.py)"
check "root containing '..' + relative interpreter still answers" "200" "$(code /dotdot/probe.py)"

kill "$SERVER_PID" 2>/dev/null || true
wait "$SERVER_PID" 2>/dev/null || true
SERVER_PID=""

# --- the tester's own layout: relative interpreter ./cgi_test ---------------

if [ -x "$ROOT/cgi_test" ] && [ -d "$ROOT/YoupiBanane" ]; then
    "$ROOT/webserv" "$ROOT/conf/tester.conf" >"$BUILD_DIR/tester.log" 2>&1 &
    SERVER_PID=$!
    sleep 1
    got=$(curl -s -o /dev/null -w '%{http_code}' --max-time 20 -X POST -d hello \
        http://127.0.0.1:8080/directory/youpi.bla 2>/dev/null || echo "curl_failed")
    check "tester layout: POST .bla with relative ./cgi_test" "200" "$got"
    kill "$SERVER_PID" 2>/dev/null || true
    wait "$SERVER_PID" 2>/dev/null || true
    SERVER_PID=""
else
    echo "skip - tester layout (cgi_test or YoupiBanane missing)"
fi

if [ "$failures" -ne 0 ]; then
    echo ""
    echo "$failures check(s) failed. tail of server log:"
    tail -30 "$BUILD_DIR/server.log"
    exit 1
fi

echo ""
echo "all CGI chdir checks passed"
