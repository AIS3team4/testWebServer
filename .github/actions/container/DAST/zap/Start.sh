#!/bin/bash
export
zap-baseline.py -t http://$WEB_PORT -r testreport.html
python3 $CHECKER/check.py testreport.html