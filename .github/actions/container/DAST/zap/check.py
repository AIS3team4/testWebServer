#!/usr/bin/python3

pattern = r'\d*? err'

import re
import sys

fname  = sys.argv[-1]
p = re.compile(pattern, re.IGNORECASE)
f = open(fname)
content = f.read()
f.close()
print(content)
if p.match(content) != None:
    sys.exit(1)
else:
    sys.exit(0)