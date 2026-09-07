#!/bin/sh

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BUILD_DIR=$(mktemp -d 2>/dev/null || mktemp -d -t multipart-form-test)
BIN="$BUILD_DIR/multipart_form_test"

cleanup() {
    rm -rf "$BUILD_DIR"
}

trap cleanup EXIT INT TERM

g++ -std=c++98 -Wall -Wextra -Werror \
    -I"$ROOT" -I"$ROOT/include" -I"$ROOT/schema" \
    "$ROOT/test/http/MultipartFormTest.cpp" \
    "$ROOT/srcs/RequestHandler.cpp" \
    "$ROOT/srcs/SessionStore.cpp" \
    "$ROOT/srcs/Random.cpp" \
    -o "$BIN"

cd "$BUILD_DIR"
mkdir -p www/uploads
"$BIN"