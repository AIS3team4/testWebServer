#!/bin/bash
export
ls -al /
echo $WEB_IP
/zap/zap-baseline.py -t http://$WEB_IP:$WEB_PORT > /tmp/testreport.html
python3 $CHECKER/check.py /tmp/testreport.html
