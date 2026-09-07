#!/bin/sh

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t integration-test)
BIN="$BUILD_DIR/integration_test"
CONFIG="${1:-$ROOT/test/test.conf}"
CONFIG=$(CDPATH= cd -- "$(dirname -- "$CONFIG")" && pwd)/$(basename -- "$CONFIG")

FIXTURE_DIR="$ROOT/test"

SERVER_PID=""

cleanup() {
    if [ -n "$SERVER_PID" ] && kill -0 "$SERVER_PID" 2>/dev/null; then
        echo "\n==> Stopping webserv (PID: $SERVER_PID)..."
        kill "$SERVER_PID" 2>/dev/null || true
        wait "$SERVER_PID" 2>/dev/null || true
    fi
    rm -rf "$BUILD_DIR"
}

trap cleanup EXIT INT TERM

for PORT in 8080 8081; do
    if nc -z 127.0.0.1 "$PORT" 2>/dev/null; then
        echo "Error: 127.0.0.1:$PORT is already in use." >&2
        echo "       Stop the process first (ss -ltnp | grep :$PORT)." >&2
        exit 1
    fi
done

echo "==> Building webserv..."
make -C "$ROOT"

echo "==> Building integration test binary..."
g++ -std=c++98 -Wall -Wextra -Werror \
    -I"$ROOT" -I"$ROOT/include" -I"$ROOT/schema" \
    "$ROOT/test/IntegrationTest.cpp" \
    -o "$BIN"

mkdir -p "$FIXTURE_DIR/www/uploads"

SERVER_LOG="$BUILD_DIR/webserv.log"

cd "$FIXTURE_DIR"

echo "==> Starting webserv in background..."
"$ROOT/webserv" "$CONFIG" > "$SERVER_LOG" 2>&1 &
SERVER_PID=$!

echo "==> Waiting for webserv to listen on port 8080..."
MAX_RETRIES=30
COUNT=0
while :; do
    if ! kill -0 "$SERVER_PID" 2>/dev/null; then
        echo "Error: webserv exited prematurely. Server log:" >&2
        cat "$SERVER_LOG" >&2
        exit 1
    fi
    if nc -z 127.0.0.1 8080 2>/dev/null; then
        break
    fi
    sleep 0.1 2>/dev/null || sleep 1
    COUNT=$((COUNT + 1))
    if [ "$COUNT" -ge "$MAX_RETRIES" ]; then
        echo "Error: Timeout waiting for webserv to open port 8080. Server log:" >&2
        cat "$SERVER_LOG" >&2
        exit 1
    fi
done

echo "==> Running integration tests..."
"$BIN"
