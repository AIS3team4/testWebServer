#!/bin/bash
make test
file webserver_fork > ./result.txt
python3 $CHECKER/check.py ./result.txt