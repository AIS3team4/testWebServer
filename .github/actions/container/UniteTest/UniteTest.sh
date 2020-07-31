#!/bin/bash
PATH=/bin:/usr/bin
/usr/bin/make test
/usr/bin/file webserver_fork > ./result.txt
/usr/bin/python3 checker/check.py ./result.txt