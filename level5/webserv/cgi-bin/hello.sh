#!/bin/sh
# Driven by /bin/sh, not by this shebang: the server always execve()s the
# program named by cgi_path and hands the script over as its first argument.
# Nothing about the CGI path is python specific.
#
# The exit status matters: CgiExecutor::finish() answers 502 unless the child
# exits 0, so the last command must not be a failing test.
printf 'Content-Type: text/plain\r\n\r\n'
printf 'hello from sh\n'
printf 'interpreter=/bin/sh script=%s\n' "$0"
printf 'method=%s query=%s path_info=%s\n' "$REQUEST_METHOD" "$QUERY_STRING" "$PATH_INFO"
# cwd is the script's own directory (subject IV.3), so a neighbour file opens
# by relative path.
printf 'cwd=%s neighbour=%s\n' "$(pwd)" "$(cat data.txt 2>/dev/null || echo MISSING)"
if [ "$REQUEST_METHOD" = POST ]; then
	printf 'body=%s\n' "$(cat)"
fi
exit 0
