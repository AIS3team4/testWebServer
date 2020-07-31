#!/bin/bash
export
ls
/zap/zap-baseline.py -t http://$WEB_IP:$WEB_PORT -r testreport.html
python3 $CHECKER/check.py testreport.html