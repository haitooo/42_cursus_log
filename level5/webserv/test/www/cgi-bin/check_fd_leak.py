#!/usr/bin/env python3
import os
import sys

sys.stdout.write("Status: 200 OK\r\n")
sys.stdout.write("Content-Type: text/plain\r\n\r\n")
sys.stdout.flush()

leaked = []

for fd in range(3, 1024):
    try:
        os.fstat(fd)

        proc_path = f"/proc/self/fd/{fd}"
        if os.path.islink(proc_path):
            try:
                target = os.readlink(proc_path)
                leaked.append(f"FD {fd} -> {target}")
            except OSError:
                leaked.append(f"FD {fd} (open)")
        else:
            leaked.append(f"FD {fd} (open)")
    except OSError:
        pass

if leaked:
    print("[FAIL] Leaked file descriptors detected:")
    for item in leaked:
        print(f"  {item}")
else:
    print("[SUCCESS] No leaked file descriptors found (Clean: only 0, 1, 2 open).")