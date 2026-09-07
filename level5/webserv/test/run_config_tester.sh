#!/bin/sh

set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
PROJECT_ROOT=$(CDPATH= cd -- "$SCRIPT_DIR/.." && pwd)

CXX=${CXX:-c++}
CXXFLAGS=${CXXFLAGS:--Wall -Wextra -Werror -std=c++98}
BINARY=$(mktemp "${TMPDIR:-/tmp}/config_tester.XXXXXX")
trap 'rm -f "$BINARY"' EXIT HUP INT TERM

cd "$PROJECT_ROOT"

$CXX $CXXFLAGS -Iinclude -Ischema -Isrcs \
	test/Config_tester.cpp \
	srcs/Config.cpp \
	srcs/ConfigParser.cpp \
	srcs/Parser.cpp \
	srcs/TokenStream.cpp \
	srcs/Tokenizer.cpp \
	-o "$BINARY"

"$BINARY" "$@"
