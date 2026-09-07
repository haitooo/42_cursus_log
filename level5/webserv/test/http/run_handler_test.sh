#!/bin/sh

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t request-handler-test)
BIN="$BUILD_DIR/request_parser_test"

cleanup() {
    rm -rf "$BUILD_DIR"
}

trap cleanup EXIT INT TERM

g++ -std=c++98 -Wall -Wextra -Werror \
    -I"$ROOT" \
    -I"$ROOT/include" \
    -I"$ROOT/schema" \
    "$ROOT/test/http/HandlerTest.cpp" \
    "$ROOT/srcs/RequestHandler.cpp" \
    "$ROOT/srcs/SessionStore.cpp" \
    "$ROOT/srcs/Random.cpp" \
    -o "$BIN"

cd "$BUILD_DIR"
"$BIN"