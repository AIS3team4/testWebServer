#!/bin/bash
make test
file webserver_fork > ./result.txt
python3 .github/container/UniteTest/check.py ./result.txt