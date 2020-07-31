#!/bin/bash
export
ls
/zap/zap-baseline.py -t http://$WEB_IP:$WEB_PORT > $CHECKER/testreport.html
find / -name testreport.html
python3 $CHECKER/check.py $CHECKER/testreport.html