#!/usr/bin/python3

pattern = 'err*'

import re
import sys

fname  = sys.argv[-1]
if fname ==None:
    print("file not found **********")
p = re.compile(pattern, re.IGNORECASE)
f = open(fname)
content = f.read().decode("utf8")
f.close()
if p.match(content) != None:
    print("err exit")
    sys.exit(1)
else:
    print("exit")
    sys.exit(0)