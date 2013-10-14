#!/bin/sh

sh gen-docutils.sh install && \
echo "Generating DocUtils Docs..." && \
cd docutils/tools && \
python2.5 buildhtml.py

cd ../..


