#!/usr/bin/python3
import sys
file_name=sys.argv[-1]

fd=open(file_name,"r")
for line in fd:
    if "Fail" in line:
        print("fail the unit test")
        sys.exit(1)

print("pass the unit test!")
sys.exit(0)