#!/usr/bin/env python3
# Writes one chunk per second for ?seconds=N, so it makes steady progress while
# never finishing quickly. Separates the two CGI clocks: a run shorter than the
# total cap must survive no matter how far it passes the no-progress deadline,
# and a run past the cap must be killed even though it never stalls.
import os
import sys
import time

seconds = 5
query = os.environ.get("QUERY_STRING", "")
for field in query.split("&"):
    if field.startswith("seconds="):
        seconds = int(field[len("seconds="):])

sys.stdout.write("Content-Type: text/plain\r\n\r\n")
sys.stdout.flush()
for i in range(seconds):
    sys.stdout.write("tick %d\n" % i)
    sys.stdout.flush()
    time.sleep(1)
