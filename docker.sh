#!/bin/bash
docker run -v ~/:/zap/wrk/:rw --net=host -t owasp/zap2docker-weekly zap-baseline.py -t http://localhost:8080/index.html -r testreport.html

echo $?
