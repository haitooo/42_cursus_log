#!/bin/sh

# Integration test for the CGI timeouts. Unlike the other tests under
# test/http, this one has to drive the real event loop: the timeouts fire from
# poll() coming back on its own, so there is nothing to exercise at the
# RequestHandler level.
#
# The server is rebuilt with short deadlines rather than waiting out the
# production ones, which is why CGI_IDLE_TIMEOUT_SEC / CGI_TOTAL_TIMEOUT_SEC
# are overridable at compile time.

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t run-cgi-timeout-test)
BIN="$BUILD_DIR/webserv_timeout"
CONF="$BUILD_DIR/cgi_timeout.conf"
PORT=8099
URL="http://127.0.0.1:$PORT"

IDLE=2
TOTAL=8

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
    echo "SKIP: python3 not found, the CGI scripts cannot run"
    exit 0
fi

echo "building webserv with CGI_IDLE_TIMEOUT_SEC=$IDLE CGI_TOTAL_TIMEOUT_SEC=$TOTAL"
c++ -std=c++98 -Wall -Wextra -Werror \
    -I"$ROOT/include" \
    -I"$ROOT/schema" \
    -I"$ROOT/srcs" \
    -DCGI_IDLE_TIMEOUT_SEC="$IDLE" \
    -DCGI_TOTAL_TIMEOUT_SEC="$TOTAL" \
    "$ROOT"/srcs/*.cpp \
    -o "$BIN"

cat > "$CONF" <<EOF
server {
    listen 127.0.0.1:$PORT;

    root ./www;
    index index.html;

    client_max_body_size 20971520;

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
"$BIN" "$CONF" >"$BUILD_DIR/server.log" 2>&1 &
SERVER_PID=$!

waited=0
while [ "$waited" -lt 50 ]; do
    if curl -s -o /dev/null --max-time 2 "$URL/" 2>/dev/null; then
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

report() {
    # $1 = name, $2 = expected, $3 = actual
    if [ "$2" = "$3" ]; then
        echo "ok   - $1 ($3)"
    else
        echo "FAIL - $1: expected '$2', got '$3'"
        failures=$((failures + 1))
    fi
}

# A CGI that never writes anything must be killed by the no-progress deadline
# and answered 504, not left hanging.
started=$(date +%s)
code=$(curl -s -o /dev/null -w '%{http_code}' --max-time 30 \
    "$URL/cgi-bin/hang.py" || echo "curl_failed")
elapsed=$(( $(date +%s) - started ))
report "hung CGI answers 504" "504" "$code"
if [ "$elapsed" -gt $((IDLE + 5)) ]; then
    echo "FAIL - hung CGI took ${elapsed}s, expected around ${IDLE}s"
    failures=$((failures + 1))
else
    echo "ok   - hung CGI killed after ${elapsed}s"
fi

# Steady progress past the no-progress deadline must survive: this is the case
# a single absolute deadline would have killed.
slow=$((IDLE + 3))
body=$(curl -s --max-time 30 "$URL/cgi-bin/drip.py?seconds=$slow" \
    || echo "curl_failed")
ticks=$(printf '%s\n' "$body" | grep -c '^tick ' || true)
report "CGI dripping for ${slow}s completes" "$slow" "$ticks"

# ... but the total cap still stops a CGI that drips forever.
started=$(date +%s)
code=$(curl -s -o /dev/null -w '%{http_code}' --max-time 40 \
    "$URL/cgi-bin/drip.py?seconds=600" || echo "curl_failed")
elapsed=$(( $(date +%s) - started ))
report "endlessly dripping CGI answers 504" "504" "$code"
if [ "$elapsed" -lt "$TOTAL" ] || [ "$elapsed" -gt $((TOTAL + 5)) ]; then
    echo "FAIL - dripping CGI took ${elapsed}s, expected around ${TOTAL}s"
    failures=$((failures + 1))
else
    echo "ok   - dripping CGI killed after ${elapsed}s"
fi

# A body far larger than the pipe buffer has to cross it in both directions
# without the deadline firing on it.
PAYLOAD="$BUILD_DIR/payload.bin"
dd if=/dev/urandom of="$PAYLOAD" bs=1048576 count=5 2>/dev/null
ECHOED="$BUILD_DIR/echoed.bin"
code=$(curl -s -o "$ECHOED" -w '%{http_code}' --max-time 60 \
    -H 'Content-Type: application/octet-stream' \
    --data-binary "@$PAYLOAD" "$URL/cgi-bin/echo.py" || echo "curl_failed")
report "5MB body through CGI answers 200" "200" "$code"
sent=$(wc -c < "$PAYLOAD" | tr -d ' ')
got=$(wc -c < "$ECHOED" | tr -d ' ')
report "5MB body echoed back intact" "$sent" "$got"

# Killed CGIs must be reaped, not left as zombies.
zombies=$(ps -o stat= --ppid "$SERVER_PID" 2>/dev/null | grep -c 'Z' || true)
report "no zombie CGI children" "0" "$zombies"

children=$(ps -o pid= --ppid "$SERVER_PID" 2>/dev/null | wc -l | tr -d ' ')
report "no surviving CGI children" "0" "$children"

if [ "$failures" -ne 0 ]; then
    echo ""
    echo "$failures check(s) failed. tail of server log:"
    tail -40 "$BUILD_DIR/server.log"
    exit 1
fi

echo ""
echo "all CGI timeout checks passed"
