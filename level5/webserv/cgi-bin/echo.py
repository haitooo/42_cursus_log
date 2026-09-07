#!/usr/bin/env python3
# Reads the whole request body and writes it back, so a body far larger than
# the pipe buffer has to cross it in both directions. Guards the timeout
# against killing a transfer that is simply big.
import sys

body = sys.stdin.buffer.read()
sys.stdout.write("Content-Type: application/octet-stream\r\n")
sys.stdout.write("Content-Length: %d\r\n\r\n" % len(body))
sys.stdout.flush()
sys.stdout.buffer.write(body)
sys.stdout.buffer.flush()
