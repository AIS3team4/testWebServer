#!/bin/bash
make test
python3 $CHECKER/check.py unit_test_report