#!/bin/bash
PATH=/bin:/usr/bin
/usr/bin/make test
/usr/bin/file webserver_fork > ./result.txt
/usr/share/gcc-8/python checker/check.py ./result.txt