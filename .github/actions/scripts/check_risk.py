import sys
file_name="flawfinder_report.html"

fd=open(file_name,"r")

for line in fd:
    if "Hits@level+" in line:
        print(line)
        out=line.split('=',1)
        num=out[1].split(' ')
        print(num)
        arr=list()
        for i in range(len(num)):
            if(num[i]!=''):
                arr.append(num[i])

        print(arr)
        risk=list()
        for i in range(6):
            risk.append(arr[1+i*2])
        print(risk)
        if(int(risk[2])>=1000):
            print("your program is too risk")
            sys.exit(1)
        else:
            print("pass")
            sys.exit(0)

