#!/usr/bin/env python3
# Reports the working directory it was started in and whether the file sitting
# next to it opens by a bare relative name -- the whole of what IV.3 asks for.
import os
import sys

sys.stdout.write("Content-Type: text/plain\r\n\r\n")
sys.stdout.write("cwd=%s\n" % os.getcwd())
try:
    with open("data.txt") as f:
        f.read()
    sys.stdout.write("read=OK\n")
except Exception as e:
    sys.stdout.write("read=FAILED:%s\n" % type(e).__name__)
