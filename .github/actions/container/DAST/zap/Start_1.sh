#!/bin/bash
export
ls -al /
echo $WEB_URL
/zap/zap-baseline.py -t http://$WEB_URL > /tmp/testreport.html
python3 $CHECKER/check.py /tmp/testreport.html
