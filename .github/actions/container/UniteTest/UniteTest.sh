#!/bin/bash
make test
python3 $CHECKER/check.py test_report.txt