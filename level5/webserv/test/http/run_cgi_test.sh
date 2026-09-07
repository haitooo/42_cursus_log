#!/bin/sh

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t run-cgi-test)
BIN="$BUILD_DIR/run_cgi_test"

cleanup() {
    rm -rf "$BUILD_DIR"
}

trap cleanup EXIT INT TERM

#-Wall -Wextra -Werror \
g++ -std=c++98 -Wall -Wextra -Werror \
    -I"$ROOT" \
    -I"$ROOT/include" \
    -I"$ROOT/schema" \
    "$ROOT/test/http/CgiTest.cpp" \
    "$ROOT/srcs/RequestHandler.cpp" \
    "$ROOT/srcs/SessionStore.cpp" \
    "$ROOT/srcs/Random.cpp" \
    -o "$BIN"

cd "$BUILD_DIR"
"$BIN"