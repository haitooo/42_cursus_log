#!/usr/bin/env python3
# Never writes anything and never exits: the "hung CGI" case the timeout exists
# for. Without a timeout the connection serving this waits forever.
import time

while True:
    time.sleep(1)
