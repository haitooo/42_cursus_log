#!/bin/sh

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t chunked-keep-alive-test)
BIN="$BUILD_DIR/chunked_keep_alive_test"

cleanup() {
    rm -rf "$BUILD_DIR"
}

trap cleanup EXIT INT TERM

g++ -std=c++98 -Wall -Wextra -Werror \
    -I"$ROOT" -I"$ROOT/include" -I"$ROOT/schema" \
    "$ROOT/test/http/ChunkedKeepAliveTest.cpp" \
    "$ROOT/srcs/RequestParser.cpp" \
    -o "$BIN"

"$BIN"