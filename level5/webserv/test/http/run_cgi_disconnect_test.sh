#!/bin/sh

# T-K: "Your server must remain non-blocking at all times and properly handle
# client disconnections when necessary." (subject IV.1)
#
# A client waiting on a CGI used to be dropped from the poll set entirely, so
# its FIN never arrived and the script kept running for output nobody would
# read -- until the CGI deadline killed it, up to a minute later.
#
# The other half of watching that socket is that bytes may arrive on it too:
# a pipelined next request. Those must not be mistaken for a disconnect, nor
# fed to the parser while the CGI it already parsed is still running.

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t run-cgi-disconnect-test)
CONF="$BUILD_DIR/cgi_disconnect.conf"
PORT=8094

SERVER_PID=""
failures=0

cleanup() {
    if [ -n "$SERVER_PID" ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        kill "$SERVER_PID" 2>/dev/null || true
        wait "$SERVER_PID" 2>/dev/null || true
    fi
    rm -rf "$BUILD_DIR"
}

trap cleanup EXIT INT TERM

PYTHON=$(command -v python3 || true)
if [ -z "$PYTHON" ]; then
    echo "SKIP: python3 not found"
    exit 0
fi

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

    location /cgi-bin {
        methods GET POST;
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
    if [ "$2" = "$3" ]; then
        echo "ok   - $1 ($3)"
    else
        echo "FAIL - $1: expected '$2', got '$3'"
        failures=$((failures + 1))
    fi
}

children() {
    pgrep -P "$SERVER_PID" 2>/dev/null | wc -l | tr -d ' '
}

# A script that keeps writing is the case the CGI deadlines cannot help with:
# it makes progress, so only the disconnect itself can end it early.
"$PYTHON" - "$PORT" <<'EOF' &
import socket, sys, time
s = socket.create_connection(("127.0.0.1", int(sys.argv[1])), timeout=10)
s.sendall(b"GET /cgi-bin/drip.py?seconds=600 HTTP/1.1\r\nHost: x\r\n\r\n")
time.sleep(2)
s.close()
EOF
CLIENT=$!
sleep 1
running=$(children)
wait "$CLIENT" 2>/dev/null || true

# The client has sent its FIN. Give the loop a few poll rounds.
gone_after=""
elapsed=0
while [ "$elapsed" -lt 15 ]; do
    if [ "$(children)" = "0" ]; then
        gone_after="$elapsed"
        break
    fi
    sleep 1
    elapsed=$((elapsed + 1))
done

check "the CGI was running before the disconnect" "1" "$running"
if [ -z "$gone_after" ]; then
    echo "FAIL - CGI still running 15s after the client disconnected"
    failures=$((failures + 1))
elif [ "$gone_after" -le 5 ]; then
    echo "ok   - CGI reaped ${gone_after}s after the disconnect"
else
    echo "FAIL - CGI took ${gone_after}s to be reaped, expected under 5s"
    failures=$((failures + 1))
fi

check "no CGI children left" "0" "$(children)"

# Bytes arriving during a CGI are a pipelined request, not a disconnect: the
# CGI must still finish and answer in full.
result=$("$PYTHON" - "$PORT" <<'EOF'
import socket, sys
s = socket.create_connection(("127.0.0.1", int(sys.argv[1])), timeout=30)
s.sendall(b"GET /cgi-bin/drip.py?seconds=3 HTTP/1.1\r\nHost: x\r\n\r\n")
import time
time.sleep(0.5)
# Pipelined while the CGI is still running.
s.sendall(b"GET / HTTP/1.1\r\nHost: x\r\n\r\n")
data = b""
while True:
    try:
        chunk = s.recv(65536)
    except socket.timeout:
        break
    if not chunk:
        break
    data += chunk
s.close()
head = data.split(b"\r\n", 1)[0].decode(errors="replace")
print("%s ticks=%d responses=%d" % (head, data.count(b"tick "), data.count(b"HTTP/1.1 ")))
EOF
)
check "pipelined bytes during a CGI do not abort it" "HTTP/1.1 200 OK" \
    "$(printf '%s' "$result" | sed 's/ ticks=.*//')"
check "the CGI answered in full" "ticks=3" \
    "$(printf '%s' "$result" | grep -o 'ticks=[0-9]*')"
check "the pipelined request was answered too" "responses=2" \
    "$(printf '%s' "$result" | grep -o 'responses=[0-9]*')"

# Nothing above broke the ordinary path.
check "a normal CGI still answers" "200" \
    "$(curl -s -o /dev/null -w '%{http_code}' --max-time 10 \
        "http://127.0.0.1:$PORT/cgi-bin/hello.py" 2>/dev/null || echo curl_failed)"
check "still no CGI children left" "0" "$(children)"

if [ "$failures" -ne 0 ]; then
    echo ""
    echo "$failures check(s) failed. tail of server log:"
    tail -30 "$BUILD_DIR/server.log"
    exit 1
fi

echo ""
echo "all CGI disconnect checks passed"
