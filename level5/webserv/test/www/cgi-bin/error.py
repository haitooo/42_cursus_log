#!/usr/bin/env python3
import sys

def main():
    sys.stderr.write("CGI script encountered a fatal error!\n")
    sys.exit(1)

    # raise RuntimeError("Intentional CGI crash")

if __name__ == "__main__":
    main()