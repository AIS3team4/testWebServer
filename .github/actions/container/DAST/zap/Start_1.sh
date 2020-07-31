#!/bin/bash
export
ls -al 
cd /github/workspace
ls -la
make
./webserver_fork 8080 &
/zap/zap-baseline.py -t http://localhost:8080/index.html > /tmp/testreport.html
#python3 $CHECKER/check.py /tmp/testreport.html
kill %1
