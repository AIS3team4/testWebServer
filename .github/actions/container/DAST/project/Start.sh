#!/bin/bash
export
make
# ./webserver_fork
echo "*********************** Start Waiting ***********************"
python3 -m http.server 0.0.0.0:8080