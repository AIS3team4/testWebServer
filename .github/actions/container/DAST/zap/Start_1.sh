#!/bin/bash
export
ls -al /github/workspace
/github/workspace/make
/github/workspace/webserver_fork 8080 &
/zap/zap-baseline.py -t http://localhost:8080 > /tmp/testreport.html
python3 $CHECKER/check.py /tmp/testreport.html
kill %1
