#!/bin/bash
PATH=/bin:/usr/bin
/bin/make test
/bin/file webserver_fork > ./result.txt
/usr/bin/python3 checker/check.py ./result.txt