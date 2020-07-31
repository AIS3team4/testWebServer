#!/bin/bash
ls
make test
python3 $CHECKER/check.py test_report.txt