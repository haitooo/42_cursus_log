import os
print("Content-Type: text/plain\r\n\r\n", end="")
print("cwd=" + os.getcwd())
try:
    open("data.txt").read(); print("read=OK")
except Exception as e:
    print("read=FAILED: " + type(e).__name__)
