#!/usr/bin/env python3
import os
import sys
import html

def main():
    sys.stdout.write("Content-Type: text/html\r\n\r\n")

    query_string = html.escape(os.environ.get("QUERY_STRING", ""))
    path_info = html.escape(os.environ.get("PATH_INFO", ""))
    request_method = html.escape(os.environ.get("REQUEST_METHOD", ""))
    content_length = html.escape(os.environ.get("CONTENT_LENGTH", ""))

    try:
        length = int(content_length)
    except ValueError:
        length = 0
    body = html.escape(sys.stdin.read(length)) if length > 0 else ""

    html_content = f"""<html>
<head>
    <title>CGI Test</title>
</head>
<body>
    <h1>Hello from CGI!</h1>
    <p>query_string: {query_string}</p>
    <p>path_info: {path_info}</p>
    <p>request_method: {request_method}</p>
    <p>content_length: {content_length}</p>
    <p>body: {body}</p>
</body>
</html>"""

    print(html_content)

if __name__ == "__main__":
    main()
