#!/bin/bash
make test
file webserver_fork > ./result.txt
python $CHECKER/check.py ./result.txt