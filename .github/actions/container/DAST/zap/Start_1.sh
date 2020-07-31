#!/bin/bash
export
ls -al /
echo $URL
/zap/zap-baseline.py -t $URL > /tmp/testreport.html
python3 $CHECKER/check.py /tmp/testreport.html
