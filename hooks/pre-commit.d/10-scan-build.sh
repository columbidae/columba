#!/bin/sh

if [ -e Makefile ]; then
   make clean
   scan-build --status-bugs make || exit -1
fi
