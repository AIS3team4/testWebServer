#!/bin/bash
export
ls -al /
/zap/zap-baseline.py -t http://127.0.0.1 > /tmp/testreport.html
python3 $CHECKER/check.py /tmp/testreport.html