#!/bin/bash
export
ls -al /
/zap/zap-baseline.py -t http://$WEB_IP:$WEB_PORT > /tmp/testreport.html
find / -name testreport.html
python3 $CHECKER/check.py /tmp/testreport.html