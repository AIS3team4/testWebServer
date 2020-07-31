import sys
file_name="unit_test_report"

fd=open(file_name,"r")

for line in fd:
    if "Fail" in line:
        print("fail the unit test")
        sys.exit(1)

print("pass the unit test!")
sys.exit(0)
