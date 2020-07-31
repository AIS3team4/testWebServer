#!/bin/bash
make
make test
python3 $CHECKER/check_unit_test.py unit_test_report
