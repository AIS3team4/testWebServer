#!/bin/bash
ls
make test
python $CHECKER/check.py test_report.txt