#!/bin/bash
export
ls -al 
whoami
/zap/zap-baseline.py -t http://172.28.0.4:8080 > /tmp/testreport.html
python3 $CHECKER/check.py /tmp/testreport.html
