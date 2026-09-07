#!/bin/sh

set -e

root=YoupiBanane

rm -rf "$root"
mkdir -p "$root/nop" "$root/Yeah"

printf 'youpi.bad_extension at the root of YoupiBanane\n' > "$root/youpi.bad_extension"
printf 'youpi.bla at the root of YoupiBanane\n'           > "$root/youpi.bla"
printf 'youpi.bad_extension inside nop\n'                 > "$root/nop/youpi.bad_extension"
printf 'other.pouic inside nop\n'                         > "$root/nop/other.pouic"
printf 'not_happy.bad_extension inside Yeah\n'            > "$root/Yeah/not_happy.bad_extension"

echo "created:"
find "$root" -print | sort

echo
cgi_bin=""
for name in cgi_test cgi_tester; do
	if [ -f "$name" ]; then
		cgi_bin="$name"
		break
	fi
done

if [ -z "$cgi_bin" ]; then
	echo "WARNING: CGI tester not found. Get it from 42 intra and put it in this directory."
	echo "         Either name works: cgi_test or cgi_tester."
	echo "         Without it, the .bla CGI tests cannot pass."
else
	chmod +x "$cgi_bin"
	if [ "$cgi_bin" != cgi_test ]; then
		rm -f cgi_test
		ln -s "$cgi_bin" cgi_test 2>/dev/null || cp "$cgi_bin" cgi_test
		chmod +x cgi_test
		echo "cgi: $cgi_bin is now reachable through ./cgi_test"
	else
		echo "cgi: using ./cgi_test"
	fi
fi
