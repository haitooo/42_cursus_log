#!/bin/sh

set -u

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
HTTP_DIR="$ROOT/test/http"

GREEN=''
RED=''
BOLD=''
RESET=''
if [ -t 1 ]; then
	GREEN=$(printf '\033[32m')
	RED=$(printf '\033[31m')
	BOLD=$(printf '\033[1m')
	RESET=$(printf '\033[0m')
fi

passed=0
failed=0
failed_names=''

for script in "$HTTP_DIR"/run_*.sh; do
	[ -f "$script" ] || continue
	name=$(basename "$script" .sh)

	printf '%s=== %s ===%s\n' "$BOLD" "$name" "$RESET"
	if sh "$script"; then
		printf '%s[PASS]%s %s\n\n' "$GREEN" "$RESET" "$name"
		passed=$((passed + 1))
	else
		status=$?
		printf '%s[FAIL]%s %s (exit %d)\n\n' "$RED" "$RESET" "$name" "$status"
		failed=$((failed + 1))
		failed_names="$failed_names $name"
	fi
done

total=$((passed + failed))
printf '%s%d/%d suites passed%s\n' "$BOLD" "$passed" "$total" "$RESET"

if [ "$failed" -ne 0 ]; then
	printf '%sfailed:%s%s\n' "$RED" "$failed_names" "$RESET"
	exit 1
fi
exit 0
