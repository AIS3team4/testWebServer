#!/bin/bash
export
ls -al /
#echo $WEB_URL
/zap/zap-baseline.py -t http://172.28.0.4:8080/index.html > /tmp/testreport.html
python3 $CHECKER/check.py /tmp/testreport.html
