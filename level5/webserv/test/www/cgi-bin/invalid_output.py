#!/usr/bin/env python3
import sys

def main():
    sys.stdout.write("This is missing HTTP headers!\n")

    # sys.stdout.write("Content-Type: text/html\n")
    # sys.stdout.write("Invalid-Body-Without-CRLFCRLF")

    # sys.stdout.write("X-Custom-Header: value\r\n\r\n")

if __name__ == "__main__":
    main()