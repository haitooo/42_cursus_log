#ifndef HTTP_STATUS_HPP
# define HTTP_STATUS_HPP

# define STATUS_OK 200
# define STATUS_CREATED 201
# define STATUS_NO_CONTENT 204
# define STATUS_MOVED_PERMANENTLY 301
# define STATUS_FOUND 302
# define STATUS_BAD_REQUEST 400
# define STATUS_FORBIDDEN 403
# define STATUS_NOT_FOUND 404
# define STATUS_METHOD_NOT_ALLOWED 405
# define STATUS_REQUEST_TIMEOUT 408
# define STATUS_LENGTH_REQUIRED 411
# define STATUS_PAYLOAD_TOO_LARGE 413
# define STATUS_URI_TOO_LONG 414
# define STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE 431
# define STATUS_INTERNAL_SERVER_ERROR 500
# define STATUS_NOT_IMPLEMENTED 501
# define STATUS_BAD_GATEWAY 502
# define STATUS_GATEWAY_TIMEOUT 504
# define STATUS_HTTP_VERSION_NOT_SUPPORTED 505

# define REASON_OK "OK"
# define REASON_CREATED "Created"
# define REASON_NO_CONTENT "No Content"
# define REASON_MOVED_PERMANENTLY "Moved Permanently"
# define REASON_FOUND "Found"
# define REASON_BAD_REQUEST "Bad Request"
# define REASON_FORBIDDEN "Forbidden"
# define REASON_NOT_FOUND "Not Found"
# define REASON_METHOD_NOT_ALLOWED "Method Not Allowed"
# define REASON_REQUEST_TIMEOUT "Request Timeout"
# define REASON_LENGTH_REQUIRED "Length Required"
# define REASON_PAYLOAD_TOO_LARGE "Payload Too Large"
# define REASON_URI_TOO_LONG "URI Too Long"
# define REASON_REQUEST_HEADER_FIELDS_TOO_LARGE "Request Header Fields Too Large"
# define REASON_INTERNAL_SERVER_ERROR "Internal Server Error"
# define REASON_NOT_IMPLEMENTED "Not Implemented"
# define REASON_BAD_GATEWAY "Bad Gateway"
# define REASON_GATEWAY_TIMEOUT "Gateway Timeout"
# define REASON_HTTP_VERSION_NOT_SUPPORTED "HTTP Version Not Supported"

# define HTTP_VERSION_1_1 "HTTP/1.1"
# define HTTP_VERSION_1_0 "HTTP/1.0"

# define CONTENT_TYPE_HTML "text/html"
# define CONTENT_TYPE_CSS "text/css"
# define CONTENT_TYPE_PLAIN "text/plain"
# define CONTENT_TYPE_JAVASCRIPT "application/javascript"
# define CONTENT_TYPE_JSON "application/json"
# define CONTENT_TYPE_XML "application/xml"
# define CONTENT_TYPE_PNG "image/png"
# define CONTENT_TYPE_JPEG "image/jpeg"
# define CONTENT_TYPE_GIF "image/gif"
# define CONTENT_TYPE_SVG "image/svg+xml"
# define CONTENT_TYPE_OCTET "application/octet-stream"
# define CONTENT_TYPE_MULTIPART_FORM_DATA "multipart/form-data"
# define CONTENT_TYPE_URLENCODED "application/x-www-form-urlencoded"
# define CONTENT_TYPE_TEXT_CSV "text/csv"
# define CONTENT_TYPE_MP4 "video/mp4"
# define CONTENT_TYPE_MPEG "video/mpeg"
# define CONTENT_TYPE_PDF "application/pdf"
# define CONTENT_TYPE_ZIP "application/zip"
# define CONTENT_TYPE_MP3 "audio/mpeg"
# define CONTENT_TYPE_AAC "audio/aac"

// response header fields
# define FIELD_CONTENT_TYPE "Content-Type"
# define FIELD_CONTENT_LENGTH "Content-Length"
# define FIELD_LOCATION "Location"
# define FIELD_ALLOW "Allow"
# define FIELD_HOST "Host"
# define FIELD_CONNECTION "Connection"
# define FIELD_CONTENT_DISPOSITION "Content-Disposition"
# define FIELD_SET_COOKIE "Set-Cookie"

#endif
