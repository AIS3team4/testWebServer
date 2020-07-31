#!/bin/bash
export
zap-baseline.py -t http://$WEB_PORT_80_TCP_ADDR -r testreport.html
python3 $CHECKER/check.py testreport.html